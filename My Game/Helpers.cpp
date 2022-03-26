

#include "ComponentIncludes.h"

///< Create a quaternion from roll, with yaw andd pitch zero.
/// \param roll Amount of roll.
/// \return A roll quaternion.

Quaternion QuaternionFromRoll(const float roll){
  return Quaternion::CreateFromYawPitchRoll(0, 0, roll);
} //QuaternionFromRoll

///< Get the roll from a quaternion.
/// \param q A quaternion.
/// \return The roll specified by the quaternion.

float RollFromQuaternion(const Quaternion& q){
  const float y = 2.0f*(q.w*q.z + q.x*q.y);
  const float x = 1.0f - 2.0f*(q.y*q.y + q.z*q.z);  
  return atan2f(y, x);
} //RollFromQuaternion