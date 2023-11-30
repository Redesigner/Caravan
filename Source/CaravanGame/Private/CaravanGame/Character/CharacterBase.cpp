// Copyright (c) 2023 Stephen Melnick

#include "CaravanGame/Character/CharacterBase.h"

#include "CaravanGame.h"
#include "CaravanGame/Player/CaravanPlayerController.h"

#include "CaravanAbility/GameplayAbilities/Components/CharacterAbilitySystemComponent.h"
#include "CaravanAbility/GameplayAbilities/Abilities/MeleeAbility.h"

#include "CaravanGame/Character/Components/CharacterBaseMovementComponent.h"
#include "CaravanGame/Character/Components/DecalFadeComponent.h" 
#include "CaravanGame/Dialog/DialogHandler.h"
#include "CaravanGame/Inventory/InventoryContainer.h"

#include "GameFramework/CharacterMovementComponent.h"
#include "GameFramework/SpringArmComponent.h"
#include "Camera/CameraComponent.h"
#include "Components/SphereComponent.h"


#include "Net/UnrealNetwork.h"


ACharacterBase::ACharacterBase(const FObjectInitializer& ObjectInitializer) :
	ACharacter(ObjectInitializer.SetDefaultSubobjectClass(ACharacter::CharacterMovementComponentName, UCharacterBaseMovementComponent::StaticClass()))
{
	bUseControllerRotationPitch = false;
	bUseControllerRotationYaw = false;
	bUseControllerRotationRoll = false;

	// Configure character movement
	GetCharacterMovement()->bOrientRotationToMovement = true; // Character moves in the direction of input...	
	GetCharacterMovement()->RotationRate = FRotator(0.0f, 500.0f, 0.0f); // ...at this rotation rate
	
	CameraBoom = CreateDefaultSubobject<USpringArmComponent>(TEXT("CameraBoom"));
	CameraBoom->SetupAttachment(RootComponent);
	CameraBoom->TargetArmLength = 400.0f;
	CameraBoom->bUsePawnControlRotation = true;

	FollowCamera = CreateDefaultSubobject<UCameraComponent>(TEXT("FollowCamera"));
	FollowCamera->SetupAttachment(CameraBoom, USpringArmComponent::SocketName);
	FollowCamera->bUsePawnControlRotation = false; // Camera does not rotate relative to arm

	HitboxController = CreateDefaultSubobject<UHitboxController>(TEXT("Hitbox Controller"));
	HitboxController->SetupAttachment(GetMesh());

	AbilitySystem = CreateDefaultSubobject<UCharacterAbilitySystemComponent>(TEXT("CharacterAbility"));
	AbilitySystem->SetHitboxController(HitboxController);

	InteractionVolume = CreateDefaultSubobject<USphereComponent>(TEXT("Interaction Volume"));
	InteractionVolume->SetupAttachment(RootComponent);

	AttributeSet = CreateDefaultSubobject<UCharacterBaseAttributeSet>(TEXT("Character attribute set"));
	AttributeSet->OnDeath.BindUObject(this, &ACharacterBase::OnDeath);
}

void ACharacterBase::BeginPlay()
{
	Super::BeginPlay();

	GrantAbilities();

	if (!AbilitySystem)
	{
		return;
	}

	if (!HasAuthority())
	{
		AbilitySystem->InitAbilityActorInfo(this, this);
	}
}

void ACharacterBase::Tick(float DeltaTime)
{
	Super::Tick(DeltaTime);
}


void ACharacterBase::SetupPlayerInputComponent(UInputComponent* PlayerInputComponent)
{
	PlayerInputComponent->BindAxis("MovementY", this, &ACharacterBase::MoveForward);
	PlayerInputComponent->BindAxis("MovementX", this, &ACharacterBase::MoveRight);

	PlayerInputComponent->BindAxis("LookX", this, &ACharacterBase::LookRight);
	PlayerInputComponent->BindAxis("LookY", this, &ACharacterBase::LookUp);

	// PlayerInputComponent->BindAction("Interact", EInputEvent::IE_Pressed, this, &ACharacterBase::Interact);
	
	AbilitySystem->BindAbilityActivationToInputComponent(PlayerInputComponent, FGameplayAbilityInputBinds("ConfirmInput", "CancelInput", "EMeleeInputID"));


	if (TargetingReticle)
	{
		TargetingReticle->InputComponent = PlayerInputComponent;
	}
	else
	{
		UE_LOG(LogCaravanGame, Display, TEXT("%s Attempting to assign targeting reticle controller for actor %s, but no targeting reticle was found."),
			GetNetMode() == ENetMode::NM_Client ? TEXT("[client]") : TEXT("[server]"),
			*GetName())
	}
}


