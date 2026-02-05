#pragma once
#include "generated_assumptions/all.h"
#include "library/core/assumption.h"
#include "library/type_db.h"

namespace GodotObjectCompiler {

  namespace AssumedGodotTypes {

    inline Assumption<AssumeType<Class>> Object{
        {"Object"}, "Assume that a class named \"Object\" exists in the TypeDB."};

    inline Assumption<AssumeType<Class>> RefCounted{
        {"RefCounted"}, "Assume that a class named \"RefCounted\" exists in the TypeDB."};

    inline Assumption<AssumeType<Class>> Resource{
        {"Resource"}, "Assume that a class named \"Resource\" exists in the TypeDB."};

    inline Assumption<AssumeType<Class>> Node{{"Node"}, "Assume that a class named \"Node\" exists in the TypeDB."};

    inline Assumption<AssumeType<Class>> GodotRef{
        {"Ref", 1}, "Assume that a class named \"Ref\" with 1 template parameter exists in the TypeDB."};

    inline Assumption<AssumeType<Class>> Array{{"Array"}, "Assume that a class named \"Array\" exists in the TypeDB."};

    inline Assumption<AssumeType<Class>> TypedArray {
            {"TypedArray", 1}, "Assume that a class named \"TypedArray\" with 1 template parameter exists in the TypeDB."};

    inline Assumption<AssumeType<Class>> Dictionary{{"Dictionary"}, "Assume that a class named \"Dictionary\" exists in the TypeDB."};

    inline Assumption<AssumeType<Class>> TypedDictionary {
            {"TypedDictionary", 2}, "Assume that a class named \"TypedDictionary\" with 2 template parameter exists in the TypeDB."};

    inline Assumption<AssumeType<Enum>> ModuleInitializationLevel{{"ModuleInitializationLevel"},
        "Assume that an enum type of name \"ModuleInitializationLevel\" exists in the TypeDB."};

    inline Assumption<AssumeType<Enum>> PropertyHintEnum{
        {"PropertyHint"}, "Assume that an enum type of name \"PropertyHint\" exists in the TypeDB."};

    inline Assumption<AssumeType<Enum>> PropertyUsageFlagsEnum{
        {"PropertyUsageFlags"}, "Assume that an enum type of name \"ModuleInitializationLevel\" exists in the TypeDB."};

    inline Assumption<AssumeType<Class>> Variant{
          {"Variant"}, "Assume that a class named \"Variant\" exists in the TypeDB"};

    inline Assumption<AssumeType<Enum>> VariantTypeEnum{
        {"Variant::Type"}, "Assume that an enum type of name \"Variant::Type\" exists in the TypeDB"};

    inline Assumption<AssumeType<Define>> GDREGISTER_CLASS{
        {"GDREGISTER_CLASS"}, "Assume that a macro of name \"GDREGISTER_CLASS\" exisits in the TypeDB."};

    inline Assumption<AssumeType<Define>> GDREGISTER_VIRTUAL_CLASS{{"GDREGISTER_VIRTUAL_CLASS"},
        "Assume that a macro of name \"GDREGISTER_VIRTUAL_CLASS\" exisits in the TypeDB."};

    inline Assumption<AssumeType<Define>> GDREGISTER_ABSTRACT_CLASS{{"GDREGISTER_ABSTRACT_CLASS"},
        "Assume that a macro of name \"GDREGISTER_ABSTRACT_CLASS\" exisits in the TypeDB."};

    inline Assumption<AssumeType<Define>> GDREGISTER_INTERNAL_CLASS{{"GDREGISTER_INTERNAL_CLASS"},
        "Assume that a macro of name \"GDREGISTER_INTERNAL_CLASS\" exisits in the TypeDB."};

    inline Assumption<AssumeType<Define>> GDREGISTER_RUNTIME_CLASS{{"GDREGISTER_RUNTIME_CLASS"},
        "Assume that a macro of name \"GDREGISTER_RUNTIME_CLASS\" exisits in the TypeDB."};

    inline Assumption<AssumeType<Define>> GDREGISTER_NATIVE_STRUCT{{"GDREGISTER_NATIVE_STRUCT"},
        "Assume that a macro of name \"GDREGISTER_NATIVE_STRUCT\" exisits in the TypeDB."};

    bool validate_assumptions();
  }  // namespace GodotTypeAssumptions

}  // namespace GodotObjectCompiler