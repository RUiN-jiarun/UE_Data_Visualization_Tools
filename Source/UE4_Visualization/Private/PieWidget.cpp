// Fill out your copyright notice in the Description page of Project Settings.


#include "PieWidget.h"

void UPieWidget::SetValues(TArray<float> InValues)
{
	if (InValues.Num() < 1)
		return;

	Angles.Empty();

	Angles.Add(0);

	float Total = 0;
	for (int32 Index = 0; Index < InValues.Num(); Index++)
	{
		Total += InValues[Index];
	}

	float CurrentTotal = 0;
	for (int32 Index = 0; Index < InValues.Num(); Index++)
	{
		CurrentTotal += InValues[Index];
		int32 Angle = int32((CurrentTotal / Total) * 360);
		Angles.Add(Angle);
	}
}

void UPieWidget::SetColors(TArray<FLinearColor> InColors)
{
	Colors = InColors;
}

int32 UPieWidget::NativePaint(const FPaintArgs& Args, const FGeometry& AllottedGeometry, const FSlateRect& MyCullingRect, FSlateWindowElementList& OutDrawElements, int32 LayerId, const FWidgetStyle& InWidgetStyle, bool bParentEnabled) const
{
	for (int32 Index = 0; Index < Angles.Num() - 1; Index++)
	{
		int32 BegingAngle = Angles[Index];
		int32 EndAngle = Angles[Index + 1];
		DrawFan(OutDrawElements, AllottedGeometry, LayerId, FVector2D(300.f, 300.f), 250.f, BegingAngle, EndAngle, GetColorByIndex(Index));
	}

	return LayerId++;
}

void UPieWidget::DrawFan(
	FSlateWindowElementList& OutDrawElements,
	const FGeometry& AllottedGeometry,
	int32 Layer,
	const FVector2D& CenterPosition,
	float Radius,
	int32 BeginAngle,
	int32 EndAngle,
	FColor Color) const
{
	if (EndAngle <= BeginAngle)
		return;
	if (Radius <= 0.f)
		return;

	TArray<FSlateVertex> SlateVertexArray;
	TArray<SlateIndex> SlateIndexArray;

	for (int32 CurrentAngle = BeginAngle; CurrentAngle < EndAngle; CurrentAngle++)
	{
		FSlateVertex CurrentSlateVertex;
		FSlateVertex NextSlateVertex;
		FSlateVertex CenterSlateVertex;

		FVector2D CurrentSlateVertextPosition = FVector2D(
			CenterPosition.X + Radius * FMath::Cos(FMath::DegreesToRadians(CurrentAngle)),
			CenterPosition.Y + Radius * FMath::Sin(FMath::DegreesToRadians(CurrentAngle))
		);

		FVector2D NextSlateVertextPosition = FVector2D(
			CenterPosition.X + Radius * FMath::Cos(FMath::DegreesToRadians(CurrentAngle + 1)),
			CenterPosition.Y + Radius * FMath::Sin(FMath::DegreesToRadians(CurrentAngle + 1))
		);

		CurrentSlateVertex.Position = AllottedGeometry.ToPaintGeometry().GetAccumulatedRenderTransform().TransformPoint(CurrentSlateVertextPosition);
		NextSlateVertex.Position = AllottedGeometry.ToPaintGeometry().GetAccumulatedRenderTransform().TransformPoint(NextSlateVertextPosition);
		CenterSlateVertex.Position = AllottedGeometry.ToPaintGeometry().GetAccumulatedRenderTransform().TransformPoint(CenterPosition);

		CurrentSlateVertex.Color = Color;
		NextSlateVertex.Color = Color;
		CenterSlateVertex.Color = Color;

		int32 IndexOfCurrentSlateVertex = SlateVertexArray.Add(CurrentSlateVertex);
		int32 IndexOfNextSlateVertex = SlateVertexArray.Add(NextSlateVertex);
		int32 IndexOfCenterSlateVertex = SlateVertexArray.Add(CenterSlateVertex);

		SlateIndexArray.Add(IndexOfCurrentSlateVertex);
		SlateIndexArray.Add(IndexOfCenterSlateVertex);
		SlateIndexArray.Add(IndexOfNextSlateVertex);
	}

	for (FSlateVertex& TempSlateVertex : SlateVertexArray)
	{
		TempSlateVertex.TexCoords[0] = 0.0f;
		TempSlateVertex.TexCoords[1] = 0.0f;
		TempSlateVertex.TexCoords[2] = 0.0f;
		TempSlateVertex.TexCoords[3] = 0.0f;
	}

	const FSlateBrush* SlateBrush = FCoreStyle::Get().GetBrush("ColorSpectrum.Spectrum");
	FSlateResourceHandle SlateResourceHandle = FSlateApplication::Get().GetRenderer()->GetResourceHandle(*SlateBrush);

	FSlateDrawElement::MakeCustomVerts(
		OutDrawElements,
		Layer,
		SlateResourceHandle,
		SlateVertexArray,
		SlateIndexArray,
		nullptr, 0, 0
	);
}

FColor UPieWidget::GetColorByIndex(int32 InIndex) const
{
	if (Colors.Num() < 1)
		return FColor::White;

	return Colors[InIndex % Colors.Num()].ToFColor(true);
}

