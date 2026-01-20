#include "main.h"

#include "library/core/core.h"
#include "library/core/db.h"
#include "library/core/io/config.h"
#include "library/parser/parser.h"
#include "library/tree/all.h"
#include "library/writer/nodes/output.h"
#include "library/writer/nodes/string_writer.h"

using namespace GodotObjectCompiler;

int main() {
  // IParser* parser = new TreeSitterParser();
  //
  // String content = read_file(
  //     "/home/luca/Repositories/godot/editor/gui/editor_quick_open_dialog.h");
  // // String content =
  // // read_file("/home/luca/Repositories/godot-object-compiler/test_files/simple_class_header.h");
  // Namespace* ns = (Namespace*)parser->parse(content);
  //
  // DB db = DB::init(ns);
  // db.write_to_config("db.txt");
  //
  // Class* cl = ns->find_decendant<Class>();
  //
  // print_ln(cl->clone()->pretty_print());
  // print_ln(cl->qualified_name());
  //
  // //
  // // if (cl != nullptr) {
  // // 	Vector<Namespace*> namespaces = cl->namespaces();
  // // 	String name = cl->name();
  // // 	String qualified = cl->qualified_name();
  // // 	Vector<Function*> functions = cl->functions();
  // // 	Vector<Field*> fields = cl->fields();
  // // 	Vector<Field*> fields2 = cl->fields();
  // //
  // // 	for (auto field : functions) {
  // // 		print_ln(field->qualified_name());
  // // 	}
  // // }
  //
  // print_ln(ns->pretty_print());

  StringWriter writer = StringWriter();

  using namespace Writer;
  auto test = Braces({
    Indent(2, {
      Lines({
        Text("Hallo Welt!"),
        Indent(4,{
          Lines({
            FuncCall("main_func", {Param("a"), FuncCall("main_func", {Param("a"), Param("x"), Param("&p")}), Param("&p")}),
            StringLiteral("This is a StringLiteral"),
            Chevrons({
              Text("int"),
            }),
            NoSep(
              {
              Text("call_func"),
              Brackets({
              Writer::Params({
                Spaces({Text("int"), Text("a")}),
                Spaces({Text("int"), Text("b")}),
              }),
            }),
              Semicolon()}),
          })
        })
      })
    })
  });

  test->get_output(&writer);
  print_ln(writer.get_string());

  return 0;
};
