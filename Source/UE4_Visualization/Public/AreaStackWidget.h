// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "Blueprint/UserWidget.h"
#include "AreaStackWidget.generated.h"

class Group
{
public:
	TArray<float> Values;
};

/**
 * 
 */
UCLASS()
class UE4_VISUALIZATION_API UAreaStackWidget : public UUserWidget
{
	GENERATED_BODY()
	
public:
	UFUNCTION(BlueprintCallable)
		void AddGroup(TArray<float> InValues);

	UPROPERTY(EditAnywhere, Category = "AreaStack")
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
	void DrawArea(
		FSlateWindowElementList& OutDrawElements,
		const FGeometry& InAllottedGeometry,
		int32 InLayerId,
		TArray<FVector2D> BegingPoints,
		TArray<FVector2D> EndPoints,
		FColor InColor
	)const;

	TArray<Group> Groups;

	void SetArrayToZero(TArray<float>& InValues, int32 NumberOfValuePerGroup)const;
};
