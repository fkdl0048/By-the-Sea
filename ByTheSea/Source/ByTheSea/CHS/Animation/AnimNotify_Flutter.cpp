// Fill out your copyright notice in the Description page of Project Settings.


#include "CHS/Animation/AnimNotify_Flutter.h"

void UAnimNotify_Flutter::Notify(USkeletalMeshComponent* MeshComp, UAnimSequenceBase* Animation, const FAnimNotifyEventReference& EventReference)
{
	Super::Notify(MeshComp, Animation, EventReference);

    AActor* MeshComOwner = MeshComp->GetOwner();
	if (IsValid(MeshComOwner))
	{
        UFunction* FlutterFunction = MeshComOwner->FindFunction(FName(TEXT("Flutter")));

        if (FlutterFunction)
        {
            MeshComOwner->ProcessEvent(FlutterFunction, nullptr);
        }
        else
        {
            UE_LOG(LogTemp, Log, TEXT("FlutterFunc Not Found"))
        }
	}
}
