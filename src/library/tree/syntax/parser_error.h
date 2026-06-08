/**************************************************************************/
/* parser_error.h                                                         */
/*                        ___  ___  ___   ___ _____                       */
/*                       / __|/ _ \|   \ / _ \_   _|                      */
/*                      | (_ | (_) | |) | (_) || |                        */
/*                       \___|\___/|___/ \___/ |_|                        */
/*   ___  ___    _ ___ ___ _____    ___ ___  __  __ ___ ___ _    ___ ___  */
/*  / _ \| _ )_ | | __/ __|_   _|  / __/ _ \|  \/  | _ \_ _| |  | __| _ \ */
/* | (_) | _ \ || | _| (__  | |   | (_| (_) | |\/| |  _/| || |__| _||   / */
/*  \___/|___/\__/|___\___| |_|    \___\___/|_|  |_|_| |___|____|___|_|_\ */
/*                                                                        */
/*              This file is part of Godot Object Compiler                */
/*                  Copyright (c) 2026 Luca Ian Tuerk                     */
/**************************************************************************/
/*                            MIT LICENCE                                 */
/* Permission is hereby granted, free of charge, to any person obtaining  */
/* a copy of this software and associated documentation files (the        */
/* "Software"), to deal in the Software without restriction, including    */
/* without limitation the rights to use, copy, modify, merge, publish,    */
/* distribute, sublicense, and/or sell copies of the Software, and to     */
/* permit persons to whom the Software is furnished to do so, subject to  */
/* the following conditions:                                              */
/*                                                                        */
/* The above copyright notice and this permission notice shall be         */
/* included in all copies or substantial portions of the Software.        */
/*                                                                        */
/* THE SOFTWARE IS PROVIDED "AS IS", WITHOUT WARRANTY OF ANY KIND,        */
/* EXPRESS OR IMPLIED, INCLUDING BUT NOT LIMITED TO THE WARRANTIES OF     */
/* MERCHANTABILITY, FITNESS FOR A PARTICULAR PURPOSE AND NONINFRINGEMENT. */
/* IN NO EVENT SHALL THE AUTHORS OR COPYRIGHT HOLDERS BE LIABLE FOR ANY   */
/* CLAIM, DAMAGES OR OTHER LIABILITY, WHETHER IN AN ACTION OF CONTRACT,   */
/* TORT OR OTHERWISE, ARISING FROM, OUT OF OR IN CONNECTION WITH THE      */
/* SOFTWARE OR THE USE OR OTHER DEALINGS IN THE SOFTWARE.                 */
/**************************************************************************/
#pragma once
#include "library/core/core.h"
#include "library/core/reader_writer.h"

#if GOC_TREE_SITTER_PARSER_ENABLED
#include "library/parsers/tree-sitter/parser_context.h"
#endif

#include "node.h"

namespace GodotObjectCompiler
{

    class Error : public Node
    {
        NODE_TYPE(Error);

      public:
        explicit Error(const ErrorLevel level, String message)
            : error_level(level), message(std::move(message))
        {
        }

        ~Error() override;

        String to_string() const override;
        bool copy_to(const Ref<Node>& p_other) const override;
        void write_to(IStructuredWriter* p_writer) override;
        void read_from(IStructuredReader* p_reader) override;
        void set_handled();

        ErrorLevel error_level = ERROR;
        String message;

        static inline const Ref<Error> OK = nullptr;

      private:
        bool handled = false;
    };

    class GeneratorError : public Error
    {
        NODE_TYPE(GeneratorError);

      public:
        explicit GeneratorError(const ErrorLevel level, const String& message)
            : Error(level, message)
        {
        }

        GeneratorError(const Ref<Error>& p_error) : Error(p_error->error_level, p_error->message)
        {
        }

        explicit GeneratorError(
            ErrorLevel level, const String& generator_name, const String& message, Ref<Node> node);

        static inline const Ref<GeneratorError> OK = nullptr;
    };

    class ParserError : public Error
    {
        NODE_TYPE(ParserError);

      public:
        explicit ParserError(const ErrorLevel level, const String& message) : Error(level, message)
        {
        }

#if GOC_TREE_SITTER_PARSER_ENABLED
        explicit ParserError(
            ErrorLevel level, const Ref<TreeSitterNode>& node, const String& message);
#endif

        explicit ParserError(
            ErrorLevel level, const String& parser_name, const String& message,
            const String& file_path, const String& file_content, Size line, Size column);

        static inline const Ref<ParserError> OK = nullptr;
    };

#define ERROR_CAST(type, error)                                                                    \
    (error->is<type>() ? error->as<type>() : node_new<type>(error->error_level, error->message))

#define ERROR(...) return node_new<Error>(ErrorLevel::ERROR, format(__VA_ARGS__))

#define ERROR_COND(condition, ...)                                                                 \
    if (condition) {                                                                               \
        ERROR(__VA_ARGS__);                                                                        \
    }

    template <typename T> Result<T> INodeReader::read_from_file(const String& p_path)
    {
        Result<Node> result = read_from_file(p_path);
        if (result.has_error()) {
            return result.get_error();
        }

        Ref<T> node = result.get_result()->as<T>();
        if (node == nullptr) {
            ERROR(
                "Node read from file has invalid type. Expected %s "
                "but got %s",
                T::get_type_static().c_str(), result.get_result()->get_type().c_str());
        }
        return node;
    }

#define PARSER_ERROR(...) return node_new<ParserError>(ErrorLevel::ERROR, format(__VA_ARGS__))

#define PARSER_ERROR_COND(condition, ...)                                                          \
    do {                                                                                           \
        if (condition) {                                                                           \
            PARSER_ERROR(__VA_ARGS__);                                                             \
        }                                                                                          \
    } while (false)

} // namespace GodotObjectCompiler
