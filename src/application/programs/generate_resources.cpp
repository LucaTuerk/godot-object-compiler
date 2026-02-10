/**************************************************************************/
/* generate_resources.cpp                                                 */
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

#ifdef DEV_BUILD
#include "generate_resources.h"

#include "library/attribute_db.h"
#include "library/core/file_system_utilities.h"
#include "library/core/string_utilities.h"
#include "library/core/string_writer.h"

namespace GodotObjectCompiler {

  Ref<ProgramError> GenerateResources::run(ApplicationContext& p_context) {
    // Generate Program help resources
    Dictionary<ProgramPath, Ref<IProgram>> programs = Programs::instance()->get_programs();
    for (const auto& [path, program] : programs) {
      String file_stem = string_vector_combine(path, "_");
      String file_path = path_concat_ext("resources/help", file_stem, "txt");
      if (!file_exists(file_path)) {
        FileWriter writer(file_path);
        writer.write("No help available");
      }
    }

    // Generate macro help docs
    for (const String& macro : AttributeDB::instance()->get_all_macros()) {
      String doc_file = path_concat_ext("./resources/doc", macro, "txt");
      ensure_file_exists(doc_file, "No documentation available");

      Vector<Ref<IAttributeParameterType>> params = AttributeDB::instance()->get_parameters_for_macro(macro);

      for (const auto& param : params) {
        auto doc_file = path_concat_ext("resources/doc", param->get_type(), "txt");
        auto doc_dir = path_concat("resources/doc", param->get_type());
        create_dir_recursive(doc_dir);
        ensure_file_exists(doc_file, "No documentation available");

        for (const auto& value_name : param->get_value_names()) {
          auto value_doc_path = path_concat_ext(doc_dir, value_name, "txt");
          ensure_file_exists(value_doc_path, "No documentation available");
        }
      }
    }

    return ProgramError::OK;
  }

}
#endif