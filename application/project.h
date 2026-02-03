#pragma once

#include "../library/core/core.h"
#include "../library/core/reader_writer.h"

namespace GodotObjectCompiler {

  struct Project {
    String project_name;
    Size godot_target_major_version = 4;
    Size godot_target_minor_version = 2;
    String paths_root;
    String paths_generated;
    String paths_cache;
    String paths_goc;
    Vector<String> paths_include;
    Vector<String> paths_ignore;

    void read_from(IStructuredReader* reader);
    void write_to(IStructuredWriter* writer);

    bool read_from_file(const String& path);
    bool write_to_file(const String& path);

   private:

    static Vector<String> from_comma_separated_string(const String& str);
    String to_comma_separated_string(const Vector<String>& str);
  };

}  // namespace GodotObjectCompiler
