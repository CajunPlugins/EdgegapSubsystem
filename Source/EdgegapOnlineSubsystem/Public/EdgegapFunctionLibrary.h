#pragma once

#include "Kismet/BlueprintFunctionLibrary.h"
#include "EdgegapFunctionLibrary.generated.h"

/* 
*	Function library class.
*/
UCLASS()
class UEdgegapFunctionLibrary : public UBlueprintFunctionLibrary
{
	GENERATED_UCLASS_BODY()

	UFUNCTION(BlueprintCallable, meta = (DisplayName = "Execute Sample function", Keywords = "EdgegapOnlineSubsystem sample test testing"), Category = "EdgegapOnlineSubsystemTesting")
	static float EdgegapOnlineSubsystemSampleFunction(float Param);
};
