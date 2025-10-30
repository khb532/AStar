#pragma once

#include "CoreMinimal.h"
#include "BlockActor.h"
#include "GameFramework/Pawn.h"
#include "AStarPawn.generated.h"

class FBlockCompare
{
public:
	const TArray<ABlockActor*>& blocks;
	FBlockCompare(const TArray<ABlockActor*>& inblocks) : blocks(inblocks) {}
	bool operator() (const int32& a, const int32& b) const
	{
		//	Sort 할때 자리 바꾸는 조건
		//	최종 FCost 작은값 앞으로
		if (blocks[a]->m_FCostValue < blocks[b]->m_FCostValue) return true;

		//	최종 FCost 같을때
		//	시작 GCost 큰값 앞으로
		if (blocks[a]->m_FCostValue == blocks[b]->m_FCostValue && blocks[a]->m_GCostValue > blocks[b]->m_GCostValue) return true;
		
		return false;
	}
};

UCLASS()
class ASTAR_API AAStarPawn : public APawn
{
	GENERATED_BODY()

public:
	AAStarPawn();

	virtual void BeginPlay() override;

	virtual void Tick(float DeltaTime) override;

	virtual void SetupPlayerInputComponent(class UInputComponent* PlayerInputComponent) override;

	void StartFindPath();

	void FindPath();

	//	Calc Cost & Add Open Array
	void AddOpenArray(FVector dir);

	//	세로 nx 번, 가로 ny 번 Block, 10 X 10 범위 안에 존재하는지
	bool CheckIndex(int32 nx, int32 ny);

public:
	UPROPERTY(EditAnywhere)
	TSubclassOf<class ABlockActor> m_BlockActor_Class;

	UPROPERTY()
	TArray<TObjectPtr<class ABlockActor>> blocks;

	//	Open Array (block 기준 상하좌우,대각 calc cost)
	UPROPERTY()
	TArray<int32> m_Arr_OpenArray;

	//	Close Array (검색 끝난 blocks)
	UPROPERTY()
	TArray<int32> m_Arr_CloseArray;
	
	//  Destination block index
	int32 m_GoalBlockIdx = -1;

	//	현재 중심 block index
	int32 m_CurrBlockIdx = -1;
	
};

