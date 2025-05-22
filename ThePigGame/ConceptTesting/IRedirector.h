#pragma once

class IRedirectReceiver;



class IRedirector {
	public:
	virtual ~IRedirector() = default;
};

class IRedirectorToCore : public IRedirector {
	public:
	IRedirectorToCore()=default; // todo remove temp
	IRedirectorToCore(std::unique_ptr<IRedirectReceiver>&& receiver);

	protected:
	IRedirectReceiver* GetRedirectReceiverBase();

	private:
	virtual IRedirectReceiver* GetRedirectReceiver() = 0;

	std::unique_ptr<IRedirectReceiver> m_pReceiver = nullptr; // todo make unreals object not get deleted
};

class IRedirectorToUE : public IRedirector {
	public:
	IRedirectorToUE(IRedirectReceiver* receiver);

	protected:
	IRedirectReceiver* GetRedirectReceiverBase();

	private:
	virtual IRedirectReceiver* GetRedirectReceiver() = 0;

	IRedirectReceiver* m_pReceiver = nullptr;

};