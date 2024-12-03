#pragma once

#include <cstdio>
#include <unordered_map>
#include <vector>

#include "ASTNode.h"

#define CHEMSKRSTYPE_IS_DECLARED
using CHEMSKRSTYPE = Chemskr::ASTNode *;

#ifndef NO_YYPARSE
#include "formula.h"
#endif

using yy_size_t = size_t;

using leng_t = yy_size_t;

extern FILE *chemskrin;
extern char *chemskrtext;
extern leng_t chemskrleng;
extern int chemskr_flex_debug;

namespace Chemskr {
	class Parser;

	class Lexer {
		private:
			Parser *parser;
			leng_t *leng;
			ASTNode **lval;

		public:
			ASTLocation location {0, 1};
			std::string line;
			int lastYylength = 0;
			std::unordered_map<int, std::string> lines;
			bool failed = false;
			std::vector<std::pair<std::string, ASTLocation>> errors;

			Lexer(Parser &, leng_t &, ASTNode *&);
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
