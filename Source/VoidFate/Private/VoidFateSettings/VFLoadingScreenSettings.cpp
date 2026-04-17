// Paradise NiseMono All Rights Reserved


#include "VoidFateSettings/VFLoadingScreenSettings.h"
#include "Blueprint/UserWidget.h"

TSubclassOf<UUserWidget> UVFLoadingScreenSettings::GetLoadingScreenWidgetClassChecked() const
{
	checkf(!SoftLoadingScreenClass.IsNull(), TEXT("Forgot to assign a valid widget blueprint in project setting as loading screen"));

	TSubclassOf<UUserWidget> LoadedLoadingScreenWidget = SoftLoadingScreenClass.LoadSynchronous();

	return LoadedLoadingScreenWidget;
}
