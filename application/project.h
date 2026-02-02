//
// Created by luca on 02.02.26.
//

#ifndef GODOT_OBJECT_COMPILER_PROJECT_H
#define GODOT_OBJECT_COMPILER_PROJECT_H
#include "../library/core/core.h"
#include "../library/core/reader_writer.h"

namespace GodotObjectCompiler {

  struct Project {
    String project_name;
    Size godot_target_major_version;
    Size godot_target_minor_version;
    String paths_root;
    String paths_generated;
    String paths_cache;
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

#endif  // GODOT_OBJECT_COMPILER_PROJECT_H
