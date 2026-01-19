#pragma once
#include "library/tree/function.h"

#include <utility>

#include "library/tree/node.h"

namespace GodotObjectCompiler {

	class IGeneratorNode {
		public:
			virtual String generate() const = 0;
	};

	class OutputFile : public Context, public IGeneratorNode {
		NODE_TYPE(OutputFile);
		public:
			OutputFile() = default;
			explicit OutputFile(String path) :
					path(std::move(path)) {}

			String generate() const override;
			String path;
	};

	class Comment : public Node, public IGeneratorNode {
		NODE_TYPE(Comment)
		public:
			Comment() = default;
			explicit Comment(String content) :
					content(std::move(content)) {}

			String generate() const override;
			String content;
	};

	class CodeSnippet : public Node, public IGeneratorNode {
		NODE_TYPE(CodeSnippet)
		public:
			CodeSnippet() = default;
			explicit CodeSnippet(String content) :
					content(std::move(content)) {}
			String generate() const override;
			String content;
	};

	class FunctionDefinition : public Context, public IGeneratorNode {
		NODE_TYPE(FunctionDefinition)
		public:
			FunctionDefinition() = default;
			explicit FunctionDefinition(Function *target) :
						target(target) {}

			String generate() const override;
			Function * target = nullptr;
	};

	class FunctionImplementation : public Context, public IGeneratorNode {
		NODE_TYPE(FunctionDefinition)
		public:
			FunctionImplementation() = default;
			explicit FunctionImplementation(Function *target) :
						target(target) {}

			String generate() const override;
			Function * target = nullptr;
	};

} //namespace GodotObjectCompiler
