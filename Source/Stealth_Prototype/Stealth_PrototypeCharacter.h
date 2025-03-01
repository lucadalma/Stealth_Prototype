// Copyright Epic Games, Inc. All Rights Reserved.

#pragma once

#include "CoreMinimal.h"
#include "GameFramework/Character.h"
#include "Logging/LogMacros.h"
#include "Stealth_PrototypeCharacter.generated.h"

class USpringArmComponent;
class UCameraComponent;
class UInputMappingContext;
class UInputAction;
struct FInputActionValue;
class Enemy;


//Enum per il piede destro e sinistro
UENUM(BlueprintType)
enum class EFoot : uint8
{
	Left UMETA(DisplayName = "Left"),
	Right UMETA(DislayName = "Right")

};

DECLARE_LOG_CATEGORY_EXTERN(LogTemplateCharacter, Log, All);

UCLASS(config=Game)
class AStealth_PrototypeCharacter : public ACharacter
{
	GENERATED_BODY()

public:
	UPROPERTY(EditDefaultsOnly, Category = "Stats")
	float MaxHealth = 100;
	UPROPERTY(VisibleAnywhere, Category = "Stats")
	float Health;

	/** Camera boom positioning the camera behind the character */
	UPROPERTY(VisibleAnywhere, BlueprintReadOnly, Category = Camera, meta = (AllowPrivateAccess = "true"))
	USpringArmComponent* CameraBoom;

	/** Follow camera */
	UPROPERTY(VisibleAnywhere, BlueprintReadOnly, Category = Camera, meta = (AllowPrivateAccess = "true"))
	UCameraComponent* FollowCamera;
	
	/** MappingContext */
	UPROPERTY(EditAnywhere, BlueprintReadOnly, Category = Input, meta = (AllowPrivateAccess = "true"))
	UInputMappingContext* DefaultMappingContext;

	/** Jump Input Action */
	UPROPERTY(EditAnywhere, BlueprintReadOnly, Category = Input, meta = (AllowPrivateAccess = "true"))
	UInputAction* JumpAction;

	/** Move Input Action */
	UPROPERTY(EditAnywhere, BlueprintReadOnly, Category = Input, meta = (AllowPrivateAccess = "true"))
	UInputAction* MoveAction;

	/** Look Input Action */
	UPROPERTY(EditAnywhere, BlueprintReadOnly, Category = Input, meta = (AllowPrivateAccess = "true"))
	UInputAction* LookAction;

	// Crouch Input Action 
	UPROPERTY(EditAnywhere, BlueprintReadOnly, Category = Input, meta = (AllowPrivateAccess = "true"))
	UInputAction* CrouchAction;

	//Stealth kill Input Action
	UPROPERTY(EditAnywhere, BlueprintReadOnly, Category = Input, meta = (AllowPrivateAccess = "true"))
	UInputAction* StealthKillAction;

	//FootStep component per il rumore dei piedi
	UPROPERTY(BlueprintReadOnly)
	class UFootstep_Component* FootstepsComponent;

	/** Returns CameraBoom subobject **/
	FORCEINLINE class USpringArmComponent* GetCameraBoom() const { return CameraBoom; }
	/** Returns FollowCamera subobject **/
	FORCEINLINE class UCameraComponent* GetFollowCamera() const { return FollowCamera; }

	class UFootstep_Component* GetFootstepComponent() const;

	//Animazione di morte
	UPROPERTY(EditAnywhere, Category = "Animation")
	UAnimMontage* DeathMontage;

private:
	//Sta eseguendo una kill
	bool bIsPerformingStealthKill = false;
	//Stimolo AI
	class UAIPerceptionStimuliSourceComponent* StimulusSource;
	//Nemico trovato da uccidere
	UPROPERTY(VisibleAnywhere, BlueprintReadOnly, Category = "Stealth", meta = (AllowPrivateAccess = "true"))
	class AEnemy* TargetEnemy;
	//Animazione di kill
	UPROPERTY(EditDefaultsOnly, Category = "Stealth")
	class UAnimMontage* StealthKillMontage;


public:
	//Costruttore
	AStealth_PrototypeCharacter();
	//Funzione di check morte
	UFUNCTION(BlueprintPure)
	bool IsDead() const;


public:
	//Ragdoll
	void EnableRagdoll();

	//Funzioni per gestire la health
	float GetHealth() const;
	float GetMaxHealth() const;
	void SetHealth(float const NewHealth);

			


protected:

	/** Called for movement input */
	void Move(const FInputActionValue& Value);

	/** Called for looking input */
	void Look(const FInputActionValue& Value);
	// APawn interface
	virtual void SetupPlayerInputComponent(class UInputComponent* PlayerInputComponent) override;
	
	// To add mapping context
	virtual void BeginPlay();

private:

	//Set up Stimolo
	void SetupStimulusSource();
	//Funzione Check Dietro al nemico
	bool IsBehindEnemy(AEnemy* targetEnemy);

	//Crouch
	void OnCrouchActionStarted(const FInputActionValue& Value);
	void OnCrouchActionEnded(const FInputActionValue& Value);

	//Stealth Kill
	void FindStealthKillTarget();
	void PerformStealthKill();
	void FinishStealthKill();

};

