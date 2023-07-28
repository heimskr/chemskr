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

CHEMSKR_ELEMENT			("Ac"|"Ag"|"Al"|"Am"|"Ar"|"As"|"At"|"Au"|"B"|"Ba"|"Be"|"Bh"|"Bi"|"Bk"|"Br"|"C"|"Ca"|"Cd"|"Ce"|"Cf"|"Cl"|"Cm"|"Cn"|"Co"|"Cr"|"Cs"|"Cu"|"Db"|"Ds"|"Dy"|"Er"|"Es"|"Eu"|"F"|"Fe"|"Fl"|"Fm"|"Fr"|"Ga"|"Gd"|"Ge"|"H"|"He"|"Hf"|"Hg"|"Ho"|"Hs"|"I"|"In"|"Ir"|"K"|"Kr"|"La"|"Li"|"Lr"|"Lu"|"Lv"|"Mc"|"Md"|"Mg"|"Mn"|"Mo"|"Mt"|"N"|"Na"|"Nb"|"Nd"|"Ne"|"Nh"|"Ni"|"No"|"Np"|"O"|"Og"|"Os"|"P"|"Pa"|"Pb"|"Pd"|"Pm"|"Po"|"Pr"|"Pt"|"Pu"|"Ra"|"Rb"|"Re"|"Rf"|"Rg"|"Rh"|"Rn"|"Ru"|"S"|"Sb"|"Sc"|"Se"|"Sg"|"Si"|"Sm"|"Sn"|"Sr"|"Ta"|"Tb"|"Tc"|"Te"|"Th"|"Ti"|"Tl"|"Tm"|"Ts"|"U"|"V"|"W"|"Xe"|"Y"|"Yb"|"Zn"|"Zr")
CHEMSKR_INT				[0-9]+

%%

" "+					{}
"("						{ CHEMSKR_RTOKEN(LPAREN)  }
")"						{ CHEMSKR_RTOKEN(RPAREN)  }
{CHEMSKR_ELEMENT}		{ CHEMSKR_RTOKEN(ELEMENT) }
{CHEMSKR_INT}			{ CHEMSKR_RTOKEN(INT)     }
.						{ Chemskr::lexer.badchar(*yytext); }

%%

#undef CHEMSKR_RTOKEN
