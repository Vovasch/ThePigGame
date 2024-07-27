#pragma once

#include <type_traits>

class IProperty {
	private:
	virtual void MakePolymorphic(){}
};

class TBoolProperty : public IProperty {

	public:
	TBoolProperty() {}
	TBoolProperty(bool value) : m_bValue(value) {}

	bool GetValue() const {
		return m_bValue;
	}

	protected:
	bool m_bValue = false;
};

class IPropertyTickProvider;

template<typename MinMaxType, typename TickingType, typename CurrentModyficationType>
class TPropertyBase : public IProperty {

	public:
	TPropertyBase()=default;

	public:
	TPropertyBase(float startingCurrent) : m_fCurrent(startingCurrent) {}

	public:
	virtual float GetCurrent() const { return m_fCurrent; }
	virtual const float& GetCurrentRef() const { return m_fCurrent; }
	virtual const float* GetCurrentPtr() const { return &m_fCurrent; }

	public:
	MinMaxType& GetMinMaxType() { return m_xMinMax; }
	TickingType& GetTickingType() { return m_xTickingType; }
	const CurrentModyficationType& GetCurrentModifycationType() { return m_xCurrentModificationType; }

	public:
	void Init(IPropertyTickProvider* tickProvider=nullptr) {

		m_xCurrentModificationType.SetAdder([this](float value) {
			this->Update(this->GetCurrent() + value);
		});

		m_xCurrentModificationType.SetSetter([this](float value) {
			this->Update(value);
		});

		if(tickProvider) InitTickingType(tickProvider);
	}

	protected:
	virtual void Update() {}
	void Update(float newCurrent) { m_fCurrent = m_xMinMax.ClampNewCurrent(newCurrent); }

	protected:
	virtual void InitTickingType(IPropertyTickProvider* tickProvider) {
		m_xTickingType.Init(tickProvider);
		m_xTickingType.Subscribe([this](float deltaTime) {
			this->Update();
		});
	}

	protected:
	MinMaxType m_xMinMax;
	TickingType m_xTickingType;
	CurrentModyficationType m_xCurrentModificationType;
	
	private:
	float m_fCurrent = 0.f;

};

template<typename MinMaxType, typename TickingType, typename CurrentModyficationType>
class TIncrementalProperty : public TPropertyBase<MinMaxType, TickingType, CurrentModyficationType> {

	public:
	TIncrementalProperty()=default;

	public:
	TIncrementalProperty(float startingCurrent) : TPropertyBase<MinMaxType, TickingType, CurrentModyficationType>(startingCurrent) {}

	public:
	void SetDelta(float delta) { m_fDelta = delta; }
	
	public:
	using TPropertyBase<MinMaxType, TickingType, CurrentModyficationType>::GetCurrent;

	protected:
	using TPropertyBase<MinMaxType, TickingType, CurrentModyficationType>::Update;
	

	protected:
	virtual void Update() override { Update(GetCurrent() + m_fDelta); }

	protected:
	float m_fDelta = 0.f;
};

class TInstantTicking;

template<typename MinMaxType, typename TickingType, typename CurrentModyficationType>
class TDeltaTimeIncrementalProperty : public TPropertyBase<MinMaxType, TickingType, CurrentModyficationType> {

	public:
	TDeltaTimeIncrementalProperty() {
		static_assert(std::is_base_of_v<TickingType, TInstantTicking>, "TDeltaTimeIncrementalProperty should have some ticking type, otherwise it has no sense");
	}
	
	public:
	TDeltaTimeIncrementalProperty(float startingCurrent) : TPropertyBase<MinMaxType, TickingType, CurrentModyficationType>(startingCurrent) {}

	protected:
	using TPropertyBase<MinMaxType, TickingType, CurrentModyficationType>::Update;

	using TPropertyBase<MinMaxType, TickingType, CurrentModyficationType>::m_xTickingType;

	protected:
	virtual void InitTickingType(IPropertyTickProvider* tickProvider) override {
		m_xTickingType.Init(tickProvider);

		m_xTickingType.Subscribe([this](float deltaTime) {
			this->Update(this->GetCurrent() + deltaTime);
		});
	}

	protected:
	virtual void Update() override {}
};

class THasMinMax;

template<typename MinMaxType, typename TickingType, typename CurrentModyficationType>
class TCoefficientalPropertyBase : public TPropertyBase<MinMaxType, TickingType, CurrentModyficationType> {
	
	public:
	TCoefficientalPropertyBase() {
		static_assert(!std::is_base_of_v<MinMaxType, THasMinMax>, "TCoefficientalProperty doesn't work without Min Max values available");
	}

	using TPropertyBase<MinMaxType, TickingType, CurrentModyficationType>::m_xMinMax;

	public:
	void RecalcCoef() {
		m_fCoeff = (m_xMinMax.GetMax() - m_xMinMax.GetMin()) / (GetMaxTo()  - GetMinTo());
	}
	
	protected:
	virtual float GetMinTo() = 0;
	virtual float GetMaxTo() = 0;

	protected:
	using TPropertyBase<MinMaxType, TickingType, CurrentModyficationType>::Update;

	protected:
	virtual void Update() override { Update(m_fCoeff * (*m_pDependentValue - GetMinTo()) + m_xMinMax.GetMin()); }

	protected:
	float m_fCoeff = 0.f;
	//shit shit shit
	const float* m_pDependentValue = nullptr;
	
};



template<typename MinMaxType, typename TickingType, typename CurrentModyficationType>
class TCoefficientalProperty : public TCoefficientalPropertyBase<MinMaxType, TickingType, CurrentModyficationType> {

