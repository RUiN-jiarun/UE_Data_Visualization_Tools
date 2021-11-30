// Fill out your copyright notice in the Description page of Project Settings.


#include "NightingaleWidget.h"

void UNightingaleWidget::SetValue(TArray<float> InValues)
{
	if (InValues.Num() < 1)
		return;

	ERadius = InValues;
}

int32 UNightingaleWidget::NativePaint(
	const FPaintArgs& Args, 
	const FGeometry& AllottedGeometry, 
	const FSlateRect& MyCullingRect, 
	FSlateWindowElementList& OutDrawElements,
	int32 LayerId, 
	const FWidgetStyle& InWidgetStyle,
	bool bParentEnabled) const
{
	if (ERadius.Num() == 0)
		return LayerId;

	int32 Angle = 360 / ERadius.Num();

	for (int32 Index = 0; Index < ERadius.Num(); Index++)
	{
		DrawDoughnutPart(
			OutDrawElements,
			AllottedGeometry,
			LayerId,
			IR,
			ERadius[Index],
			Angle*Index,
			Angle*(Index + 1),
			GetColorByIndex(Index).ToFColor(true)
		);
	}
	
	return LayerId;
}

void UNightingaleWidget::DrawDoughnutPart(
	FSlateWindowElementList& OutDrawElements,
	const FGeometry& InAllottedGeometry,
	int InLayerId,
	float InIR,
	float InER,
	int32 InBeginAngle,
	int32 InEndAngle,
	const FColor& InColor) const
{
	if (InEndAngle <= InBeginAngle)
		return;

	TArray<FSlateVertex> SlateVertexes;
	TArray<SlateIndex> SlateIndices;

	for (int32 Angle = InBeginAngle; Angle < InEndAngle; Angle++)
	{
		FSlateVertex CurrentEVertex;
		FSlateVertex NextEVertex;
		FSlateVertex CurrentIVertex;
		FSlateVertex NextIVertex;

		CurrentEVertex.Color = InColor;
		NextEVertex.Color = InColor;
		CurrentIVertex.Color = InColor;
		NextIVertex.Color = InColor;

		FVector2D CurrentEVPosition = FVector2D(
			CenterPosition.X + InER * FMath::Cos(FMath::DegreesToRadians(Angle)),
			CenterPosition.Y - InER * FMath::Sin(FMath::DegreesToRadians(Angle))
		);

		FVector2D NexEVPosition = FVector2D(
			CenterPosition.X + InER * FMath::Cos(FMath::DegreesToRadians(Angle + 1)),
			CenterPosition.Y - InER * FMath::Sin(FMath::DegreesToRadians(Angle + 1))
		);

		FVector2D CurrentIVPosition = FVector2D(
			CenterPosition.X + InIR * FMath::Cos(FMath::DegreesToRadians(Angle)),
			CenterPosition.Y - InIR * FMath::Sin(FMath::DegreesToRadians(Angle))
		);

		FVector2D NextIVPosition = FVector2D(
			CenterPosition.X + InIR * FMath::Cos(FMath::DegreesToRadians(Angle + 1)),
			CenterPosition.Y - InIR * FMath::Sin(FMath::DegreesToRadians(Angle + 1))
		);

		const FSlateRenderTransform& SlateRenderTransform = InAllottedGeometry.ToPaintGeometry().GetAccumulatedRenderTransform();
		CurrentEVPosition = SlateRenderTransform.TransformPoint(CurrentEVPosition);
		NexEVPosition = SlateRenderTransform.TransformPoint(NexEVPosition);
		CurrentIVPosition = SlateRenderTransform.TransformPoint(CurrentIVPosition);
		NextIVPosition = SlateRenderTransform.TransformPoint(NextIVPosition);

		CurrentEVertex.Position = CurrentEVPosition;
		NextEVertex.Position = NexEVPosition;
		CurrentIVertex.Position = CurrentIVPosition;
		NextIVertex.Position = NextIVPosition;

		int32 IndexOfCurrentEV = SlateVertexes.Add(CurrentEVertex);
		int32 IndexOfNextEV = SlateVertexes.Add(NextEVertex);
		int32 IndexOfCurrentIV = SlateVertexes.Add(CurrentIVertex);
		int32 IndexOfNextIV = SlateVertexes.Add(NextIVertex);

		SlateIndices.Add(IndexOfCurrentIV);
		SlateIndices.Add(IndexOfNextIV);
		SlateIndices.Add(IndexOfNextEV);

		SlateIndices.Add(IndexOfCurrentIV);
		SlateIndices.Add(IndexOfNextEV);
		SlateIndices.Add(IndexOfCurrentEV);
	}

	for (FSlateVertex& SlateVertex : SlateVertexes)
	{
		SlateVertex.TexCoords[0] = 0.0f;
		SlateVertex.TexCoords[1] = 0.0f;
	}

	const FSlateBrush* Brush = FCoreStyle::Get().GetBrush("ColorSpectrum.Spectrum");
	FSlateResourceHandle ResourceHandle =
		FSlateApplication::Get().GetRenderer()->GetResourceHandle(*Brush);
	FSlateDrawElement::MakeCustomVerts(
		OutDrawElements,
		InLayerId,
		ResourceHandle,
		SlateVertexes,
		SlateIndices,
		nullptr, 0, 0);
}

FLinearColor UNightingaleWidget::GetColorByIndex(int Index) const
{
	if (Index < 0 || Index >= Colors.Num())
		return FLinearColor::White;

	return Colors[Index % Colors.Num()];
}