// Fill out your copyright notice in the Description page of Project Settings.


#include "BarWidget.h"

void UBarWidget::SetValues(TArray<float> InValues)
{
	if (InValues.Num() < 1)
		return;

	Bars.Empty();

	float Space = 150;
	float BarWidth = 100;

	for (int32 Index = 0; Index < InValues.Num(); Index++)
	{
		Bar B;
		B.Position = FVector2D(Space * Index, 500);
		B.Size = FVector2D(BarWidth, InValues[Index]);
		B.CurrentValue = 0.0f;

		Bars.Add(B);
	}

}

int32 UBarWidget::NativePaint(
	const FPaintArgs& Args, 
	const FGeometry& AllottedGeometry,
	const FSlateRect& MyCullingRect,
	FSlateWindowElementList& OutDrawElements,
	int32 LayerId, 
	const FWidgetStyle& InWidgetStyle,
	bool bParentEnabled) const
{
	for (int32 Index = 0; Index < Bars.Num(); Index++)
	{
		const Bar& B = Bars[Index];

		FColor Color;
		if (Colors.Num() == 0)
		{
			Color = FColor::White;
		}
		else
		{
			Color = Colors[Index % Colors.Num()].ToFColor(true);
		}
		DrawBar(OutDrawElements, AllottedGeometry, LayerId, B.Position, FVector2D(B.Size.X,B.CurrentValue),Color );
	}

	return LayerId++;
}

void UBarWidget::NativeTick(const FGeometry& MyGeometry, float InDeltaTime)
{
	Super::NativeTick(MyGeometry,InDeltaTime);

	for (int32 Index = 0; Index < Bars.Num(); Index++)
	{
		Bar& B = Bars[Index];
		if(FMath::Abs(B.CurrentValue - B.Size.Y) <=1.0f)
			continue;
		if (B.CurrentValue > B.Size.Y) 
		{
			float FinalSpeed = 300 * InDeltaTime;
			B.CurrentValue -= FinalSpeed;
		}
			
		else
		{
			float FinalSpeed = 300 * InDeltaTime;
			B.CurrentValue += FinalSpeed;
		}
			
	}
}

void UBarWidget::NativeConstruct()
{
	Super::NativeConstruct();
}

void UBarWidget::DrawBar(
	FSlateWindowElementList& OutDrawElements, 
	const FGeometry& InAllottedGeometry, 
	int32 InLayerId, 
	FVector2D InPosition, 
	FVector2D InSize, 
	FColor InColor) const
{
	FSlateVertex LeftDownVertex;
	FSlateVertex RighDownVertex;
	FSlateVertex RightUpVertex;
	FSlateVertex LeftUpVertex;

	LeftDownVertex.Color = InColor;
	RighDownVertex.Color = InColor;
	RightUpVertex.Color = InColor;
	LeftUpVertex.Color = InColor;

	FVector2D LeftDownPosition = InPosition;
	FVector2D RightDownPosition = FVector2D(InPosition.X + InSize.X, InPosition.Y);
	FVector2D RightUpPosition = FVector2D(InPosition.X + InSize.X, InPosition.Y - InSize.Y);
	FVector2D LeftUpPosition = FVector2D(InPosition.X, InPosition.Y - InSize.Y);

	const FSlateRenderTransform& SlateRenderTransform = InAllottedGeometry.ToPaintGeometry().GetAccumulatedRenderTransform();

	LeftDownPosition = SlateRenderTransform.TransformPoint(LeftDownPosition);
	RightDownPosition = SlateRenderTransform.TransformPoint(RightDownPosition);
	RightUpPosition = SlateRenderTransform.TransformPoint(RightUpPosition);
	LeftUpPosition = SlateRenderTransform.TransformPoint(LeftUpPosition);

	LeftDownVertex.Position = LeftDownPosition;
	RighDownVertex.Position = RightDownPosition;
	RightUpVertex.Position = RightUpPosition;
	LeftUpVertex.Position = LeftUpPosition;

	TArray<FSlateVertex> SlateVertexes;

	int32 IndexOfLeftDown = SlateVertexes.Add(LeftDownVertex);
	int32 IndexOfRightDown = SlateVertexes.Add(RighDownVertex);
	int32 IndexOfRightUp = SlateVertexes.Add(RightUpVertex);
	int32 IndexOfLeftUp = SlateVertexes.Add(LeftUpVertex);

	TArray<SlateIndex> SlateIndices;
	
	SlateIndices.Add(IndexOfLeftDown);
	SlateIndices.Add(IndexOfRightDown);
	SlateIndices.Add(IndexOfRightUp);

	SlateIndices.Add(IndexOfLeftDown);
	SlateIndices.Add(IndexOfRightUp);
	SlateIndices.Add(IndexOfLeftUp);

	for (FSlateVertex& SlateVertex : SlateVertexes)
	{
		SlateVertex.TexCoords[0] = 0.0f;
		SlateVertex.TexCoords[1] = 0.0f;
	}

	const FSlateBrush* Brush = FCoreStyle::Get().GetBrush("ColorSpectrum.Spectrum");
	FSlateResourceHandle ResourceHandle = FSlateApplication::Get().GetRenderer()->GetResourceHandle(*Brush);
	FSlateDrawElement::MakeCustomVerts(
		OutDrawElements,
		InLayerId,
		ResourceHandle,
		SlateVertexes,
		SlateIndices,
		nullptr,
		0,
		0
	);
}
