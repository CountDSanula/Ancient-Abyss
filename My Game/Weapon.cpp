/// \file Weapon.cpp
/// \brief Code for the weapon object class CWeapon.

#include "Weapon.h"
#include "ComponentIncludes.h"
#include "GameDefines.h"
#include "Particle.h"
#include "ParticleEngine.h"
#include "ObjectManager.h"


CWeapon::CWeapon() {

	attackDelay = 0.4f; //adjust attack delays here

}//CWeapon


///CWeapon object constructor
/// \param t sprite for weapon object
/// \param p position of the weapon
CWeapon::CWeapon(eSpriteType t, const Vector2& p) : CObject(t,p) {

	attackDelay = 0.4f; //adjust attack delays here

}//CWeapon


/// Returns the owner of this weapon
CObject* CWeapon::getOwner() {
	return owner;
}//getOwner


/// Changes the owner of this weapon
/// \param o new owner of this weapon
void CWeapon::setOwner(CObject* o) {
	owner = o;
}//setOwner


/// Move the weapon, only should just follow owner
void CWeapon::move() {

	m_vPos = owner->m_vPos;
	m_fRoll = owner->m_fRoll;

	CObject::move();

}//move


void CWeapon::CollisionResponseWall() {} //CollisionResponse
void CWeapon::CollisionResponseObject(CObject* p) {}//CollisionResponseObject

void CWeapon::PrimaryAction() {}//PrimaryAction
void CWeapon::SecondaryAction() {}//SecondaryAction