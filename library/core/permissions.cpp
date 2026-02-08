
#include "permissions.h"

#include "helpers.h"

namespace GodotObjectCompiler {

  void Permissions::clear() { allowed_write_paths.clear(); }

  void Permissions::add_write_path(const String& p_path) {
    String absolute = path_absolute(p_path);
    allowed_write_paths.insert(absolute);
  }

  bool Permissions::is_allowed_write_path(const String& p_path) {
    String absolute = path_absolute(p_path);
    return std::find_if(allowed_write_paths.begin(), allowed_write_paths.end(), [absolute](const String& allowed) {
      return string_prefix(absolute, allowed);
    }) != allowed_write_paths.end();
  }

  void Permissions::ensure_is_allowed_write_path(const String& p_path) {
    PANIC_COND(!is_allowed_write_path(p_path), "Trying to write to \"%s\" but it is not an allowed write path!",
        path_absolute(p_path).c_str())
  }

}  // namespace GodotObjectCompiler