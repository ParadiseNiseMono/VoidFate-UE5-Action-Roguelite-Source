#pragma once

UENUM(BlueprintType)
enum class EConfirmScreenType : uint8
{
	Ok,
	YesNo,
	OkCancel,
	UnKnown  UMETA(Hidden)
};

UENUM(BlueprintType)
enum class EConfirmScreenButtonType : uint8
{
	Confirmed,
	Canceled,
	Closed,
	Unknown  UMETA(Hidden)
};

UENUM(BlueprintType)
enum class EOptionsListDataModifyReason : uint8
{
	DirectlyModified,
	DependencyModified,
	ResetToDefault
};

UENUM()
enum class EVFConfirmType : uint8
{
	Yes,
	No
};

UENUM()
enum class EVFValidType : uint8
{
	Valid,
	Invalid
};

UENUM()
enum class EVFSuccessType : uint8
{
	Successful,
	Failed
};

UENUM()
enum class EVFCountDownActionInput : uint8
{
	Start,
	Cancel
};

UENUM()
enum class EVFCountDownActionOutput : uint8
{
	Updated,
	Completed,
	Cancelled
};

/*
 * 設定遊戲的難度，但目前還沒想好要用甚麼方式設定，之後有需要再弄
 */
UENUM(BlueprintType)
enum class EVFGameDifficulty : uint8
{
	Easy,
	Normal,
	Hard,
	VeryHard
};

UENUM(BlueprintType)
enum class EVFInputMode : uint8
{
	GameOnly,
	UIOnly
};