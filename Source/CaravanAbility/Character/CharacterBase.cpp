// Fill out your copyright notice in the Description page of Project Settings.


#include "CharacterBase.h"

#include "CharacterBaseMovementComponent.h"
#include "Camera/CameraComponent.h"
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

	UE_LOG(LogTemp, Display, TEXT("Granting default abilities to character"));
	for (int i = 0; i < DefaultAbilities.Num(); i++)
	{
		// Only bind the first ability, for now. I need to change this later
		TSubclassOf<UCaravanGameplayAbility>& StartupAbility = DefaultAbilities[i];
		if (i == 0)
		{
			AbilitySystem->GiveAbility(FGameplayAbilitySpec(StartupAbility, 0, i));
		}
		else
		{
			AbilitySystem->GiveAbility(FGameplayAbilitySpec(StartupAbility, 0, -1));
		}
	}

	AbilitySystem->bAbilitiesInitialized = true;
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