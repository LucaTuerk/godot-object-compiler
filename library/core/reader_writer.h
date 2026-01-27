#pragma once
#include "core.h"

namespace GodotObjectCompiler {

  class IStringWriter {
   public:

    virtual ~IStringWriter() = default;
    virtual void write(const String& value) = 0;

    template <typename T>
    void write_generic(const T& value);

    virtual String get_string() = 0;
    virtual Size current_length() = 0;
  };

  class IStructuredWriter {
   public:

    virtual ~IStructuredWriter() {}

    template <typename K, typename V>
    void write(const K& key, const V& value);

    template <typename S>
    void write_to_section(const S& section);

    virtual void write_to_file(const String& path) = 0;

   protected:

    virtual void _write_to_section(const String& section) = 0;
    virtual void _write(const String& key, const String& value) = 0;
  };

  class IStructuredReader {
   public:

    virtual ~IStructuredReader() {}

    template <typename K, typename V>
    V read(const K& key);

    template <typename S>
    void read_from_section(const S& section);

    virtual void read_from_file(const String& path) = 0;

   protected:

    virtual void _read_from_section(const String& section) = 0;
    virtual String _read(const String& key) = 0;
  };

  template <typename T>
  void IStringWriter::write_generic(const T& value) {
    std::stringstream strstr;
    strstr << value;
    write(strstr.str());
  }

  template <typename K, typename V>
  void IStructuredWriter::write(const K& key, const V& value) {
    std::stringstream key_str, value_str;
    key_str << key;
    value_str << value;

    _write(key_str.str(), value_str.str());
  }

  template <typename S>
  void IStructuredWriter::write_to_section(const S& section) {
    std::stringstream section_str;
    section_str << section;

    _write_to_section(section_str.str());
  }

  template <typename K, typename V>
  V IStructuredReader::read(const K& key) {
    std::stringstream key_str;
    key_str << key;

    std::istringstream ss(_read(key_str.str()));
    V result;
    ss >> result;

    return result;
  }

  template <typename S>
  void IStructuredReader::read_from_section(const S& section) {
    std::stringstream section_str;
    section_str << section;

    _read_from_section(section_str.str());
  }

}  // namespace GodotObjectCompiler