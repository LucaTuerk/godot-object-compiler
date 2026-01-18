#include "library/core/db.h"
#include "library/core/config.h"
#include "library/tree/node.h"

namespace GodotObjectCompiler {
    DB DB::init(Namespace* root) {
        DB result;
        result._root = root ? root : new Namespace();
        return result;
    }

    DB DB::read_from_file(const String& path) {
        auto _path = path;
        return DB();
    }

    void dump_node(Config& config, Node* node) {
        config.switch_section(node->get_id());
        node->dump_properties(config);

        if( Context* context = dynamic_cast<Context*>(node) ) {
            for( size_t i = 0; i < context->get_child_count(); ++i) {
                dump_node(config, context->get_child(i));
            }
        }
    }

    void DB::write_to_file(const String& path) const {
        auto _path = path;

        Config config;
        dump_node(config, _root);
        config.write_to_file(_path);
    }

    Namespace* DB::get_root() const {
        return _root;
    }
}
