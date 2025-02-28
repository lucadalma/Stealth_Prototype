// Copyright Epic Games, Inc. All Rights Reserved.

#include "Stealth_PrototypeCharacter.h"
#include "Engine/LocalPlayer.h"
#include "Camera/CameraComponent.h"
#include "Components/CapsuleComponent.h"
#include "GameFramework/CharacterMovementComponent.h"
#include "GameFramework/SpringArmComponent.h"
#include "GameFramework/Controller.h"
#include "EnhancedInputComponent.h"
#include "EnhancedInputSubsystems.h"
#include "BehaviorTree/BlackboardComponent.h"
#include "InputActionValue.h"
#include "Enemy.h"
#include "Footstep_Component.h"
#include "Perception/AIPerceptionStimuliSourceComponent.h"
#include "Perception/AISense_Sight.h"
#include "StealthAIController.h"

DEFINE_LOG_CATEGORY(LogTemplateCharacter);

//////////////////////////////////////////////////////////////////////////
// AStealth_PrototypeCharacter

AStealth_PrototypeCharacter::AStealth_PrototypeCharacter()
{
	// Set size for collision capsule
	GetCapsuleComponent()->InitCapsuleSize(42.f, 96.0f);
		
	// Don't rotate when the controller rotates. Let that just affect the camera.
	bUseControllerRotationPitch = false;
	bUseControllerRotationYaw = false;
	bUseControllerRotationRoll = false;

	// Configure character movement
	GetCharacterMovement()->bOrientRotationToMovement = true; // Character moves in the direction of input...	
	GetCharacterMovement()->RotationRate = FRotator(0.0f, 500.0f, 0.0f); // ...at this rotation rate

	// Note: For faster iteration times these variables, and many more, can be tweaked in the Character Blueprint
	// instead of recompiling to adjust them
	GetCharacterMovement()->JumpZVelocity = 700.f;
	GetCharacterMovement()->AirControl = 0.35f;
	GetCharacterMovement()->MaxWalkSpeed = 500.f;
	GetCharacterMovement()->MinAnalogWalkSpeed = 20.f;
	GetCharacterMovement()->BrakingDecelerationWalking = 2000.f;
	GetCharacterMovement()->BrakingDecelerationFalling = 1500.0f;

	// Create a camera boom (pulls in towards the player if there is a collision)
	CameraBoom = CreateDefaultSubobject<USpringArmComponent>(TEXT("CameraBoom"));
	CameraBoom->SetupAttachment(RootComponent);
	CameraBoom->TargetArmLength = 400.0f; // The camera follows at this distance behind the character	
	CameraBoom->bUsePawnControlRotation = true; // Rotate the arm based on the controller

	// Create a follow camera
	FollowCamera = CreateDefaultSubobject<UCameraComponent>(TEXT("FollowCamera"));
	FollowCamera->SetupAttachment(CameraBoom, USpringArmComponent::SocketName); // Attach the camera to the end of the boom and let the boom adjust to match the controller orientation
	FollowCamera->bUsePawnControlRotation = false; // Camera does not rotate relative to arm


	FootstepsComponent = CreateDefaultSubobject<UFootstep_Component>(TEXT("FootstepComponent"));

	// Note: The skeletal mesh and anim blueprint references on the Mesh component (inherited from Character) 
	// are set in the derived blueprint asset named ThirdPersonCharacter (to avoid direct content references in C++)

	SetupStimulusSource();
}

void AStealth_PrototypeCharacter::BeginPlay()
{
	// Call the base class  
	Super::BeginPlay();

	//Add Input Mapping Context
	if (APlayerController* PlayerController = Cast<APlayerController>(Controller))
	{
		if (UEnhancedInputLocalPlayerSubsystem* Subsystem = ULocalPlayer::GetSubsystem<UEnhancedInputLocalPlayerSubsystem>(PlayerController->GetLocalPlayer()))
		{
			Subsystem->AddMappingContext(DefaultMappingContext, 0);
		}
	}

	SetHealth(MaxHealth);
}

//////////////////////////////////////////////////////////////////////////
// Input

