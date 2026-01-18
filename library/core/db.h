#pragma once

#include "library/tree/namespace.h"

namespace GodotObjectCompiler {
    class DB {
        public:
            static DB init(Namespace* root = nullptr);
            static DB read_from_config(const String& path);
            void write_to_config(const String& path) const;
            Namespace* get_root() const;

        private:
            Namespace* _root = nullptr;
    };
}
