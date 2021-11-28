// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "Blueprint/UserWidget.h"
#include "SmoothedLineWidget.generated.h"

/**
 *
 */
UCLASS()
class UE4_VISUALIZATION_API USmoothedLineWidget : public UUserWidget
{
	GENERATED_BODY()

public:
	UFUNCTION(BlueprintCallable)
		void SetValues(TArray<float> InValues);
protected:
	virtual int32 NativePaint(
		const FPaintArgs& Args,
		const FGeometry& AllottedGeometry,
		const FSlateRect& MyCullingRect,
		FSlateWindowElementList& OutDrawElements,
		int32 LayerId,
		const FWidgetStyle& InWidgetStyle,
		bool bParentEnabled) const;

private:
	void DrawSmoothedLine(
		FSlateWindowElementList& OutDrawElement,
		int InLayerId,
		const FGeometry& InAllottedGeometry,
		TArray<FVector2D> InPoints,
		float InThickness,
		FColor InColor
	)const;


	TArray<FVector2D> MultiplyPoint;
};
