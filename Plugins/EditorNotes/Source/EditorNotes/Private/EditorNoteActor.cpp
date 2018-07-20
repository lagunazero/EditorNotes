#include "EditorNoteActor.h"
#include "WidgetComponent.h"
#include "EditorViewportClient.h"
#include "UnrealEd.h"

AEditorNoteActor::AEditorNoteActor()
{
	PrimaryActorTick.bCanEverTick = true;

	Widget = CreateDefaultSubobject<UWidgetComponent>(TEXT("Widget"));
}

void AEditorNoteActor::Tick(float DeltaSeconds)
{
	Super::Tick(DeltaSeconds);

	UpdateText();

	// Rotate to editor camera.
	if (UWorld* World = GetWorld())
	{
		if (World->WorldType == EWorldType::Editor)
		{
			if (GEditor)
			{
				if (FEditorViewportClient* ViewportClient = (FEditorViewportClient*)GEditor->GetActiveViewport()->GetClient())
				{
					SetActorRotation((ViewportClient->GetViewLocation() - GetActorLocation()).Rotation());
				}
			}
		}
		else if (World->WorldType == EWorldType::PIE)
		{
			UE_LOG(LogTemp, Log, TEXT("PIE"));
		}
		else if (World->WorldType == EWorldType::Game)
		{
			UE_LOG(LogTemp, Log, TEXT("Game"));
		}
	}
}

void AEditorNoteActor::InitNote()
{
	ResetDate();
	ResetUser();
	ResetPosition();
}

void AEditorNoteActor::ResetDate()
{
	Date = FDateTime::Now();
}

void AEditorNoteActor::ResetUser()
{
	Creator = UKismetSystemLibrary::GetPlatformUserName();
}

void AEditorNoteActor::ResetPosition()
{
	if (!GEditor) return;

	FEditorViewportClient* ViewportClient = (FEditorViewportClient*)GEditor->GetActiveViewport()->GetClient();
	if (!ViewportClient) return;

	const FVector EditorCameraLocation = ViewportClient->GetViewLocation();
	const FRotator EditorCameraRotation = ViewportClient->GetViewRotation();
	const FVector EditorCameraForwardVector = EditorCameraRotation.Vector();

	SetActorLocation(EditorCameraLocation + EditorCameraForwardVector * 500.f);
	SetActorRotation(FRotator(EditorCameraRotation.Pitch + 180.f, EditorCameraRotation.Yaw, EditorCameraRotation.Roll + 180.f));
}

FString AEditorNoteActor::PrioAsString() const
{
	switch (Prio)
	{
	case ENotePrioType::NPT_LOW: return "Low";
	case ENotePrioType::NPT_MID: return "Mid";
	case ENotePrioType::NPT_HIGH: return "High";
	case ENotePrioType::NPT_COMMENT: return "Comment";
	default: return "Invalid";
	}
}
