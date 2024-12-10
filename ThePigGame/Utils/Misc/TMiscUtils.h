#pragma once

#include "CoreMinimal.h"

namespace utils {

// ignores Z coordinate
FVector CutSegment2D(const FVector& segmentStart, const FVector& segmentEnd, double lengthToCut);

FVector GetActorExtent(const AActor* actor);

template<typename EnumType>
concept EnumHasNone = requires {
	EnumType::None;
	requires std::is_enum_v<EnumType>;
};

template<typename T> requires std::is_enum_v<T>
struct IsEnumHasNone {
	constexpr static bool value = false;
};

template<EnumHasNone EnumType>
struct IsEnumHasNone<EnumType> {
	constexpr static bool value = true;
};

template<typename EnumType, typename ElementType, uint32 NumElements>
void VerifyContainerElements(const TStaticArray<ElementType, NumElements>& container) {

	auto foundNullptr = false;
	// If EnumType has None, skip first element
	constexpr auto startingValue = IsEnumHasNone<EnumType>::value ? 1 : 0;
	for(uint32 i = startingValue; i < uint32(EnumType::Size); ++i) {
		if(!container[i]) {
			foundNullptr = true;
			UE_LOG(LogTemp, Error, TEXT("%s is a nullptr"), *UEnum::GetDisplayValueAsText(EnumType(i)).ToString());
		}
	}

	if(foundNullptr) {
		UE_LOG(LogTemp, Fatal, TEXT("One or more container elements are nullptr"));
	}
}

}