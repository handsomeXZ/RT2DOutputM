// Fill out your copyright notice in the Description page of Project Settings.


#include "RTR32fBoxComponent.h"

UNewBoxComponent::UNewBoxComponent(const FObjectInitializer& ObjectInitializer)
	: Super(ObjectInitializer)
{
	LineThickness = 2.0f;
	bUseEditorCompositing = true;
}


void UNewBoxComponent::ChangeLine(float thickness) {
	LineThickness = thickness;
}