// clang-format off
#include "parameter_types.h"
#include "library_godot/attributes/godot_class_type.h"
#include "library_godot/attributes/godot_module_init_level.h"
#include "library_godot/attributes/godot_variant_type.h"
#include "library_godot/attributes/godot_property_hint.h"
#include "library_godot/attributes/godot_property_usage_flags.h"
#include "library_godot/attributes/godot_rpc.h"
#include "library_godot/attributes/godot_virtual.h"
namespace GodotObjectCompiler
{
  namespace AssumedParameterValues 
  {
      bool validate_assumptions () 
      {
            bool success = true;
            
            {
                    Ref<GodotClassTypeParameterType> validator = make_ref<GodotClassTypeParameterType>();
                    success &= RegisterCustom.validate(validator.get()) == STATE_VALID;
                    success &= GodotClass.validate(validator.get()) == STATE_VALID;
                    success &= VirtualClass.validate(validator.get()) == STATE_VALID;
                    success &= AbstractClass.validate(validator.get()) == STATE_VALID;
                    success &= InternalClass.validate(validator.get()) == STATE_VALID;
                    success &= RuntimeClass.validate(validator.get()) == STATE_VALID;
            }
            
            {
                    Ref<GodotModuleInitializationLevelParameterType> validator = make_ref<GodotModuleInitializationLevelParameterType>();
                    success &= LevelCore.validate(validator.get()) == STATE_VALID;
                    success &= LevelServers.validate(validator.get()) == STATE_VALID;
                    success &= LevelScene.validate(validator.get()) == STATE_VALID;
                    success &= LevelEditor.validate(validator.get()) == STATE_VALID;
                    success &= LevelMax.validate(validator.get()) == STATE_VALID;
            }
            
            {
                    Ref<GodotVariantTypeParameterType> validator = make_ref<GodotVariantTypeParameterType>();
                    success &= VariantTypeNil.validate(validator.get()) == STATE_VALID;
                    success &= VariantTypeBool.validate(validator.get()) == STATE_VALID;
                    success &= VariantTypeInt.validate(validator.get()) == STATE_VALID;
                    success &= VariantTypeFloat.validate(validator.get()) == STATE_VALID;
                    success &= VariantTypeString.validate(validator.get()) == STATE_VALID;
                    success &= VariantTypeVector2.validate(validator.get()) == STATE_VALID;
                    success &= VariantTypeVector2i.validate(validator.get()) == STATE_VALID;
                    success &= VariantTypeRect2.validate(validator.get()) == STATE_VALID;
                    success &= VariantTypeRect2i.validate(validator.get()) == STATE_VALID;
                    success &= VariantTypeVector3.validate(validator.get()) == STATE_VALID;
                    success &= VariantTypeVector3i.validate(validator.get()) == STATE_VALID;
                    success &= VariantTypeTransform2d.validate(validator.get()) == STATE_VALID;
                    success &= VariantTypeVector4.validate(validator.get()) == STATE_VALID;
                    success &= VariantTypeVector4i.validate(validator.get()) == STATE_VALID;
                    success &= VariantTypePlane.validate(validator.get()) == STATE_VALID;
                    success &= VariantTypeQuaternion.validate(validator.get()) == STATE_VALID;
                    success &= VariantTypeAabb.validate(validator.get()) == STATE_VALID;
                    success &= VariantTypeBasis.validate(validator.get()) == STATE_VALID;
                    success &= VariantTypeTransform3d.validate(validator.get()) == STATE_VALID;
                    success &= VariantTypeProjection.validate(validator.get()) == STATE_VALID;
                    success &= VariantTypeColor.validate(validator.get()) == STATE_VALID;
                    success &= VariantTypeStringName.validate(validator.get()) == STATE_VALID;
                    success &= VariantTypeNodePath.validate(validator.get()) == STATE_VALID;
                    success &= VariantTypeRid.validate(validator.get()) == STATE_VALID;
                    success &= VariantTypeObject.validate(validator.get()) == STATE_VALID;
                    success &= VariantTypeCallable.validate(validator.get()) == STATE_VALID;
                    success &= VariantTypeSignal.validate(validator.get()) == STATE_VALID;
                    success &= VariantTypeDictionary.validate(validator.get()) == STATE_VALID;
                    success &= VariantTypeArray.validate(validator.get()) == STATE_VALID;
                    success &= VariantTypePackedByteArray.validate(validator.get()) == STATE_VALID;
                    success &= VariantTypePackedInt32Array.validate(validator.get()) == STATE_VALID;
                    success &= VariantTypePackedInt64Array.validate(validator.get()) == STATE_VALID;
                    success &= VariantTypePackedFloat32Array.validate(validator.get()) == STATE_VALID;
                    success &= VariantTypePackedFloat64Array.validate(validator.get()) == STATE_VALID;
                    success &= VariantTypePackedStringArray.validate(validator.get()) == STATE_VALID;
                    success &= VariantTypePackedVector2Array.validate(validator.get()) == STATE_VALID;
                    success &= VariantTypePackedVector3Array.validate(validator.get()) == STATE_VALID;
                    success &= VariantTypePackedColorArray.validate(validator.get()) == STATE_VALID;
                    success &= VariantTypePackedVector4Array.validate(validator.get()) == STATE_VALID;
                    success &= VariantTypeVariantMax.validate(validator.get()) == STATE_VALID;
            }
            
            {
                    Ref<GodotPropertyHintParameterType> validator = make_ref<GodotPropertyHintParameterType>();
                    success &= HintNone.validate(validator.get()) == STATE_VALID;
                    success &= HintRange.validate(validator.get()) == STATE_VALID;
                    success &= HintEnum.validate(validator.get()) == STATE_VALID;
                    success &= HintEnumSuggestion.validate(validator.get()) == STATE_VALID;
                    success &= HintExpEasing.validate(validator.get()) == STATE_VALID;
                    success &= HintLink.validate(validator.get()) == STATE_VALID;
                    success &= HintFlags.validate(validator.get()) == STATE_VALID;
                    success &= HintLayers2dRender.validate(validator.get()) == STATE_VALID;
                    success &= HintLayers2dPhysics.validate(validator.get()) == STATE_VALID;
                    success &= HintLayers2dNavigation.validate(validator.get()) == STATE_VALID;
                    success &= HintLayers3dRender.validate(validator.get()) == STATE_VALID;
                    success &= HintLayers3dPhysics.validate(validator.get()) == STATE_VALID;
                    success &= HintLayers3dNavigation.validate(validator.get()) == STATE_VALID;
                    success &= HintLayersAvoidance.validate(validator.get()) == STATE_VALID;
                    success &= HintFile.validate(validator.get()) == STATE_VALID;
                    success &= HintDir.validate(validator.get()) == STATE_VALID;
                    success &= HintGlobalFile.validate(validator.get()) == STATE_VALID;
                    success &= HintGlobalDir.validate(validator.get()) == STATE_VALID;
                    success &= HintResourceType.validate(validator.get()) == STATE_VALID;
                    success &= HintMultilineText.validate(validator.get()) == STATE_VALID;
                    success &= HintExpression.validate(validator.get()) == STATE_VALID;
                    success &= HintPlaceholderText.validate(validator.get()) == STATE_VALID;
                    success &= HintColorNoAlpha.validate(validator.get()) == STATE_VALID;
                    success &= HintObjectId.validate(validator.get()) == STATE_VALID;
                    success &= HintTypeString.validate(validator.get()) == STATE_VALID;
                    success &= HintNodePathToEditedNode.validate(validator.get()) == STATE_VALID;
                    success &= HintObjectTooBig.validate(validator.get()) == STATE_VALID;
                    success &= HintNodePathValidTypes.validate(validator.get()) == STATE_VALID;
                    success &= HintSaveFile.validate(validator.get()) == STATE_VALID;
                    success &= HintGlobalSaveFile.validate(validator.get()) == STATE_VALID;
                    success &= HintIntIsObjectid.validate(validator.get()) == STATE_VALID;
                    success &= HintIntIsPointer.validate(validator.get()) == STATE_VALID;
                    success &= HintArrayType.validate(validator.get()) == STATE_VALID;
                    success &= HintDictionaryType.validate(validator.get()) == STATE_VALID;
                    success &= HintLocaleId.validate(validator.get()) == STATE_VALID;
                    success &= HintLocalizableString.validate(validator.get()) == STATE_VALID;
                    success &= HintNodeType.validate(validator.get()) == STATE_VALID;
                    success &= HintHideQuaternionEdit.validate(validator.get()) == STATE_VALID;
                    success &= HintPassword.validate(validator.get()) == STATE_VALID;
                    success &= HintToolButton.validate(validator.get()) == STATE_VALID;
                    success &= HintOneshot.validate(validator.get()) == STATE_VALID;
                    success &= HintMax.validate(validator.get()) == STATE_VALID;
            }
            
            {
                    Ref<GodotPropertyUsageFlagsParameterType> validator = make_ref<GodotPropertyUsageFlagsParameterType>();
                    success &= UsageNone.validate(validator.get()) == STATE_VALID;
                    success &= UsageStorage.validate(validator.get()) == STATE_VALID;
                    success &= UsageEditor.validate(validator.get()) == STATE_VALID;
                    success &= UsageInternal.validate(validator.get()) == STATE_VALID;
                    success &= UsageCheckable.validate(validator.get()) == STATE_VALID;
                    success &= UsageChecked.validate(validator.get()) == STATE_VALID;
                    success &= UsageGroup.validate(validator.get()) == STATE_VALID;
                    success &= UsageCategory.validate(validator.get()) == STATE_VALID;
                    success &= UsageSubgroup.validate(validator.get()) == STATE_VALID;
                    success &= UsageClassIsBitfield.validate(validator.get()) == STATE_VALID;
                    success &= UsageNoInstanceState.validate(validator.get()) == STATE_VALID;
                    success &= UsageRestartIfChanged.validate(validator.get()) == STATE_VALID;
                    success &= UsageScriptVariable.validate(validator.get()) == STATE_VALID;
                    success &= UsageStoreIfNull.validate(validator.get()) == STATE_VALID;
                    success &= UsageUpdateAllIfModified.validate(validator.get()) == STATE_VALID;
                    success &= UsageScriptDefaultValue.validate(validator.get()) == STATE_VALID;
                    success &= UsageClassIsEnum.validate(validator.get()) == STATE_VALID;
                    success &= UsageNilIsVariant.validate(validator.get()) == STATE_VALID;
                    success &= UsageArray.validate(validator.get()) == STATE_VALID;
                    success &= UsageAlwaysDuplicate.validate(validator.get()) == STATE_VALID;
                    success &= UsageNeverDuplicate.validate(validator.get()) == STATE_VALID;
                    success &= UsageHighEndGfx.validate(validator.get()) == STATE_VALID;
                    success &= UsageNodePathFromSceneRoot.validate(validator.get()) == STATE_VALID;
                    success &= UsageResourceNotPersistent.validate(validator.get()) == STATE_VALID;
                    success &= UsageKeyingIncrements.validate(validator.get()) == STATE_VALID;
                    success &= UsageDeferredSetResource.validate(validator.get()) == STATE_VALID;
                    success &= UsageEditorInstantiateObject.validate(validator.get()) == STATE_VALID;
                    success &= UsageEditorBasicSetting.validate(validator.get()) == STATE_VALID;
                    success &= UsageReadOnly.validate(validator.get()) == STATE_VALID;
                    success &= UsageSecret.validate(validator.get()) == STATE_VALID;
                    success &= UsageDefault.validate(validator.get()) == STATE_VALID;
                    success &= UsageNoEditor.validate(validator.get()) == STATE_VALID;
            }
            
            {
                    Ref<GodotVirtualParameterType> validator = make_ref<GodotVirtualParameterType>();
                    success &= NoVirtual.validate(validator.get()) == STATE_VALID;
                    success &= ScriptVirtual.validate(validator.get()) == STATE_VALID;
                    success &= ScriptVirtualRequired.validate(validator.get()) == STATE_VALID;
            }
            
            {
                    Ref<GodotRpcModeParameterType> validator = make_ref<GodotRpcModeParameterType>();
                    success &= Disabled.validate(validator.get()) == STATE_VALID;
                    success &= AnyPeer.validate(validator.get()) == STATE_VALID;
                    success &= Authority.validate(validator.get()) == STATE_VALID;
            }
            
            {
                    Ref<GodotRpcSyncParameterType> validator = make_ref<GodotRpcSyncParameterType>();
                    success &= CallRemote.validate(validator.get()) == STATE_VALID;
                    success &= CallLocal.validate(validator.get()) == STATE_VALID;
            }
            
            {
                    Ref<GodotRpcTransferModeParameterType> validator = make_ref<GodotRpcTransferModeParameterType>();
                    success &= Unreliable.validate(validator.get()) == STATE_VALID;
                    success &= UnreliableOrdered.validate(validator.get()) == STATE_VALID;
                    success &= Reliable.validate(validator.get()) == STATE_VALID;
            }
            
            {
                    Ref<GodotRpcChannelParameterType> validator = make_ref<GodotRpcChannelParameterType>();
                    success &= Channel.validate(validator.get()) == STATE_VALID;
            }
            return success;
      }
  }
}
//clang-format on