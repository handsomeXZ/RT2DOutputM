#include "3DToolCustomization.h"

#include "DetailCategoryBuilder.h"
#include "DetailLayoutBuilder.h"
#include "DetailWidgetRow.h"
#include "Capture3DTool.h"

#define LOCTEXT_NAMESPACE "EditorCustomization"

TSharedRef<IDetailCustomization> F3DToolCustomization::MakeInstance()
{
	return MakeShareable(new F3DToolCustomization());
}

void F3DToolCustomization::CustomizeDetails(IDetailLayoutBuilder& DetailBuilder)
{
	

	IDetailCategoryBuilder& Category = DetailBuilder.EditCategory("3DToolSceneCapture", FText::GetEmpty(), ECategoryPriority::Important);

	Category.AddCustomRow(LOCTEXT("RowSearchName","Magic")).NameContent()
		[
			SNew(STextBlock).Text(LOCTEXT("DetailName", "")).Font(IDetailLayoutBuilder::GetDetailFont())
		].ValueContent()
		[ 
			SNew(SButton)
			.Text(LOCTEXT("ButtonText","Refresh"))
		.HAlign(HAlign_Center)
		.ToolTipText(LOCTEXT("ButtonInfo", "Refresh range"))
		.OnClicked_Lambda([&]()
			{
				TArray<TWeakObjectPtr<UObject>> CustomizeObjects;
				DetailBuilder.GetObjectsBeingCustomized(CustomizeObjects);
				TWeakObjectPtr<ACapture3DTool> SelectedObj = Cast<ACapture3DTool>(CustomizeObjects[0].Get());
				GEngine->AddOnScreenDebugMessage(-1, 5.f, FColor::Red, TEXT("Button Clicked!"));
				if (SelectedObj.IsValid())
				{
					SelectedObj->Refresh();
				}
				else
				{
					GEngine->AddOnScreenDebugMessage(-1, 5.f, FColor::Red, TEXT("UnValid"));
				}

				return FReply::Handled();
			})

		];
	Category.AddCustomRow(LOCTEXT("RowSearchName", "Magic")).NameContent()
		[
			SNew(STextBlock).Text(LOCTEXT("DetailName", "")).Font(IDetailLayoutBuilder::GetDetailFont())
		].ValueContent()
		[
			SNew(SButton)
			.Text(LOCTEXT("ButtonText", "Create"))
		.HAlign(HAlign_Center)
		.ToolTipText(LOCTEXT("ButtonInfo", "Auto create Material"))
		.OnClicked_Lambda([&]()
			{
				TArray<TWeakObjectPtr<UObject>> CustomizeObjects;
				DetailBuilder.GetObjectsBeingCustomized(CustomizeObjects);
				TWeakObjectPtr<ACapture3DTool> SelectedObj = Cast<ACapture3DTool>(CustomizeObjects[0].Get());
				GEngine->AddOnScreenDebugMessage(-1, 5.f, FColor::Red, TEXT("Button Clicked!"));
				if (SelectedObj.IsValid())
				{
					SelectedObj->Create();
				}
				else
				{
					GEngine->AddOnScreenDebugMessage(-1, 5.f, FColor::Red, TEXT("UnValid"));
				}

				return FReply::Handled();
			})

		];

}


#undef LOCTEXT_NAMESPACE