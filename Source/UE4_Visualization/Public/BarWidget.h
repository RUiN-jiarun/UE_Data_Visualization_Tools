// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "Blueprint/UserWidget.h"
#include "BarWidget.generated.h"


class Bar
{
public:
	FVector2D Position;
	FVector2D Size;
	float CurrentValue;
};
/**
 * 
 */
UCLASS()
class UE4_VISUALIZATION_API UBarWidget : public UUserWidget
{
	GENERATED_BODY()
	
public:
	UFUNCTION(BlueprintCallable)
		void SetValues(TArray<float> InValues);

	UPROPERTY(EditAnywhere, Category = "Bar")
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
	virtual void NativeTick(const FGeometry& MyGeometry, float InDeltaTime)override;
	virtual void NativeConstruct()override;
private:
	void DrawBar(
		FSlateWindowElementList& OutDrawElements,
		const FGeometry& InAllottedGeometry,
		int32 InLayerId,
		FVector2D InPosition,
		FVector2D InSize,
		FColor InColor
	)const;

	TArray<Bar> Bars;
};
