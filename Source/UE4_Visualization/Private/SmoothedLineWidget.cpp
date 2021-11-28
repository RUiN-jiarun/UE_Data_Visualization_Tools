// Fill out your copyright notice in the Description page of Project Settings.


#include "SmoothedLineWidget.h"
#include "Components/CanvasPanelSlot.h"

void USmoothedLineWidget::SetValues(TArray<float> InValues)
{
	if (InValues.Num() < 2)
		return;

	MultiplyPoint.Empty();

	UCanvasPanelSlot* CanvsdPanelSlot = Cast<UCanvasPanelSlot>(this->Slot);
	if (CanvsdPanelSlot == nullptr)
		return;

	float WidgetWidth = CanvsdPanelSlot->GetSize().X;
	float WidgetHeight = CanvsdPanelSlot->GetSize().Y;

	float Space = WidgetWidth / (InValues.Num() - 1);
	for (int32 Index = 0; Index < InValues.Num(); Index++)
	{
		FVector2D KeyPosition(Space * Index, WidgetHeight - InValues[Index]);
		MultiplyPoint.Add(KeyPosition);
	}
}

int32 USmoothedLineWidget::NativePaint(const FPaintArgs& Args, const FGeometry& AllottedGeometry, const FSlateRect& MyCullingRect, FSlateWindowElementList& OutDrawElements, int32 LayerId, const FWidgetStyle& InWidgetStyle, bool bParentEnabled) const
{
	DrawSmoothedLine(
		OutDrawElements,
		LayerId,
		AllottedGeometry,
		MultiplyPoint,
		1.0f,
		FColor::Red);
	return LayerId++;
}

void USmoothedLineWidget::DrawSmoothedLine(
	FSlateWindowElementList& OutDrawElement,
	int InLayerId,
	const FGeometry& InAllottedGeometry,
	TArray<FVector2D> InPoints,
	float InThickness,
	FColor InColor) const
{
	if (InPoints.Num() < 2)
		return;

	FRichCurve* RichCurve = new FRichCurve();

	for (FVector2D InPoint : InPoints)
	{
		FKeyHandle KeyHandle = RichCurve->AddKey(InPoint.X, InPoint.Y);
		RichCurve->SetKeyInterpMode(KeyHandle, ERichCurveInterpMode::RCIM_Cubic);
	}

	UCanvasPanelSlot* CanvsdPanelSlot = Cast<UCanvasPanelSlot>(this->Slot);
	if (CanvsdPanelSlot == nullptr)
		return;

	TArray<FVector2D> ResultPoints;

	float WidgetWidth = CanvsdPanelSlot->GetSize().X;

	int32 Begin = 0;
	int32 End = (int32)WidgetWidth;
	for (int32 X = Begin; X < End; X++)
	{
		float Y = RichCurve->Eval(X);
		FVector2D ResultPoint(X, Y);
		ResultPoints.Add(ResultPoint);
	}

	delete RichCurve;

	FSlateDrawElement::MakeLines(
		OutDrawElement,
		InLayerId,
		InAllottedGeometry.ToPaintGeometry(),
		ResultPoints,
		ESlateDrawEffect::None,
		InColor,
		true,
		InThickness
	);
}
