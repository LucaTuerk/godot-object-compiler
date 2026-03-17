/**************************************************************************/
/* parameter_types.h                                                      */
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

#include "library/core/assumption.h"
namespace GodotObjectCompiler {
namespace AssumedParameterValues {
inline Assumption<String> RegisterCustom{
    "RegisterCustom",
    "Assume that the value \"RegisterCustom\" exists in the "
    "GOC_GodotClassType generated type."};
inline Assumption<String> GodotClass{
    "GodotClass",
    "Assume that the value \"GodotClass\" exists in the "
    "GOC_GodotClassType generated type."};
inline Assumption<String> VirtualClass{
    "VirtualClass",
    "Assume that the value \"VirtualClass\" exists in the "
    "GOC_GodotClassType generated type."};
inline Assumption<String> AbstractClass{
    "AbstractClass",
    "Assume that the value \"AbstractClass\" exists in the "
    "GOC_GodotClassType generated type."};
inline Assumption<String> InternalClass{
    "InternalClass",
    "Assume that the value \"InternalClass\" exists in the "
    "GOC_GodotClassType generated type."};
inline Assumption<String> RuntimeClass{
    "RuntimeClass",
    "Assume that the value \"RuntimeClass\" exists in the "
    "GOC_GodotClassType generated type."};
inline Assumption<String> LevelCore{
    "LevelCore",
    "Assume that the value \"LevelCore\" exists in the "
    "GOC_ModuleInitializationLevel generated type."};
inline Assumption<String> LevelServers{
    "LevelServers",
    "Assume that the value \"LevelServers\" exists in the "
    "GOC_ModuleInitializationLevel generated type."};
inline Assumption<String> LevelScene{
    "LevelScene",
    "Assume that the value \"LevelScene\" exists in the "
    "GOC_ModuleInitializationLevel generated type."};
inline Assumption<String> LevelEditor{
    "LevelEditor",
    "Assume that the value \"LevelEditor\" exists in the "
    "GOC_ModuleInitializationLevel generated type."};
inline Assumption<String> LevelMax{
    "LevelMax",
    "Assume that the value \"LevelMax\" exists in the "
    "GOC_ModuleInitializationLevel generated type."};
inline Assumption<String> VariantTypeNil{
    "VariantTypeNil",
    "Assume that the value \"VariantTypeNil\" exists in the "
    "GOC_VariantType generated type."};
inline Assumption<String> VariantTypeBool{
    "VariantTypeBool",
    "Assume that the value \"VariantTypeBool\" exists in "
    "the GOC_VariantType generated type."};
inline Assumption<String> VariantTypeInt{
    "VariantTypeInt",
    "Assume that the value \"VariantTypeInt\" exists in the "
    "GOC_VariantType generated type."};
inline Assumption<String> VariantTypeFloat{
    "VariantTypeFloat",
    "Assume that the value \"VariantTypeFloat\" exists in "
    "the GOC_VariantType generated type."};
inline Assumption<String> VariantTypeString{
    "VariantTypeString",
    "Assume that the value \"VariantTypeString\" exists "
    "in the GOC_VariantType generated type."};
inline Assumption<String> VariantTypeVector2{
    "VariantTypeVector2",
    "Assume that the value \"VariantTypeVector2\" exists "
    "in the GOC_VariantType generated type."};
inline Assumption<String> VariantTypeVector2i{
    "VariantTypeVector2i",
    "Assume that the value \"VariantTypeVector2i\" "
    "exists in the GOC_VariantType generated type."};
inline Assumption<String> VariantTypeRect2{
    "VariantTypeRect2",
    "Assume that the value \"VariantTypeRect2\" exists in "
    "the GOC_VariantType generated type."};
inline Assumption<String> VariantTypeRect2i{
    "VariantTypeRect2i",
    "Assume that the value \"VariantTypeRect2i\" exists "
    "in the GOC_VariantType generated type."};
inline Assumption<String> VariantTypeVector3{
    "VariantTypeVector3",
    "Assume that the value \"VariantTypeVector3\" exists "
    "in the GOC_VariantType generated type."};
inline Assumption<String> VariantTypeVector3i{
    "VariantTypeVector3i",
    "Assume that the value \"VariantTypeVector3i\" "
    "exists in the GOC_VariantType generated type."};
inline Assumption<String> VariantTypeTransform2d{
    "VariantTypeTransform2d",
    "Assume that the value \"VariantTypeTransform2d\" exists in the "
    "GOC_VariantType generated type."};
inline Assumption<String> VariantTypeVector4{
    "VariantTypeVector4",
    "Assume that the value \"VariantTypeVector4\" exists "
    "in the GOC_VariantType generated type."};
inline Assumption<String> VariantTypeVector4i{
    "VariantTypeVector4i",
    "Assume that the value \"VariantTypeVector4i\" "
    "exists in the GOC_VariantType generated type."};
inline Assumption<String> VariantTypePlane{
    "VariantTypePlane",
    "Assume that the value \"VariantTypePlane\" exists in "
    "the GOC_VariantType generated type."};
inline Assumption<String> VariantTypeQuaternion{
    "VariantTypeQuaternion",
    "Assume that the value \"VariantTypeQuaternion\" "
    "exists in the GOC_VariantType generated type."};
inline Assumption<String> VariantTypeAabb{
    "VariantTypeAabb",
    "Assume that the value \"VariantTypeAabb\" exists in "
    "the GOC_VariantType generated type."};
inline Assumption<String> VariantTypeBasis{
    "VariantTypeBasis",
    "Assume that the value \"VariantTypeBasis\" exists in "
    "the GOC_VariantType generated type."};
inline Assumption<String> VariantTypeTransform3d{
    "VariantTypeTransform3d",
    "Assume that the value \"VariantTypeTransform3d\" exists in the "
    "GOC_VariantType generated type."};
inline Assumption<String> VariantTypeProjection{
    "VariantTypeProjection",
    "Assume that the value \"VariantTypeProjection\" "
    "exists in the GOC_VariantType generated type."};
inline Assumption<String> VariantTypeColor{
    "VariantTypeColor",
    "Assume that the value \"VariantTypeColor\" exists in "
    "the GOC_VariantType generated type."};
inline Assumption<String> VariantTypeStringName{
    "VariantTypeStringName",
    "Assume that the value \"VariantTypeStringName\" "
    "exists in the GOC_VariantType generated type."};
inline Assumption<String> VariantTypeNodePath{
    "VariantTypeNodePath",
    "Assume that the value \"VariantTypeNodePath\" "
    "exists in the GOC_VariantType generated type."};
inline Assumption<String> VariantTypeRid{
    "VariantTypeRid",
    "Assume that the value \"VariantTypeRid\" exists in the "
    "GOC_VariantType generated type."};
inline Assumption<String> VariantTypeObject{
    "VariantTypeObject",
    "Assume that the value \"VariantTypeObject\" exists "
    "in the GOC_VariantType generated type."};
inline Assumption<String> VariantTypeCallable{
    "VariantTypeCallable",
    "Assume that the value \"VariantTypeCallable\" "
    "exists in the GOC_VariantType generated type."};
inline Assumption<String> VariantTypeSignal{
    "VariantTypeSignal",
    "Assume that the value \"VariantTypeSignal\" exists "
    "in the GOC_VariantType generated type."};
inline Assumption<String> VariantTypeDictionary{
    "VariantTypeDictionary",
    "Assume that the value \"VariantTypeDictionary\" "
    "exists in the GOC_VariantType generated type."};
inline Assumption<String> VariantTypeArray{
    "VariantTypeArray",
    "Assume that the value \"VariantTypeArray\" exists in "
    "the GOC_VariantType generated type."};
inline Assumption<String> VariantTypePackedByteArray{
    "VariantTypePackedByteArray",
    "Assume that the value \"VariantTypePackedByteArray\" exists in the "
    "GOC_VariantType generated type."};
inline Assumption<String> VariantTypePackedInt32Array{
    "VariantTypePackedInt32Array",
    "Assume that the value \"VariantTypePackedInt32Array\" exists in the "
    "GOC_VariantType generated type."};
inline Assumption<String> VariantTypePackedInt64Array{
    "VariantTypePackedInt64Array",
    "Assume that the value \"VariantTypePackedInt64Array\" exists in the "
    "GOC_VariantType generated type."};
inline Assumption<String> VariantTypePackedFloat32Array{
    "VariantTypePackedFloat32Array",
    "Assume that the value \"VariantTypePackedFloat32Array\" exists in the "
    "GOC_VariantType generated type."};
inline Assumption<String> VariantTypePackedFloat64Array{
    "VariantTypePackedFloat64Array",
    "Assume that the value \"VariantTypePackedFloat64Array\" exists in the "
    "GOC_VariantType generated type."};
inline Assumption<String> VariantTypePackedStringArray{
    "VariantTypePackedStringArray",
    "Assume that the value \"VariantTypePackedStringArray\" exists in the "
    "GOC_VariantType generated type."};
inline Assumption<String> VariantTypePackedVector2Array{
    "VariantTypePackedVector2Array",
    "Assume that the value \"VariantTypePackedVector2Array\" exists in the "
    "GOC_VariantType generated type."};
inline Assumption<String> VariantTypePackedVector3Array{
    "VariantTypePackedVector3Array",
    "Assume that the value \"VariantTypePackedVector3Array\" exists in the "
    "GOC_VariantType generated type."};
inline Assumption<String> VariantTypePackedColorArray{
    "VariantTypePackedColorArray",
    "Assume that the value \"VariantTypePackedColorArray\" exists in the "
    "GOC_VariantType generated type."};
inline Assumption<String> VariantTypePackedVector4Array{
    "VariantTypePackedVector4Array",
    "Assume that the value \"VariantTypePackedVector4Array\" exists in the "
    "GOC_VariantType generated type."};
inline Assumption<String> VariantTypeVariantMax{
    "VariantTypeVariantMax",
    "Assume that the value \"VariantTypeVariantMax\" "
    "exists in the GOC_VariantType generated type."};
inline Assumption<String> HintNone{"HintNone",
                                   "Assume that the value \"HintNone\" exists "
                                   "in the GOC_PropertyHint generated type."};
inline Assumption<String> HintRange{
    "HintRange",
    "Assume that the value \"HintRange\" exists in the "
    "GOC_PropertyHint generated type."};
inline Assumption<String> HintEnum{"HintEnum",
                                   "Assume that the value \"HintEnum\" exists "
                                   "in the GOC_PropertyHint generated type."};
inline Assumption<String> HintEnumSuggestion{
    "HintEnumSuggestion",
    "Assume that the value \"HintEnumSuggestion\" exists "
    "in the GOC_PropertyHint generated type."};
inline Assumption<String> HintExpEasing{
    "HintExpEasing",
    "Assume that the value \"HintExpEasing\" exists in the "
    "GOC_PropertyHint generated type."};
inline Assumption<String> HintLink{"HintLink",
                                   "Assume that the value \"HintLink\" exists "
                                   "in the GOC_PropertyHint generated type."};
inline Assumption<String> HintFlags{
    "HintFlags",
    "Assume that the value \"HintFlags\" exists in the "
    "GOC_PropertyHint generated type."};
inline Assumption<String> HintLayers2dRender{
    "HintLayers2dRender",
    "Assume that the value \"HintLayers2dRender\" exists "
    "in the GOC_PropertyHint generated type."};
inline Assumption<String> HintLayers2dPhysics{
    "HintLayers2dPhysics",
    "Assume that the value \"HintLayers2dPhysics\" "
    "exists in the GOC_PropertyHint generated type."};
inline Assumption<String> HintLayers2dNavigation{
    "HintLayers2dNavigation",
    "Assume that the value \"HintLayers2dNavigation\" exists in the "
    "GOC_PropertyHint generated type."};
inline Assumption<String> HintLayers3dRender{
    "HintLayers3dRender",
    "Assume that the value \"HintLayers3dRender\" exists "
    "in the GOC_PropertyHint generated type."};
inline Assumption<String> HintLayers3dPhysics{
    "HintLayers3dPhysics",
    "Assume that the value \"HintLayers3dPhysics\" "
    "exists in the GOC_PropertyHint generated type."};
inline Assumption<String> HintLayers3dNavigation{
    "HintLayers3dNavigation",
    "Assume that the value \"HintLayers3dNavigation\" exists in the "
    "GOC_PropertyHint generated type."};
inline Assumption<String> HintLayersAvoidance{
    "HintLayersAvoidance",
    "Assume that the value \"HintLayersAvoidance\" "
    "exists in the GOC_PropertyHint generated type."};
inline Assumption<String> HintFile{"HintFile",
                                   "Assume that the value \"HintFile\" exists "
                                   "in the GOC_PropertyHint generated type."};
inline Assumption<String> HintDir{"HintDir",
                                  "Assume that the value \"HintDir\" exists in "
                                  "the GOC_PropertyHint generated type."};
inline Assumption<String> HintGlobalFile{
    "HintGlobalFile",
    "Assume that the value \"HintGlobalFile\" exists in the "
    "GOC_PropertyHint generated type."};
inline Assumption<String> HintGlobalDir{
    "HintGlobalDir",
    "Assume that the value \"HintGlobalDir\" exists in the "
    "GOC_PropertyHint generated type."};
inline Assumption<String> HintResourceType{
    "HintResourceType",
    "Assume that the value \"HintResourceType\" exists in "
    "the GOC_PropertyHint generated type."};
inline Assumption<String> HintMultilineText{
    "HintMultilineText",
    "Assume that the value \"HintMultilineText\" exists "
    "in the GOC_PropertyHint generated type."};
inline Assumption<String> HintExpression{
    "HintExpression",
    "Assume that the value \"HintExpression\" exists in the "
    "GOC_PropertyHint generated type."};
inline Assumption<String> HintPlaceholderText{
    "HintPlaceholderText",
    "Assume that the value \"HintPlaceholderText\" "
    "exists in the GOC_PropertyHint generated type."};
inline Assumption<String> HintColorNoAlpha{
    "HintColorNoAlpha",
    "Assume that the value \"HintColorNoAlpha\" exists in "
    "the GOC_PropertyHint generated type."};
inline Assumption<String> HintObjectId{
    "HintObjectId",
    "Assume that the value \"HintObjectId\" exists in the "
    "GOC_PropertyHint generated type."};
inline Assumption<String> HintTypeString{
    "HintTypeString",
    "Assume that the value \"HintTypeString\" exists in the "
    "GOC_PropertyHint generated type."};
inline Assumption<String> HintNodePathToEditedNode{
    "HintNodePathToEditedNode",
    "Assume that the value \"HintNodePathToEditedNode\" exists in the "
    "GOC_PropertyHint generated type."};
inline Assumption<String> HintObjectTooBig{
    "HintObjectTooBig",
    "Assume that the value \"HintObjectTooBig\" exists in "
    "the GOC_PropertyHint generated type."};
inline Assumption<String> HintNodePathValidTypes{
    "HintNodePathValidTypes",
    "Assume that the value \"HintNodePathValidTypes\" exists in the "
    "GOC_PropertyHint generated type."};
inline Assumption<String> HintSaveFile{
    "HintSaveFile",
    "Assume that the value \"HintSaveFile\" exists in the "
    "GOC_PropertyHint generated type."};
inline Assumption<String> HintGlobalSaveFile{
    "HintGlobalSaveFile",
    "Assume that the value \"HintGlobalSaveFile\" exists "
    "in the GOC_PropertyHint generated type."};
inline Assumption<String> HintIntIsObjectid{
    "HintIntIsObjectid",
    "Assume that the value \"HintIntIsObjectid\" exists "
    "in the GOC_PropertyHint generated type."};
inline Assumption<String> HintIntIsPointer{
    "HintIntIsPointer",
    "Assume that the value \"HintIntIsPointer\" exists in "
    "the GOC_PropertyHint generated type."};
inline Assumption<String> HintArrayType{
    "HintArrayType",
    "Assume that the value \"HintArrayType\" exists in the "
    "GOC_PropertyHint generated type."};
inline Assumption<String> HintDictionaryType{
    "HintDictionaryType",
    "Assume that the value \"HintDictionaryType\" exists "
    "in the GOC_PropertyHint generated type."};
inline Assumption<String> HintLocaleId{
    "HintLocaleId",
    "Assume that the value \"HintLocaleId\" exists in the "
    "GOC_PropertyHint generated type."};
inline Assumption<String> HintLocalizableString{
    "HintLocalizableString",
    "Assume that the value \"HintLocalizableString\" "
    "exists in the GOC_PropertyHint generated type."};
inline Assumption<String> HintNodeType{
    "HintNodeType",
    "Assume that the value \"HintNodeType\" exists in the "
    "GOC_PropertyHint generated type."};
inline Assumption<String> HintHideQuaternionEdit{
    "HintHideQuaternionEdit",
    "Assume that the value \"HintHideQuaternionEdit\" exists in the "
    "GOC_PropertyHint generated type."};
inline Assumption<String> HintPassword{
    "HintPassword",
    "Assume that the value \"HintPassword\" exists in the "
    "GOC_PropertyHint generated type."};
inline Assumption<String> HintToolButton{
    "HintToolButton",
    "Assume that the value \"HintToolButton\" exists in the "
    "GOC_PropertyHint generated type."};
inline Assumption<String> HintOneshot{
    "HintOneshot",
    "Assume that the value \"HintOneshot\" exists in the "
    "GOC_PropertyHint generated type."};
inline Assumption<String> HintMax{"HintMax",
                                  "Assume that the value \"HintMax\" exists in "
                                  "the GOC_PropertyHint generated type."};
inline Assumption<String> UsageNone{
    "UsageNone",
    "Assume that the value \"UsageNone\" exists in the "
    "GOC_PropertyUsageFlags generated type."};
inline Assumption<String> UsageStorage{
    "UsageStorage",
    "Assume that the value \"UsageStorage\" exists in the "
    "GOC_PropertyUsageFlags generated type."};
inline Assumption<String> UsageEditor{
    "UsageEditor",
    "Assume that the value \"UsageEditor\" exists in the "
    "GOC_PropertyUsageFlags generated type."};
inline Assumption<String> UsageInternal{
    "UsageInternal",
    "Assume that the value \"UsageInternal\" exists in the "
    "GOC_PropertyUsageFlags generated type."};
inline Assumption<String> UsageCheckable{
    "UsageCheckable",
    "Assume that the value \"UsageCheckable\" exists in the "
    "GOC_PropertyUsageFlags generated type."};
inline Assumption<String> UsageChecked{
    "UsageChecked",
    "Assume that the value \"UsageChecked\" exists in the "
    "GOC_PropertyUsageFlags generated type."};
inline Assumption<String> UsageGroup{
    "UsageGroup",
    "Assume that the value \"UsageGroup\" exists in the "
    "GOC_PropertyUsageFlags generated type."};
inline Assumption<String> UsageCategory{
    "UsageCategory",
    "Assume that the value \"UsageCategory\" exists in the "
    "GOC_PropertyUsageFlags generated type."};
inline Assumption<String> UsageSubgroup{
    "UsageSubgroup",
    "Assume that the value \"UsageSubgroup\" exists in the "
    "GOC_PropertyUsageFlags generated type."};
inline Assumption<String> UsageClassIsBitfield{
    "UsageClassIsBitfield",
    "Assume that the value \"UsageClassIsBitfield\" exists in the "
    "GOC_PropertyUsageFlags generated type."};
inline Assumption<String> UsageNoInstanceState{
    "UsageNoInstanceState",
    "Assume that the value \"UsageNoInstanceState\" exists in the "
    "GOC_PropertyUsageFlags generated type."};
inline Assumption<String> UsageRestartIfChanged{
    "UsageRestartIfChanged",
    "Assume that the value \"UsageRestartIfChanged\" exists in the "
    "GOC_PropertyUsageFlags generated type."};
inline Assumption<String> UsageScriptVariable{
    "UsageScriptVariable",
    "Assume that the value \"UsageScriptVariable\" exists in the "
    "GOC_PropertyUsageFlags generated type."};
inline Assumption<String> UsageStoreIfNull{
    "UsageStoreIfNull",
    "Assume that the value \"UsageStoreIfNull\" exists in "
    "the GOC_PropertyUsageFlags generated type."};
inline Assumption<String> UsageUpdateAllIfModified{
    "UsageUpdateAllIfModified",
    "Assume that the value \"UsageUpdateAllIfModified\" exists in the "
    "GOC_PropertyUsageFlags generated type."};
inline Assumption<String> UsageScriptDefaultValue{
    "UsageScriptDefaultValue",
    "Assume that the value \"UsageScriptDefaultValue\" exists in the "
    "GOC_PropertyUsageFlags generated type."};
inline Assumption<String> UsageClassIsEnum{
    "UsageClassIsEnum",
    "Assume that the value \"UsageClassIsEnum\" exists in "
    "the GOC_PropertyUsageFlags generated type."};
inline Assumption<String> UsageNilIsVariant{
    "UsageNilIsVariant",
    "Assume that the value \"UsageNilIsVariant\" exists "
    "in the GOC_PropertyUsageFlags generated type."};
inline Assumption<String> UsageArray{
    "UsageArray",
    "Assume that the value \"UsageArray\" exists in the "
    "GOC_PropertyUsageFlags generated type."};
inline Assumption<String> UsageAlwaysDuplicate{
    "UsageAlwaysDuplicate",
    "Assume that the value \"UsageAlwaysDuplicate\" exists in the "
    "GOC_PropertyUsageFlags generated type."};
inline Assumption<String> UsageNeverDuplicate{
    "UsageNeverDuplicate",
    "Assume that the value \"UsageNeverDuplicate\" exists in the "
    "GOC_PropertyUsageFlags generated type."};
inline Assumption<String> UsageHighEndGfx{
    "UsageHighEndGfx",
    "Assume that the value \"UsageHighEndGfx\" exists in "
    "the GOC_PropertyUsageFlags generated type."};
inline Assumption<String> UsageNodePathFromSceneRoot{
    "UsageNodePathFromSceneRoot",
    "Assume that the value \"UsageNodePathFromSceneRoot\" exists in the "
    "GOC_PropertyUsageFlags generated type."};
inline Assumption<String> UsageResourceNotPersistent{
    "UsageResourceNotPersistent",
    "Assume that the value \"UsageResourceNotPersistent\" exists in the "
    "GOC_PropertyUsageFlags generated type."};
inline Assumption<String> UsageKeyingIncrements{
    "UsageKeyingIncrements",
    "Assume that the value \"UsageKeyingIncrements\" exists in the "
    "GOC_PropertyUsageFlags generated type."};
inline Assumption<String> UsageDeferredSetResource{
    "UsageDeferredSetResource",
    "Assume that the value \"UsageDeferredSetResource\" exists in the "
    "GOC_PropertyUsageFlags generated type."};
inline Assumption<String> UsageEditorInstantiateObject{
    "UsageEditorInstantiateObject",
    "Assume that the value \"UsageEditorInstantiateObject\" exists in the "
    "GOC_PropertyUsageFlags generated type."};
inline Assumption<String> UsageEditorBasicSetting{
    "UsageEditorBasicSetting",
    "Assume that the value \"UsageEditorBasicSetting\" exists in the "
    "GOC_PropertyUsageFlags generated type."};
inline Assumption<String> UsageReadOnly{
    "UsageReadOnly",
    "Assume that the value \"UsageReadOnly\" exists in the "
    "GOC_PropertyUsageFlags generated type."};
inline Assumption<String> UsageSecret{
    "UsageSecret",
    "Assume that the value \"UsageSecret\" exists in the "
    "GOC_PropertyUsageFlags generated type."};
inline Assumption<String> UsageDefault{
    "UsageDefault",
    "Assume that the value \"UsageDefault\" exists in the "
    "GOC_PropertyUsageFlags generated type."};
inline Assumption<String> UsageNoEditor{
    "UsageNoEditor",
    "Assume that the value \"UsageNoEditor\" exists in the "
    "GOC_PropertyUsageFlags generated type."};
inline Assumption<String> NoVirtual{
    "NoVirtual",
    "Assume that the value \"NoVirtual\" exists in the "
    "GOC_Virtual generated type."};
inline Assumption<String> ScriptVirtual{
    "ScriptVirtual",
    "Assume that the value \"ScriptVirtual\" exists in the "
    "GOC_Virtual generated type."};
inline Assumption<String> ScriptVirtualRequired{
    "ScriptVirtualRequired",
    "Assume that the value \"ScriptVirtualRequired\" "
    "exists in the GOC_Virtual generated type."};
inline Assumption<String> Disabled{"Disabled",
                                   "Assume that the value \"Disabled\" exists "
                                   "in the GOC_RpcMode generated type."};
inline Assumption<String> AnyPeer{"AnyPeer",
                                  "Assume that the value \"AnyPeer\" exists in "
                                  "the GOC_RpcMode generated type."};
inline Assumption<String> Authority{
    "Authority",
    "Assume that the value \"Authority\" exists in the "
    "GOC_RpcMode generated type."};
inline Assumption<String> CallRemote{
    "CallRemote",
    "Assume that the value \"CallRemote\" exists in the "
    "GOC_RpcSync generated type."};
inline Assumption<String> CallLocal{
    "CallLocal",
    "Assume that the value \"CallLocal\" exists in the "
    "GOC_RpcSync generated type."};
inline Assumption<String> Unreliable{
    "Unreliable",
    "Assume that the value \"Unreliable\" exists in the "
    "GOC_TransferMode generated type."};
inline Assumption<String> UnreliableOrdered{
    "UnreliableOrdered",
    "Assume that the value \"UnreliableOrdered\" exists "
    "in the GOC_TransferMode generated type."};
inline Assumption<String> Reliable{"Reliable",
                                   "Assume that the value \"Reliable\" exists "
                                   "in the GOC_TransferMode generated type."};
inline Assumption<String> Channel{"Channel",
                                  "Assume that the value \"Channel\" exists in "
                                  "the GOC_RpcChannel generated type."};
bool validate_assumptions();
}  // namespace AssumedParameterValues
}  // namespace GodotObjectCompiler
