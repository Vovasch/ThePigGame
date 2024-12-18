#pragma once

#include "CoreMinimal.h"
#include "Containers/StaticArray.h"
#include "Containers/Map.h"
#include <type_traits>
#include <unordered_map>

template<typename EnumType>
concept EnumHasSize = requires {
	requires std::is_enum_v<EnumType>;
	EnumType::Size;
};

template<typename EnumType, class... Args>
requires EnumHasSize<EnumType>
class TEventHandler {

	public:
	using THandlerFunc = TFunction<void(Args...)>;

	public:
	void Subscribe(void* owner, EnumType enumVal, THandlerFunc func) const {
		auto& handlersArray = m_vHandlers[(uint32)enumVal];
		handlersArray.emplace(owner, func);
	}

	public:
	void Unsubscribe(void* owner) const {
		if(!owner) return;

		for(auto& handlerPools : m_vHandlers) {
			handlerPools.erase(owner);
		}
	}

	protected:
	void OnEvent(EnumType enumVal, Args... args) {
		auto handlers = m_vHandlers[(uint32)enumVal]; // copy because we can unsubscribe on event
		for(auto& handler : handlers)
			handler.second(args...);
	}

	private:
	mutable TStaticArray<std::unordered_multimap<void*, THandlerFunc>, (uint32)EnumType::Size> m_vHandlers;
};