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

using leng_t = int;

extern FILE *chemskrin;
extern char *chemskrtext;
extern yy_size_t chemskrleng;
extern int chemskr_flex_debug;

namespace Chemskr {
	class Parser;

	class Lexer {
		private:
			Parser *parser;
			yy_size_t *leng;
			ASTNode **lval;

		public:
			ASTLocation location {0, 1};
			std::string line;
			yy_size_t lastYylength = 0;
			std::unordered_map<int, std::string> lines;
			bool failed = false;
			std::vector<std::pair<std::string, ASTLocation>> errors;

			Lexer(Parser &, yy_size_t &, ASTNode *&);
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
