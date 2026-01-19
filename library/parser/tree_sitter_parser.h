#pragma once

#include "library/parser/parser.h"
#include "library/parser/tree_sitter_parser/parser_context.h"
#include "library/tree/all.h"
#include <tree_sitter/api.h>

class TSParser;
class TSTree;
class TSNode;

namespace GodotObjectCompiler {
	class INodeHandler;

    class TreeSitterParser : public IParser {
        public:
            ~TreeSitterParser() = default;

    		Node* parse(const String& input) override;

    		template<typename T>
    		static bool register_handler();

        private:
			static inline Vector<INodeHandler*> _handlers{};
            ParserContext context;

    };

	template <typename T>
	bool TreeSitterParser::register_handler() {
		_handlers.push_back(new T());
		return true;
	}

} //namespace GodotObjectCompiler
