#include "MyBTTask_FindPlayerLocation.h"
#include "Kismet/GameplayStatics.h"
#include "NavigationSystem.h"
#include "GameFramework/Character.h"
#include "BehaviorTree/BlackboardComponent.h"

//Costruttore
UMyBTTask_FindPlayerLocation::UMyBTTask_FindPlayerLocation(FObjectInitializer const& ObjectInitializer) :
	UBTTask_BlackboardBase{ ObjectInitializer }
{
	NodeName = TEXT("Find Player Location");
}

//ExecuteTask
EBTNodeResult::Type UMyBTTask_FindPlayerLocation::ExecuteTask(UBehaviorTreeComponent& OwnerComp, uint8* NodeMemory)
{
	//Ottiene il player
	if (auto* const Player = UGameplayStatics::GetPlayerCharacter(GetWorld(), 0))
	{
		//Ottiene la posizione del player
		auto const PlayerLocation = Player->GetActorLocation();

		//Controlla se deve cercare una posizione casuale intorno al giocatore
		if (SearchRandom)
		{
			//Memorizzo posizione valida nella navigazione
			FNavLocation Loc;  

			//Ottiengo il UNavigationSystemV1
			if (auto* const NavSys = UNavigationSystemV1::GetCurrent(GetWorld()))
			{
				//Trova una posizione casuale all'interno del raggio specificato attorno al giocatore
				if (NavSys->GetRandomPointInNavigableRadius(PlayerLocation, SearchRadius, Loc))
				{
					//Setto la variabile nella blackboard
					OwnerComp.GetBlackboardComponent()->SetValueAsVector(GetSelectedBlackboardKey(), Loc.Location);

					//Finisco Task
					FinishLatentTask(OwnerComp, EBTNodeResult::Succeeded);
					return EBTNodeResult::Succeeded;
				}
			}
		}
		else
		{
			OwnerComp.GetBlackboardComponent()->SetValueAsVector(GetSelectedBlackboardKey(), PlayerLocation);

			FinishLatentTask(OwnerComp, EBTNodeResult::Succeeded);
			return EBTNodeResult::Succeeded;
		}
	}

	return EBTNodeResult::Failed;
}
