#include "CameraWorldSubsystem.h"

#include "CameraFollowTarget.h"
#include "CameraSettings.h"
#include "Kismet/GameplayStatics.h"

class UCameraSettings;

void UCameraWorldSubsystem::PostInitialize()
{
	Super::PostInitialize();
}

void UCameraWorldSubsystem::OnWorldBeginPlay(UWorld& InWorld)
{
	CameraSettings = GetDefault<UCameraSettings>();
	Super::OnWorldBeginPlay(InWorld);
	CameraMain = FindCameraByTag(CameraSettings->CameraMainTag);

	AActor* CameraBoundsActor = FindCameraBoundsActor();
	if (CameraBoundsActor != nullptr)
	{
		InitCameraBounds(CameraBoundsActor);
	}

	InitCameraZoomParameters();
}

void UCameraWorldSubsystem::Tick(float DeltaTime)
{
	Super::Tick(DeltaTime);
	TickUpdateCameraZoom(DeltaTime);
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

AActor* UCameraWorldSubsystem::FindCameraBoundsActor()
{
	TArray<AActor*> FoundActors;
	UGameplayStatics::GetAllActorsWithTag(GetWorld(), CameraSettings->CameraBoundsTag, FoundActors);
	return FoundActors[0];
}

void UCameraWorldSubsystem::InitCameraBounds(AActor* CameraBoundsActor)
{
	FVector BoundsCenter;
	FVector BoundsExtents;
	CameraBoundsActor->GetActorBounds(false, BoundsCenter, BoundsExtents);

	CameraBoundsMin.X = BoundsCenter.X - BoundsExtents.X;
	CameraBoundsMin.Y = BoundsCenter.Z - BoundsExtents.Z;
	CameraBoundsMax.X = BoundsCenter.X + BoundsExtents.X;
	CameraBoundsMax.Y = BoundsCenter.Z + BoundsExtents.Z;

	CameraBoundsYProjectionCenter = BoundsCenter.Y;
}

void UCameraWorldSubsystem::ClampPositionIntoCameraBounds(FVector& Position)
{
	FVector2D ViewportBoundsMin, ViewportBoundsMax;
	GetViewportBounds(ViewportBoundsMin, ViewportBoundsMax);

	FVector WorldBoundsMin = CalculateWorldPositionFromViewportPosition(ViewportBoundsMin);
	FVector WorldBoundsMax = CalculateWorldPositionFromViewportPosition(ViewportBoundsMax);

	FVector2D WorldBoundsExtents;
	WorldBoundsExtents.X = (WorldBoundsMax.X - WorldBoundsMin.X) / 2.0f;
	WorldBoundsExtents.Y = (WorldBoundsMax.Z - WorldBoundsMin.Z) / 2.0f;

	float XMin = CameraBoundsMin.X + WorldBoundsExtents.X;
	float YMin = CameraBoundsMin.Y - WorldBoundsExtents.Y;

	float XMax = CameraBoundsMax.X - WorldBoundsExtents.X;
	float YMax = CameraBoundsMax.Y - WorldBoundsExtents.Y;

	Position.X = FMath::Clamp(Position.X, XMin, XMax);
	Position.Z = FMath::Clamp(Position.Z, YMin, YMax);
}

void UCameraWorldSubsystem::GetViewportBounds(FVector2D& OutViewportBoundsMin, FVector2D& OutViewportBoundsMax)
{
	UGameViewportClient* ViewportClient = GetWorld()->GetGameViewport();
	if (ViewportClient == nullptr) return;

	FViewport* Viewport = ViewportClient->Viewport;
	if (Viewport == nullptr) return;

	FIntRect ViewRect(Viewport->GetInitialPositionXY(), Viewport->GetInitialPositionXY() + Viewport->GetSizeXY());
	FIntRect ViewportRect = Viewport->CalculateViewExtents(CameraMain->AspectRatio, ViewRect);

	OutViewportBoundsMin.X = ViewportRect.Min.X;
	OutViewportBoundsMin.Y = ViewportRect.Min.Y;

	OutViewportBoundsMax.X = ViewportRect.Max.X;
	OutViewportBoundsMax.Y = ViewportRect.Max.Y;
}

FVector UCameraWorldSubsystem::CalculateWorldPositionFromViewportPosition(const FVector2D& ViewportPosition)
{
	if (CameraMain == nullptr) return FVector::Zero();

	APlayerController* PlayerController = UGameplayStatics::GetPlayerController(GetWorld(), 0);
	if (PlayerController == nullptr) return FVector::Zero();

	float YDistanceToCenter = CameraMain->GetOwner()->GetActorLocation().Y - CameraBoundsYProjectionCenter;

	FVector CameraWorldProjectDir;
	FVector WorldPosition;
	UGameplayStatics::DeprojectScreenToWorld(PlayerController, ViewportPosition, WorldPosition, CameraWorldProjectDir);

	WorldPosition += CameraWorldProjectDir * YDistanceToCenter;

	return WorldPosition;
}

void UCameraWorldSubsystem::TickUpdateCameraPosition(float DeltaTime)
{
	FVector CameraAverage = CalculateAveragePositionBetweenTargets();
	CameraAverage.Y = CameraMain->GetOwner()->GetActorLocation().Y;
	ClampPositionIntoCameraBounds(CameraAverage);
	CameraAverage = FMath::Lerp(CameraMain->GetOwner()->GetActorLocation(), CameraAverage, DeltaTime * CameraSettings->PositionDampingFactor);	
	
	CameraMain->GetOwner()->SetActorLocation(CameraAverage);
}

void UCameraWorldSubsystem::TickUpdateCameraZoom(float DeltaTime)
{
	if (CameraMain == nullptr) return;

	float GreatestDistanceBetweenTargets = CalculateGreatestDistanceBetweenTargets();
	float Percentage = (GreatestDistanceBetweenTargets - CameraZoomDistanceBetweenTargetsMin) / (CameraZoomDistanceBetweenTargetsMax - CameraZoomDistanceBetweenTargetsMin);
	Percentage = FMath::Clamp(Percentage, 0.f, 1.f);
	
	FVector NewPos = CameraMain->GetOwner()->GetActorLocation();
	NewPos.Y = FMath::Lerp(CameraZoomYMin, CameraZoomYMax, Percentage);
	NewPos.Y = FMath::Lerp(CameraMain->GetOwner()->GetActorLocation().Y, NewPos.Y, DeltaTime * CameraSettings->SizeDampingFactor);	
	CameraMain->GetOwner()->SetActorLocation(NewPos);
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

float UCameraWorldSubsystem::CalculateGreatestDistanceBetweenTargets()
{
	float GreatestDistance = 0;
	for (int i = 0; i < FollowTargets.Num(); i++)
	{
		ICameraFollowTarget* followTarget = Cast<ICameraFollowTarget>(FollowTargets[i]);
		
		for (int j = 0; j < FollowTargets.Num(); j++)
		{
			if (j == i) continue;
			ICameraFollowTarget* otherCharacter = Cast<ICameraFollowTarget>(FollowTargets[j]);
			float distBetweenChar = FVector::Dist(followTarget->GetFollowPosition() ,otherCharacter->GetFollowPosition());
			if (followTarget->IsFollowable() && distBetweenChar > GreatestDistance)
			{
				GreatestDistance = distBetweenChar;
			}
		}
	}
	return GreatestDistance;
}

UCameraComponent* UCameraWorldSubsystem::FindCameraByTag(const FName& Tag) const
{
	TArray<AActor*> FoundActors;
	UGameplayStatics::GetAllActorsWithTag(GetWorld(), Tag, FoundActors);
	return FoundActors[0]->GetComponentByClass<UCameraComponent>();
}

void UCameraWorldSubsystem::InitCameraZoomParameters()
{
	UCameraComponent* camMAX = FindCameraByTag(CameraSettings->CameraDistanceMaxTag);
	UCameraComponent* camMIN = FindCameraByTag(CameraSettings->CameraDistanceMinTag);

	CameraZoomYMax = camMAX->GetComponentLocation().Y;
	CameraZoomYMin = camMIN->GetComponentLocation().Y;

	CameraZoomDistanceBetweenTargetsMin = CameraSettings->CameraDistanceBetweenTargetsMin;
	CameraZoomDistanceBetweenTargetsMax = CameraSettings->CameraDistanceBetweenTargetsMax;

}
