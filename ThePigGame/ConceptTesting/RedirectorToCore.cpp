//#include "RedirectorToCore.h"
//
//#include "CoreActor.h"
//
//RedirectorToCore::RedirectorToCore(CoreActor* coreActor) : IRedirector(coreActor) {
//
//}
//
//void RedirectorToCore::Tick(float delta) {
//	GetRedirectReceiver()->Tick(delta);
//}
//
//CoreActor* RedirectorToCore::GetRedirectReceiver() {
//	return dynamic_cast<CoreActor*>(GetRedirectReceiverBase());
//}
