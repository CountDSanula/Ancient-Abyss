
#pragma once

#include "ComponentIncludes.h"

Quaternion QuaternionFromRoll(const float roll); ///< Create quaternion from roll.
float RollFromQuaternion(const Quaternion& q); ///< Get roll from quaternion.
