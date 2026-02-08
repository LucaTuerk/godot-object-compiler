#pragma once

#include "library/core/core.h"
#include "library/core/reader_writer.h"

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

    void read_from(IStructuredReader* p_reader);

    void write_to(IStructuredWriter* p_writer);

    bool read_from_file(const String& p_path);

    bool write_to_file(const String& p_path);

   private:

    static Vector<String> from_comma_separated_string(const String& p_str);

    String to_comma_separated_string(const Vector<String>& p_str);
  };

}  // namespace GodotObjectCompiler
