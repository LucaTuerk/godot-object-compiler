#pragma once

#include "library/core/core.h"
#include "library/core/db.h"
#include "library/generator/generator.h"

namespace GodotObjectCompiler {
    int create_headers(const Vector<String>& files, const String& generated_path, const String& db_path, IGenerator* generator);
    int create_header(const String& file, const String& generated_path, const DB& db, IGenerator* generator);
}
