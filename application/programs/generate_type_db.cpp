
#include "generate_type_db.h"

#include "../../library/type_db.h"
#include "application/application_context.h"
#include "library/core/config.h"
#include "library/core/helpers.h"
#include "library/parser/parser.h"
#include "library/tree/syntax/class.h"
#include "library/tree/syntax/enum.h"
#include "library/tree/syntax/namespace.h"

namespace GodotObjectCompiler {

  int GenerateTypeDB::run(ApplicationContext& context) {
    TypeDB* type_db = TypeDB::instance();
    type_db->set_cache_directory(context.cache_root);
    TreeSitterParser parser;

    Config times;
    auto time_path = path_concat(context.cache_root, "last_modified_times.goct");
    if (file_exists(time_path)) {
      times.read_from_file(time_path);
    }

    for (const String& include_path : context.include_paths) {
      for (const String& file : directory_files_recursive(include_path)) {
        if (!string_suffix(file, ".h")) {
          continue;
        }

        if (string_contains(file, "thirdparty") || string_contains(file, ".gen.h") ||
            string_contains(file, ".generated.h") || string_contains(file, "godot/platform") ||
            string_contains(file, "godot/drivers")) {
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

        Ref<Node> generated = parser.parse_file(file);
        Ref<Namespace> ns = generated->as<Namespace>();

        if (ns) {
          Vector<Ref<Class>> classes = ns->classes_recursive();
          for (Ref<Class> cls : classes) {
            type_db->save_type_data(cls);
          }

          Vector<Ref<Enum>> enums = ns->enums_recursive();
          for (Ref<Enum> e : enums) {
            type_db->save_type_data(e);
          }
        }
      }
    }
    return 0;
  }

}  // namespace GodotObjectCompiler