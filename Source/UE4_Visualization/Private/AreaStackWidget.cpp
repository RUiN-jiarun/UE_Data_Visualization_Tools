// Fill out your copyright notice in the Description page of Project Settings.


#include "AreaStackWidget.h"

void UAreaStackWidget::AddGroup(TArray<float> InValues)
{
	if (InValues.Num() < 2)
		return;
	if (Groups.Num() > 0)
	{
		if (Groups[0].Values.Num() != InValues.Num())
			return;
	}

	Group G;

	G.Values = InValues;

	Groups.Add(G);
}

int32 UAreaStackWidget::NativePaint(
	const FPaintArgs& Args,
	const FGeometry& AllottedGeometry,
	const FSlateRect& MyCullingRect,
	FSlateWindowElementList& OutDrawElements,
	int32 LayerId,
	const FWidgetStyle& InWidgetStyle,
	bool bParentEnabled) const
{

	if (Groups.Num() < 1)
		return LayerId;

	int32 NumberOfValuePerGroup = Groups[0].Values.Num();

	TArray<float> AccumulatedValues;
	SetArrayToZero(AccumulatedValues, NumberOfValuePerGroup);

	float Space = 150.f;

	for (int32 IndexOfGroup = 0; IndexOfGroup < Groups.Num(); IndexOfGroup++)
	{
		TArray<FVector2D> BeginPoints;
		TArray<FVector2D> EndPoints;
		for (int32 IndexOfValue = 0; IndexOfValue < NumberOfValuePerGroup; IndexOfValue++)
		{
			FVector2D BeginPoint = FVector2D(Space * IndexOfValue, 1000 - AccumulatedValues[IndexOfValue]);
			AccumulatedValues[IndexOfValue] = AccumulatedValues[IndexOfValue] + Groups[IndexOfGroup].Values[IndexOfValue];
			FVector2D EndPoint = FVector2D(Space * IndexOfValue, 1000 - AccumulatedValues[IndexOfValue]);
			BeginPoints.Add(BeginPoint);
			EndPoints.Add(EndPoint);

			if (IndexOfGroup == Groups.Num() - 1)
			{
				FSlateFontInfo SlateFontInfo = FCoreStyle::GetDefaultFontStyle("Regular", 10);
				FSlateDrawElement::MakeText(
					OutDrawElements,
					LayerId + 1,
					AllottedGeometry.ToOffsetPaintGeometry(FVector2D(EndPoint.X - 10, EndPoint.Y - 20)),
					FText::FromString(FString::FromInt(AccumulatedValues[IndexOfValue])),
					SlateFontInfo,
					ESlateDrawEffect::None,
					FLinearColor::White
				);
			}
		}

		FColor Color;
		if (Colors.Num() < 1)
			Color = FColor::White;
		else
			Color = Colors[IndexOfGroup % Colors.Num()].ToFColor(true);
		DrawArea(OutDrawElements, AllottedGeometry, LayerId, BeginPoints, EndPoints, Color);
	}
	/*TArray<FVector2D> BeginPoints;
	BeginPoints.Add(FVector2D(0,100));
	BeginPoints.Add(FVector2D(100, 150));
	BeginPoints.Add(FVector2D(200, 50));
	TArray<FVector2D> EndPoints;
	EndPoints.Add(FVector2D(0, 110));
	EndPoints.Add(FVector2D(100, 250));
	EndPoints.Add(FVector2D(200, 150));*/

	return LayerId++;
}

void UAreaStackWidget::DrawArea(
	FSlateWindowElementList& OutDrawElements,
	const FGeometry& InAllottedGeometry,
	int32 InLayerId,
	TArray<FVector2D> BegingPoints,
	TArray<FVector2D> EndPoints,
	FColor InColor) const
{
	if (BegingPoints.Num() != EndPoints.Num())
		return;

	if (BegingPoints.Num() < 2)
		return;

	TArray<FSlateVertex> SlateVertexes;
	TArray<SlateIndex> SlateIndices;

	for (int32 Index = 0; Index < BegingPoints.Num() - 1; Index++)
	{
		FSlateVertex CurrentBeginVertex;
		FSlateVertex NextBeginVertex;
		FSlateVertex CurrentEndVertex;
		FSlateVertex NextEndVertex;

		CurrentBeginVertex.Color = InColor;
		NextBeginVertex.Color = InColor;
		CurrentEndVertex.Color = InColor;
		NextEndVertex.Color = InColor;

		FVector2D CurrentBeginVertexPosition = BegingPoints[Index];
		FVector2D NextBeginVertexPosition = BegingPoints[Index + 1];
		FVector2D CurrentEndVertexPosition = EndPoints[Index];
		FVector2D NextEndVertexPosition = EndPoints[Index + 1];

		const FSlateRenderTransform& T = InAllottedGeometry.ToPaintGeometry().GetAccumulatedRenderTransform();

		CurrentBeginVertexPosition = T.TransformPoint(CurrentBeginVertexPosition);
		NextBeginVertexPosition = T.TransformPoint(NextBeginVertexPosition);
		CurrentEndVertexPosition = T.TransformPoint(CurrentEndVertexPosition);
		NextEndVertexPosition = T.TransformPoint(NextEndVertexPosition);

		CurrentBeginVertex.Position = CurrentBeginVertexPosition;
		NextBeginVertex.Position = NextBeginVertexPosition;
		CurrentEndVertex.Position = CurrentEndVertexPosition;
		NextEndVertex.Position = NextEndVertexPosition;

		int32 IndexOfCurrentBeginVertex = SlateVertexes.Add(CurrentBeginVertex);
		int32 IndexOfNextBeginVertex = SlateVertexes.Add(NextBeginVertex);
		int32 IndexOfCurrentEndVertex = SlateVertexes.Add(CurrentEndVertex);
		int32 IndexOfNextEndVertex = SlateVertexes.Add(NextEndVertex);

		SlateIndices.Add(IndexOfCurrentBeginVertex);
		SlateIndices.Add(IndexOfNextBeginVertex);
		SlateIndices.Add(IndexOfNextEndVertex);

		SlateIndices.Add(IndexOfCurrentBeginVertex);
		SlateIndices.Add(IndexOfNextEndVertex);
		SlateIndices.Add(IndexOfCurrentEndVertex);

	}

	for (FSlateVertex& SlateVertex : SlateVertexes)
	{
		SlateVertex.TexCoords[0] = 0.0f;
		SlateVertex.TexCoords[1] = 0.0f;
	}

	const FSlateBrush* MyBrush = FCoreStyle::Get().GetBrush("ColorSpectrum.Spectrum");
	FSlateResourceHandle Handle = FSlateApplication::Get().GetRenderer()->GetResourceHandle(*MyBrush);
	FSlateDrawElement::MakeCustomVerts(
		OutDrawElements,
		InLayerId,
		Handle,
		SlateVertexes,
		SlateIndices,
		nullptr,
		0,
		0
	);
}

void UAreaStackWidget::SetArrayToZero(TArray<float>& InValues, int32 NumberOfValuePerGroup) const
{
	for (int32 Index = 0; Index < NumberOfValuePerGroup; Index++)
	{
		InValues.Add(0);
	}
}
