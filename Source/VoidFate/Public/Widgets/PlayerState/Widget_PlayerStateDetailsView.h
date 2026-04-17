// Paradise NiseMono All Rights Reserved

#pragma once

#include "CoreMinimal.h"
#include "Blueprint/UserWidget.h"
#include "Widgets/Options/DataObjects/ListDataObject_Base.h"
#include "Widget_PlayerStateDetailsView.generated.h"

class UCommonRichTextBlock;
class UCommonLazyImage;
class UCommonTextBlock;
class UListDataObject_Base;
/**
 * 
 */
UCLASS()
class VOIDFATE_API UWidget_PlayerStateDetailsView : public UUserWidget
{
	GENERATED_BODY()

public:
	void UpdateDetailsViewInfo(UListDataObject_Base* InDataObject);
	void ClearDetailsViewInfo();

protected:
	virtual void NativeOnInitialized() override;

private:
	//***** Bind Widget *****//
	UPROPERTY(meta=(BindWidget))
	UCommonTextBlock* CommonTextBlock_Title;

	UPROPERTY(meta=(BindWidget))
	UCommonLazyImage* CommonLazyImage_DescriptionImage;

	UPROPERTY(meta=(BindWidget))
	UCommonRichTextBlock* CommonRichText_Description;
	//***** Bind Widget *****//
};
