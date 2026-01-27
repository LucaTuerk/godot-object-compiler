#pragma once

#include "/home/luca/Repositories/godot/core/io/resource.h"
#include "/home/luca/Repositories/godot/core/object/class_db.h"
#include "/home/luca/Repositories/godot/core/object/object.h"
#include "/home/luca/Repositories/godot/core/variant/variant.h"
#include "/home/luca/Repositories/godot/scene/3d/camera_3d.h"
#include "macros.h"

// clang-format off
#include "/home/luca/Repositories/godot-object-compiler/test_files/simple_class_header.h.generated.h"
// clang-format on

namespace TestNamespace {

  GODOT_CLASS();

  class A : public Camera3D {
    GODOT_GENERATED_BODY();

   public:

    GODOT_ENUM();

    enum Test { DAS, IST, EIN, TEST };

    GODOT_ENUM();

    enum Test2 { TEST2_DAS, TEST2_IST, TEST2_NOCH, TEST2_EIN, TEST2_TEST };

    GODOT_PROPERTY();
    int property1;

    GODOT_PROPERTY();
    int property2;

    GODOT_PROPERTY();
    Camera3D* parent;
  };

}  // namespace TestNamespace

GODOT_CLASS();

class Hää : public Resource {
  GODOT_GENERATED_BODY();

  GODOT_PROPERTY();
  int property3 = 10;

  GODOT_FUNCTION();
  int test_method(int hello);
};

GODOT_CLASS();

class B : public TestNamespace::A {
  GODOT_GENERATED_BODY();

 public:

  GODOT_ENUM();

  enum Test { DAS, IST, EIN, TEST };

  GODOT_PROPERTY();
  float float_property1;
};

GODOT_GENERATED_GLOBAL();

// COMMENT