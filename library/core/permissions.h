#pragma once
#include "core.h"

namespace GodotObjectCompiler {

  class Permissions {
  public:
    static Permissions* instance() {
      static Permissions instance;
      return &instance;
    }

    void clear();
    void add_write_path(const String& path);
    bool is_allowed_write_path(const String& path);
    void ensure_is_allowed_write_path(const String& path);

  private:
    HashSet<String> allowed_write_paths;
  };

}  // namespace GodotObjectCompiler

