#pragma once

#include "library/core/core.h"
#include "library/parser/parser.h"

namespace GodotObjectCompiler {
    int create_db(const Vector<String>& files, const String& db_path, IParser* parser);
}
