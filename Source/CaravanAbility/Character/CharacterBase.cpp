// Fill out your copyright notice in the Description page of Project Settings.


#include "CharacterBase.h"

#include "CaravanAbility/CaravanAbility.h"

#include "CharacterBaseMovementComponent.h"
#include "Camera/CameraComponent.h"
#include "CaravanAbility/DecalFadeComponent.h" 
#include "CaravanAbility/GameplayAbilities/Components/CharacterAbilitySystemComponent.h"
#include "CaravanAbility/GameplayAbilities/MeleeAbility.h"
#include "GameFramework/CharacterMovementComponent.h"
#include "GameFramework/SpringArmComponent.h"

// Sets default values
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

	AbilitySystem = CreateDefaultSubobject<UCharacterAbilitySystemComponent>(TEXT("CharacterAbility"));
	// AbilitySystem->SetIsReplicated(true);

	HitboxController = CreateDefaultSubobject<UHitboxController>(TEXT("Hitbox Controller"));
	HitboxController->SetupAttachment(RootComponent);

	AttributeSet = CreateDefaultSubobject<UCharacterBaseAttributeSet>(TEXT("Character attribute set"));
}

// Called when the game starts or when spawned
void ACharacterBase::BeginPlay()
{
	Super::BeginPlay();

	GrantAbilities();

	if (!HasAuthority() && AbilitySystem)
	{
		AbilitySystem->InitAbilityActorInfo(this, this);
	}
}

// Called every frame
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
	
	AbilitySystem->BindAbilityActivationToInputComponent(PlayerInputComponent, FGameplayAbilityInputBinds("ConfirmInput", "CancelInput", "EMeleeInputID"));
}


void ACharacterBase::PossessedBy(AController* NewController)
{
	Super::PossessedBy(NewController);
	
	if (AbilitySystem)
	{
		AbilitySystem->InitAbilityActorInfo(this, this);
	}

	if (TargetingReticle.IsValid())
	{
		// Reticles are owned by the controller, rather than the server
		TargetingReticle->SetOwner(NewController);
		UE_LOG(LogTargetingSystem, Display, TEXT("%s Making targeting reticle an autonomous proxy."),
			GetNetMode() == ENetMode::NM_Client ? TEXT("[client]") : TEXT("[server]"));
		TargetingReticle->SetAutonomousProxy(true);
		TargetingReticle->SetController(NewController);
	}
	else
	{
		UE_LOG(LogTargetingSystem, Display, TEXT("%s Attempting to assign targeting reticle controller for actor %s, but no targeting reticle was found."),
			GetNetMode() == ENetMode::NM_Client ? TEXT("[client]") : TEXT("[server]"),
			*GetName())
	}
}


void ACharacterBase::OnConstruction(const FTransform& Transform)
{
	if (HasAuthority())
	{
		SpawnTargetingReticle();
	}
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

	UE_LOG(LogAbilitySystem, Display, TEXT("Granting default abilities to character"));
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
	if (TargetingReticle.IsValid())
	{
		// Destroy our old targeting reticle
		TargetingReticle->Destroy();
	}
	if (TargetingReticleClass)
	{
		UE_LOG(LogTargetingSystem, Display, TEXT("%s %s spawned new Targeting Reticle Actor."),
			GetNetMode() == ENetMode::NM_Client ? TEXT("[client]") : TEXT("[server]"),
			*GetName());
		TargetingReticle = GetWorld()->SpawnActor<ATargetingReticleActor>(TargetingReticleClass, GetActorTransform());
	}
}


void ACharacterBase::ShowTargetingReticle()
{
	if (TargetingReticle.IsValid())
	{
		TargetingReticle->FadeIn();
	}
}


const FVector ACharacterBase::HideTargetingReticle()
{
	if (TargetingReticle.IsValid())
	{
		TargetingReticle->FadeOut();
		return TargetingReticle->GetGroundLocation();
	}
	UE_LOG(LogTargetingSystem, Warning, TEXT("Unable to find a targeting reticle"));
	return FVector(0.0f, 0.0f, 100.0f);
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