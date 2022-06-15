// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "Factories\Factory.h"
#include "TextureRenderTarget2DR32f.h"
#include "RT2DFactory.generated.h"

/**
 * 
 */
UCLASS()
class URT2DFactory : public UFactory
{
	GENERATED_UCLASS_BODY()
public:


	virtual UObject* FactoryCreateNew(UClass* InClass, UObject* InParent, FName InName, EObjectFlags Flags, UObject* Context, FFeedbackContext* Warn) override;


	virtual bool ShouldShowInNewMenu() const override;

};

