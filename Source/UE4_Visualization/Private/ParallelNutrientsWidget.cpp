// Fill out your copyright notice in the Description page of Project Settings.


#include "ParallelNutrientsWidget.h"
#include "Engine/Font.h"
#include "Widgets/Images/SImage.h"

UParallelNutrientsWidget::UParallelNutrientsWidget(const FObjectInitializer& ObjectInitializer)
	:UUserWidget(ObjectInitializer)
{
	float LeftOffset = 350;
	float TopOffset = 50;
	float Width = 100;
	float Height = 600;

	Ruler Ruler1(FVector2D(LeftOffset + 0, TopOffset + Height), FVector2D(Width, Height), 26000.f, 36000.f, 5, "id");
	Ruler Ruler2(FVector2D(LeftOffset + 100, TopOffset + Height), FVector2D(Width, Height), 0.f, 100.f, 5, "protein");
	Ruler Ruler3(FVector2D(LeftOffset + 200, TopOffset + Height), FVector2D(Width, Height), 0.f, 40.f, 4, "sodium");
	Ruler Ruler4(FVector2D(LeftOffset + 300, TopOffset + Height), FVector2D(Width, Height), 0.f, 8.f, 8, "calcium");
	Ruler Ruler5(FVector2D(LeftOffset + 400, TopOffset + Height), FVector2D(Width, Height), 0.f, 80.f, 4, "fiber");
	Ruler Ruler6(FVector2D(LeftOffset + 500, TopOffset + Height), FVector2D(Width, Height), 0.f, 2.5f, 5, "vitaminc");
	Ruler Ruler7(FVector2D(LeftOffset + 600, TopOffset + Height), FVector2D(Width, Height), 0.f, 18.f, 6, "potassium");
	Ruler Ruler8(FVector2D(LeftOffset + 700, TopOffset + Height), FVector2D(Width, Height), 0.f, 100.f, 5, "carbohydrate");
	Ruler Ruler9(FVector2D(LeftOffset + 800, TopOffset + Height), FVector2D(Width, Height), 0.f, 100.f, 5, "sugars");
	Ruler Ruler10(FVector2D(LeftOffset + 900, TopOffset + Height), FVector2D(Width, Height), 0.f, 100.f, 5, "fat");
	Ruler Ruler11(FVector2D(LeftOffset + 1000, TopOffset + Height), FVector2D(Width, Height), 0.f, 100.f, 5, "water");
	Ruler Ruler12(FVector2D(LeftOffset + 1100, TopOffset + Height), FVector2D(Width, Height), 0.f, 1000.f, 5, "calories");
	Ruler Ruler13(FVector2D(LeftOffset + 1200, TopOffset + Height), FVector2D(Width, Height), 0.f, 100.f, 5, "saturated");
	Ruler Ruler14(FVector2D(LeftOffset + 1300, TopOffset + Height), FVector2D(Width, Height), 0.f, 100.f, 5, "monounsat");
	Ruler Ruler15(FVector2D(LeftOffset + 1400, TopOffset + Height), FVector2D(Width, Height), 0.f, 80.f, 8, "polyunsat");

	Rulers.Add(Ruler1);
	Rulers.Add(Ruler2);
	Rulers.Add(Ruler3);
	Rulers.Add(Ruler4);
	Rulers.Add(Ruler5);
	Rulers.Add(Ruler6);
	Rulers.Add(Ruler7);
	Rulers.Add(Ruler8);
	Rulers.Add(Ruler9);
	Rulers.Add(Ruler10);
	Rulers.Add(Ruler11);
	Rulers.Add(Ruler12);
	Rulers.Add(Ruler13);
	Rulers.Add(Ruler14);
	Rulers.Add(Ruler15);
}

int32 UParallelNutrientsWidget::NativePaint(
	const FPaintArgs& Args,
	const FGeometry& AllottedGeometry,
	const FSlateRect& MyCullingRect,
	FSlateWindowElementList& OutDrawElements,
	int32 LayerId,
	const FWidgetStyle& InWidgetStyle,
	bool bParentEnabled) const
{

	for (int32 IndexOfNutrient = 0; IndexOfNutrient < Nutrients.Num(); IndexOfNutrient++)
	{
		const Nutrient& N = Nutrients[IndexOfNutrient];
		DrawSmoothedLine(AllottedGeometry, OutDrawElements, LayerId, 0.5f, FLinearColor::White, N);
	}

	for (int32 IndexOfRuler = 0; IndexOfRuler < Rulers.Num(); IndexOfRuler++)
	{
		const Ruler& R = Rulers[IndexOfRuler];
		DrawRuler(AllottedGeometry, OutDrawElements, LayerId + 1, R);
	}
	return LayerId += 2;
}

