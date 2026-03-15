/**************************************************************************/
/* godot_attributes.h                                                     */
/*                        ___  ___  ___   ___ _____                       */
/*                       / __|/ _ \|   \ / _ \_   _|                      */
/*                      | (_ | (_) | |) | (_) || |                        */
/*                       \___|\___/|___/ \___/ |_|                        */
/*   ___  ___    _ ___ ___ _____    ___ ___  __  __ ___ ___ _    ___ ___  */
/*  / _ \| _ )_ | | __/ __|_   _|  / __/ _ \|  \/  | _ \_ _| |  | __| _ \ */
/* | (_) | _ \ || | _| (__  | |   | (_| (_) | |\/| |  _/| || |__| _||   / */
/*  \___/|___/\__/|___\___| |_|    \___\___/|_|  |_|_| |___|____|___|_|_\ */
/*                                                                        */
/*              This file is part of Godot Object Compiler                */
/*                  Copyright (c) 2026 Luca Ian Tuerk                     */
/**************************************************************************/
/*                            MIT LICENCE                                 */
/* Permission is hereby granted, free of charge, to any person obtaining  */
/* a copy of this software and associated documentation files (the        */
/* "Software"), to deal in the Software without restriction, including    */
/* without limitation the rights to use, copy, modify, merge, publish,    */
/* distribute, sublicense, and/or sell copies of the Software, and to     */
/* permit persons to whom the Software is furnished to do so, subject to  */
/* the following conditions:                                              */
/*                                                                        */
/* The above copyright notice and this permission notice shall be         */
/* included in all copies or substantial portions of the Software.        */
/*                                                                        */
/* THE SOFTWARE IS PROVIDED "AS IS", WITHOUT WARRANTY OF ANY KIND,        */
/* EXPRESS OR IMPLIED, INCLUDING BUT NOT LIMITED TO THE WARRANTIES OF     */
/* MERCHANTABILITY, FITNESS FOR A PARTICULAR PURPOSE AND NONINFRINGEMENT. */
/* IN NO EVENT SHALL THE AUTHORS OR COPYRIGHT HOLDERS BE LIABLE FOR ANY   */
/* CLAIM, DAMAGES OR OTHER LIABILITY, WHETHER IN AN ACTION OF CONTRACT,   */
/* TORT OR OTHERWISE, ARISING FROM, OUT OF OR IN CONNECTION WITH THE      */
/* SOFTWARE OR THE USE OR OTHER DEALINGS IN THE SOFTWARE.                 */
/**************************************************************************/
#pragma once
#include "enum_generator_options.h"
#include "godot_class_type.h"
#include "godot_custom_property_bind.h"
#include "godot_module_init_level.h"
#include "godot_property_hint.h"
#include "godot_property_usage_flags.h"
#include "godot_rpc.h"
#include "godot_variant_type.h"
#include "godot_virtual.h"
#include "library/attribute_db.h"
#include "library/tree/output/output.h"
#include "library/tree/syntax/attribute.h"
#include "library/tree/syntax/attributes.h"
#include "library/tree/syntax/enum.h"
#include "property_access_specifier.h"
#include "property_generator_options.h"
#include "string_literal_parameter_type.h"

namespace GodotObjectCompiler {

class GodotAttributeWithParams : public Attribute {
	ATTRIBUTE_TYPE(GodotAttributeWithParams, Node, NONE)
	Ref<IAttributeArgumentParser> get_argument_parser() override;

	LAZY(GodotAttributeWithParams, Ref<Arguments>, arguments)
};

class GodotGeneratedBodyAttribute : public GeneratedBodyAttribute {
	ATTRIBUTE_TYPE(GodotGeneratedBodyAttribute, Class, CONTAINING)
	ATTRIBUTE_DEFAULT_MACRO(GODOT_GENERATED_BODY)
};

class GodotGeneratedGlobalAttribute : public GeneratedGlobalAttribute {
	ATTRIBUTE_TYPE(GodotGeneratedGlobalAttribute, Namespace, CONTAINING)
	ATTRIBUTE_DEFAULT_MACRO(GODOT_GENERATED_GLOBAL)
};

class GodotClassAttribute : public GodotAttributeWithParams {
	ATTRIBUTE_TYPE(GodotClassAttribute, Class, NEXT)
	ATTRIBUTE_DEFAULT_MACRO(GODOT_CLASS)

