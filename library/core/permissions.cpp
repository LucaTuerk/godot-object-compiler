
#include "permissions.h"

#include "helpers.h"

namespace GodotObjectCompiler {

  void Permissions::clear() {
    allowed_write_paths.clear();
  }

  void Permissions::add_write_path(const String& path) {
    String absolute = path_absolute(path);
    allowed_write_paths.insert(absolute);
  }

  bool Permissions::is_allowed_write_path(const String& path) {
    String absolute = path_absolute(path);
    return std::find_if(allowed_write_paths.begin(), allowed_write_paths.end(),
               [absolute](const String& allowed) { return string_prefix(absolute, allowed); }) != allowed_write_paths.end();
  }

  void Permissions::ensure_is_allowed_write_path(const String& path) {
    PANIC_COND(!is_allowed_write_path(path), "Trying to write to \"%s\" but it is not an allowed write path!",
        path_absolute(path).c_str())
  }

}  // namespace GodotObjectCompiler