	public:
	using TCoefficientalPropertyBase<MinMaxType, TickingType, CurrentModyficationType>::m_pDependentValue;
	using TCoefficientalPropertyBase<MinMaxType, TickingType, CurrentModyficationType>::RecalcCoef;
	using TCoefficientalPropertyBase<MinMaxType, TickingType, CurrentModyficationType>::Update;

	void CalcCoeff(const float* depentedntValue, float toMin, float toMax) {
		m_pDependentValue = depentedntValue;
		m_fMinTo = toMin;
		m_fMaxTo = toMax;
		RecalcCoef();
		Update();
	}

	protected:
	virtual float GetMinTo() override {
		return m_fMinTo;
	}

	protected:
	virtual float GetMaxTo() override {
		return m_fMaxTo;
	}

	protected:
	float m_fMinTo = 0.f;
	float m_fMaxTo = 0.f;

};

// тут зробити, щоб сщуфіцієнт перераховувася кожен тік
template<typename MinMaxType, typename TickingType, typename CurrentModyficationType>
class TDynamicCoefficientalProperty : public TCoefficientalProperty<MinMaxType, TickingType, CurrentModyficationType> {
	private:
	using ParentType = TCoefficientalProperty<MinMaxType, TickingType, CurrentModyficationType>;

	public:
	TDynamicCoefficientalProperty()=default;
	
	using ParentType::RecalcCoef;
	using ParentType::Update;
	using ParentType::m_pDependentValue;
	
	public:
	void CalcCoeff(const float* depentedntValue, const float* toMin, const float* toMax) {
		m_pDependentValue = depentedntValue;
		m_pMinTo = toMin;
		m_pMaxTo = toMax;
		RecalcCoef();
		Update();
	}

	protected:
	virtual void Update() {
		RecalcCoef();
		TCoefficientalProperty<MinMaxType, TickingType, CurrentModyficationType>::Update();
	}

	protected:
	virtual float GetMinTo() override {
		return *m_pMinTo;
	}

	protected:
	virtual float GetMaxTo() override {
		return *m_pMaxTo;
	}

	protected:
	const float* m_pMinTo = nullptr;
	const float* m_pMaxTo = nullptr;
	
};

class TMinMaxBase {

	public:
	TMinMaxBase() = default;
	
	public:
	virtual float ClampNewCurrent(float newCurrent) = 0;
};

class TNoMinMax : public TMinMaxBase {

	public:
	TNoMinMax()=default;

	public:
	virtual float ClampNewCurrent(float newCurrent) override;
};

class THasMinMax : public TMinMaxBase {

	public:
	virtual float GetMin()=0;
	virtual float GetMax()=0;

	virtual float ClampNewCurrent(float newCurrent) override;
};

class TMinMaxStatic : public THasMinMax {
	
	public:
	TMinMaxStatic()=default;

	public:
	virtual float GetMin() override;
	virtual float GetMax() override;

	public:
	void SetMinMax(float min, float max);

	protected:
	float m_fMin = 0.f;
	float m_fMax = 0.f;
};

class TMinMaxDynamic : public THasMinMax {
	
	public:
	TMinMaxDynamic() = default;

	public:
	void SetMinMax(const float* min, const float* max);

	public:
	virtual float GetMin() override;
	virtual float GetMax() override;

	protected:
	const float* m_fMin = nullptr;
	const float* m_fMax = nullptr;
};


class ITicking {

	public:
	ITicking() = default;

	public:
	using TTickingFunction = TFunction<void(float)>;

	public:
	virtual void Init(IPropertyTickProvider* tickProvider)=0;
	virtual void Tick(float delta)=0;

	public:
	virtual void Subscribe(TTickingFunction function)=0;
};

class TNoTickType : public ITicking {

	public:
	virtual void Init(IPropertyTickProvider* tickProvider) override;
	virtual void Tick(float delta) override;

	public:
	virtual void Subscribe(TTickingFunction function) override;
};


class TInstantTicking : public ITicking {

	public:
	virtual void Tick(float delta) override;
	virtual void Init(IPropertyTickProvider* tickProvider) override;

	public:
	virtual void Subscribe(TTickingFunction function) override;

	protected:
	IPropertyTickProvider* m_pTickProvider = nullptr;

	protected:
	TArray<TTickingFunction> m_vSubs;
};

// todo remove because may be unused 
class TEventualTicking : public TInstantTicking {
	public:
	virtual void Init(IPropertyTickProvider* tickProvider) override;

	public:
	void StartTicking();
	void StopTicking();
};

// todo remove because may be unused 
class TConditionalTicking : public TInstantTicking {
	
	using TCondition = TFunction<bool()>;

	public:
	virtual void Tick(float delta) override;

	public:
	void SetCondition(TCondition condition);

	protected:
	TCondition m_xCondition = nullptr;
};

class IPropertyTickProvider {

	public:
	IPropertyTickProvider() = default;

	public:
	void SubscribeOnTick(TInstantTicking* tickSub);
	void UnsubscribeFromTick(TInstantTicking* tickSub);

	public:
	void TickProviderTick(float delta);

	TSet<ITicking*> m_xSubs;

};

class TRestrictedCurrentModification {

	public:
	TRestrictedCurrentModification() = default;

	protected:
	using TSetter = TFunction<void(float value)>;
	using TAdder = TFunction<void(float value)>;

	public:
	virtual void SetSetter(TSetter setter) {}
	virtual void SetAdder(TAdder adder) {}

};


class TAllowedCurrentModification : public TRestrictedCurrentModification {
	
	public:
	void Set(float value) const;
	void Add(float value) const;

	public:
	virtual void SetSetter(TSetter setter) override;
	virtual void SetAdder(TAdder adder) override;

	protected:
	TSetter m_xSetter = nullptr;
	TAdder m_xAdder = nullptr;
};