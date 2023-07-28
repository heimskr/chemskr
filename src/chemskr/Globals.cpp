#include "chemskr/Lexer.h"
#include "chemskr/Parser.h"

namespace Chemskr {
	Parser parser;
	Lexer lexer(parser, chemskrleng, chemskrlval);
}
