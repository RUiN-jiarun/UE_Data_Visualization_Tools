// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "Blueprint/UserWidget.h"
#include "DoughnutWidget.generated.h"

/**
 * 
 */
UCLASS()
class UE4_VISUALIZATION_API UDoughnutWidget : public UUserWidget
{
	GENERATED_BODY()

		

public:
	UFUNCTION(BlueprintCallable)
		void SetValues(TArray<float> InValues);
	UPROPERTY(EditAnywhere, Category = "Doughnut")
		FVector2D CenterPosition = FVector2D(300.f, 300.f);
	UPROPERTY(EditAnywhere, Category = "Doughnut")
		float IR = 100.f;
	UPROPERTY(EditAnywhere, Category = "Doughnut")
		float ER = 150.f;
	UPROPERTY(EditAnywhere, Category = "Doughnut")
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

	virtual FReply NativeOnMouseMove(const FGeometry& InGeometry, const FPointerEvent& InMouseEvent)override;
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
	
	TArray<float> Angles;

	FLinearColor GetColorByIndex(int Index)const;

	int IsInDoughnutPart(const FVector2D& InPoints)const;
	int32 Which = -1;
};
