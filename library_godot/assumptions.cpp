#include "assumptions.h"

namespace GodotObjectCompiler {
  namespace AssumedGodotTypes {

    bool validate_assumptions() {
      bool success = true;

      success &= Object.validate(TypeDB::instance()) == STATE_VALID;
      success &= RefCounted.validate(TypeDB::instance()) == STATE_VALID;
      success &= Resource.validate(TypeDB::instance()) == STATE_VALID;
      success &= Node.validate(TypeDB::instance()) == STATE_VALID;
      // success &= GodotRef.validate(TypeDB::instance()) == STATE_VALID; // TODO: Fix this
      success &= ModuleInitializationLevel.validate(TypeDB::instance()) == STATE_VALID;
      success &= PropertyHintEnum.validate(TypeDB::instance()) == STATE_VALID;
      success &= PropertyUsageFlagsEnum.validate(TypeDB::instance()) == STATE_VALID;
      success &= Variant.validate(TypeDB::instance()) == STATE_VALID;
      success &= VariantTypeEnum.validate(TypeDB::instance()) == STATE_VALID;
      success &= GDREGISTER_CLASS.validate(TypeDB::instance()) == STATE_VALID;
      success &= GDREGISTER_VIRTUAL_CLASS.validate(TypeDB::instance()) == STATE_VALID;
      success &= GDREGISTER_ABSTRACT_CLASS.validate(TypeDB::instance()) == STATE_VALID;
      success &= GDREGISTER_INTERNAL_CLASS.validate(TypeDB::instance()) == STATE_VALID;
      success &= GDREGISTER_RUNTIME_CLASS.validate(TypeDB::instance()) == STATE_VALID;
      success &= GDREGISTER_NATIVE_STRUCT.validate(TypeDB::instance()) == STATE_VALID;

      return success;
    }

  }  // namespace AssumedGodotTypes
}  // namespace GodotObjectCompiler
