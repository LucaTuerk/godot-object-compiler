#pragma once

#include <sstream>
#include <string>

#include "library/core/core.h"
#include "reader_writer.h"

namespace GodotObjectCompiler {

  class Config : public IStructuredWriter, public IStructuredReader {
   public:

    Config();
    ~Config() override = default;

    const Vector<String>& get_sections();

    bool has_config_value(const String& p_key);

    bool write_to_file(const String& p_path) override;

    bool read_from_file(const String& p_path) override;

   protected:

    void _write_to_section(const String& p_section) override;

    void _write(const String& p_key, const String& p_value) override;

    void _read_from_section(const String& p_section) override;

    String _read(const String& p_key) override;

   private:

    Size _find_section_index(const String& p_key);

    Size _current_section;
    Dictionary<String, Size> _section_indices;
    Vector<String> _sections;
    Dictionary<Size, Dictionary<String, String>> config_values;
  };

}  // namespace GodotObjectCompiler
