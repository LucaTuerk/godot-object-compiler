#include "main.h"

#include "library/core/core.h"
#include "library/core/db.h"
#include "library/core/io/config.h"
#include "library/parser/parser.h"
#include "library/tree/all.h"

using namespace GodotObjectCompiler;

int main() {
  IParser* parser = new TreeSitterParser();

  String content = read_file(
      "/home/luca/Repositories/godot/editor/gui/editor_quick_open_dialog.h");
  // String content =
  // read_file("/home/luca/Repositories/godot-object-compiler/test_files/simple_class_header.h");
  Namespace* ns = (Namespace*)parser->parse(content);

  DB db = DB::init(ns);
  db.write_to_config("db.txt");

  Class* cl = ns->find_decendant<Class>();

  print_ln(cl->clone()->pretty_print());
  print_ln(cl->qualified_name());

  //
  // if (cl != nullptr) {
  // 	Vector<Namespace*> namespaces = cl->namespaces();
  // 	String name = cl->name();
  // 	String qualified = cl->qualified_name();
  // 	Vector<Function*> functions = cl->functions();
  // 	Vector<Field*> fields = cl->fields();
  // 	Vector<Field*> fields2 = cl->fields();
  //
  // 	for (auto field : functions) {
  // 		print_ln(field->qualified_name());
  // 	}
  // }

  print_ln(ns->pretty_print());

  return 0;
};
