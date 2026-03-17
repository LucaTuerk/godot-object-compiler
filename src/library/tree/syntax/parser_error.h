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
#include "library/parser/parser_context.h"
#include "node.h"

namespace GodotObjectCompiler {

class Error : public Node {
  NODE_TYPE(Error);

 public:
  explicit Error(ErrorLevel level, const String& message)
      : error_level(level), message(message) {}

  ~Error() override;

  String to_string() const override;
  bool copy_to(const Ref<Node>& p_other) const override;
  void write_to(IStructuredWriter* p_writer) override;
  void read_from(IStructuredReader* p_reader) override;
  void set_handled();

  ErrorLevel error_level;
  String message;

  static inline const Ref<Error> OK = nullptr;

 private:
  bool handled = false;
};

class GeneratorError : public Error {
  NODE_TYPE(GeneratorError);

 public:
  explicit GeneratorError(ErrorLevel level, const String& message)
      : Error(level, message) {}

  explicit GeneratorError(ErrorLevel level, const String& generator_name,
                          const String& message, Ref<Node> node);

  static inline const Ref<GeneratorError> OK = nullptr;
};

class ParserError : public Error {
  NODE_TYPE(ParserError);

 public:
  explicit ParserError(ErrorLevel level, const String& message)
      : Error(level, message) {}

  explicit ParserError(ErrorLevel level, const Ref<TreeSitterNode>& node,
                       const String& message);

  explicit ParserError(ErrorLevel level, const String& parser_name,
                       const String& message, const String& file_path,
                       const String& file_content, Size line, Size column);

  static inline const Ref<ParserError> OK = nullptr;
};

}  // namespace GodotObjectCompiler
