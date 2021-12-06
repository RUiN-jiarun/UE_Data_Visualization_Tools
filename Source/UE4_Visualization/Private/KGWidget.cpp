// Fill out your copyright notice in the Description page of Project Settings.


#include "KGWidget.h"

void UKGWidget::setValue(const FString& InValue)
{
	if (InValue.IsEmpty())
		return;

	TSharedRef< TJsonReader<> > Reader = TJsonReaderFactory<>::Create(InValue);

	TSharedPtr<FJsonValue> KGJsonValue;
	if (FJsonSerializer::Deserialize(Reader, KGJsonValue))
	{
		TSharedPtr<FJsonObject> KGJsonObject = KGJsonValue->AsObject();
		if (KGJsonObject->HasField(TEXT("nodes")))
		{
			const TArray<TSharedPtr<FJsonValue>> NodeJsonValues = KGJsonObject->GetArrayField(TEXT("nodes"));
			for (TSharedPtr<FJsonValue> NodeJsonValue : NodeJsonValues)
			{
				LNode LN;
				TSharedPtr<FJsonObject> NodeJsonObject = NodeJsonValue->AsObject();
				LN.id = NodeJsonObject->GetStringField(TEXT("id"));
				LN.name = NodeJsonObject->GetStringField(TEXT("name"));
				LN.symbolSize = NodeJsonObject->GetNumberField(TEXT("symbolSize"));
				LN.x = NodeJsonObject->GetNumberField(TEXT("x"));
				LN.y = NodeJsonObject->GetNumberField(TEXT("y"));
				LN.value = NodeJsonObject->GetNumberField(TEXT("value"));
				LN.category = NodeJsonObject->GetIntegerField(TEXT("category"));

				LNodes.Add(LN.id, LN);
			}
		}
		if (KGJsonObject->HasField(TEXT("links")))
		{
			const TArray<TSharedPtr<FJsonValue>> LinkJsonValues = KGJsonObject->GetArrayField(TEXT("links"));
			for (TSharedPtr<FJsonValue> LinkJsonValue : LinkJsonValues)
			{
				LLink LL;
				TSharedPtr<FJsonObject> LinkJsonObject = LinkJsonValue->AsObject();
				LL.source = LinkJsonObject->GetStringField(TEXT("source"));
				LL.target = LinkJsonObject->GetStringField(TEXT("target"));

				LLinks.Add(LL);
			}
		}
		if (KGJsonObject->HasField(TEXT("categories")))
		{
			const TArray<TSharedPtr<FJsonValue>> CategoryJsonValues = KGJsonObject->GetArrayField(TEXT("categories"));
			for (TSharedPtr<FJsonValue> CategoryJsonValue : CategoryJsonValues)
			{
				LCategory LC;
				TSharedPtr<FJsonObject> CategoryJsonObject = CategoryJsonValue->AsObject();
				LC.name = CategoryJsonObject->GetStringField(TEXT("name"));
				
				LCategories.Add(LC);
			}
		}
	}
}

int32 UKGWidget::NativePaint(
	const FPaintArgs& Args, 
	const FGeometry& AllottedGeometry,
	const FSlateRect& MyCullingRect, 
	FSlateWindowElementList& OutDrawElements,
	int32 LayerId, 
	const FWidgetStyle& InWidgetStyle,
	bool bParentEnabled) const
{
	//           0 TargetNode               |
	//           |                          |
	//           |                          |
	//           |_____0  Offset2           |
	//           |                          Length
	//           |                          |
	//           |_____0  Offset1           |
	//           |                          |
	//          /|\                          |
	//           |                          |
	//           0  SourceNode              |

	for (const LLink& Link : LLinks)
	{
		const LNode& SourceNode = LNodes[Link.source];
		const LNode& TargetNode = LNodes[Link.target];

		FVector2D SourceNodePosition(SourceNode.x, SourceNode.y);
		FVector2D TargetNodePosition(TargetNode.x, TargetNode.y);

		FVector2D Direction = TargetNodePosition - SourceNodePosition;
		float Length = Direction.Size();

		FVector2D Normal = Direction.GetSafeNormal();
		FVector2D VDirection = Normal.GetRotated(90);
		VDirection = -VDirection;

		FVector2D OffsetPosition1 = SourceNodePosition + Normal * Length * 0.3333 + VDirection * Length * 0.25;
		FVector2D OffsetPosition2 = SourceNodePosition + Normal * Length * 0.6667 + VDirection * Length * 0.25;

		FVector2D BezierPoint0(SourceNodePosition);
		FVector2D BezierPoint1(OffsetPosition1);
		FVector2D BezierPoint2(OffsetPosition2);
		FVector2D BezierPoint3(TargetNodePosition);

		FSlateDrawElement::MakeCubicBezierSpline(
			OutDrawElements,
			LayerId,
			AllottedGeometry.ToPaintGeometry(),
			BezierPoint0, BezierPoint1, BezierPoint2, BezierPoint3,
			0.5f,
			ESlateDrawEffect::None,
			GetColorByCategory(SourceNode.category)
		);
	}

	return LayerId++;
}

FLinearColor UKGWidget::GetColorByCategory(int InCategory) const
{
	if (InCategory < 0 || InCategory >= Colors.Num())
		return FLinearColor::White;

	return Colors[InCategory % Colors.Num()];
}
