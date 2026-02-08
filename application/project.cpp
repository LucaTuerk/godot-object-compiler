//
// Created by luca on 02.02.26.
//

#include "project.h"

#include "library/core/config.h"
#include "library/core/helpers.h"
#include "library/core/string_writer.h"

namespace GodotObjectCompiler {

  void Project::read_from(IStructuredReader* p_reader) {
    p_reader->read_from_section("Project");
    project_name = p_reader->read<String, String>("Name");

    p_reader->read_from_section("Godot");
    godot_target_major_version = p_reader->read<String, Size>("TargetMajorVersion");
    godot_target_minor_version = p_reader->read<String, Size>("TargetMinorVersion");

    p_reader->read_from_section("Paths");
    paths_root = p_reader->read<String, String>("RootPath");
    paths_goc = p_reader->read<String, String>("GOCPath");
    paths_generated = p_reader->read<String, String>("GeneratedPath");
    paths_cache = p_reader->read<String, String>("CachePath");
    paths_include = from_comma_separated_string(p_reader->read<String, String>("IncludePaths"));
    paths_ignore = from_comma_separated_string(p_reader->read<String, String>("IgnorePaths"));
  }

  void Project::write_to(IStructuredWriter* p_writer) {
    p_writer->write_to_section("Project");
    p_writer->write<String, String>("Name", project_name);

    p_writer->write_to_section("Godot");
    p_writer->write<String, Size>("TargetMajorVersion", godot_target_major_version);
    p_writer->write<String, Size>("TargetMinorVersion", godot_target_minor_version);

    p_writer->write_to_section("Paths");
    p_writer->write<String, String>("RootPath", paths_root);
    p_writer->write<String, String>("GeneratedPath", paths_generated);
    p_writer->write<String, String>("CachePath", paths_cache);
    p_writer->write<String, String>("GOCPath", paths_goc);
    p_writer->write<String, String>("IncludePaths", to_comma_separated_string(paths_include));
    p_writer->write<String, String>("IgnorePaths", to_comma_separated_string(paths_ignore));
  }

  bool Project::read_from_file(const String& p_path) {
    Config config;
    if (!config.read_from_file(p_path)) {
      return false;
    }

    read_from(&config);
    return true;
  }

  bool Project::write_to_file(const String& p_path) {
    Config config;
    write_to(&config);
    return config.write_to_file(p_path);
  }

  Vector<String> Project::from_comma_separated_string(const String& p_str) {
    Vector<String> result = string_split(p_str, ",");
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

  String Project::to_comma_separated_string(const Vector<String>& p_str) {
    StreamWriter writer;
    for (Size i = 0; i < p_str.size(); i++) {
      if (i != 0 && i != p_str.size() - 1) {
        writer.write(",");
      }
      writer.write(string_trim(p_str[i]));
    }
    return writer.get_string();
  }

}  // namespace GodotObjectCompiler