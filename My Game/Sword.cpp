/// \file Sword.cpp
/// \brief Code for the sword type weapon

#include "Sword.h"
#include "ComponentIncludes.h"
#include "GameDefines.h"
#include "Particle.h"
#include "ParticleEngine.h"
#include "ObjectManager.h"

///Default Constructor
Sword::Sword() : CWeapon() {

	damage = 1;
	attackDelay = 0.5f;
	viewAngle = XM_PI / 4; //45 degrees on each side

}//Sword


///Constructor
Sword::Sword(eSpriteType t, const Vector2& p) : CWeapon(t,p) {

	damage = 2;
	attackDelay = 0.5f;
	viewAngle = XM_PI / 4; //45 degrees on each side

}//Sword


void Sword::move() {

	m_vPos = owner->m_vPos;
	m_fRoll = owner->m_fRoll;

	m_nCurrentFrame = owner->m_nCurrentFrame;

	CObject::move();

}


/// Respond to collision with another object
void Sword::CollisionResponseObject(CObject* p) {
	eSpriteType t = (eSpriteType)p->m_nSpriteIndex;

	switch (t) {
	case GOBLIN_SPRITE:
	case SKELETON_SPRITE:
	case  DEMON_SPRITE:
	case BOSS_SPRITE:
		if (m_pPlayer->isSwinging) {
			Vector2 toEnemy = p->m_vPos - owner->m_vPos;
			float ThetaEnemy = (float)(atan2(toEnemy.y, toEnemy.x) - XM_PI / 2);
			float ThetaView = owner->GetOrientation();
			float DeltaTheta = ThetaEnemy - ThetaView;

			if (viewAngle >= DeltaTheta || DeltaTheta >= (2 * XM_PI - viewAngle)) {
				
				if (logic) {
					m_pAudio->play(SWORDHIT_SOUND);
					p->Damage(damage);
				}
				onenemy = true;
				logic = false;
			}
		}
		else {

			logic = true;
		}
		break;
	}//swtichd
	//isSwinging = false;

}//CollisionResponseObject


///Perform swords primary action
///Swing the sword
void Sword::PrimaryAction() {

	if (m_pStepTimer->GetTotalSeconds() > m_fAttackTimer + attackDelay) {
		m_pPlayer->isSwinging = true;

		//Play sword swing sound
		if (!onenemy) {
			
			m_pAudio->play(SWORDWOOSH_SOUND);
		}
		else {

			onenemy = false;
		}
		
		m_fAttackTimer = m_pStepTimer->GetTotalSeconds();
	}

}//PrimaryAction