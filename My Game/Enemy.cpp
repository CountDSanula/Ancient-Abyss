/// \file Enemy.cpp
/// \brief Code for the enemy object CEnemy

#include "Enemy.h"
#include "ComponentIncludes.h"
#include "GameDefines.h"
#include "Particle.h"
#include "ParticleEngine.h"
#include "ObjectManager.h"
#include "Helpers.h"

/// Default Constructor, req. for inheritance
CEnemy::CEnemy() : CEnemy((eSpriteType)0, Vector2::Zero) {} //CEnemy


/// CEnemy object constructor
/// \param t sprite for enemy object
/// \param p position of the enemy
CEnemy::CEnemy(eSpriteType t, const Vector2& p) : CObject::CObject(t,p){

	health = 3;
	m_bClipping=true;
}//CEnemy


///Move function for the enemy
void CEnemy::move() {}//move


/// Rotate to face the player, if the player is visible. Rotation
/// is smoothed out by using quaternions.
void CEnemy::RotateToFacePlayer() {
	const float r = m_pPlayer->GetBoundingSphere().Radius;
	bool bVisible = m_pTileManager->Visible(m_vPos, m_pPlayer->m_vPos, r);

	if (bVisible) { //get desired roll
		const Vector2 v = m_pPlayer->m_vPos - m_vPos;
		m_fDesiredRoll = atan2(v.y, v.x) - XM_PI / 2.0f;
	} //if

	if (m_bRolling) {
		const float fDeltaRoll = fabs(m_fRoll - m_fDesiredRoll); //amount of rolling

		if (fDeltaRoll < XM_PI / 16) { //small amount of roll
			m_fRoll = m_fDesiredRoll; //snap to it
			m_bRolling = false; //done rolling
		} //if

		else { //large amount of roll, use quaternion slerp
			Quaternion q0 = QuaternionFromRoll(m_fRoll); //rolling from
			Quaternion q1 = QuaternionFromRoll(m_fDesiredRoll); //rolling to

			Quaternion q = XMQuaternionSlerp(q0, q1, m_fSlerpControlFactor); //current roll

			m_fRoll = RollFromQuaternion(q); //get roll from current quaternion

			const float t = m_pStepTimer->GetElapsedSeconds();
			const unsigned n = unsigned(16.0f * fDeltaRoll / (XM_2PI * t)); //number of roll increments
			m_fSlerpControlFactor += 1.0f / n; //increment roll control
		} //else
	} //if

	else if (bVisible) { //not rolling but visible
		m_bRolling = true; //start rolling
		m_fSlerpControlFactor = 0; //from zero
	} //else if

	else if (!bVisible) { //not visible, reset to default orientation
		m_fDesiredRoll = 0; //rest orientation
		m_bRolling = true; //start rolling
		m_fSlerpControlFactor = 0; //from zero
	} //else
} //RotateToFacePlayer


void CEnemy::DeathFX() {
	CParticleDesc2D d;

	d.m_nSpriteIndex = SPARK_SPRITE;
	d.m_vPos = m_vPos;  d.m_fLifeSpan = 10.0f;
	d.m_fScaleInFrac = 0.0f;
	d.m_fFadeOutFrac = 0.5f;
	d.m_fMaxScale = 1.0f;
	d.m_f4Tint = XMFLOAT4(Colors::Red);

	m_pParticleEngine->create(d);
}//DeathFX


/// Respond to a collision with a wall
void CEnemy::CollisionResponseWall() {
	m_vPos = m_vOldPos;
} //CollisionResponseWall


/// Respond to a collisoin with another object
void CEnemy::CollisionResponseObject(CObject* p) {
	eSpriteType t = (eSpriteType)p->m_nSpriteIndex;

	switch (t) {
	case CROSS_SPRITE:
		health--;
		if (health <= 0)
			kill();
		break;
	}//switch(t)


}//CollisionResponseObject


/// Deal damage to this enemy
/// \param d the amount of damage dealt.
void CEnemy::Damage(int d) {

	health -= d;

}//Damage


/// Perform promary action
/// In this case, fire a gun
void CEnemy::PrimaryAction() {

	//const Vector2 v = m_pPlayer->m_vPos - m_vPos;
	//m_fRoll = atan2(v.y, v.x) - XM_PI / 2.0f;
	equipedWeapon->PrimaryAction();

}//PrimaryAction


/// Equip a weapon to the enemy
void CEnemy::equipWeapon(CWeapon* w, CObject* o) {

	equipedWeapon = w;
	equipedWeapon->setOwner(o);

}//equipWeapon
