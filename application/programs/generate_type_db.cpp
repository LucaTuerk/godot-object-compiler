
#include "generate_type_db.h"

#include "../../library/type_db.h"
#include "application/application_context.h"
#include "library/core/config.h"
#include "library/core/helpers.h"
#include "library/parser/parser.h"
#include "library/tree/output/output.h"
#include "library/tree/syntax/class.h"
#include "library/tree/syntax/define.h"
#include "library/tree/syntax/enum.h"
#include "library/tree/syntax/namespace.h"

namespace GodotObjectCompiler {

  Ref<ProgramError> GenerateTypeDB::run(ApplicationContext& context) {
    TreeSitterParser parser;

    Config times;
    auto time_path = path_concat(context.paths_cache, "last_modified_times.goct");
    if (file_exists(time_path)) {
      times.read_from_file(time_path);
    }

    for (const String& include_path : context.paths_include) {
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
          Vector<Ref<Class>> classes = global_namespace->classes_recursive();
          for (Ref<Class> cls : classes) {
            TypeDB::instance()->save_type_data(cls);
          }

          Vector<Ref<Enum>> enums = global_namespace->enums_recursive();
          for (Ref<Enum> e : enums) {
            TypeDB::instance()->save_type_data(e);
          }

          Vector<Ref<Define>> defines = global_namespace->find_children<Define>(true);
          for (Ref<Define> define : defines) {
            TypeDB::instance()->save_type_data(define);
          }
        }
      }
    }

    return ProgramError::OK;
  }

}  // namespace GodotObjectCompiler