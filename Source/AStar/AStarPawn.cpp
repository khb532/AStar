
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
		blocks.Add(block);
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
		StartFindPath();

	if (GetWorld()->GetFirstPlayerController()->WasInputKeyJustPressed(EKeys::RightMouseButton))
	{
		// 목표 지점이 설정되지 않았으면 무시
		if (m_GoalBlockIdx == -1) return;

		if (m_CurrBlockIdx == m_GoalBlockIdx)
		{
			ABlockActor* tmp = blocks[m_GoalBlockIdx];
			while (tmp->m_parent_idx != -1)
			{
				tmp->ChangeColor_Outline(FLinearColor::Black);
				tmp = blocks[tmp->m_parent_idx];
			}
		}
		else
		{
			FindPath();
		}
	}
}

void AAStarPawn::SetupPlayerInputComponent(UInputComponent* PlayerInputComponent)
{
	Super::SetupPlayerInputComponent(PlayerInputComponent);
}

void AAStarPawn::StartFindPath()
{
	//	Start Location (player)
	FVector pos = GetActorLocation();
	int32 ny = pos.Y / 100;	// ( Y / Single Raw size)
	int32 nx = pos.X / 100;	// ( X / Single Col size)

	int32 start_idx = ny + nx * 10;
	UE_LOG(LogTemp, Warning, TEXT("Start : %d"), start_idx);
	//	Start Idx, Add to Open Array
	m_Arr_OpenArray.HeapPush(start_idx, FBlockCompare(blocks));

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

		if (block->b_CanMove)
			m_GoalBlockIdx = block->m_arr_idx;
	}
	//	시작 지점과 도착지점이 설정되었다면
	if (m_GoalBlockIdx != -1)
		FindPath();
}

void AAStarPawn::FindPath()
{
	//	OpenArray 에서 우선순위 되는 value Get, array에서 제거
	m_Arr_OpenArray.HeapPop(m_CurrBlockIdx, FBlockCompare(blocks));

	//	IF Closed Block return
	if (m_Arr_CloseArray.Contains(m_CurrBlockIdx)) return;

	AddOpenArray(FVector::ForwardVector);
	AddOpenArray(FVector::BackwardVector);
	AddOpenArray(FVector::RightVector);
	AddOpenArray(FVector::LeftVector);
	AddOpenArray(FVector::ForwardVector + FVector::RightVector);
	AddOpenArray(FVector::ForwardVector + FVector::LeftVector);
	AddOpenArray(FVector::BackwardVector + FVector::RightVector);
	AddOpenArray(FVector::BackwardVector + FVector::LeftVector);

	m_Arr_CloseArray.Add(m_CurrBlockIdx);
	blocks[m_CurrBlockIdx]->ChangeColor_Outline(FLinearColor::Blue);
}

void AAStarPawn::AddOpenArray(FVector dir)
{
	//	Next Y axis Index
	int32 next_Y = blocks[m_CurrBlockIdx]->m_y_idx + dir.Y;
	//	Next X axis Index
	int32 next_X = blocks[m_CurrBlockIdx]->m_x_idx + dir.X;

	//	block 범위 밖이라면 return
	if (CheckIndex(next_X, next_Y) == false) return;

	//	Target Block Index
	int32 target_block_idx = m_CurrBlockIdx;

	if (dir.X == 1)				//	forward
		target_block_idx += 10;

	else if (dir.X == -1)		//	backward
		target_block_idx -= 10;

	else if (dir.Y == 1)		//	Right
		target_block_idx++;

	else if (dir.Y == -1)		//	Left
		target_block_idx--;

	//	if Cant Moveable Block return
	if (blocks[target_block_idx]->b_CanMove == false) return;

	//	Calc Cost of Target Block
	blocks[target_block_idx]->SetCost(blocks[m_CurrBlockIdx], blocks[m_GoalBlockIdx]);

	//	Add Block in Open Array
	m_Arr_OpenArray.HeapPush(target_block_idx, FBlockCompare(blocks));

	blocks[target_block_idx]->ChangeColor_Outline(FLinearColor::Yellow);
	
}

bool AAStarPawn::CheckIndex(int32 nx, int32 ny)
{
	if (0 > nx || 10/*세로*/ <= nx) return false;
	if (0 > ny || 10/*가로*/ <= ny) return false;

	return true;
}

