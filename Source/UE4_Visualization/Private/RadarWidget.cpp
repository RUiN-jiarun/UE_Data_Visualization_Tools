// Fill out your copyright notice in the Description page of Project Settings.


#include "RadarWidget.h"

void URadarWidget::SetValues(TArray<float> InValues)
{
	if (InValues.Num() != SideNumber)
		return;

	Lines.Empty();

	float MaxValue = 0.0f;
	for (int32 Index = 0; Index < InValues.Num(); Index++)
	{
		if (InValues[Index] > MaxValue)
			MaxValue = InValues[Index];
	}

	float DeltaAngle = 360 / SideNumber;

	FVector2D FirstPosition;
	for (int32 Index = 0; Index < SideNumber; Index++)
	{
		float R = ERadius * (InValues[Index] / MaxValue);
		FVector2D CurrentPosition = FVector2D(
			CenterPosition.X + R * FMath::Cos(FMath::DegreesToRadians(DeltaAngle * Index)),
			CenterPosition.Y + R * FMath::Sin(FMath::DegreesToRadians(DeltaAngle * Index))
		);
		if (Index == 0)
			FirstPosition = CurrentPosition;
		Lines.Add(CurrentPosition);
	}
	Lines.Add(FirstPosition);

}

int32 URadarWidget::NativePaint(
	const FPaintArgs& Args,
	const FGeometry& AllottedGeometry,
	const FSlateRect& MyCullingRect,
	FSlateWindowElementList& OutDrawElements,
	int32 LayerId,
	const FWidgetStyle& InWidgetStyle,
	bool bParentEnabled) const
{

	if (SideNumber < 3)
		return LayerId;
	if (SegmentNumber < 1)
		return LayerId;

	float DeltaRadius = ERadius / SegmentNumber;
	for (int32 Index = 0; Index < SegmentNumber; Index++)
	{
		DrawPolygon(
			OutDrawElements,
			AllottedGeometry,
			LayerId,
			CenterPosition,
			SideNumber,
			DeltaRadius * Index,
			DeltaRadius * (Index + 1),
			(Index % 2) == 0 ? BgColor1.ToFColor(true) : BgColor2.ToFColor(true)
		);
	}

	if (Lines.Num() > 3)
		FSlateDrawElement::MakeLines(
			OutDrawElements, LayerId + 1, AllottedGeometry.ToPaintGeometry(), Lines, ESlateDrawEffect::None, FColor::Green, true, Thickness
		);

	LayerId += 2;
	return LayerId;
}

void URadarWidget::DrawPolygon(
	FSlateWindowElementList& OutDrawElements,
	const FGeometry& InGeometry,
	int InLayer,
	FVector2D InCenterPosition,
	int32 InSides,
	float IR,
	float ER,
	FColor InColor) const
{
	if (InSides < 3)
		return;

	TArray<FSlateVertex> SlateVertexs;
	TArray<SlateIndex> SlateIndexs;

	float DeltaAngle = 360 / InSides;

	for (int32 Angle = 0; Angle < 360; Angle += DeltaAngle)
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
			InCenterPosition.X + ER * FMath::Cos(FMath::DegreesToRadians(Angle)),
			InCenterPosition.Y + ER * FMath::Sin(FMath::DegreesToRadians(Angle))
		);

		FVector2D NextEVPosition = FVector2D(
			InCenterPosition.X + ER * FMath::Cos(FMath::DegreesToRadians(Angle + DeltaAngle)),
			InCenterPosition.Y + ER * FMath::Sin(FMath::DegreesToRadians(Angle + DeltaAngle))
		);

		FVector2D CurrentIVPosition = FVector2D(
			InCenterPosition.X + IR * FMath::Cos(FMath::DegreesToRadians(Angle)),
			InCenterPosition.Y + IR * FMath::Sin(FMath::DegreesToRadians(Angle))
		);

		FVector2D NextIVPosition = FVector2D(
			InCenterPosition.X + IR * FMath::Cos(FMath::DegreesToRadians(Angle + DeltaAngle)),
			InCenterPosition.Y + IR * FMath::Sin(FMath::DegreesToRadians(Angle + DeltaAngle))
		);

		CurrentEVPosition = InGeometry.ToPaintGeometry().GetAccumulatedRenderTransform().TransformPoint(CurrentEVPosition);
		NextEVPosition = InGeometry.ToPaintGeometry().GetAccumulatedRenderTransform().TransformPoint(NextEVPosition);
		CurrentIVPosition = InGeometry.ToPaintGeometry().GetAccumulatedRenderTransform().TransformPoint(CurrentIVPosition);
		NextIVPosition = InGeometry.ToPaintGeometry().GetAccumulatedRenderTransform().TransformPoint(NextIVPosition);

		CurrentEVertex.Position = CurrentEVPosition;
		NextEVertex.Position = NextEVPosition;
		CurrentIVertex.Position = CurrentIVPosition;
		NextIVertex.Position = NextIVPosition;

		int32 IndexOfCurrentEV = SlateVertexs.Add(CurrentEVertex);
		int32 IndexOfNextEV = SlateVertexs.Add(NextEVertex);
		int32 IndexOfCurrentIV = SlateVertexs.Add(CurrentIVertex);
		int32 IndexOfNextIV = SlateVertexs.Add(NextIVertex);

		SlateIndexs.Add(IndexOfCurrentIV);
		SlateIndexs.Add(IndexOfNextIV);
		SlateIndexs.Add(IndexOfNextEV);

		SlateIndexs.Add(IndexOfCurrentIV);
		SlateIndexs.Add(IndexOfNextEV);
		SlateIndexs.Add(IndexOfCurrentEV);
	}

	for (FSlateVertex& SlateVertex : SlateVertexs)
	{
		SlateVertex.TexCoords[0] = 0.0f;
		SlateVertex.TexCoords[1] = 0.0f;
	}


	const FSlateBrush* Brush = FCoreStyle::Get().GetBrush("ColorSpectrum.Spectrum");

	FSlateResourceHandle ResourceHandle = FSlateApplication::Get().GetRenderer()->GetResourceHandle(*Brush);
	FSlateDrawElement::MakeCustomVerts(
		OutDrawElements,
		InLayer,
		ResourceHandle,
		SlateVertexs,
		SlateIndexs,
		nullptr, 0, 0);
}
