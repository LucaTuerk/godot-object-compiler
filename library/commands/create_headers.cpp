#include "library/commands/create_headers.h"

int GodotObjectCompiler::create_headers(const Vector<String>& files,
                                        const String& generated_path,
                                        const String& db_path,
                                        IGenerator* generator) {
  DB db = DB::read_from_config(db_path);

  // Implementation of create_headers function
  auto _files = files;
  auto _generated_path = generated_path;
  auto _db_path = db_path;

  for (const String& file : _files) {
    create_header(file, _generated_path, db, generator);
  }

  return 0;
}

// int GodotObjectCompiler::create_header(const String& file, const String&
// generated_path, const DB& db, IGenerator* generator) {
//     auto _file = file;
//     auto _generated_path = generated_path;
//     auto _root = db.get_root();
//     return 0;
// }
