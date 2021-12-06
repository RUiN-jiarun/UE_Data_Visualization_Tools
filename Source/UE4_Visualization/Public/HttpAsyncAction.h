// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "Kismet/BlueprintAsyncActionBase.h"
#include "Interfaces/IHttpRequest.h"
#include "Interfaces/IHttpResponse.h"

#include "HttpAsyncAction.generated.h"

DECLARE_DYNAMIC_MULTICAST_DELEGATE_OneParam(FRequestCompleteDelegate, const FString&, Data);
/**
 *
 */
UCLASS()
class UE4_VISUALIZATION_API UHttpAsyncAction : public UBlueprintAsyncActionBase
{
	GENERATED_BODY()

public:

	UHttpAsyncAction();
	UFUNCTION(BlueprintCallable, Category = "Http")
		static UHttpAsyncAction* AsyncHttpURLRequest(const FString& URL);

	void HttpRequestStart(const FString& URL);

	void HttpRequest_Handle(FHttpRequestPtr InRequest, FHttpResponsePtr Response, bool bWasSuccessful);

	UPROPERTY(BlueprintAssignable)
		FRequestCompleteDelegate OnSucceeded;
	UPROPERTY(BlueprintAssignable)
		FRequestCompleteDelegate OnFailed;
};
