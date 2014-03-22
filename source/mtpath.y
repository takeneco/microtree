/* NAME : mtpath.y
 * AUTH : T.Kato
 * VER  : 0.0.6
 * COPY : (C) T.Kato
 * DESC : bisonでMTPathの構文解析クラスを生成する。
 *        コンパイル：
 *        bison -o mtpath.yacc.cpp -d mtpath.y
 */

%{

#include <cassert>
#include <iostream>
#include <stdlib.h>
#include <string.h>
#include "basetypes.h"
#include "mttypes.h"

#undef yyFlexLexer
#define yyFlexLexer yymtpFlexLexer
#include <FlexLexer.h>

char* concat_text(char*, char*);

#define YYPARSE_PARAM  param
#define YYLEX_PARAM  param
#define yyerror(msg)  yymtp_error(msg, param)

int yymtpFlexLexer::yywrap() {
	return 1;
}

namespace yymtp {

// yyparseの引数
struct yyparse_param {
	// yyparseの戻り値

	yyparse_param(std::istream* str_in) :
		path(0),
		lexer(str_in, 0)
	{}

	// 成功した場合に解析済みのパスを格納する
	mt_path* path;

	// 解析機
	yymtpFlexLexer lexer;
};

inline int yylex(void* param) {
	yyparse_param* p = reinterpret_cast<yyparse_param*>(param);
	assert(p);
	return p->lexer.yylex();
}

static int yymtp_error(char const* str, void* param);

%}

%union {
	char* text;
	mt_hop* hop;
	mt_path* path;
}

%token SEP
%token <text> TEXT
%type <text> text
%type <hop> hop
%type <path> path path_part path_part_sep
%%

path : path_part
	| path_part_sep

path_part : /* empty */
	{
		$$ = new mt_path;
		yyparse_param* p = reinterpret_cast<yyparse_param*>(param);
		assert(p);
		p->path = $$;
	}
	| path_part hop
	{
		assert($1);
		if ($1->get_depth() == 0) {
			// 深さ0で'/'から始まっていないので相対パス
			$1->set_relative(true);
		}
		else {
			assert(0);
		}
		$1->grow_tail($2);
		$$ = $1;
	}
	| path_part_sep hop
	{
		assert($1);
		$1->grow_tail($2);
		$$ = $1;
	}

path_part_sep : path_part SEP
	{
		assert($1);
		if ($1->get_depth() == 0) {
			// 深さ0で'/'から始まるので絶対パス
			$1->set_relative(false);
		}
		$$ = $1;
	}

hop : text
	{
		$$ = new mt_hop($1);
		if (!$$) {
			YYERROR;
		}
	}

text : TEXT
	{
		$$ = $1;
	}

%%
int yymtp_error(char const * str, void* param)
{
	yyparse_param* p = reinterpret_cast<yyparse_param*>(param);
	assert(p);

	std::cerr << "parse error("
		<< p->lexer.lineno()
		<< ") near "
		<< p->lexer.YYText()
		<< " "
		<< str
		<< std::endl;
	return 0;
}

} // namespace yymtp

/* mtpath_parse
 * 引数：
 *  mt_path** path
 *    解析に成功した場合はパスが格納される
 *
 * 戻り値：
 *  result::OK   : 成功
 *  result::FAIL : 失敗
 */
int mtpath_parse(mt_path** path, std::istream* in)
{
	yymtp::yyparse_param param(in);
	param.path = NULL;

	if (yymtp::yyparse(&param) == 0) {
		*path = param.path;
		return result::OK;
	}

	return result::FAIL;
}
