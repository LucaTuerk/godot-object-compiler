
#include "help.h"

#include "library/core/helpers.h"
#include "library/core/resources.h"
#include "library/core/string_writer.h"

namespace GodotObjectCompiler {

  String HelpEntry::get_help_text() const {
    if (path.empty()) {
      return "";
    }
    String file_stem = string_vector_combine(path, "_");
    String res_path = "res://" + path_concat_ext("help", file_stem, "txt");
    if (!Resources::instance()->has_resource(res_path)) {
      return "";
    }

    return Resources::instance()->load_text_resource(res_path);
  }

  Ref<ProgramError> Help::run(ApplicationContext& p_context) {
    Dictionary<ProgramPath, Ref<IProgram>> programs = Programs::instance()->get_programs();

    const Ref<Context> help = node_new<HelpEntry>(ProgramPath(), "Available Programs: ", false);

    for (const auto& [path, program] : programs) {
#ifdef DEV_BUILD
      String file_stem = string_vector_combine(path, "_");
      String file_path = path_concat_ext("resources/help", file_stem, "txt");
      if (!file_exists(file_path)) {
        FileWriter writer(file_path);
        writer.write("No help available");
      }
#endif

      Ref<Context> current = help;
      for (Size i = 0; i < path.size(); ++i) {
        String val = path.at(i);
        if (const Ref<HelpEntry> existing =
                current->find_child<HelpEntry>(0, [val](Ref<HelpEntry> entry) { return entry->name == val; })) {
          if (i == path.size() - 1) {
            existing->path = path;
            existing->is_program = true;
          } else {
            current = existing;
          }
        } else {
          if (i == path.size() - 1) {
            current->build_child<HelpEntry>(path, val, true);
          } else {
            const Ref<HelpEntry> new_entry = current->build_child<HelpEntry>(path, val, false);
            current = new_entry;
          }
        }
      }
    }

    print_ln(help->pretty_print());
    return ProgramError::OK;
  }

  String HelpEntry::to_string() const {
    if (!path.empty()) {
      return format("%s\n%s", name.c_str(), get_help_text().c_str());
    } else {
      return name;
    }
  }

}  // namespace GodotObjectCompiler