#include "library/commands/create_db.h"
#include "library/core/core.h"
#include "library/core/db.h"
#include "library/tree/namespace.h"

int GodotObjectCompiler::create_db(const Vector<String>& files, const String& db_path, IParser* parser) {
    DB db = DB::init();

    for (const String& file : files) {
        String content = read_file(file);
        if( Node* parsed = parser->parse(content); parsed) {
            db.get_root()->add_child(parsed);
        }
    }

    db.write_to_config(db_path);
    return 0;
}
