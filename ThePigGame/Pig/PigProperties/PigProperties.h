#include "../../Utils/Property/Property.h"



using Age = TDeltaTimeIncrementalProperty<TMinMaxStatic, TInstantTicking, TRestrictedCurrentModification>;
using Scale = TCoefficientalProperty<TMinMaxStatic, TInstantTicking, TRestrictedCurrentModification>; // coef to age
using Bellyful = TIncrementalProperty<TMinMaxStatic, TInstantTicking, TAllowedCurrentModification>;
using MaxWeight = TCoefficientalProperty<TMinMaxStatic, TInstantTicking, TRestrictedCurrentModification>; // coef to age
using CriticalWeight = TCoefficientalProperty<TMinMaxStatic, TInstantTicking, TRestrictedCurrentModification>; // coef to age
using Weight = TIncrementalProperty<TMinMaxDynamic, TConditionalTicking, TAllowedCurrentModification>;
using Energy = TIncrementalProperty<TMinMaxStatic, TInstantTicking, TAllowedCurrentModification>;
using Morph = TDynamicCoefficientalProperty<TMinMaxStatic, TInstantTicking, TRestrictedCurrentModification>; // coef to weight