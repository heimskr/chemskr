#pragma once

#include <memory>
#include <mutex>
#include <string>

using YY_BUFFER_STATE = struct yy_buffer_state *;

namespace Chemskr {
	class ASTNode;

	class Parser {
		private:
			char *buffer = nullptr;
			YY_BUFFER_STATE bufferState = nullptr;
			std::mutex mutex;

		public:
			ASTNode *root = nullptr;
			int errorCount = 0;

			Parser() = default;

			void in(const std::string &text);
			void debug(bool flex, bool bison) const;
			void parse() const;
			void done();

			[[nodiscard]] const char * getName(int symbol);
			[[nodiscard]] std::string getBuffer() const;

			static std::unique_ptr<ASTNode> parse(const std::string &);
	};

	extern Parser parser;
}
