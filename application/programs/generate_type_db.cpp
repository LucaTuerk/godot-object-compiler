/**************************************************************************/
/* generate_type_db.cpp                                                   */
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

#include "generate_type_db.h"

#include "application/application_context.h"
#include "library/core/config.h"
#include "library/core/file_system_utilities.h"
#include "library/core/string_utilities.h"
#include "library/parser/parser.h"
#include "library/tree/syntax/class.h"
#include "library/tree/syntax/define.h"
#include "library/tree/syntax/enum.h"
#include "library/tree/syntax/namespace.h"
#include "library/tree/syntax/struct.h"
#include "library/type_db.h"

namespace GodotObjectCompiler {

  Ref<ProgramError> GenerateTypeDB::run(ApplicationContext& p_context) {
    TreeSitterParser parser;

    Config times;
    auto time_path = path_concat(p_context.paths_cache, "last_modified_times.goct");
    if (file_exists(time_path)) {
      times.read_from_file(time_path);
    }

    for (const String& include_path : p_context.paths_include) {
      for (const String& file : directory_files_recursive(include_path)) {
        if (!string_suffix(file, ".h") && !string_suffix(file, ".hpp")) {
          continue;
        }

        if (string_contains(file, "thirdparty") || string_contains(file, ".gen.h") ||
            string_contains(file, ".generated.h") || string_contains(file, "godot/platform") ||
            string_contains(file, "godot/drivers") || string_contains(file, "godot/tests")) {
          // make this configurable
          continue;
        }

        Size current_modified = file_write_time(file);
        if (times.has_config_value(file)) {
          Size last_modified = times.read<String, Size>(file);

          if (last_modified == current_modified) {
            continue;
          }
        }

        times.write<String, Size>(file, current_modified);
        times.write_to_file(time_path);

        Ref<Namespace> global_namespace = node_new<Namespace>();
        Ref<ParserError> error = parser.parse_file(file, global_namespace);

        if (error != ParserError::OK) {
          continue;
        }

        if (global_namespace) {
          Vector<Ref<NamedContext>> found =
              global_namespace->find_children<NamedContext>(true, [](Ref<NamedContext> node) {
                return node->is<Class>() || node->is<Struct>() || node->is<Enum>() || node->is<Define>();
              });

          for (const Ref<NamedContext>& node : found) {
            TypeDB::instance()->save_type_data(node);
          }
        }
      }
    }

    return ProgramError::OK;
  }

}