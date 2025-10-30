
#include "AStarPawn.h"

#include "BlockActor.h"
#include "Blueprint/WidgetBlueprintLibrary.h"


AAStarPawn::AAStarPawn()
{
	PrimaryActorTick.bCanEverTick = true;
}

void AAStarPawn::BeginPlay()
{
	Super::BeginPlay();

	//	10 x 10 create block
	ABlockActor* block = nullptr;
	FVector pos = FVector();
	for (int i = 0; i < 100; ++i)
	{
		pos.Y =  i % 10 * 100;
		pos.X = i / 10 * 100;
		block = GetWorld()->SpawnActor<ABlockActor>(m_BlockActor_Class, pos, FRotator::ZeroRotator);

		block->Init(pos.Y, pos.X);
	}

	//	Show Mouse Cursor
	APlayerController* pc = GetWorld()->GetFirstPlayerController();
	UWidgetBlueprintLibrary::SetInputMode_GameAndUIEx(pc);
	pc->SetShowMouseCursor(true);
}

void AAStarPawn::Tick(float DeltaTime)
{
	Super::Tick(DeltaTime);

	if (GetWorld()->GetFirstPlayerController()->WasInputKeyJustPressed(EKeys::LeftMouseButton))
		FindPath();
}

void AAStarPawn::SetupPlayerInputComponent(UInputComponent* PlayerInputComponent)
{
	Super::SetupPlayerInputComponent(PlayerInputComponent);
}

void AAStarPawn::FindPath()
{
	//	Start Location (player)
	FVector pos = GetActorLocation();
	int32 ny = pos.Y / 100;	// ( Y / Single Raw size)
	int32 nx = pos.X / 100;	// ( X / Single Col size)

	int32 start_idx = ny + nx * 10;
	UE_LOG(LogTemp, Warning, TEXT("Start : %d"), start_idx);
	//	Start Idx, Add to Open Array
	m_arr_openarray.HeapPush(start_idx, FBlockCompare(blocks));

	//	Arrived Location Block
	FVector start;
	FVector end;
	FVector dir;

	FCollisionQueryParams params;
	FHitResult hitresult;
	
	//	Mouse Click Location to 3D loc&rot
	GetWorld()->GetFirstPlayerController()->DeprojectMousePositionToWorld(start, dir);
	end = start + dir * 100000;
	params.AddIgnoredActor(this);
	GetWorld()->LineTraceSingleByChannel(hitresult, start, end, ECC_Visibility, params);

	if (ABlockActor* block = Cast<ABlockActor>(hitresult.GetActor()))
	{
		UE_LOG(LogTemp, Warning, TEXT("Goal : %d"), block->m_arr_idx);
	}
}

