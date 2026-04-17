// Paradise NiseMono All Rights Reserved


#include "Widgets/Options/DataObjects/ListDataObject_StringResolution.h"

#include "VFDebugHelper.h"
#include "Kismet/KismetSystemLibrary.h"
#include "VoidFateSettings/VFGameUserSettings.h"
#include "Widgets/Options/OptionsDataInteractionHelper.h"

void UListDataObject_StringResolution::InitResolutionValues()
{
	TArray<FIntPoint> AvailableResolutions;
	UKismetSystemLibrary::GetSupportedFullscreenResolutions(AvailableResolutions);

	AvailableResolutions.Sort(
		[](const FIntPoint& A, const FIntPoint& B)->bool{
			return A.SizeSquared() < B.SizeSquared();
		}
		);
	
	for (const FIntPoint& Resolution : AvailableResolutions)
	{
		AddDynamicOption(ResToValueString(Resolution), ResToDisplayText(Resolution));
	}

	MaximumAllowedResolution = ResToValueString(AvailableResolutions.Last());

	SetDefaultValueFromString(MaximumAllowedResolution);
}

void UListDataObject_StringResolution::OnDataObjectInitialized()
{
	Super::OnDataObjectInitialized();

	if (!TrySetDisplayTextFromStringValue(CurrentStringValue))
	{
		CurrentDisplayText = ResToDisplayText(UVFGameUserSettings::Get()->GetScreenResolution());
	}
}

FString UListDataObject_StringResolution::ResToValueString(const FIntPoint& InResolution)
{
	//Resolution value from dynamic getter : (X=1920,Y=1080)
	return FString::Printf(TEXT("(X=%i,Y=%i)"), InResolution.X, InResolution.Y);
}

FText UListDataObject_StringResolution::ResToDisplayText(const FIntPoint& InResolution)
{
	const FString DisplayString = FString::Printf(TEXT("%i X %i"), InResolution.X, InResolution.Y);

	return FText::FromString(DisplayString);
}
