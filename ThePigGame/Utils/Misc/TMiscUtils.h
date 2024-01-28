#include "CoreMinimal.h"


namespace utils {

// ignores Z coordinate
FVector CutSegment2D(const FVector& segmentStart, const FVector& segmentEnd, double lengthToCut);

FVector GetActorExtent(const AActor* actor);

}