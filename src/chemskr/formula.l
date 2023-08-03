%{

#include "chemskr/Lexer.h"
#ifdef YY_USER_ACTION
#error "YY_USER_ACTION is already defined"
#endif
#define YY_USER_ACTION { Chemskr::lexer.advance(chemskrtext); }
#define CHEMSKR_RTOKEN(x) return Chemskr::lexer.token(chemskrtext, CHEMSKRTOK_##x);

%}

%option 8bit
%option debug
%option nobackup
%option nodefault
%option noinput
%option nounput
%option noyywrap
%option warn

CHEMSKR_ELEMENT			([A-Z][a-z]*)
CHEMSKR_INT				[0-9]+

%%

" "+					{}
"("						{ CHEMSKR_RTOKEN(LPAREN)  }
")"						{ CHEMSKR_RTOKEN(RPAREN)  }
("->"|"→")				{ CHEMSKR_RTOKEN(ARROW)   }
"+"						{ CHEMSKR_RTOKEN(PLUS)    }
{CHEMSKR_ELEMENT}		{ CHEMSKR_RTOKEN(ELEMENT) }
{CHEMSKR_INT}			{ CHEMSKR_RTOKEN(INT)     }
.						{ Chemskr::lexer.badchar(*yytext); }

%%

#undef CHEMSKR_RTOKEN
