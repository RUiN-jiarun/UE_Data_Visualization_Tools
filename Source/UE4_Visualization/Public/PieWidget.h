// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "Blueprint/UserWidget.h"
#include "PieWidget.generated.h"

/**
 * 
 */
UCLASS()
class UE4_VISUALIZATION_API UPieWidget : public UUserWidget
{
	GENERATED_BODY()
	
public:
	UFUNCTION(BlueprintCallable)
		void SetValues(TArray<float> InValues);

	UFUNCTION(BlueprintCallable)
		void SetColors(TArray<FLinearColor> InColors);
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
	void DrawFan(
		FSlateWindowElementList& OutDrawElements,
		const FGeometry& AllottedGeometry,
		int32 Layer,
		const FVector2D& CenterPosition,
		float Radius,
		int32 BeginAngle,
		int32 EndAngle,
		FColor Color
	)const;

	TArray<int32> Angles;
	TArray<FLinearColor> Colors;

	FColor GetColorByIndex(int32 InIndex)const;
};