void AStealth_PrototypeCharacter::SetupPlayerInputComponent(UInputComponent* PlayerInputComponent)
{
	// Set up action bindings
	if (UEnhancedInputComponent* EnhancedInputComponent = Cast<UEnhancedInputComponent>(PlayerInputComponent)) {
		
		// Jumping
		EnhancedInputComponent->BindAction(JumpAction, ETriggerEvent::Started, this, &ACharacter::Jump);
		EnhancedInputComponent->BindAction(JumpAction, ETriggerEvent::Completed, this, &ACharacter::StopJumping);

		// Moving
		EnhancedInputComponent->BindAction(MoveAction, ETriggerEvent::Triggered, this, &AStealth_PrototypeCharacter::Move);

		// Looking
		EnhancedInputComponent->BindAction(LookAction, ETriggerEvent::Triggered, this, &AStealth_PrototypeCharacter::Look);

		// Crouching
		EnhancedInputComponent->BindAction(CrouchAction, ETriggerEvent::Started, this, &AStealth_PrototypeCharacter::OnCrouchActionStarted);
		EnhancedInputComponent->BindAction(CrouchAction, ETriggerEvent::Completed, this, &AStealth_PrototypeCharacter::OnCrouchActionEnded);
	
		EnhancedInputComponent->BindAction(StealthKillAction, ETriggerEvent::Started, this, &AStealth_PrototypeCharacter::PerformStealthKill);
	}
	else
	{
		UE_LOG(LogTemplateCharacter, Error, TEXT("'%s' Failed to find an Enhanced Input component! This template is built to use the Enhanced Input system. If you intend to use the legacy system, then you will need to update this C++ file."), *GetNameSafe(this));
	}
}

void AStealth_PrototypeCharacter::Move(const FInputActionValue& Value)
{
	// input is a Vector2D
	FVector2D MovementVector = Value.Get<FVector2D>();

	if (Controller != nullptr)
	{
		// find out which way is forward
		const FRotator Rotation = Controller->GetControlRotation();
		const FRotator YawRotation(0, Rotation.Yaw, 0);

		// get forward vector
		const FVector ForwardDirection = FRotationMatrix(YawRotation).GetUnitAxis(EAxis::X);
	
		// get right vector 
		const FVector RightDirection = FRotationMatrix(YawRotation).GetUnitAxis(EAxis::Y);

		// add movement 
		AddMovementInput(ForwardDirection, MovementVector.Y);
		AddMovementInput(RightDirection, MovementVector.X);
	}
}

void AStealth_PrototypeCharacter::Look(const FInputActionValue& Value)
{
	// input is a Vector2D
	FVector2D LookAxisVector = Value.Get<FVector2D>();

	if (Controller != nullptr)
	{
		// add yaw and pitch input to controller
		AddControllerYawInput(LookAxisVector.X);
		AddControllerPitchInput(LookAxisVector.Y);
	}
}

UFootstep_Component* AStealth_PrototypeCharacter::GetFootstepComponent() const
{
	return FootstepsComponent;
}

float AStealth_PrototypeCharacter::GetHealth() const
{
	return Health;
}

float AStealth_PrototypeCharacter::GetMaxHealth() const
{
	return MaxHealth;
}

void AStealth_PrototypeCharacter::SetHealth(float const NewHealth)
{
	Health = NewHealth;
	
}

void AStealth_PrototypeCharacter::SetupStimulusSource()
{
	StimulusSource = CreateDefaultSubobject<UAIPerceptionStimuliSourceComponent>(TEXT("Stimulus"));
	if (StimulusSource) 
	{
		StimulusSource->RegisterForSense(TSubclassOf<UAISense_Sight>());
		StimulusSource->RegisterWithPerceptionSystem();
	}
}

void AStealth_PrototypeCharacter::OnCrouchActionStarted(const FInputActionValue& Value)
{
	ACharacter* Character = this;
	Character->Crouch();

}

void AStealth_PrototypeCharacter::OnCrouchActionEnded(const FInputActionValue& Value)
{
	ACharacter* Character = this;
	Character->UnCrouch();
}

bool AStealth_PrototypeCharacter::IsDead() const
{
	return Health <= 0;
}


