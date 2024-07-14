#include "TMiscUtils.h"
#include <cmath>

namespace utils {

FVector CutSegment2D(const FVector& segmentStart, const FVector& segmentEnd, double lengthToCut) {

	auto XDiff = std::abs(segmentStart.X - segmentEnd.X);
	auto YDiff = std::abs(segmentStart.Y - segmentEnd.Y);

	auto segmentLength = std::sqrt(XDiff * XDiff + YDiff * YDiff);
	auto segmentLengthAfterCutX = segmentStart.X < segmentEnd.X ? segmentLength - lengthToCut : lengthToCut;
	auto segmentLengthAfterCutY = segmentStart.Y < segmentEnd.Y ? segmentLength - lengthToCut : lengthToCut;

	auto percentageX = 100 * segmentLengthAfterCutX / segmentLength;
	auto percentageY = 100 * segmentLengthAfterCutY / segmentLength;


	auto XRes = XDiff * percentageX / 100 + std::min(segmentStart.X, segmentEnd.X);
	auto YRes = YDiff * percentageY / 100 + std::min(segmentStart.Y, segmentEnd.Y);

	return FVector(XRes, YRes, segmentStart.Z);
}

FVector GetActorExtent(const AActor* actor) {
	auto origin = FVector::ZeroVector;
	auto extent = FVector::ZeroVector;

	actor->GetActorBounds(true, origin, extent);

	return extent;

}

}