// Fill out your copyright notice in the Description page of Project Settings.


#include "DoughnutWidget.h"

void UDoughnutWidget::SetValues(TArray<float> InValues)
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

		int Angle = (int32)((CurrentTotal / Total) * 360);
		Angles.Add(Angle);
	}
}

int32 UDoughnutWidget::NativePaint(
	const FPaintArgs& Args, 
	const FGeometry& AllottedGeometry,
	const FSlateRect& MyCullingRect,
	FSlateWindowElementList& OutDrawElements, 
	int32 LayerId, 
	const FWidgetStyle& InWidgetStyle,
	bool bParentEnabled) const
{
	for (int32 Index = 0; Index < Angles.Num() - 1; Index++)
	{
		int32 BeginAngle = Angles[Index];
		int32 EndAngle = Angles[Index + 1];
		DrawDoughnutPart(
			OutDrawElements,
			AllottedGeometry,
			LayerId,
			IR,
			Index == Which ? ER + 10 : ER,
			BeginAngle,
			EndAngle,
			GetColorByIndex(Index).ToFColor(true)
		);
	}
	
	return LayerId++;
}

FReply UDoughnutWidget::NativeOnMouseMove(const FGeometry& InGeometry, const FPointerEvent& InMouseEvent)
{
	FVector2D PixelUnderMouse = InGeometry.AbsoluteToLocal(InMouseEvent.GetScreenSpacePosition());
	Which = IsInDoughnutPart(PixelUnderMouse);

	return Super::NativeOnMouseMove(InGeometry, InMouseEvent);
}

void UDoughnutWidget::DrawDoughnutPart(
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

	for (int32 Angle = InBeginAngle; Angle <= InEndAngle; Angle++)
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

		//第一个三角
		SlateIndices.Add(IndexOfCurrentIV);
		SlateIndices.Add(IndexOfNextIV);
		SlateIndices.Add(IndexOfNextEV);

		//第二个三角
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

FLinearColor UDoughnutWidget::GetColorByIndex(int Index) const
{
	if (Index < 0 || Index >= Colors.Num())
		return FLinearColor::White;

	return Colors[Index % Colors.Num()];
}

int UDoughnutWidget::IsInDoughnutPart(const FVector2D& InPoints) const
{
	FVector2D TempInPoint = FVector2D(InPoints.X, -InPoints.Y);
	FVector2D TempCenterPosition = FVector2D(CenterPosition.X, -CenterPosition.Y);

	FVector2D Direction = TempInPoint - TempCenterPosition;
	float LenghtSquared = Direction.SizeSquared();

	//判断在大圈里
	if (LenghtSquared > ER * ER)
		return -1;
	//判断在小圈外
	if (LenghtSquared < IR * IR)
		return -2;

	FVector2D NormalDirection = Direction.GetSafeNormal();

	// NormalDirection.X / 1 = cos@
	//                y
	//                |    InPoint
	//                |  /|
	//                | / |
	//       _________|/__|___________x
	//                |CenterPosition
	//                |
	//                |
	//                |
	
	float Theta = FMath::Acos(NormalDirection.X);

	if (NormalDirection.Y < 0)
		Theta = PI * 2 - Theta;

	float Degree = FMath::RadiansToDegrees(Theta);

	for (int32 Index = 0; Index < Angles.Num() - 1; Index++)
	{
		if (Degree < Angles[Index + 1])
			return Index;
	}

	return -3;
}
