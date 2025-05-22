#include "IRedirector.h"
#include "IRedirectReceiver.h"

IRedirectorToCore::IRedirectorToCore(std::unique_ptr<IRedirectReceiver>&& receiver) : m_pReceiver(std::move(receiver)){
}

IRedirectReceiver* IRedirectorToCore::GetRedirectReceiverBase() {
	return m_pReceiver.get();
}

IRedirectorToUE::IRedirectorToUE(IRedirectReceiver* receiver) : m_pReceiver(receiver) {
}

IRedirectReceiver* IRedirectorToUE::GetRedirectReceiverBase() {
	return m_pReceiver;
}
