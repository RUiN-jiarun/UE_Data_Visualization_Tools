// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "Blueprint/UserWidget.h"
#include "Widgets/SCanvas.h"
#include "Brushes/SlateColorBrush.h"
#include "ParallelNutrientsWidget.generated.h"

class Nutrient
{
public:
	FString name;
	FString group;
	double protein;
	double calcium;
	double sodium;
	double fiber;
	double vitaminc;
	double potassium;
	double carbohydrate;
	double sugars;
	double fat;
	double water;
	double calories;
	double saturated;
	double monounsat;
	double polyunsat;
	int32 id;

	TArray<double> ToValues()const
	{
		TArray<double> Results;
		Results.Add(id);
		Results.Add(protein);
		Results.Add(calcium);
		Results.Add(sodium);
		Results.Add(fiber);
		Results.Add(vitaminc);
		Results.Add(potassium);
		Results.Add(carbohydrate);
		Results.Add(sugars);
		Results.Add(fat);
		Results.Add(water);
		Results.Add(calories);
		Results.Add(saturated);
		Results.Add(monounsat);
		Results.Add(polyunsat);
		return Results;
	}
};

class Ruler
{
public:
	Ruler(
		const FVector2D& InPosition,
		const FVector2D& InSize,
		float InBeginScale,
		float InEndScale,
		float InSegment,
		const FString& InTitle
	)
		:Position(InPosition)
		,Size(InSize)
		,BeginScale(InBeginScale)
		,EndScale(InEndScale)
		,Segment(InSegment)
		,Title(InTitle)
	{

	}
	FVector2D Position;
	FVector2D Size;
	float BeginScale;
	float EndScale;
	int Segment;
	FString Title;
};
/**
 * 
 */
UCLASS()
class UE4_VISUALIZATION_API UParallelNutrientsWidget : public UUserWidget
{
	GENERATED_UCLASS_BODY()
	
public:
	UFUNCTION(BlueprintCallable)
		void SetValue(const FString& InValue);

protected:
	virtual TSharedRef<SWidget> RebuildWidget() override;
	virtual int32 NativePaint(
		const FPaintArgs& Args,
		const FGeometry& AllottedGeometry, 
		const FSlateRect& MyCullingRect, 
		FSlateWindowElementList& OutDrawElements, 
		int32 LayerId, 
		const FWidgetStyle& InWidgetStyle, 
		bool bParentEnabled) const override;

private:
	void AddLegends(
		TSharedPtr<SCanvas> InCanvas
	);

	void DrawRuler(
		const FGeometry& InAllottedGeometry,
		FSlateWindowElementList& OutDrawElements,
		int32 InLayerId,
		const Ruler& InRuler
	)const;

	void DrawSmoothedLine(
		const FGeometry& InAllottedGeometry,
		FSlateWindowElementList& OutDrawElements,
		int32 InLayerId,
		float InThickness,
		const FLinearColor& InLinearColor,
		const Nutrient& InNutrient
	)const;

	TArray<Nutrient> Nutrients;

	TMap<FString,FLinearColor> GroupColorMap;
	TMap<FString, FSlateColorBrush> GroupSlateColorBrushMap;

	TSharedPtr<SBorder> Border;

	TArray<Ruler> Rulers;

	const FLinearColor& GetLinearColorByGroup(const FString& InGroup)const;
	const FSlateColorBrush& GetSlateColorBrushByGroup(const FString& InGroup)const;
};