void UParallelNutrientsWidget::DrawSmoothedLine(
	const FGeometry& InAllottedGeometry,
	FSlateWindowElementList& OutDrawElements,
	int32 InLayerId,
	float InThickness,
	const FLinearColor& InLinearColor,
	const Nutrient& InNutrient
)const
{
	const TArray<double>& Values = InNutrient.ToValues();

	for (int32 Index = 0; Index < Values.Num() - 1; Index++)
	{
		Ruler CurrentRuler = Rulers[Index];
		Ruler NextRuler = Rulers[Index + 1];

		float CurrentY;
		if(Values[Index] > CurrentRuler.EndScale)
			CurrentY = CurrentRuler.Size.Y;
		else
			CurrentY = CurrentRuler.Size.Y * (Values[Index] - CurrentRuler.BeginScale) / (CurrentRuler.EndScale - CurrentRuler.BeginScale);

		float NextY;
		if (Values[Index + 1] > NextRuler.EndScale)
			NextY = NextRuler.Size.Y;
		else
			NextY = NextRuler.Size.Y * (Values[Index+1] - NextRuler.BeginScale) / (NextRuler.EndScale - NextRuler.BeginScale);

		FVector2D CurrentPosition(CurrentRuler.Position + FVector2D(0, -CurrentY));
		FVector2D NextPosition(NextRuler.Position + FVector2D(0, -NextY));

		FSlateDrawElement::MakeSpline(
			OutDrawElements,
			InLayerId,
			InAllottedGeometry.ToPaintGeometry(),
			CurrentPosition,
			FVector2D(100, 0),
			NextPosition,
			FVector2D(100, 0),
			InThickness,
			ESlateDrawEffect::None,
			GetLinearColorByGroup(InNutrient.group)
		);
	}
}

void UParallelNutrientsWidget::DrawRuler(
	const FGeometry& InAllottedGeometry,
	FSlateWindowElementList& OutDrawElements,
	int32 InLayerId,
	const Ruler& InRuler
)const
{
	TArray<FVector2D> Lines;
	Lines.Add(InRuler.Position);
	Lines.Add(InRuler.Position + FVector2D(0, -InRuler.Size.Y));
	FSlateDrawElement::MakeLines(
		OutDrawElements,
		InLayerId,
		InAllottedGeometry.ToPaintGeometry(),
		Lines
	);

	if (InRuler.Segment < 1)
		return;

	float YSpace = InRuler.Size.Y / InRuler.Segment;

	float NumberSpace = (InRuler.EndScale - InRuler.BeginScale) / InRuler.Segment;

	const FSlateFontInfo& FontInfo = FCoreStyle::GetDefaultFontStyle("Regular", 10);

	for (int32 IndexOfSegment = 0; IndexOfSegment <= InRuler.Segment; IndexOfSegment++)
	{
		FSlateDrawElement::MakeText(
			OutDrawElements,
			InLayerId,
			InAllottedGeometry.ToOffsetPaintGeometry(InRuler.Position + FVector2D(10, -5-YSpace * IndexOfSegment)),
			FText::FromString(FString::SanitizeFloat(InRuler.BeginScale + NumberSpace * IndexOfSegment)),
			FontInfo
		);

		TArray<FVector2D> Points;
		Points.Add(InRuler.Position + FVector2D(0, -YSpace * IndexOfSegment));
		Points.Add(InRuler.Position + FVector2D(5, -YSpace * IndexOfSegment));

		FSlateDrawElement::MakeLines(
			OutDrawElements,
			InLayerId,
			InAllottedGeometry.ToPaintGeometry(),
			Points
		);
	}

	const UFont* Font = GEngine->GetSmallFont();

	if (Font != nullptr)
	{
		int32 TextWidth;
		int32 TextHeight;

		Font->GetStringHeightAndWidth(InRuler.Title, TextHeight, TextWidth);

		FSlateDrawElement::MakeText(
			OutDrawElements,
			InLayerId,
			InAllottedGeometry.ToOffsetPaintGeometry(InRuler.Position + FVector2D(-TextWidth * 0.5, -TextHeight - 15 - InRuler.Size.Y)),
			FText::FromString(InRuler.Title),
			FontInfo
		);
	}
}

