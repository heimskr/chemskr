%{
// Parser for a slightly limited subset of Why assembly.
// Some pseudoinstructions and the "&="-type shorthands aren't supported.

#include <cassert>
#include <cstdarg>
#include <initializer_list>

#define NO_YYPARSE
#include "chemskr/Lexer.h"
#include "chemskr/ASTNode.h"
#include "chemskr/Parser.h"

// Disable PVS-Studio warnings about branches that do the same thing.
//-V::1037

template <typename ...Args>
void D(Args && ...args) {
	(void) std::initializer_list<int> {
		((void) delete args, 0)...
	};
}

template <typename T>
const Chemskr::ASTLocation & L(std::initializer_list<const T *> nodes) {
	for (const T *node: nodes)
		if (node)
			return node->location;
	throw std::runtime_error("Couldn't find location: all nodes are null");
}

using AN = Chemskr::ASTNode;

#pragma GCC diagnostic push
#pragma GCC diagnostic ignored "-Wunused-function"
%}

%debug
%defines
%define parse.error verbose
%token-table
%verbose

%define api.prefix {chemskr}

%initial-action {
	Chemskr::parser.root = new Chemskr::ASTNode(Chemskr::parser, CHEMSKRTOK_ROOT, Chemskr::ASTLocation(0, 0), "");
}

%token CHEMSKRTOK_ROOT
%token CHEMSKRTOK_LPAREN "("
%token CHEMSKRTOK_RPAREN ")"
%token CHEMSKRTOK_ARROW "->"
%token CHEMSKRTOK_PLUS "+"
%token CHEMSKRTOK_ELEMENT
%token CHEMSKRTOK_INT
%token CHEMSKR_LIST
%token CHEMSKR_SUM

%start start

%%

start: top { $$ = Chemskr::parser.root->adopt($1); };

top: list | equation;

equation: sum "->" sum { $$ = $2->adopt({$1, $3}); };

sum: sum "+" list { $$ = $1->adopt($3); D($2); }
   | list { $$ = (new AN(Chemskr::parser, CHEMSKR_SUM))->locate($1)->adopt($1); };

element: CHEMSKRTOK_ELEMENT;

number: CHEMSKRTOK_INT;

list: list expr { $$ = $1->adopt($2); }
    | expr { $$ = (new AN(Chemskr::parser, CHEMSKR_LIST))->locate($1)->adopt($1); };

expr: "(" list ")" { $$ = $2; D($1, $3); }
    | number "(" list ")" { $$ = $1->adopt($3); D($2, $4); }
    | "(" list ")" number { $$ = $4->adopt($2); D($1, $3); }
    | element number { $$ = $1->adopt($2); }
    | element;

%%

#pragma GCC diagnostic pop

const char * Chemskr::Parser::getName(int symbol) {
	return yytname[YYTRANSLATE(symbol)];
}
