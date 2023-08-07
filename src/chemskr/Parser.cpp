#include "chemskr/Chemskr.h"
#include "chemskr/Lexer.h"
#include "chemskr/Parser.h"
#include "chemskr/StringSet.h"

#include <cassert>
#include <cstring>
#include <sstream>

int chemskrwrap() { return 1; }

extern YY_BUFFER_STATE chemskr_scan_buffer(char *, size_t);
extern void chemskr_delete_buffer(YY_BUFFER_STATE);

namespace Chemskr {
	void Parser::in(const std::string &text) {
		errorCount = 0;
		buffer = new char[text.size() + 2];
		std::strncpy(buffer, text.c_str(), text.size() + 1);
		buffer[text.size() + 1] = '\0'; // Input to flex needs two null terminators.
		bufferState = chemskr_scan_buffer(buffer, text.size() + 2);
	}

	void Parser::debug(bool flex, bool bison) const {
		chemskr_flex_debug = int(flex);
		chemskrdebug = int(bison);
	}

	void Parser::parse() const {
		chemskrparse();
	}

	void Parser::done() {
		chemskrlex_destroy();
		delete root;
		delete buffer;
		root = nullptr;
		buffer = nullptr;
		errorCount = 0;
	}

	std::string Parser::getBuffer() const {
		return buffer != nullptr? buffer : "";
	}

	std::unique_ptr<ASTNode> Parser::parse(const std::string &input) {
		std::unique_lock lock(parser.mutex);
		parser.in(input);
		parser.debug(false, false);
		parser.parse();

		if (parser.errorCount == 0) {
			assert(parser.root != nullptr);
			ASTNode *old_root = parser.root;
			chemskrlex_destroy();
			delete parser.buffer;
			parser.root = nullptr;
			parser.buffer = nullptr;
			parser.errorCount = 0;
			return std::unique_ptr<ASTNode>(old_root);
		}

		parser.done();
		throw ParserError("Parsing failed with " + std::to_string(parser.errorCount) + " error" +
			(parser.errorCount == 1? "" : "s"));
	}
}