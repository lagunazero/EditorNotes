#include "EditorNoteActor.h"
#include "WidgetComponent.h"
#include "EditorViewportClient.h"
#include "DrawDebugHelpers.h"
#include "UnrealEd.h"
#include "Components/SceneComponent.h"

AEditorNoteActor::AEditorNoteActor()
{
	PrimaryActorTick.bCanEverTick = true;

	Root = CreateDefaultSubobject<USceneComponent>(TEXT("Root"));
	RootComponent = Root;

	Widget = CreateDefaultSubobject<UWidgetComponent>(TEXT("Widget"));
}

void AEditorNoteActor::BeginPlay()
{
	// TODO: Doesn't seem to work. Too early? Calling in first Tick instead.
	UpdateText();
}

void AEditorNoteActor::Tick(float DeltaSeconds)
{
	Super::Tick(DeltaSeconds);

	if (!bHasTicked)
	{
		bHasTicked = true;
		UpdateText();
	}

	if (IsSelected())
	{
		if (!bWasSelected)
		{
			bWasSelected = true;
			OnSelectionChange(true);
		}

		UpdateText();
		ShowRelatedActors();
	}
	else if (bWasSelected)
	{
		bWasSelected = false;
		OnSelectionChange(false);
	}

	// Rotate to camera.
	if (UEditorNotesSettings::GetAutoRotate())
	{
		AutoRotate();
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
	const FViewport* Viewport = GEditor->GetActiveViewport();
	if (!Viewport) return;
	const FEditorViewportClient* ViewportClient = (FEditorViewportClient*)Viewport->GetClient();
	if (!ViewportClient) return;

	const FVector EditorCameraLocation = ViewportClient->GetViewLocation();
	const FRotator EditorCameraRotation = ViewportClient->GetViewRotation();
	const FVector EditorCameraForwardVector = EditorCameraRotation.Vector();

	SetActorLocation(EditorCameraLocation + EditorCameraForwardVector * 500.f);
	SetActorRotation(FRotator(EditorCameraRotation.Pitch + 180.f, EditorCameraRotation.Yaw, EditorCameraRotation.Roll + 180.f));
}

void AEditorNoteActor::ShowRelatedActors()
{
	const FColor LineColor = UEditorNotesSettings::GetRelatedActorLineColor();
	const FColor LineColorBorder = UEditorNotesSettings::GetRelatedActorLineColorBorder();
	const float LineWidth = UEditorNotesSettings::GetRelatedActorLineWidth();
	const float LineWidthBorder = UEditorNotesSettings::GetRelatedActorLineWidthBorder();
	for (TSoftObjectPtr<AActor> RelatedActor : RelatedActors)
	{
		if (!RelatedActor.IsValid()) continue;

		if (LineWidthBorder > 0)
		{
			DrawDebugLine(GetWorld(), GetActorLocation(), RelatedActor->GetActorLocation(), LineColorBorder, false, -1.f, 0, LineWidth + LineWidthBorder * 2.f);
		}
		if (LineWidth > 0)
		{
			DrawDebugLine(GetWorld(), GetActorLocation(), RelatedActor->GetActorLocation(), LineColor, false, -1.f, 0, LineWidth);
		}
	}
}

void AEditorNoteActor::AutoRotate()
{
	if (UWorld* World = GetWorld())
	{
		if (World->WorldType == EWorldType::Editor)
		{
			// Get editor camera.
			if (GEditor)
			{
				if (FViewport* Viewport = GEditor->GetActiveViewport())
				{
					if (FEditorViewportClient* ViewportClient = (FEditorViewportClient*)Viewport->GetClient())
					{
						SetActorRotation((ViewportClient->GetViewLocation() - GetActorLocation()).Rotation());
					}
				}
			}
		}
		//else if (World->WorldType == EWorldType::PIE)
		//{
		//	UE_LOG(LogTemp, Log, TEXT("PIE"));
		//}
		//else if (World->WorldType == EWorldType::Game)
		//{
		//	UE_LOG(LogTemp, Log, TEXT("Game"));
		//}
	}
}

FString AEditorNoteActor::GetPrioAsString() const
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
