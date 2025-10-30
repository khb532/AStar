#pragma once

#include "CoreMinimal.h"
#include "GameFramework/Actor.h"
#include "BlockActor.generated.h"

UCLASS()
class ASTAR_API ABlockActor : public AActor
{
	GENERATED_BODY()

public:
	ABlockActor();

protected:
	virtual void BeginPlay() override;

public:
	virtual void Tick(float DeltaTime) override;

	//	Change Outline
	void ChangeColor_Outline(FLinearColor color);

	//	Calc Cost
	void SetCost(ABlockActor* start, ABlockActor* goal);

	void Init(int32 ny, int32 nx);



public:
	UPROPERTY()
	TObjectPtr<class UImage> m_Outline;

	UPROPERTY()
	TObjectPtr<class UTextBlock> FCost;
	float m_FCostValue = -1;

	UPROPERTY()
	TObjectPtr<class UTextBlock> GCost;
	float m_GCostValue = -1;

	UPROPERTY()
	TObjectPtr<class UTextBlock> HCost;
	float m_HCostValue = -1;

	//	Movable var
	bool b_CanMove = true;

	//	Row
	int32 m_x_idx;
	//	Col
	int32 m_y_idx;
	//	Array Index
	int32 m_arr_idx;

	//	Parent Idx ( 누구로부터 cost 계산했는지 )
	int32 m_parent_idx = -1;
};
