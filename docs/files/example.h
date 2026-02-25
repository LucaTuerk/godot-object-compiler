// clang-format off
#include <godot-cpp/classes/node.h>

#include "godot_object_compiler/macros.h"
#include "example.generated.h"

using namespace godot;

GODOT_CLASS();
class ExampleClass : public Node {
	GODOT_GENERATED_BODY();

public:
	GODOT_ENUM();
	enum Regular {
		REGULAR_VALUE_A,
		REGULAR_VALUE_B,
		REGULAR_VALUE_C,
	};

	GODOT_ENUM(EnumFlags);
	enum Flags {
		FLAG_NONE = 0,
		FLAG_VALUE_A = 1,
		FLAG_VALUE_B = 1 << 1,
		FLAG_VALUE_C = 1 << 2,
	};

	GODOT_SIGNAL();
	void something_happened(int p_param);

	GODOT_FUNCTION();
	Ref<Resource> function(Node* p_node);

	GODOT_FUNCTION(ScriptVirtual);
	void virtual_function(Node* p_node);

	GODOT_FUNCTION(Authority, UnreliableOrdered, CallLocal, Channel(1));
	void rpc_function(int p_param);

	GODOT_CATEGORY("Properties");

	GODOT_PROPERTY();
	Ref<Resource> resource;

	GODOT_PROPERTY(PublicGet, PrivateSet);
	TypedDictionary<int, Resource> resources;

	GODOT_PROPERTY(HintRange("0,1,0.1"));
	float range = 0;

	GODOT_GROUP("Enum Properties");
	GODOT_PROPERTY();
	Regular enum_property = REGULAR_VALUE_A;

	GODOT_PROPERTY();
	Flags flags_property = FLAG_NONE;
};
GODOT_GENERATED_GLOBAL();
// clang-format on
