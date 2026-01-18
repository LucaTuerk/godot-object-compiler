#include "main.h"
#include "library/core/core.h"
#include "library/core/db.h"
#include "library/core/config.h"
#include "library/parser/parser.h"
#include "library/parser/tree_sitter_parser.h"
#include "library/tree/namespace.h"
#include "library/tree/class.h"
#include "library/tree/function.h"
#include "library/tree/identifier.h"
#include "library/tree/node.h"

using namespace GodotObjectCompiler;

int main() {
    IParser* parser = new TreeSitterParser();

    String content = read_file("/home/luca/Repositories/godot-object-compiler/test_files/simple_class_header.h");
    Namespace* ns = (Namespace*) parser->parse(content);

    DB db = DB::init(ns);
    db.write_to_file("db.txt");
    return 0;
};
