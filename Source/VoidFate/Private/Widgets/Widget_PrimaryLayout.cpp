// Paradise NiseMono All Rights Reserved


#include "Widgets/Widget_PrimaryLayout.h"


UCommonActivatableWidgetContainerBase* UWidget_PrimaryLayout::FindWidgetStackByTag(UPARAM(meta = (Categories = "Frontend.WidgetStack")) FGameplayTag InTag) const
{
	checkf(RegisteredWidgetStackMap.Contains(InTag), TEXT("cannot find widget stack map for tag: %s"), *InTag.ToString());

	return RegisteredWidgetStackMap.FindRef(InTag);
}

void UWidget_PrimaryLayout::RegisterWidgetStack(UPARAM(meta = (Categories = "Frontend.WidgetStack")) FGameplayTag InStackTag, UCommonActivatableWidgetContainerBase* InStack)
{
	if (!IsDesignTime())
	{
		if (!RegisteredWidgetStackMap.Contains(InStackTag))
		{
			RegisteredWidgetStackMap.Add(InStackTag, InStack);
		}
	}
}