void ACharacterBase::PossessedBy(AController* NewController)
{
	Super::PossessedBy(NewController);
	
	if (AbilitySystem)
	{
		AbilitySystem->InitAbilityActorInfo(this, this);
	}

	if (TargetingReticle)
	{
		TargetingReticle->SetOwner(GetController());
		TargetingReticle->SetAutonomousProxy(true);
	}
	if (UDialogHandler* DialogHandlerRef = NewController->FindComponentByClass<UDialogHandler>())
	{
		DialogHandler = DialogHandlerRef;
	}
	if (UActorComponent* Inventory = NewController->GetComponentByClass(UInventoryContainer::StaticClass()))
	{
		InventoryContainer = Cast<UInventoryContainer>(Inventory);
	}
}


void ACharacterBase::OnConstruction(const FTransform& Transform)
{
	if (HasAuthority())
	{
		SpawnTargetingReticle();
	}
}


void ACharacterBase::GetLifetimeReplicatedProps(TArray<FLifetimeProperty>& OutLifetimeProps) const
{
	DOREPLIFETIME(ACharacterBase, TargetingReticle);

	Super::GetLifetimeReplicatedProps(OutLifetimeProps);
}

const TArray<FString>& ACharacterBase::GetResponses() const
{
	return DialogResponses;
}

FDialogTagHandle* ACharacterBase::GetRootTag()
{
	return &RootTag;
}


UAbilitySystemComponent* ACharacterBase::GetAbilitySystemComponent() const
{
	return AbilitySystem;
}


UCharacterBaseAttributeSet* ACharacterBase::GetAttributeSetBase() const
{
	return AttributeSet;
}


void ACharacterBase::GrantAbilities()
{
	// Grant abilities, but only on the server	
	if (!AbilitySystem || AbilitySystem->bAbilitiesInitialized || !HasAuthority() )
	{
		return;
	}

	AbilitySystem->InitAbilityActorInfo(this, this);

	// UE_LOG(LogAbilitySystem, Display, TEXT("Granting default abilities to character"));
	for (TSubclassOf<UCaravanGameplayAbility> Ability: DefaultAbilities)
	{
		if (UCaravanGameplayAbility* CaravanAbility = Cast<UCaravanGameplayAbility>(Ability.GetDefaultObject() ))
		{
			if (CaravanAbility->InputBinding == EMeleeInputID::None)
			{
				AbilitySystem->GiveAbility(FGameplayAbilitySpec(Ability, 0, -1));
			}
			else
			{
				AbilitySystem->GiveAbility(FGameplayAbilitySpec(Ability, 0, static_cast<uint8>(CaravanAbility->InputBinding)));
			}
		}
	}
	AbilitySystem->bAbilitiesInitialized = true;
}


void ACharacterBase::SpawnTargetingReticle()
{
	if (TargetingReticle)
	{
		return;
	}
	if (!TargetingReticleClass)
	{
		UE_LOG(LogCaravanGame, Warning, TEXT("Cannot spawn Targeting Reticle, TargetingReticleClass was not set."))
		return;
	}
	UE_LOG(LogCaravanGame, Display, TEXT("%s %s spawned new Targeting Reticle Actor."),
		GetNetMode() == ENetMode::NM_Client ? TEXT("[client]") : TEXT("[server]"),
		*GetName());
	TargetingReticle = GetWorld()->SpawnActor<ATargetingReticleActor>(TargetingReticleClass, GetActorTransform());
	TargetingReticle->SetOwningPawn(this);
}


void ACharacterBase::ShowTargetingReticle()
{
	if (TargetingReticle)
	{
		TargetingReticle->FadeIn();
	}
	UE_LOG(LogCaravanGame, Warning, TEXT("Cannot show the targeting reticle, it was nullptr."));
}


void ACharacterBase::HideTargetingReticle()
{
	if (!TargetingReticle)
	{
		UE_LOG(LogCaravanGame, Warning, TEXT("%s Unable to find a targeting reticle, it was nullptr"),
			GetNetMode() == ENetMode::NM_Client ? TEXT("[client]") : TEXT("[server]"));
		return;
	}
	TargetingReticle->FadeOut();
	return;
}

FVector ACharacterBase::GetTargetingReticleLocation() const
{
	if (!TargetingReticle)
	{
		UE_LOG(LogCaravanGame, Warning, TEXT("%s Unable to find a targeting reticle, it was nullptr"));
		return GetActorLocation();
	}
	return TargetingReticle->GetGroundLocation();
}


void ACharacterBase::MoveForward(float Scale)
{
	if ((Controller != nullptr) && (Scale != 0.0f))
	{
		const FRotator Rotation = Controller->GetControlRotation();
		const FRotator YawRotation(0, Rotation.Yaw, 0);

		const FVector Direction = FRotationMatrix(YawRotation).GetUnitAxis(EAxis::X);
		AddMovementInput(Direction, Scale);
	}
}


