// Fill out your copyright notice in the Description page of Project Settings.


#include "BasicAttributeSet.h"

#include "GameplayEffectExtension.h"
#include "Net/UnrealNetwork.h"

UBasicAttributeSet::UBasicAttributeSet()
{
	Health = 100.0f;
	MaxHealth = 100.0f;
	Stamina = 100.0f;
	MaxStamina = 100.0f;
}

void UBasicAttributeSet::OnRep_Health(const FGameplayAttributeData& OldValue) const
{
	GAMEPLAYATTRIBUTE_REPNOTIFY(UBasicAttributeSet, Health, OldValue);
}

void UBasicAttributeSet::OnRep_MaxHealth(const FGameplayAttributeData& OldValue) const
{
	GAMEPLAYATTRIBUTE_REPNOTIFY(UBasicAttributeSet, MaxHealth, OldValue);
}

void UBasicAttributeSet::OnRep_Stamina(const FGameplayAttributeData& OldValue) const
{
	GAMEPLAYATTRIBUTE_REPNOTIFY(UBasicAttributeSet, Stamina, OldValue);
}

void UBasicAttributeSet::OnRep_MaxStamina(const FGameplayAttributeData& OldValue) const
{
	GAMEPLAYATTRIBUTE_REPNOTIFY(UBasicAttributeSet, MaxStamina, OldValue);
}

void UBasicAttributeSet::GetLifetimeReplicatedProps(TArray<FLifetimeProperty>& OutLifetimeProps) const
{
	Super::GetLifetimeReplicatedProps(OutLifetimeProps);
	
	DOREPLIFETIME_CONDITION_NOTIFY(UBasicAttributeSet, Health, COND_None, REPNOTIFY_Always);
	DOREPLIFETIME_CONDITION_NOTIFY(UBasicAttributeSet, MaxHealth, COND_None, REPNOTIFY_Always);
	DOREPLIFETIME_CONDITION_NOTIFY(UBasicAttributeSet, Stamina, COND_None, REPNOTIFY_Always);
	DOREPLIFETIME_CONDITION_NOTIFY(UBasicAttributeSet, MaxStamina, COND_None, REPNOTIFY_Always);
}

void UBasicAttributeSet::PreAttributeChange(const FGameplayAttribute& Attribute, float& NewValue)
{
	Super::PreAttributeChange(Attribute, NewValue);
	
	if (Attribute == GetHealthAttribute())
	{
		NewValue = FMath::Clamp(NewValue, 0.0f, GetMaxHealth());
	}

	if (Attribute == GetStaminaAttribute())
	{
		NewValue = FMath::Clamp(NewValue, 0.0f, GetMaxStamina());
	}
}

void UBasicAttributeSet::PostGameplayEffectExecute(const struct FGameplayEffectModCallbackData& Data)
{
	Super::PostGameplayEffectExecute(Data);
	
	if (Data.EvaluatedData.Attribute == GetHealthAttribute())
	{
		SetHealth(GetHealth());
		
		if (Data.EffectSpec.Def->GetAssetTags().HasTag(FGameplayTag::RequestGameplayTag("Effects.HitReaction")) && Data.EvaluatedData.Magnitude != 0.0f)
		{
			FGameplayTagContainer HitReactionTagContainer;
			HitReactionTagContainer.AddTag(FGameplayTag::RequestGameplayTag("GameplayAbility.HitReaction"));
			GetOwningAbilitySystemComponent()->TryActivateAbilitiesByTag(HitReactionTagContainer);
		}
	}
	else if (Data.EvaluatedData.Attribute == GetStaminaAttribute())
	{
		SetStamina(GetStamina());
	}
}

void UBasicAttributeSet::PostAttributeChange(const FGameplayAttribute& Attribute, float OldValue, float NewValue)
{
	Super::PostAttributeChange(Attribute, OldValue, NewValue);
	
	if (Attribute == GetHealthAttribute() && NewValue <= 0.0f)
	{
		FGameplayTagContainer DeathAbilityTagContainer;
		DeathAbilityTagContainer.AddTag(FGameplayTag::RequestGameplayTag("GameplayAbility.Death"));
		GetOwningAbilitySystemComponent()->TryActivateAbilitiesByTag(DeathAbilityTagContainer);
	}
}
