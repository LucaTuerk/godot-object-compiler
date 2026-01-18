#pragma once

#include "library/core/io/reader_writer.h"
#include "library/core/core.h"
#include <sstream>
#include <string>

namespace GodotObjectCompiler {
    class Config : public IWriter, public IReader {
        public:
            Config();
            ~Config() = default;

            bool has_config_value(const String& key);

		protected:
			void _write_to_section(const String &section) override;
    		void _write(const String &key, const String &value) override;
    		void _read_from_section(const String &section) override;
			String _read(const String &key) override;

		public:
    		void write_to_file(const String& path);
			void read_from_file(const String &path) override;

		protected:

		private:
            String current_section;
            Dictionary<String, Dictionary<String, String>> config_values;
    };

}