void ACharacterBase::MoveRight(float Scale)
{
	if ( (Controller != nullptr) && (Scale != 0.0f) )
	{
		const FRotator Rotation = Controller->GetControlRotation();
		const FRotator YawRotation(0, Rotation.Yaw, 0);
		
		const FVector Direction = FRotationMatrix(YawRotation).GetUnitAxis(EAxis::Y);
		AddMovementInput(Direction, Scale);
	}
}


void ACharacterBase::LookUp(float Scale)
{
	AddControllerPitchInput(LookSpeed * -Scale * GetWorld()->GetDeltaSeconds());
}


void ACharacterBase::LookRight(float Scale)
{
	AddControllerYawInput(LookSpeed * Scale * GetWorld()->GetDeltaSeconds());
}


void ACharacterBase::Interact()
{
	if (!InteractionVolume)
	{
		return;
	}
	TArray<AActor*> OverlappingActors;
	InteractionVolume->GetOverlappingActors(OverlappingActors);
	for (AActor* Target : OverlappingActors)
	{
		if (Target->Implements<UInteractableInterface>())
		{
			IInteractableInterface* Interactable = Cast<IInteractableInterface>(Target);
			const FGameplayInteraction& InteractionResult = Interactable->HandleInteraction(FGameplayInteraction::None());
			HandleInteraction(InteractionResult);
		}
	}
}

void ACharacterBase::DialogStart()
{
	PauseMovement();
	OnDialogStart();
}

void ACharacterBase::DialogEnd()
{
	UnpauseMovement();
	OnDialogEnd();
}

FGameplayInteraction ACharacterBase::HandleInteraction(const FGameplayInteraction& Interaction)
{
	switch (Interaction.InteractionType)
	{
	case EGameplayInteractionType::ShowDialog:
	{
		if (DialogHandler.IsValid())
		{
			// Let our dialog handler know who to send dialog responses to, if it needs to
			DialogHandler->SetTarget(Interaction.Instigator);
			DialogHandler->QueueDialog(Interaction.Payload);
			// We don't have an interaction to send... yet...
			// If we want to respond to a text option, we will do that later
			return FGameplayInteraction::None();
		}
	}break;
	case EGameplayInteractionType::Respond:
	{
		if (FName* Reply = ResponseMap.Find(Interaction.Payload))
		{
			FGameplayInteraction ResponseInteraction = FGameplayInteraction(this, *Reply, EGameplayInteractionType::ShowDialog);
			return ResponseInteraction;
		}
	}break;
	case EGameplayInteractionType::Give:
	{
		if (InventoryContainer.IsValid())
		{
			InventoryContainer->GiveItem(FInventoryEntry::Empty());
		}
	}break;
	}
	// We handled two default cases: ShowDialog and Respond are special for the character.
	// The rest can be customized in blueprint
	return OnHandleInteraction(Interaction);
}

FGameplayInteraction ACharacterBase::OnHandleInteraction_Implementation(FGameplayInteraction Interaction)
{
	return FGameplayInteraction::None();
}


void ACharacterBase::PauseMovement()
{
	PauseMovementServer();
	PauseMovementLocal();
}


void ACharacterBase::UnpauseMovement()
{
	UnpauseMovementServer();
	UnpauseMovementLocal();
}

void ACharacterBase::AddDialogResponse(FName Received, FName DialogId)
{
	ResponseMap.Add(Received, DialogId);
}


void ACharacterBase::PauseMovementLocal()
{
	UCharacterMovementComponent* CharacterMovementComponent = GetCharacterMovement();
	TSharedPtr<FRootMotionSource_ConstantForce> RootMotionSource = MakeShared<FRootMotionSource_ConstantForce>();
	RootMotionSource->Duration = -1.0f;
	RootMotionSource->AccumulateMode = ERootMotionAccumulateMode::Override;
	RootMotionSource->InstanceName = TEXT("HoldStillRootMotion");
	RootMotionSource->bInLocalSpace = false;

	CharacterMovementComponent->ApplyRootMotionSource(RootMotionSource);
}

void ACharacterBase::UnpauseMovementLocal()
{
	UCharacterMovementComponent* CharacterMovementComponent = GetCharacterMovement();
	CharacterMovementComponent->RemoveRootMotionSource(TEXT("HoldStillRootMotion"));
}

void ACharacterBase::PauseMovementServer_Implementation()
{
	PauseMovementLocal();
}

void ACharacterBase::UnpauseMovementServer_Implementation()
{
	UnpauseMovementLocal();
}
