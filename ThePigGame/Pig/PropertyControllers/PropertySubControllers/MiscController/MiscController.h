#pragma once

#include "CoreMinimal.h"
#include "ThePigGame/Pig/PigProperties/PigProperties.h"
#include "ThePigGame/Pig/PropertyControllers/PropertySubControllers/PropertySubControllerBase.h"
#include "MiscController.generated.h"

UCLASS()
class THEPIGGAME_API UMiscController : public UPropertySubControllerBase {
	GENERATED_BODY()

	public:
	const Scale* GetScale();
	const Morph* GetMorph();

	public:
	virtual void Tick(float delta) override;

	public:
	virtual void InitProperties() override;

	protected:
	void SetPigScale();
	void SetMeshMorphs();

	protected:
	Scale m_xScale;
	Morph m_xMorph;
};