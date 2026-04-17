// Paradise NiseMono All Rights Reserved


#include "Widgets/Options/OptionsDataInteractionHelper.h"

#include "VoidFateSettings/VFGameUserSettings.h"


FOptionsDataInteractionHelper::FOptionsDataInteractionHelper(const FString& InSetterOrGetterFuncPath)
	: CachedDynamicFunctionPath(InSetterOrGetterFuncPath)
{
	CachedWeakGameUserSettings = UVFGameUserSettings::Get();
}

FString FOptionsDataInteractionHelper::GetValueAsString()
{
	FString OutputStringValue;
	
	PropertyPathHelpers::GetPropertyValueAsString(
		CachedWeakGameUserSettings.Get(),
		CachedDynamicFunctionPath,
		OutputStringValue);
	
	return OutputStringValue;
}

void FOptionsDataInteractionHelper::SetValueFromString(const FString& InStringValue)
{
	PropertyPathHelpers::SetPropertyValueFromString(
		CachedWeakGameUserSettings.Get(),
		CachedDynamicFunctionPath,
		InStringValue
		);
}
