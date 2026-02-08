#pragma once
#include "library/core/core.h"
#include "library/core/reader_writer.h"
#include "library/parser/parser_context.h"
#include "node.h"

namespace GodotObjectCompiler {

  class Error : public Node {
    NODE_TYPE(Error);

    explicit Error(ErrorLevel level, const String& message) : error_level(level), message(message) {}

    ~Error() override;

    String to_string() const override;
    bool copy_to(Ref<Node> other) const override;
    void write_to(IStructuredWriter* writer) override;
    void read_from(IStructuredReader* reader) override;
    void set_handled();

    ErrorLevel error_level;
    String message;

    static inline const Ref<Error> OK = nullptr;

   private:

    bool handled = false;
  };

  class GeneratorError : public Error {
    NODE_TYPE(GeneratorError);

    explicit GeneratorError(ErrorLevel level, const String& message) : Error(level, message) {}

    explicit GeneratorError(ErrorLevel level, const String& generator_name, const String& message, Ref<Node> node);

    static inline const Ref<GeneratorError> OK = nullptr;
  };

  class ParserError : public Error {
    NODE_TYPE(ParserError);

    explicit ParserError(ErrorLevel level, const String& message) : Error(level, message) {}

    explicit ParserError(ErrorLevel level, const Ref<TreeSitterNode>& node, const String& message);

    explicit ParserError(ErrorLevel level, const String& parser_name, const String& message, const String& file_path,
        const String& file_content, Size line, Size column);

    static inline const Ref<ParserError> OK = nullptr;
  };

}  // namespace GodotObjectCompiler
