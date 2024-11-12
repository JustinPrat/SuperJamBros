#include "CameraWorldSubsystem.h"

#include "CameraFollowTarget.h"
#include "Kismet/GameplayStatics.h"

void UCameraWorldSubsystem::PostInitialize()
{
	Super::PostInitialize();
}

void UCameraWorldSubsystem::OnWorldBeginPlay(UWorld& InWorld)
{
	Super::OnWorldBeginPlay(InWorld);
	CameraMain = FindCameraByTag(TEXT("CameraMain"));
}

void UCameraWorldSubsystem::Tick(float DeltaTime)
{
	Super::Tick(DeltaTime);
	TickUpdateCameraPosition(DeltaTime);
}

void UCameraWorldSubsystem::AddFollowTarget(UObject* FollowTarget)
{
	FollowTargets.Add(FollowTarget);
}

void UCameraWorldSubsystem::RemoveFollowTarget(UObject* FollowTarget)
{
	FollowTargets.Remove(FollowTarget);
}

void UCameraWorldSubsystem::TickUpdateCameraPosition(float DeltaTime)
{
	FVector CameraAverage = CalculateAveragePositionBetweenTargets();
	CameraAverage.Y = CameraMain->GetOwner()->GetActorLocation().Y;
	CameraMain->GetOwner()->SetActorLocation(CameraAverage);
}

FVector UCameraWorldSubsystem::CalculateAveragePositionBetweenTargets()
{
	FVector average = FVector::ZeroVector;
	for (int i = 0; i < FollowTargets.Num(); i++)
	{
		ICameraFollowTarget* followTarget = Cast<ICameraFollowTarget>(FollowTargets[i]);
		if (followTarget && followTarget->IsFollowable())
		{
			average += followTarget->GetFollowPosition();
		}
	}
	
	return average / FollowTargets.Num();
}

UCameraComponent* UCameraWorldSubsystem::FindCameraByTag(const FName& Tag) const
{
	TArray<AActor*> FoundActors;
	UGameplayStatics::GetAllActorsWithTag(GetWorld(), Tag, FoundActors);
	return FoundActors[0]->GetComponentByClass<UCameraComponent>();
}