	ATTRIBUTE_REGISTER_PARAMETERS(GodotClassType)
	ATTRIBUTE_REGISTER_PARAMETERS(GodotModuleInitializationLevel)
};

class GodotEnumAttribute : public GodotAttributeWithParams {
	ATTRIBUTE_TYPE(GodotEnumAttribute, Enum, NEXT)
	ATTRIBUTE_DEFAULT_MACRO(GODOT_ENUM)

	ATTRIBUTE_REGISTER_PARAMETERS(EnumGeneratorOptions)
};

class GodotPropertyAttribute : public GodotAttributeWithParams {
	NODE_TYPE(GodotPropertyAttribute)
	ATTRIBUTE_DEFAULT_MACRO(GODOT_PROPERTY)

public:
	Ref<Field> TargetField();
	Opt<GodotCustomPropertyBind> CustomBind();

protected:
	bool _verify_target_class(Ref<Node> p_resolved) const override;
	Target _get_target() const override;

private:
	ATTRIBUTE_REGISTER_PARAMETERS(StringLiteral)
	ATTRIBUTE_REGISTER_PARAMETERS(GodotCustomPropertyGetSet)
	ATTRIBUTE_REGISTER_PARAMETERS(GodotVariantType)
	ATTRIBUTE_REGISTER_PARAMETERS(GodotPropertyHint)
	ATTRIBUTE_REGISTER_PARAMETERS(GodotPropertyUsageFlags)
	ATTRIBUTE_REGISTER_PARAMETERS(PropertyGetAccessSpecifier)
	ATTRIBUTE_REGISTER_PARAMETERS(PropertySetAccessSpecifier)
	ATTRIBUTE_REGISTER_PARAMETERS(PropertyGeneratorOptions)
};

class GodotFunctionAttribute : public GodotAttributeWithParams {
	ATTRIBUTE_TYPE(GodotFunctionAttribute, Function, NEXT)
	ATTRIBUTE_DEFAULT_MACRO(GODOT_FUNCTION)

	ATTRIBUTE_REGISTER_PARAMETERS(StringLiteral)
	ATTRIBUTE_REGISTER_PARAMETERS(GodotVirtual);
	ATTRIBUTE_REGISTER_PARAMETERS(GodotRpcMode);
	ATTRIBUTE_REGISTER_PARAMETERS(GodotRpcSync);
	ATTRIBUTE_REGISTER_PARAMETERS(GodotRpcTransferMode);
	ATTRIBUTE_REGISTER_PARAMETERS(GodotRpcChannel);
};

class GodotSignalAttribute : public GodotAttributeWithParams {
	ATTRIBUTE_TYPE(GodotSignalAttribute, Function, NEXT)
	ATTRIBUTE_DEFAULT_MACRO(GODOT_SIGNAL)

	ATTRIBUTE_REGISTER_PARAMETERS(StringLiteral)
};

class GodotPropertyCategoryAttribute : public GodotAttributeWithParams {
	ATTRIBUTE_TYPE(GodotPropertyCategoryAttribute, Node, NONE)
	ATTRIBUTE_DEFAULT_MACRO(GODOT_CATEGORY)

	ATTRIBUTE_REGISTER_PARAMETERS(StringLiteral)

	LAZY(GodotPropertyCategoryAttribute, String, literal_content);
};

class GodotPropertyGroupAttribute : public GodotAttributeWithParams {
	ATTRIBUTE_TYPE(GodotPropertyGroupAttribute, Node, NONE)
	ATTRIBUTE_DEFAULT_MACRO(GODOT_GROUP)

	ATTRIBUTE_REGISTER_PARAMETERS(StringLiteral)

	LAZY(GodotPropertyGroupAttribute, String, literal_content);
};

class GodotPropertySubgroupAttribute : public GodotAttributeWithParams {
	ATTRIBUTE_TYPE(GodotPropertySubgroupAttribute, Node, NONE)
	ATTRIBUTE_DEFAULT_MACRO(GODOT_SUBGROUP)

	ATTRIBUTE_REGISTER_PARAMETERS(StringLiteral)

	LAZY(GodotPropertySubgroupAttribute, String, literal_content);
};

} // namespace GodotObjectCompiler
