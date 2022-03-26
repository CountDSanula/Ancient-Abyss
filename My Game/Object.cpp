/// \file Object.cpp
/// \brief Code for the game object class CObject.

#include "Object.h"
#include "ComponentIncludes.h"
#include "GameDefines.h"
#include "Particle.h"
#include "ParticleEngine.h"
#include "TileManager.h"

/// Default Constructor, req. for inheritance
CObject::CObject() : CObject((eSpriteType)0, Vector2::Zero) {} //CObject


/// Create and initialize an object given its sprite type
/// and initial position.
/// \param t Type of sprite.
/// \param p Initial position of object.
CObject::CObject(eSpriteType t, const Vector2& p) {
	m_nSpriteIndex = t;
	m_vPos = m_vOldPos = p;

	m_pRenderer->GetSize(t, m_vRadius.x, m_vRadius.y);

	///MANUALLY SET BOUNDING BOXES//
	if (t == ARROW_SPRITE || t == CROSS_SPRITE) { //manually adjust size of bounding box for oblong sprite
		m_vRadius.x = 12;
		m_vRadius.y = 12;
	}
	if (t == PLAYER_SPRITE) {
		m_vRadius.x = 32;
		m_vRadius.y = 32;
	}
	if (t == LARGEFIREBALL_SPRITE) {
		m_vRadius.x = 64;
		m_vRadius.y = 64;
	}
	if (t == DEMONFIREBALL_SPRITE) {
		m_vRadius.x = 32;
		m_vRadius.y = 32;
	}
	if (t == SKELETON_SPRITE) {
		m_vRadius.x = 64;
		m_vRadius.y = 64;
	}

	m_vRadius *= 0.5f;

	const Vector2 topleft(-m_vRadius.x, m_vRadius.y);
	const Vector2 bottomrt(m_vRadius.x, -m_vRadius.y);
	BoundingBox::CreateFromPoints(m_Aabb, topleft + m_vPos, bottomrt + m_vPos);

	m_Sphere.Radius = max(m_vRadius.x, m_vRadius.y);
	m_Sphere.Center = (Vector3)m_vPos;
} //constructor


/// Perform a death particle effect to mark the death of an object.
void CObject::DeathFX() {
	CParticleDesc2D d;
	d.m_nSpriteIndex = SMOKE_SPRITE;
	d.m_vPos = m_vPos;

	d.m_fLifeSpan = 0.5f;
	d.m_fMaxScale = 0.5f;
	d.m_fScaleInFrac = 0.2f;
	d.m_fFadeOutFrac = 0.8f;
	d.m_fScaleOutFrac = d.m_fFadeOutFrac;

	m_pParticleEngine->create(d);
} //DeathFX


/// Kill an object by marking its "is dead" flag. The object
/// will get deleted later at the appropriate time.
void CObject::kill() {
	m_bDead = true;
	DeathFX();
} //kill


/// Reader function for the "is dead" flag.
/// \return true if marked as being dead, that is, ready for disposal.
bool CObject::IsDead() {
	return m_bDead;
} //IsDead


/// Set the object's speed, assuming that the object
/// moves according to its speed and view vector.
/// \param speed Speed.
void CObject::SetSpeed(float speed) {
	m_fSpeed = speed;
} //SetVelocity


/// Set the objects speed orthogonal to the view vector
void CObject::SetOrthoSpeed(float speed) {
	m_fOrthoSpeed = speed;
}//SetOrthoSpeed


/// Set the object's rotational speed in revolutions per second.
/// \param speed Rotational speed in RPS.
void CObject::SetRotSpeed(float speed) {
	m_fRotSpeed = speed;
} //Rotate


/// Set the strafe left flag.
void CObject::StrafeLeft() {
	m_bStrafeLeft = true;
} //StrafeLeft


/// Set the strafe right flag.
void CObject::StrafeRight() {
	m_bStrafeRight = true;
} //StrafeRight


// Set the strafe forward flag.
void CObject::StrafeForward() {
	m_bStrafeForward = true;
} //StrafeForward


/// Set the strafe back flag.
void CObject::StrafeBack() {
	m_bStrafeBack = true;
} //StrafeBack


/// Reader function for the AABB.
/// \return The AABB.
const BoundingBox& CObject::GetBoundingBox() {
	return m_Aabb;
} //GetBoundingBox


/// Reader function for the bounding sphere.
/// \return The bounding sphere.
const BoundingSphere& CObject::GetBoundingSphere() {
	return m_Sphere;
} //GetBoundingSphere


/// Reader function for position.
/// \return Position.
const Vector2& CObject::GetPos() {
	return m_vPos;
} //GetPos


/// Reader function for speed.
/// \return Speed.
float CObject::GetSpeed() {
	return m_fSpeed;
} //


/// Get the view vector.
/// \return The view vector.
Vector2 CObject::GetViewVector() {
	return Vector2(-sin(m_fRoll), cos(m_fRoll));
} //GetViewVector


/// Reader function for the orientation. A 2D object's
/// orientation is its roll amount in 3D space.
/// \return The view vector.
float CObject::GetOrientation() {
	return m_fRoll;
} //GetOrientation


/// Writer function for the orientation. A 2D object's
/// orientation is its roll amount in 3D space.
/// \param angle The new orientation.
void CObject::SetOrientation(float angle) {
	m_fRoll = angle;
} //GetOrientation


/// Reader function for velocity.
/// \return Velocity.
const Vector2& CObject::GetVelocity() {
	return m_vVelocity;
} //GetVel


/// Writer function for velocity.
/// \param v Velocity.
void CObject::SetVelocity(const Vector2& v) {
	m_vVelocity = v;
} //SetVel


/// Move and update all bounding shapes.
void CObject::move() {
	//update bounding shapes
	const Vector2 topleft(-m_vRadius.x, m_vRadius.y);
	const Vector2 bottomrt(m_vRadius.x, -m_vRadius.y);
	BoundingBox::CreateFromPoints(m_Aabb, topleft + m_vPos, bottomrt + m_vPos);

	m_Sphere.Center = (Vector3)m_vPos;
} //move


void CObject::EntityCollision(CObject* p) {

	Vector2 toOther = p->m_vPos - m_vPos;

	m_vPos += -toOther / 25;

	this->CollisionResponseWall();

}//EntityCollision


//////////////////////////
///FOR INHERITANCE ONLY///
//////////////////////////
void CObject::CollisionResponseWall() {} //CollisionResponse
void CObject::CollisionResponseObject(CObject* p) {}//CollisionResponseObject

void CObject::Damage(int d) {}//Damage

void CObject::PrimaryAction() {}//PrimaryAction
void CObject::SecondaryAction() {}//SecondaryAction

void CObject::equipWeapon(CWeapon* w, CObject* o) {}//equipWeapon

void CObject::animate() {}//animate

CObject* CObject::getOwner() { return this; }//getOwner
