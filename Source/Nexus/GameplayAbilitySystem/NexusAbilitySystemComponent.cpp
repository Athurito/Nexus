// Fill out your copyright notice in the Description page of Project Settings.


#include "NexusAbilitySystemComponent.h"

#include "Characters/NexusCharacterBase.h"


// Sets default values for this component's properties
UNexusAbilitySystemComponent::UNexusAbilitySystemComponent()
{
	// Set this component to be initialized when the game starts, and to be ticked every frame.  You can turn these features
	// off to improve performance if you don't need them.
	PrimaryComponentTick.bCanEverTick = true;

	// ...
}


// Called when the game starts
void UNexusAbilitySystemComponent::BeginPlay()
{
	Super::BeginPlay();

	// ...
	
}

void UNexusAbilitySystemComponent::OnRep_ActivateAbilities()
{
	Super::OnRep_ActivateAbilities();
	
	ANexusCharacterBase* Owner = Cast<ANexusCharacterBase>(GetOwner());
	
	if (!Owner) return;
	
	bool bAbilitiesChanged = false;
	
	if (LastActiveAbilities.Num() != ActivatableAbilities.Items.Num())
	{
		bAbilitiesChanged = true;
	}
	else
	{
		for (int32 i = 0; i < LastActiveAbilities.Num(); ++i)
		{
			if (LastActiveAbilities[i].Ability != ActivatableAbilities.Items[i].Ability)
			{
				bAbilitiesChanged = true;
				break;
			}
		}
	}
	
	if (bAbilitiesChanged)
	{
		Owner->SendAbilitiesChangedEvent();
		LastActiveAbilities = ActivatableAbilities.Items;
	}
}


// Called every frame
void UNexusAbilitySystemComponent::TickComponent(float DeltaTime, ELevelTick TickType,
                                                 FActorComponentTickFunction* ThisTickFunction)
{
	Super::TickComponent(DeltaTime, TickType, ThisTickFunction);

	// ...
}

