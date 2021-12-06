// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "Blueprint/UserWidget.h"
#include "KGWidget.generated.h"

class LNode
{
public:
	FString id;
	FString name;
	double symbolSize;
	double x;
	double y;
	double value;
	int category;
};

class LLink
{
public:
	FString source;
	FString target;
};

class LCategory
{
public:
	FString name;
};

UCLASS()
class UE4_VISUALIZATION_API UKGWidget : public UUserWidget
{
	GENERATED_BODY()
	
public:
	UFUNCTION(BlueprintCallable)
		void setValue(const FString& InValue);

	UPROPERTY(EditAnywhere, Category = "KG")
		TArray<FLinearColor> Colors;
protected:
	virtual int32 NativePaint(
		const FPaintArgs& Args, 
		const FGeometry& AllottedGeometry,
		const FSlateRect& MyCullingRect, 
		FSlateWindowElementList& OutDrawElements, 
		int32 LayerId,
		const FWidgetStyle& InWidgetStyle,
		bool bParentEnabled) const override;

private:
	TMap<FString, LNode> LNodes;
	TArray<LLink> LLinks;
	TArray<LCategory> LCategories;

	FLinearColor GetColorByCategory(int InCategory)const;
};
