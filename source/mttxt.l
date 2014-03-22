/* 
 * DESC : generate yymtFlexLexer class by flex.
          flex -+ -p -o mttxt.lex.cpp mttxt.l
 */
/*
 * (C) 2014 KATO Takeshi
 */

%option prefix="yymt"
%option c++

%{
#include "mttypes.h"

namespace yymt {
#include "mttxt.tab.h"
} // namespace yymt

using namespace yymt;

char* dupli_text(const char* src, size_t length);

%}
%s LITERAL
%%
<INITIAL>"/"	return TAGMARK;
<INITIAL>"//"	return CTLMARK;
<INITIAL>=	return KEYOP;
<INITIAL>;	return TERMINAL;
<INITIAL>\{	return OPEN_BRANCH;
<INITIAL>\}	return CLOSE_BRANCH;
<INITIAL>[^\/=;\{\}\" \t\n]+	{
	yylval.text = dupli_text(yytext, yyleng);
	return TEXT;
}
<INITIAL>[ \t]+	;
<INITIAL>\n	yylineno++;
<INITIAL>\"	BEGIN LITERAL;

<LITERAL>[^\\"\n]+	{
	yylval.text = dupli_text(yytext, yyleng);
	return LITERAL_PART;
}
<LITERAL>\\[^\n]	{
	yylval.text = new char[2];
	yylval.text[0] = yytext[1];
	yylval.text[1] = '\0';
	return LITERAL_PART;
}
<LITERAL>\n	{
	yylineno++;
	yylval.text = new char[2];
	yylval.text[0] = '\n';
	yylval.text[1] = '\0';
	return LITERAL_PART;
}
<LITERAL>\"	{
	BEGIN INITIAL;
	return CLOSE_LITERAL;
}
%%