void UParallelNutrientsWidget::SetValue(const FString& InValue)
{
	if (InValue.IsEmpty())
		return;

	TSharedRef< TJsonReader<> > Reader = TJsonReaderFactory<>::Create(InValue);

	TArray<TSharedPtr<FJsonValue>> NutrientArray;
	if (FJsonSerializer::Deserialize(Reader, NutrientArray))
	{
		for (const TSharedPtr<FJsonValue>& NutrientJsonValue : NutrientArray)
		{
			TArray<TSharedPtr<FJsonValue>> Values = NutrientJsonValue->AsArray();
			if(Values.Num() != 17)
				continue;
			Nutrient N;

			for (int32 Index = 0; Index < Values.Num(); Index++)
			{
				TSharedPtr<FJsonValue> Value = Values[Index];
				switch (Index)
				{
				case 0:
					N.name = Value->AsString();
					break;
				case 1:
					N.group = Value->AsString();
					break;
				case 2:
					N.protein = Value->AsNumber();
					break;
				case 3:
					N.calcium = Value->AsNumber();
					break;
				case 4:
					N.sodium = Value->AsNumber();
					break;
				case 5:
					N.fiber = Value->AsNumber();
					break;
				case 6:
					N.vitaminc = Value->AsNumber();
					break;
				case 7:
					N.potassium = Value->AsNumber();
					break;
				case 8:
					N.carbohydrate = Value->AsNumber();
					break;
				case 9:
					N.sugars = Value->AsNumber();
					break;
				case 10:
					N.fat = Value->AsNumber();
					break;
				case 11:
					N.water = Value->AsNumber();
					break;
				case 12:
					N.calories = Value->AsNumber();
					break;
				case 13:
					N.saturated = Value->AsNumber();
					break;
				case 14:
					N.monounsat = Value->AsNumber();
					break;
				case 15:
					N.polyunsat = Value->AsNumber();
					break;
				case 16:
					N.id = Value->AsNumber();
					break;
				default:
					break;
				}
			}

			Nutrients.Add(N);
			GroupColorMap.Add(N.group,FLinearColor::White);
		}
	}

	int32 Index = 0;
	float DeltaH = FMath::RoundToFloat(300 / (GroupColorMap.Num() - 1));
	for (TPair<FString, FLinearColor>& Pair : GroupColorMap)
	{
		Pair.Value = FLinearColor::MakeFromHSV8(7.76 * Index, 255 * 0.86, 255 * 0.73);
		Index++;

		GroupSlateColorBrushMap.Add(Pair.Key, FSlateColorBrush(Pair.Value));
	}


	if (Border.IsValid())
	{
		TSharedPtr<SCanvas> Canvas;
		SAssignNew(Canvas, SCanvas);

		AddLegends(Canvas);

		Border->SetContent(Canvas.ToSharedRef());
	}
}

TSharedRef<SWidget> UParallelNutrientsWidget::RebuildWidget()
{
	static FSlateColorBrush ColorBrush(FLinearColor::FromSRGBColor(FColor(64, 64, 64, 64)));
	SAssignNew(Border, SBorder)
		.BorderImage(&ColorBrush)
		.Padding(0)
		[
			SNullWidget::NullWidget
		];
	return Border.ToSharedRef();
}

void UParallelNutrientsWidget::AddLegends(TSharedPtr<SCanvas> InCanvas)
{
	TSharedPtr<SVerticalBox> VBox;
	SAssignNew(VBox, SVerticalBox);

	for (TPair<FString,FLinearColor>& Pair : GroupColorMap)
	{
		TSharedPtr<SHorizontalBox> HBox;
		SAssignNew(HBox, SHorizontalBox);

		HBox->AddSlot()
			.AutoWidth()
			.HAlign(EHorizontalAlignment::HAlign_Fill)
			.VAlign(EVerticalAlignment::VAlign_Fill)
			[
				SNew(SBox)
				.Padding(5)
				.WidthOverride(100.f)
				.HeightOverride(40.f)
				.HAlign(EHorizontalAlignment::HAlign_Fill)
				.VAlign(EVerticalAlignment::VAlign_Fill)
				[
					SNew(SImage)
					.Image(&GetSlateColorBrushByGroup(Pair.Key))
				]
			];

		HBox->AddSlot()
			.AutoWidth()
			.HAlign(EHorizontalAlignment::HAlign_Fill)
			.VAlign(EVerticalAlignment::VAlign_Center)
			[
				SNew(STextBlock)
				.Text(FText::FromString(Pair.Key))
			];

		VBox->AddSlot()
			[
				HBox.ToSharedRef()
			];
	}


	InCanvas->AddSlot()
		.Size(FVector2D(300.f, 900.f))
		[
			VBox.ToSharedRef()
		];
}

const FLinearColor& UParallelNutrientsWidget::GetLinearColorByGroup(const FString& InGroup)const
{
	if (GroupColorMap.Contains(InGroup))
	{
		return GroupColorMap[InGroup];
	}
	else
		return FLinearColor::White;
}

const FSlateColorBrush& UParallelNutrientsWidget::GetSlateColorBrushByGroup(const FString& InGroup)const
{
	static FSlateColorBrush DefaultSlateColorBrush = FSlateColorBrush(FLinearColor::White);
	if (GroupSlateColorBrushMap.Contains(InGroup))
	{
		return GroupSlateColorBrushMap[InGroup];
	}
	else
		return DefaultSlateColorBrush;
}