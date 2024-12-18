#pragma once

#include "CoreMinimal.h"
#include "CameraFollowTarget.h"
#include "InputActionValue.h"
#include "GameFramework/Character.h"
#include "SmashCharacter.generated.h"

class USmashCharacterInputData;
class UInputMappingContext;
class USmashCharacterStateMachine;

UCLASS()
class SMASHUE_API ASmashCharacter : public ACharacter,
									public ICameraFollowTarget
{
	GENERATED_BODY()

#pragma region Orient
public:
	float GetOrientX () const;
	void SetOrientX (float NewOrientX);
	
protected:
	UPROPERTY(BlueprintReadOnly)
	float OrientX = 1.0f;

	void RotateMeshUsingOrientX() const;
#pragma endregion

#pragma region Unreal Default
public:
	// Sets default values for this character's properties
	ASmashCharacter();

protected:
	// Called when the game starts or when spawned
	virtual void BeginPlay() override;

public:	
	// Called every frame
	virtual void Tick(float DeltaTime) override;

	// Called to bind functionality to input
	virtual void SetupPlayerInputComponent(class UInputComponent* PlayerInputComponent) override;
	
#pragma endregion

#pragma region State Machine

public:
	void CreateStateMachine();
	void InitStateMachine();
	void TickStateMachine(float DeltaTime) const;
	
protected:
	UPROPERTY(BlueprintReadOnly)
	TObjectPtr<USmashCharacterStateMachine> StateMachine;
	
#pragma endregion

#pragma region Input Data / Mapping Context
public:
	UPROPERTY()
	TObjectPtr<UInputMappingContext> InputMappingContext;
	UPROPERTY()
	TObjectPtr<USmashCharacterInputData> InputData;

protected:
	void SetupMappingContextIntoController() const;
	
#pragma endregion

#pragma region Input Move X

	DECLARE_DYNAMIC_MULTICAST_DELEGATE_OneParam(FInputMoveXEvent, float, InputMoveX);
	
public:
	float GetInputMoveX() const;
	float InputMoveXThreshold = 0;

	UPROPERTY()
	FInputMoveXEvent InputMoveXFastEvent;
	
protected:
	UPROPERTY()
	float InputMoveX = 0.0f;
	
private:
	void OnInputMoveXFast(const FInputActionValue& InputActionValue);
	void BindInputMoveXAxisAndActions(UEnhancedInputComponent* EnhancedInputComponent);
	void OnInputMoveX(const FInputActionValue& InputActionValue);
	
#pragma endregion

#pragma region Jump
public:
	DECLARE_DYNAMIC_MULTICAST_DELEGATE_OneParam(FInputJumpEvent, float, InputJump);

	UPROPERTY()
	FInputJumpEvent InputJumpEvent;

private:
	void OnInputJump (const FInputActionValue& InputActionValue);

public:
	virtual FVector GetFollowPosition() override;
	virtual bool IsFollowable() override;

private:
#pragma endregion

#pragma region Special Move
public:
	DECLARE_DYNAMIC_MULTICAST_DELEGATE(FInputSpecialEvent);

	UPROPERTY()
	FInputSpecialEvent InputSpecialEvent;

	void OnInputSpecialMove(const FInputActionValue& InputActionValue);
	
#pragma endregion

#pragma region Damage
public:
	UPROPERTY(EditAnywhere)
	float Damage = 0.0f;
	void HitDamage(float DamageAmount);

	void KnockBack(FVector Direction);
	
#pragma endregion
};
