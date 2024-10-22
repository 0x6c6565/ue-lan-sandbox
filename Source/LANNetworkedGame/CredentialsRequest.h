// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "Kismet/BlueprintAsyncActionBase.h"

#include "CredentialsRequest.generated.h"

DECLARE_DYNAMIC_MULTICAST_DELEGATE(FCredentialsRequestOutputPin);

/**
 * 
 */
UCLASS()
class LANNETWORKEDGAME_API UCredentialsRequest : public UBlueprintAsyncActionBase
{
	GENERATED_BODY()

	FProcHandle ProcessHandle;
	FString ExePath;
	FString CredentialsPath;
	int ReturnCode;

	bool ValidatePaths();
	
public:

	UPROPERTY(BlueprintAssignable)
	FCredentialsRequestOutputPin Success;

	UPROPERTY(BlueprintAssignable)
	FCredentialsRequestOutputPin Failure;

	UFUNCTION(BlueprintCallable, meta=(BlueprintInternalUseOnly="true"), Category="Credentials")
	static UCredentialsRequest* OnCredentialsRequest(const FString &Exe, const FString& Credentials);

	virtual void Activate() override;
};
