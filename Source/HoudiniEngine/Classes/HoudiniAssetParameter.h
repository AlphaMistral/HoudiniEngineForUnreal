/*
 * PROPRIETARY INFORMATION.  This software is proprietary to
 * Side Effects Software Inc., and is not to be reproduced,
 * transmitted, or disclosed in any way without written permission.
 *
 * Brokers define connection between assets (for example in content
 * browser) and actors.
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
#include "HoudiniAssetParameter.generated.h"

class FArchive;
class FReferenceCollector;
class IDetailCategoryBuilder;
class UHoudiniAssetComponent;


UCLASS(config=Editor)
class HOUDINIENGINE_API UHoudiniAssetParameter : public UObject
{
	GENERATED_UCLASS_BODY()

public:

	/** Destructor. **/
	virtual ~UHoudiniAssetParameter();

public:

	/** Create this parameter from HAPI information. **/
	virtual bool CreateParameter(UHoudiniAssetComponent* InHoudiniAssetComponent, HAPI_NodeId InNodeId, const HAPI_ParmInfo& ParmInfo);

	/** Create widget for this parameter and add it to a given category. **/
	virtual void CreateWidget(IDetailCategoryBuilder& DetailCategoryBuilder);

	/** Upload parameter value to HAPI. **/
	virtual bool UploadParameterValue();

public:

	/** Return true if this parameter has been changed. **/
	bool HasChanged() const;

	/** Return hash value for this parameter. **/
	uint32 GetParameterHash() const;

	/** Set component for this parameter. **/
	void SetHoudiniAssetComponent(UHoudiniAssetComponent* InHoudiniAssetComponent);

public:

	/** Return hash value for this parameter defining ids. **/
	static uint32 GetParameterHash(HAPI_NodeId NodeId, HAPI_ParmId ParmId);

public: /** UObject methods. **/

	virtual void Serialize(FArchive& Ar) override;
	static void AddReferencedObjects(UObject* InThis, FReferenceCollector& Collector);

protected:

	/** Set parameter and node ids. **/
	void SetNodeParmIds(HAPI_NodeId InNodeId, HAPI_ParmId InParmId);

	/** Return true if parameter and node ids are valid. **/
	bool HasValidNodeParmIds() const;

	/** Set name and label. If label does not exist, name will be used instead for label. If error occurs, false will be returned. **/
	bool SetNameAndLabel(const HAPI_ParmInfo& ParmInfo);

	/** Set name and label to be same value from string handle. **/
	bool SetNameAndLabel(HAPI_StringHandle StringHandle);

	/** Check if parameter is visible. **/
	bool IsVisible(const HAPI_ParmInfo& ParmInfo) const;

	/** Helper function to retrieve parameter name from a given param info structure. Returns false if does not exist. **/
	bool RetrieveParameterName(const HAPI_ParmInfo& ParmInfo, FString& RetrievedName) const;

	/** Helper function to retrieve label name from a given param info structure. Returns false if does not exist. **/
	bool RetrieveParameterLabel(const HAPI_ParmInfo& ParmInfo, FString& RetrievedLabel) const;

	/** Mark this parameter as changed. This occurs when user modifies the value of this parameter through UI. **/
	void MarkChanged();

	/** Return tuple size. **/
	int32 GetTupleSize() const;

	/** Return true if this parameter is an array (has tuple size larger than one). **/
	bool IsArray() const;

	/** Sets internal value index used by this parameter. **/
	void SetValuesIndex(int32 InValuesIndex);

private:

	/** Helper function to retrieve HAPI string and convert it to Unreal one. **/
	bool RetrieveParameterString(HAPI_StringHandle StringHandle, FString& RetrievedName) const;

protected:

	/** Owner component. **/
	UHoudiniAssetComponent* HoudiniAssetComponent;

	/** Name of this parameter. **/
	FString Name;

	/** Label of this parameter. **/
	FString Label;

	/** Node this parameter belongs to. **/
	HAPI_NodeId NodeId;

	/** Id of this parameter. **/
	HAPI_ParmId ParmId;

	/** Tuple size - arrays. **/
	int32 TupleSize;

	/** Internal HAPI cached value index. **/
	int32 ValuesIndex;

	/** Is set to true if value of this parameter has been changed by user. **/
	bool bChanged;
};
