/* 
 * DESC : generate yymtpFlexLexer class by flexa.
 *        flex -+ -p -o mtpath.lex.cpp mtpath.l
 */
/*
 * (C) 2014 KATO Takeshi
 */

%option prefix="yymtp"
%option c++

%{
#include "mttypes.h"

namespace yymtp {
#include "mtpath.tab.h"
} //namespace yymtp

using namespace yymtp;

char* dupli_text(const char* src, size_t length);

%}
%s LITERAL
%%
<INITIAL>"/"	return SEP;
<INITIAL>[^/]* {
	yylval.text = dupli_text(yytext, yyleng);
	return TEXT;
}
%%
