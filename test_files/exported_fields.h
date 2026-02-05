// #pragma once
// #include "../.generated/macros.h"
// #include "../library/core/core.h"
// #include "/home/luca/Repositories/godot/core/variant/variant.h"

// clang-format off
// #include "exported_fields.h.generated.h"
// clang-format on
//
// template <typename T>
// class Templated {};

//
// template <typename A, typename B>
// class DoublyTemplated {};

template <typename C>
class Templated2 : public Templated<Templated2>, public C {};

//
// class A {};
//
// class B {};
//
// class C : public A, public B {};

//
// class Untemplated {};
//
// class Untemplated2 : public Untemplated {};
//
// GODOT_CLASS(VirtualClass);
//
// class ExportedFields : Object {
//   GODOT_GENERATED_BODY()
//
//   GODOT_PROPERTY(UsageEditor | UsageArray);
//   bool _field_boolean;
//
//   GODOT_PROPERTY();
//   int8_t _field_int8;
//
//   GODOT_PROPERTY();
//   char _field_char;
//
//   GODOT_PROPERTY();
//   int16_t _field_int16;
//
//   GODOT_PROPERTY();
//   short _field_short;
//
//   GODOT_PROPERTY();
//   int32_t _field_int32;
//
//   GODOT_PROPERTY();
//   int _field_int;
//
//   GODOT_PROPERTY();
//   int64_t _field_int64;
//
//   GODOT_PROPERTY();
//   long _field_long;
//
//   GODOT_PROPERTY();
//   uint8_t _field_uint8;
//
//   GODOT_PROPERTY();
//   unsigned char _field_uchar;
//
//   GODOT_PROPERTY();
//   uint16_t _field_uint16;
//
//   GODOT_PROPERTY();
//   unsigned short _field_ushort;
//
//   GODOT_PROPERTY();
//   uint32_t _field_uint32;
//
//   GODOT_PROPERTY();
//   unsigned int _field_uint;
//
//   GODOT_PROPERTY();
//   uint64_t _field_uint64;
//
//   GODOT_PROPERTY();
//   unsigned long _field_ulong;
//
//   GODOT_PROPERTY();
//   float _field_float;
//
//   GODOT_PROPERTY();
//   double _field_double;
//
//   GODOT_PROPERTY();
//   real_t _field_real_t;
//
//   GODOT_PROPERTY();
//   String _field_string;
//
//   GODOT_PROPERTY();
//   StringName _field_stringName;
//
//   GODOT_PROPERTY();
//   Vector2 _field_vector2;
//
//   GODOT_PROPERTY();
//   Vector2i _field_vector2i;
//
//   GODOT_PROPERTY();
//   Rect2 _field_rect2;
//
//   GODOT_PROPERTY();
//   Rect2i _field_rect2i;
//
//   GODOT_PROPERTY();
//   Transform2D _field_transform2D;
//
//   GODOT_PROPERTY();
//   Vector3 _field_vector3;
//
//   GODOT_PROPERTY();
//   Vector3i _field_vector3i;
//
//   GODOT_PROPERTY();
//   Basis _field_basis;
//
//   GODOT_PROPERTY();
//   Quaternion _field_quaternion;
//
//   GODOT_PROPERTY();
//   Transform3D _fieldTransform3D;
//
//   GODOT_PROPERTY();
//   Vector4 _fieldVector4;
//
//   GODOT_PROPERTY();
//   Vector4i _fieldVector4I;
//
//   GODOT_PROPERTY();
//   Projection _fieldProjection;
//
//   GODOT_PROPERTY();
//   AABB _fieldAabb;
//
//   GODOT_PROPERTY();
//   Color _fieldColor;
//
//   GODOT_PROPERTY();
//   Plane _fieldPlane;
//
//   GODOT_PROPERTY();
//   Callable _fieldCallable;
//
//   GODOT_PROPERTY();
//   Signal _fieldSignal;
//
//   GODOT_PROPERTY();
//   Object* _fieldObject;
//
//   GODOT_PROPERTY();
//   Ref<Resource> _fieldResource;
//
//   GODOT_PROPERTY();
//   Array _arrayField;
//
//   GODOT_PROPERTY();
//   TypedArray<Node*> _fieldTypedArray;
//
//   GODOT_PROPERTY();
//   Node* _fieldNode;
// };