void AStealth_PrototypeCharacter::FindStealthKillTarget()
{
	FVector PlayerLocation = GetActorLocation();
	float SearchRadius = 200.f;

	// Definisce i parametri per il Trace
	FCollisionQueryParams QueryParams;
	QueryParams.AddIgnoredActor(this);

	// Lista di attori colpiti
	TArray<FOverlapResult> Overlaps;
	GetWorld()->OverlapMultiByChannel(Overlaps, PlayerLocation, FQuat::Identity,
		ECC_Pawn, FCollisionShape::MakeSphere(SearchRadius), QueryParams);

	TargetEnemy = nullptr;
	float ClosestDistance = FLT_MAX;

	for (const FOverlapResult& Result : Overlaps)
	{
		AEnemy* Enemy = Cast<AEnemy>(Result.GetActor());
		if (Enemy)
		{
			float Distance = FVector::Dist(PlayerLocation, Enemy->GetActorLocation());
			if (Distance < ClosestDistance)
			{
				TargetEnemy = Enemy;
				ClosestDistance = Distance;
			}
		}
	}
}

bool AStealth_PrototypeCharacter::IsBehindEnemy(AEnemy* targetEnemy)
{
	if (!targetEnemy) return false;

	// Direzione in cui il nemico sta guardando (normalizzata)
	FVector EnemyForward = targetEnemy->GetActorForwardVector().GetSafeNormal();

	// Direzione dal nemico verso il giocatore (normalizzata)
	FVector ToPlayer = (GetActorLocation() - targetEnemy->GetActorLocation()).GetSafeNormal();

	// Calcola il prodotto scalare tra i due vettori
	float DotProduct = FVector::DotProduct(EnemyForward, ToPlayer);

	// Se il DotProduct è minore di -0.7, il player è dietro (180° = -1, 90° = 0)
	return DotProduct < -0.7f;
}

void AStealth_PrototypeCharacter::PerformStealthKill()
{
	if (bIsPerformingStealthKill) return;
	bIsPerformingStealthKill = true;

	FindStealthKillTarget();

	if (!TargetEnemy)
	{
		bIsPerformingStealthKill = false;
		return;
	}

	// Controllo se il player è dietro al nemico
	if (!IsBehindEnemy(TargetEnemy))
	{
		UE_LOG(LogTemp, Warning, TEXT("Devi essere dietro al nemico per eseguire la stealth kill!"));
		bIsPerformingStealthKill = false;
		return;
	}

	AStealthAIController* AIController = Cast<AStealthAIController>(TargetEnemy->GetController());
	if (!AIController) return;

	UBlackboardComponent* BlackboardComp = AIController->GetBlackboardComponent();
	if (!BlackboardComp) return;

	bool bCanSeePlayer = BlackboardComp->GetValueAsBool("CanSeePlayer");
	if (bCanSeePlayer)
	{
		UE_LOG(LogTemp, Warning, TEXT("L'IA ti sta vedendo! Stealth Kill non eseguita."));
		bIsPerformingStealthKill = false;
		return;
	}

	// Blocca il movimento del giocatore e del nemico
	TargetEnemy->GetCharacterMovement()->DisableMovement();
	GetCharacterMovement()->DisableMovement();

	FRotator LookAtRotation = (TargetEnemy->GetActorLocation() - GetActorLocation()).Rotation();
	SetActorRotation(FRotator(0.f, LookAtRotation.Yaw, 0.f)); // Mantieni solo la rotazione Yaw

	// Riproduce animazione di Stealth Kill per il giocatore
	UAnimInstance* AnimInstance = GetMesh()->GetAnimInstance();
	if (AnimInstance && StealthKillMontage)
	{
		AnimInstance->Montage_Play(StealthKillMontage);
	}

	// Riproduce animazione di morte del nemico
	TargetEnemy->PlayDeathAnimation();

	// Timer per completare la kill e riattivare il controllo
	FTimerHandle TimerHandle;
	GetWorldTimerManager().SetTimer(TimerHandle, this, &AStealth_PrototypeCharacter::FinishStealthKill, 5.f, false);
}



void AStealth_PrototypeCharacter::FinishStealthKill()
{
	if (TargetEnemy)
	{
		TargetEnemy->EnableRagdoll();
		TargetEnemy = nullptr;
	}

	FVector FinalPosition = GetMesh()->GetComponentLocation();
	SetActorLocation(FinalPosition, true); // Il secondo parametro evita collisioni indesiderate

	GetCharacterMovement()->SetMovementMode(MOVE_Walking);
	bIsPerformingStealthKill = false; // Ora si può eseguire un'altra kill
}

void AStealth_PrototypeCharacter::EnableRagdoll()
{
	GetMesh()->SetSimulatePhysics(true);
	GetMesh()->SetCollisionEnabled(ECollisionEnabled::PhysicsOnly);
}
