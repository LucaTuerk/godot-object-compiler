#pragma once
#define EXPORT(...) GODOT_PROPERTY(__VA_ARGS__)
#define EXPORT_TOOL_BUTTON(name, ...) GODOT_PROPERTY(HintToolButton(name) __VA_ARGS__)

#include "../.generated/macros.h"
#include "/home/luca/Repositories/godot/core/io/resource.h"
#include "/home/luca/Repositories/godot/core/object/class_db.h"
#include "/home/luca/Repositories/godot/core/object/object.h"
#include "/home/luca/Repositories/godot/core/variant/variant.h"
#include "/home/luca/Repositories/godot/scene/3d/camera_3d.h"
#include "test2.h"

// clang-format off
#include "../.generated/simple_class_header.generated.h"
// clang-format on
//

namespace TestNamespace {

  GODOT_CLASS(LevelEditor, VirtualClass);

  class A : public Camera3D {
    GODOT_GENERATED_BODY();

   public:

    GODOT_ENUM();

    enum Test { DAS, IST, EIN, TEST };

    GODOT_ENUM();

    enum Test2 { TEST2_DAS, TEST2_IST, TEST2_NOCH, TEST2_EIN, TEST2_TEST };

    GODOT_PROPERTY(HintDir());
    int property1;

    GODOT_PROPERTY(HintRange(), UsageCheckable);
    int property2;

    GODOT_PROPERTY();
    Camera3D* parent;
  };

}  // namespace TestNamespace

GODOT_CLASS(LevelEditor);

class Huhu : Node3D {
  GODOT_GENERATED_BODY();

  GODOT_PROPERTY();
  float speed;

  GODOT_PROPERTY(HintToolButton("Test"));
  Callable callable;

  GODOT_PROPERTY(HintToolButton("Hallo Welt!"));
  Node3D* node_prop_faulty;

  GODOT_PROPERTY(HintNodeType("Node2D,-CharacterBody2D"));
  Node2D* node_prop;

  GODOT_PROPERTY();
  CharacterBody3D* character_body_3d;

  GODOT_PROPERTY();
  Ref<Texture2D> texture;

  GODOT_PROPERTY();
  TypedArray<Ref<Texture2D>> texture_array;

  GODOT_PROPERTY();
  TypedDictionary<String, Ref<Texture2D>> texture_dictionary;
};

GODOT_CLASS(LevelCore);

class B : public Node2D {
  GODOT_GENERATED_BODY();

 public:

  GODOT_ENUM();

  enum Test { DAS, IST, EIN, TEST };

  GODOT_PROPERTY();
  float float_property1;
};

GODOT_GENERATED_GLOBAL();

// TEst
