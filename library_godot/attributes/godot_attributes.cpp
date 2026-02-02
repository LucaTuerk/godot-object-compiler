//
// Created by luca on 20.01.26.
//

#include "library_godot/attributes/godot_attributes.h"

#include "library_godot/parsers/godot_attribute_argument_parser.h"

namespace GodotObjectCompiler {

  Ref<IAttributeArgumentParser> GodotAttributeWithParams::get_argument_parser() {
    return make_ref<GodotAttributeArgumentParser>(this->as<Attribute>());
  }

  Ref<Arguments> GodotAttributeWithParams::_arguments_lazy_get() { return find_child<Arguments>(); }

}  // namespace GodotObjectCompiler