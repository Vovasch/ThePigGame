#pragma once

#include "CoreMinimal.h"
#include "ConsumeSourcePrototype.generated.h"

UCLASS(Blueprintable, DefaultToInstanced, EditInlineNew)
class THEPIGGAME_API UConsumeSourcePrototype : public UObject {

	GENERATED_BODY()

	public:
	UPROPERTY(EditAnywhere)
	float m_fCapacity = 100.f;

	UPROPERTY(EditAnywhere)
	float m_fStaringAmount = 50.f;
};
