// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "Blueprint/UserWidget.h"
#include "RadarWidget.generated.h"

/**
 *
 */
UCLASS()
class UE4_VISUALIZATION_API URadarWidget : public UUserWidget
{
	GENERATED_BODY()

public:
	UPROPERTY(EditAnywhere, Category = "Radar")
		FLinearColor BgColor1;

	UPROPERTY(EditAnywhere, Category = "Radar")
		FLinearColor BgColor2;

	UPROPERTY(EditAnywhere, Category = "Radar")
		int32 SideNumber = 6;

	UPROPERTY(EditAnywhere, Category = "Radar")
		int32 SegmentNumber = 5;

	UPROPERTY(EditAnywhere, Category = "Radar")
		float ERadius = 250;

	UPROPERTY(EditAnywhere, Category = "Radar")
		FVector2D CenterPosition = FVector2D(300.f, 300.f);

	UPROPERTY(EditAnywhere, Category = "Radar")
		float Thickness = 1.0f;

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
		bool bParentEnabled) const override;

private:
	void DrawPolygon(
		FSlateWindowElementList& OutDrawElements,
		const FGeometry& InGeometry,
		int InLayer,
		FVector2D InCenterPosition,
		int32 InSides,
		float IR,
		float ER,
		FColor InColor
	)const;

	TArray<FVector2D> Lines;
};
