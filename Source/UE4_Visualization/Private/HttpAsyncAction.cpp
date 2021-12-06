// Fill out your copyright notice in the Description page of Project Settings.


#include "HttpAsyncAction.h"
#include "HttpModule.h"

UHttpAsyncAction::UHttpAsyncAction()
{
	if (HasAnyFlags(RF_ClassDefaultObject) == false)
	{
		AddToRoot();
	}
}

UHttpAsyncAction* UHttpAsyncAction::AsyncHttpURLRequest(const FString& URL)
{
	UHttpAsyncAction* AsyncHttpObject = NewObject<UHttpAsyncAction>();
	AsyncHttpObject->HttpRequestStart(URL);
	return AsyncHttpObject;
}

void UHttpAsyncAction::HttpRequestStart(const FString& URL)
{
	TSharedRef<IHttpRequest, ESPMode::ThreadSafe> Request = FHttpModule::Get().CreateRequest();
	Request->SetVerb("GET");
	Request->SetURL(URL);

	// Get callback
	Request->OnProcessRequestComplete().BindUObject(this, &UHttpAsyncAction::HttpRequest_Handle);
	Request->ProcessRequest();
}

void UHttpAsyncAction::HttpRequest_Handle(FHttpRequestPtr InRequest, FHttpResponsePtr Response, bool bWasSuccessful)
{
	if (bWasSuccessful && Response.IsValid() && EHttpResponseCodes::IsOk(Response->GetResponseCode()))
	{
		// Successful: get the string
		OnSucceeded.Broadcast(Response->GetContentAsString());
		RemoveFromRoot();

		return;
	}
	// Failed: return empty string
	OnFailed.Broadcast("");
	RemoveFromRoot();
}