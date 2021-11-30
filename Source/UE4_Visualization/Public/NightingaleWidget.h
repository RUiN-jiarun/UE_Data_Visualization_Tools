// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "Blueprint/UserWidget.h"
#include "NightingaleWidget.generated.h"

/**
 * 
 */
UCLASS()
class UE4_VISUALIZATION_API UNightingaleWidget : public UUserWidget
{
	GENERATED_BODY()
public:
	UFUNCTION(BlueprintCallable)
		void SetValue(TArray<float> InValues);

		UPROPERTY(EditAnywhere, Category = "Nightingale")
		FVector2D CenterPosition = FVector2D(300.f, 300.f);
		UPROPERTY(EditAnywhere, Category = "Nightingale")
			float IR = 50;
		UPROPERTY(EditAnywhere, Category = "Nightingale")
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
	void DrawDoughnutPart(
		FSlateWindowElementList& OutDrawElements,
		const FGeometry& InAllottedGeometry,
		int InLayerId,
		float InIR,
		float InER,
		int32 InBeginAngle,
		int32 InEndAngle,
		const FColor& InColor
	)const;

	FLinearColor GetColorByIndex(int Index)const;

	TArray<float> ERadius;
};
