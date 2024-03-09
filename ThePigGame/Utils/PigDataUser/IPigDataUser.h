#pragma once

#include "../../Pig/Pig.h"


class INoCachePigDataUser {	
	public:
	virtual APig* GetPig()=0;
	virtual APigAIController* GetAIController() {
		return GetPig()->GetPigAIController();
	}

	virtual UPigStateMachine* GetStateMachine() {
		return GetPig()->GetPigStateMachine();
	}

	virtual UTaskDispatcher* GetTaskDispatcher() {
		return GetPig()->GetTaskDispatcher();
	}

	virtual AFarm* GetFarm() {
		return GetPig()->GetOwnerFarm();
	}

	virtual UMovementController* GetMovementController() {
		return GetPig()->GetMovementController();
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

