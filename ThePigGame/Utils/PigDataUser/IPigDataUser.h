#pragma once

#include "../../Pig/Pig.h"

// TODO: make friend in pig and delete getters in pig
class INoCachePigDataUser {	
	public:
	virtual APig* GetPig()=0;

	virtual AFarm* GetFarm() {
		return GetPig()->GetOwnerFarm();
	}

	const UPigInitData* GetInitData() {
		return GetPig()->GetInitData();
	}

	virtual USupremePropertyController* GetPropertyController() {
		return GetPig()->GetPropertyController();
	}

	virtual UPigStateMachine* GetStateMachine() {
		return GetPig()->GetPigStateMachine();
	}

	virtual UTaskDispatcher* GetTaskDispatcher() {
		return GetPig()->GetTaskDispatcher();
	}

	virtual UMovementController* GetMovementController() {
		return GetPig()->GetMovementController();
	}

	virtual APigAIController* GetAIController() {
		return GetPig()->GetPigAIController();
	}

	virtual UAnimInstanceController* GetAnimInstanceController() {
		return GetPig()->GetAnimInstanceController();
	}
};

class ICachedPigDataUser : public INoCachePigDataUser {
	public:
	virtual void Init(APig* pig) {
		m_pPig = pig;
	}

	public:
	virtual APig* GetPig() override {
		return m_pPig;
	}

	private:
	APig* m_pPig = nullptr;
};

