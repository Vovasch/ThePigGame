#pragma once

#include "../../Utils/Property/Property.h"

using Age = TDeltaTimeIncrementalProperty<TMinMaxStatic, TInstantTicking, TRestrictedCurrentModification>;
using Scale = TCoefficientalProperty<TMinMaxStatic, TInstantTicking, TRestrictedCurrentModification>; // coef to age
// todo rename into ConsumeProperty
using Consume = TPropertyBase<TMinMaxStatic, TNoTickType, TAllowedCurrentModification>;
using MaxWeight = TCoefficientalProperty<TMinMaxStatic, TInstantTicking, TRestrictedCurrentModification>; // coef to age
using CriticalWeight = TCoefficientalProperty<TMinMaxStatic, TInstantTicking, TRestrictedCurrentModification>; // coef to age
using Weight = TIncrementalProperty<TMinMaxDynamic, TInstantTicking, TAllowedCurrentModification>;
using Energy = TIncrementalProperty<TMinMaxStatic, TInstantTicking, TAllowedCurrentModification>;
using Morph = TDynamicCoefficientalProperty<TMinMaxStatic, TInstantTicking, TRestrictedCurrentModification>; // coef to weight