#pragma once

#include <cstdio>
#include <unordered_map>
#include <vector>

#include "ASTNode.h"

#define CHEMSKRSTYPE_IS_DECLARED
using CHEMSKRSTYPE = Chemskr::ASTNode *;

#ifndef NO_YYPARSE
#include "yyparse.h"
#endif

#ifdef __APPLE__
using yysize = size_t;
#else
using yysize = int;
#endif

extern FILE *chemskrin;
extern char *chemskrtext;
extern yysize chemskrleng;
extern int chemskr_flex_debug;

namespace Chemskr {
	class Parser;

	class Lexer {
		private:
			Parser *parser;
			yysize *leng;
			ASTNode **lval;

		public:
			ASTLocation location {0, 1};
			std::string line;
			yysize lastYylength = 0;
			std::unordered_map<int, std::string> lines;
			bool failed = false;
			std::vector<std::pair<std::string, ASTLocation>> errors;

			Lexer(Parser &, yysize &, ASTNode *&);
			const std::string * filename(int fileno);
			void advance(const char *);
			void newline();
			void badchar(unsigned char);
			int token(const char *, int symbol);
	};

	extern Lexer lexer;
}

int chemskrlex();
int chemskrlex_destroy();
void chemskrerror(const std::string &);
void chemskrerror(const std::string &, const Chemskr::ASTLocation &);
