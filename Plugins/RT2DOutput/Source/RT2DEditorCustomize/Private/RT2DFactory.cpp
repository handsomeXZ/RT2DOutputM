// Fill out your copyright notice in the Description page of Project Settings.


#include "RT2DFactory.h"

URT2DFactory::URT2DFactory(const FObjectInitializer& ObjectInitializer)
	: Super(ObjectInitializer)
{
	SupportedClass = UTextureRenderTarget2DR32f::StaticClass();
	bCreateNew = true;
	bEditAfterNew = true;
	bEditorImport = false;

}

UObject* URT2DFactory::FactoryCreateNew(UClass* InClass, UObject* InParent, FName InName, EObjectFlags Flags, UObject* Context, FFeedbackContext* Warn)
{

	UTextureRenderTarget2DR32f* NewTexturePlayer = NewObject<UTextureRenderTarget2DR32f>(InParent, InClass, InName, Flags | RF_Transactional);
	NewTexturePlayer->InitAutoFormat(512.0f, 512.0f);
	return NewTexturePlayer;
}

bool URT2DFactory::ShouldShowInNewMenu() const {
	return true;
}