//
// Created by luca on 02.02.26.
//

#include "project.h"

#include "library/core/config.h"
#include "library/core/helpers.h"
#include "library/core/string_writer.h"

namespace GodotObjectCompiler {

  void Project::read_from(IStructuredReader* reader) {
    reader->read_from_section("Project");
    project_name = reader->read<String, String>("Name");

    reader->read_from_section("Godot");
    godot_target_major_version = reader->read<String, Size>("TargetMajorVersion");
    godot_target_minor_version = reader->read<String, Size>("TargetMinorVersion");

    reader->read_from_section("Paths");
    paths_root = reader->read<String, String>("RootPath");
    paths_goc = reader->read<String, String>("GOCPath");
    paths_generated = reader->read<String, String>("GeneratedPath");
    paths_cache = reader->read<String, String>("CachePath");
    paths_include = from_comma_separated_string(reader->read<String, String>("IncludePaths"));
    paths_ignore = from_comma_separated_string(reader->read<String, String>("IgnorePaths"));
  }

  void Project::write_to(IStructuredWriter* writer) {
    writer->write_to_section("Project");
    writer->write<String, String>("Name", project_name);

    writer->write_to_section("Godot");
    writer->write<String, Size>("TargetMajorVersion", godot_target_major_version);
    writer->write<String, Size>("TargetMinorVersion", godot_target_minor_version);

    writer->write_to_section("Paths");
    writer->write<String, String>("RootPath", paths_root);
    writer->write<String, String>("GeneratedPath", paths_generated);
    writer->write<String, String>("CachePath", paths_cache);
    writer->write<String, String>("GOCPath", paths_goc);
    writer->write<String, String>("IncludePaths", to_comma_separated_string(paths_include));
    writer->write<String, String>("IgnorePaths", to_comma_separated_string(paths_ignore));
  }

  bool Project::read_from_file(const String& path) {
    Config config;
    if (!config.read_from_file(path)) {
      return false;
    }

    read_from(&config);
    return true;
  }

  bool Project::write_to_file(const String& path) {
    Config config;
    write_to(&config);
    return config.write_to_file(path);
  }

  Vector<String> Project::from_comma_separated_string(const String& str) {
    Vector<String> result = string_split(str, ",");
    std::transform(result.begin(), result.end(), result.begin(), &string_trim);
    for (auto itr = result.begin(); itr != result.end();) {
      if (itr->empty()) {
        itr = result.erase(itr);
      } else {
        ++itr;
      }
    }

    return result;
  }

  String Project::to_comma_separated_string(const Vector<String>& str) {
    StreamWriter writer;
    for (Size i = 0; i < str.size(); i++) {
      if (i != 0 && i != str.size() - 1) {
        writer.write(",");
      }
      writer.write(string_trim(str[i]));
    }
    return writer.get_string();
  }

}  // namespace GodotObjectCompiler