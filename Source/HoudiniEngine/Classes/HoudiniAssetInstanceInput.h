/*
 * PROPRIETARY INFORMATION.  This software is proprietary to
 * Side Effects Software Inc., and is not to be reproduced,
 * transmitted, or disclosed in any way without written permission.
 *
 * Produced by:
 *      Mykola Konyk
 *      Side Effects Software Inc
 *      123 Front Street West, Suite 1401
 *      Toronto, Ontario
 *      Canada   M5J 2M2
 *      416-504-9876
 *
 */

#pragma once
#include "HoudiniAssetInstanceInput.generated.h"


class UStaticMesh;
class UInstancedStaticMeshComponent;


UCLASS()
class HOUDINIENGINE_API UHoudiniAssetInstanceInput : public UHoudiniAssetParameter
{
	GENERATED_UCLASS_BODY()

public:

	/** Destructor. **/
	virtual ~UHoudiniAssetInstanceInput();

public:

	/** Create sintance of this class. **/
	static UHoudiniAssetInstanceInput* Create(UHoudiniAssetComponent* InHoudiniAssetComponent, HAPI_ObjectId InObjectId, HAPI_GeoId InGeoId, HAPI_PartId InPartId);

public:

	/** Create this instance input. **/
	bool CreateInstanceInput();

	/** Create this instance input from post-loading intermediate state. **/
	bool CreateInstanceInputPostLoad();

	/** Recreates render states for used instanced static mesh components. **/
	void RecreateRenderStates();

	/** Recreates physics states for used instanced static mesh components. **/
	void RecreatePhysicsStates();

public:

	/** Create this parameter from HAPI information - this implementation does nothing as this is not a true parameter. **/
	virtual bool CreateParameter(UHoudiniAssetComponent* InHoudiniAssetComponent, HAPI_NodeId InNodeId, const HAPI_ParmInfo& ParmInfo);

	/** Create widget for this parameter and add it to a given category. **/
	virtual void CreateWidget(IDetailCategoryBuilder& DetailCategoryBuilder);

	/** Upload parameter value to HAPI. **/
	virtual bool UploadParameterValue();

public: /** UObject methods. **/

	virtual void BeginDestroy();
	virtual void Serialize(FArchive& Ar) override;
	static void AddReferencedObjects(UObject* InThis, FReferenceCollector& Collector);

public:

	/** Return true if this is an attribute instancer. **/
	bool IsAttributeInstancer() const;

	/** Return true if this is an object instancer. Attribute instancer and object instancers are not mutually exclusive. **/
	bool IsObjectInstancer() const;

protected:

	/** Set object, geo and part information. **/
	void SetObjectGeoPartIds(HAPI_ObjectId InObjectId, HAPI_GeoId InGeoId, HAPI_PartId InPartId);

	/** Adjust number of meshes / components depending on the number of objects we need to instance. **/
	void AdjustMeshComponentResources(int32 ObjectCount, int32 OldTupleSize);

	/** Sets instance transformations for a given component. **/
	void SetComponentInstanceTransformations(UInstancedStaticMeshComponent* InstancedStaticMeshComponent, const TArray<FTransform>& InstanceTransforms);

	/** Retrieve all transforms for a given path. Used by attribute instancer. **/
	void GetPathInstaceTransforms(const FString& ObjectInstancePath, const TArray<FString>& PointInstanceValues, const TArray<FTransform>& Transforms, TArray<FTransform>& OutTransforms);

	/** Used to update the component at given index when static mesh changes. **/
	void ChangeInstancedStaticMeshComponentMesh(int32 Idx);

protected:

	/** Checks existance of special instance attribute for this instancer. **/
	static bool CheckInstanceAttribute(HAPI_AssetId AssetId, HAPI_ObjectId InObjectId, HAPI_GeoId InGeoId, HAPI_PartId InPartId);

	/** Delegate used when static mesh has been drag and dropped. **/
	void OnStaticMeshDropped(UObject* InObject, UStaticMesh* StaticMesh, int32 StaticMeshIdx);

	/** Delegate used to detect if valid object has been dragged and dropped. **/
	bool OnStaticMeshDraggedOver(const UObject* InObject) const;

	/** Gets the border brush to show around thumbnails, changes when the user hovers on it. **/
	const FSlateBrush* GetStaticMeshThumbnailBorder(UStaticMesh* StaticMesh, int32 Idx) const;

	/** Handler for when static mesh thumbnail is double clicked. We open editor in this case. **/
	FReply OnThumbnailDoubleClick(const FGeometry& InMyGeometry, const FPointerEvent& InMouseEvent, UObject* Object);

	/** Construct drop down menu content for static mesh. **/
	TSharedRef<SWidget> OnGetStaticMeshMenuContent(UStaticMesh* StaticMesh, int32 StaticMeshIdx);

	/** Delegate for handling selection in content browser. **/
	void OnStaticMeshSelected(const FAssetData& AssetData, UStaticMesh* StaticMesh, int32 StaticMeshIdx);

	/** Closes the combo button. **/
	void CloseStaticMeshComboButton();

protected:

	/** Map of static meshes and corresponding thumbnail borders. **/
	TMap<int32, TSharedPtr<SBorder> > StaticMeshThumbnailBorders;

	/** Map of static meshes to combo elements. **/
	TMap<int32, TSharedPtr<SComboButton> > StaticMeshComboButtons;

	/** Widget used for dragging and input. **/
	TArray<TSharedPtr<SAssetSearchBox> > InputWidgets;

	/** Corresponding instanced static mesh components. **/
	TArray<UInstancedStaticMeshComponent*> InstancedStaticMeshComponents;

	/** Static meshes which are used for input. **/
	TArray<UStaticMesh*> StaticMeshes;

	/** Original static meshes which were used as input when instancer was constructed. **/
	TArray<UStaticMesh*> OriginalStaticMeshes;

	/** Transforms for each component. **/
	TArray<TArray<FTransform> > InstancedTransforms;

	/** Temporary geo part information, this is used during loading. **/
	TArray<FHoudiniGeoPartObject> GeoPartObjects;

	/** Delegate for filtering static meshes. **/
	FOnShouldFilterAsset OnShouldFilterStaticMesh;

	/** Corresponding object id. **/
	HAPI_ObjectId ObjectId;

	/** Id of an object to instance. **/
	HAPI_ObjectId ObjectToInstanceId;

	/** Corresponding geo id. **/
	HAPI_GeoId GeoId;

	/** Corresponding part id. **/
	HAPI_PartId PartId;

	/** Set to true if this is an attribute instancer. **/
	bool bAttributeInstancer;
};
