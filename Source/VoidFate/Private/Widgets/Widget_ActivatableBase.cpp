// Paradise NiseMono All Rights Reserved


#include "Widgets/Widget_ActivatableBase.h"
#include "Controllers/VFUIController.h"
#include "Controllers/VFNinjaController.h"

AVFUIController* UWidget_ActivatableBase::GetOwningVFPlayerController()
{
	if (!CachedOwningVFPlayerController.IsValid())
	{
		CachedOwningVFPlayerController = GetOwningPlayer<AVFUIController>();
	}
	return CachedOwningVFPlayerController.IsValid() ? CachedOwningVFPlayerController.Get() : nullptr;
}

AVFNinjaController* UWidget_ActivatableBase::GetOwningVFNanjaController()
{
	if (!CachedOwningVFNinjaController.IsValid())
	{
		CachedOwningVFNinjaController = GetOwningPlayer<AVFNinjaController>();
	}
	return CachedOwningVFNinjaController.IsValid() ? CachedOwningVFNinjaController.Get() : nullptr;
}
