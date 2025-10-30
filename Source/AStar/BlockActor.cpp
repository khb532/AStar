
#include "BlockActor.h"

#include "Components/Image.h"
#include "Components/TextBlock.h"
#include "Components/WidgetComponent.h"


// Sets default values
ABlockActor::ABlockActor()
{
	PrimaryActorTick.bCanEverTick = true;
}

void ABlockActor::BeginPlay()
{
	Super::BeginPlay();
	
	//	Get Widget
	UWidgetComponent* compwidget = GetComponentByClass<UWidgetComponent>();
	
	UUserWidget* costui = compwidget->GetWidget();

	//	Get Outline
	m_Outline = Cast<UImage>(costui->GetWidgetFromName(TEXT("Outline")));

	//	Get Costs
	FCost = Cast<UTextBlock>(costui->GetWidgetFromName(TEXT("FCost")));
	GCost = Cast<UTextBlock>(costui->GetWidgetFromName(TEXT("GCost")));
	HCost = Cast<UTextBlock>(costui->GetWidgetFromName(TEXT("HCost")));

	//	Set Random Movable
	int32 rand = FMath::RandRange(0, 99);
	if (rand < 10)
	{
		b_CanMove = false;
		ChangeColor_Outline(FLinearColor::Red);
	}
}

void ABlockActor::Tick(float DeltaTime)
{
	Super::Tick(DeltaTime);
}

void ABlockActor::ChangeColor_Outline(FLinearColor color)
{
	//	Change Color
	m_Outline->SetColorAndOpacity(color);
		
}

void ABlockActor::SetCost(ABlockActor* start, ABlockActor* goal)
{
	
	
	
}

void ABlockActor::Init(int32 ny, int32 nx)
{
	m_y_idx = ny / 100;
	m_x_idx = nx % 100;
	m_arr_idx = m_y_idx + m_x_idx * 10;
}

