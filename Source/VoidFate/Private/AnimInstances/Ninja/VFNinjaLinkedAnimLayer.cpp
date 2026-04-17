// Paradise NiseMono All Rights Reserved


#include "AnimInstances/Ninja/VFNinjaLinkedAnimLayer.h"

#include "AnimInstances/Ninja/VFNinjaAnimInstance.h"

UVFNinjaAnimInstance* UVFNinjaLinkedAnimLayer::GetNinjaAnimInstance() const
{
	return Cast<UVFNinjaAnimInstance>(GetOwningComponent()->GetAnimInstance());
}
