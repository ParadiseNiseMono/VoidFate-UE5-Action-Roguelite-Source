// Paradise NiseMono All Rights Reserved

#pragma once

#include "CoreMinimal.h"
#include "CommonTabListWidgetBase.h"
#include "VFTabListWidgetBase.generated.h"

class UVFCommonButtonBase;
/**
 * 
 */
UCLASS(Abstract, BlueprintType, meta=(DisableNativeTick))
class VOIDFATE_API UVFTabListWidgetBase : public UCommonTabListWidgetBase
{
	GENERATED_BODY()
	
public:
	void RequestRegisterTab(const FName& InTabID, const FText& InTabDisplayName);
	
private:
	//~ Begin UWidget Interface
#if WITH_EDITOR	
	virtual void ValidateCompiledDefaults(class IWidgetCompilerLog& CompileLog) const override;
#endif
	//~ End UWidget Interface
	
	UPROPERTY(EditAnywhere, BlueprintReadOnly, Category="Frontend Tab List Settings", meta=(AllowPrivateAccess="true", ClampMin = "1", ClampMax = "10"))
	int32 DebugEditorPreviewTabCount = 3;

	UPROPERTY(EditAnywhere, BlueprintReadOnly, Category="Frontend Tab List Settings", meta=(AllowPrivateAccess="true", ClampMin = "1", ClampMax = "10"))
	TSubclassOf<UVFCommonButtonBase> TabButtonEntryWidgetClass;
};
