/* NAME : mttxt.y
 * AUTH : T.Kato
 * VER  : 0.0.8
 * COPY : (C) T.Kato
 * DESC : bisonでMTの構文解析クラスを生成する。
 *        コンパイル：
 *        bison -o mttxt.yacc.cpp -d mttxt.y
 */

%{

#include <cassert>
#include <iostream>
#include <stdlib.h>
#include <string.h>
#include "basetypes.h"
#include "mttypes.h"

#undef yyFlexLexer
#define yyFlexLexer yymtFlexLexer
#include <FlexLexer.h>

char* concat_text(char*, char*);

#define YYPARSE_PARAM  param
#define YYLEX_PARAM  param
#define yyerror(msg)  yymt_error(msg, param)

int yymtFlexLexer::yywrap() {
	return 1;
}

namespace yymt {

// yyparseの引数
struct yyparse_param {
	// yyparseの戻り値

	yyparse_param(std::istream* str_in) :
		root(0),
		lexer(str_in, 0)
	{}

	// 成功した場合は木構造を格納する
	mt_node* root;

//	// 失敗した場合はエラーメッセージを格納する
//	const char* error;

	// 解析機
	yymtFlexLexer lexer;
};

inline static int yylex(void* param) {
	yyparse_param* p = reinterpret_cast<yyparse_param*>(param);
	assert(p);

	return p->lexer.yylex();
}

static int yymt_error(char const* str, void* param);

%}

%union {
	char* text;
	mt_node* node;
	mt_elem* element;
	mt_key* key;
}

%token TAGMARK CTLMARK KEYOP TERMINAL CLOSE_LITERAL
%token OPEN_BRANCH CLOSE_BRANCH
%token <text> TEXT LITERAL_PART
%type <text> text literal
%type <node> root light_node heavy_node node
%type <element> element
%type <key> key
%%

root
	: /* empty */
	{
		$$ = new mt_node();
		yyparse_param* p = reinterpret_cast<yyparse_param*>(param);
		assert(p);
		p->root = $$;
	}
	| root element
	{
		$1->grow_elemtail($2);
		$$ = $1;
	}

element
	: node
	{
		$$ = new mt_elem($1);
	}
	| text
	{
		$$ = new mt_elem($1);
	}

node
	: light_node TERMINAL
	{
		$$ = $1;
	}
	| heavy_node CLOSE_BRANCH
	{
		$$ = $1;
	}

heavy_node
	: light_node OPEN_BRANCH
	{
		$$ = $1;
	}
	| heavy_node element
	{
		$1->grow_elemtail($2);
		$$ = $1;
	}

light_node
	: TAGMARK text
	{
		$$ = new mt_node($2);
	}
	| light_node key
	{
		$1->grow_keytail($2);
		$$ = $1;
	}

key : text KEYOP text
	{
		$$ = new mt_key($1, $3);
	}

text : TEXT
	{
		$$ = $1;
	}
	| literal CLOSE_LITERAL
	{
		$$ = $1;
	}

literal
	: LITERAL_PART
	| literal LITERAL_PART
	{
		$$ = ::concat_text($1, $2);
		delete [] $1;
		delete [] $2;
	}

%%
static int yymt_error(char const * str, void* param)
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

} // namespace yymt

/* mt_parse
 * 引数：
 *  mt_node** root
 *    解析に成功した場合は木構造が格納される
 *  //const char** error
 *  //  解析に失敗した場合はエラーメッセージが格納される
 *
 * 戻り値：
 *  result::OK   : 成功
 *  result::FAIL : 失敗
 */
int mt_parse(mt_node** root, std::istream* in)
{
	yymt::yyparse_param param(in);

	if (yymt::yyparse(&param) == 0) {
		*root = param.root;
		return result::OK;
	}

	return result::FAIL;
}
