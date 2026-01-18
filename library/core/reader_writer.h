#pragma once
#include "core.h"

namespace GodotObjectCompiler {

	class IWriter {
		public:
			virtual ~IWriter() {}

			template<typename K, typename V>
			void write(const K& key, const V& value);

			template<typename S>
			void write_to_section(const S& section);

			virtual void write_to_file(const String& path) = 0;

		protected:
			virtual void _write_to_section(const String& section) = 0;
			virtual void _write(const String& key, const String& value) = 0;
	};

	class IReader {
		public:
			virtual ~IReader() {}

			template<typename K, typename V>
			V read(const K& key);

			template<typename S>
			void read_from_section(const S& section);

			virtual void read_from_file(const String& path) = 0;

		protected:
			virtual void _read_from_section(const String& section) = 0;
			virtual String _read(const String& key) = 0;
	};

	template <typename K, typename V>
	void IWriter::write(const K &key, const V &value) {
		std::stringstream key_str, value_str;
		key_str << key;
		value_str << value;

		_write(key_str.str(), value_str.str());
	}

	template <typename S>
	void IWriter::write_to_section(const S &section) {
		std::stringstream section_str;
		section_str << section;

		_write_to_section(section_str.str());
	}

	template <typename K, typename V>
	V IReader::read(const K &key) {
		std::stringstream key_str;
		key_str << key;

		std::istringstream ss(_read(key_str.str()));
		V result;
		ss >> result;

		return result;
	}

	template <typename S>
	void IReader::read_from_section(const S &section) {
		std::stringstream section_str;
		section_str << section;

		_read_from_section(section_str.str());
	}

} //namespace GodotObjectCompiler