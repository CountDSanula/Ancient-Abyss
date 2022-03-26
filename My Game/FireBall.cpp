/// \file Bow.cpp
/// \brief Code for the bow type weapon

#include "FireBall.h"
#include "ComponentIncludes.h"
#include "GameDefines.h"
#include "Particle.h"
#include "ParticleEngine.h"
#include "ObjectManager.h"


FireBall::FireBall() : CWeapon() {

	attackDelay = 3.0f;

}

FireBall::FireBall(eSpriteType t, const Vector2& p) : CWeapon(t, p) {

	attackDelay = 3.0f;

}//Bow


/// Create a large fireball object and a smoke particle effect.
/// It is assumed that the object is round and that the fireball
/// appears at the edge of the object in the direction
/// that it is facing and continues moving in that direction.
/// \param pObj Pointer to an object.
/// \param largefireball Sprite type of largefireball.
void FireBall::FireProjectile(CObject* pObj, eSpriteType largefireball, eSoundType sound) {
	m_pAudio->play(sound);

	const Vector2 view = pObj->GetViewVector();
	Vector2 pos = pObj->GetPos() +
		0.5f * m_pRenderer->GetWidth(pObj->m_nSpriteIndex) * view;

	//create bullet object

	CObject* pBullet = new CProjectile(largefireball, pos, owner); //create bullet
	m_pObjectManager->addObject(pBullet);

	const Vector2 norm(view.y, -view.x); //normal to direction
	const float m = 2.0f * m_pRandom->randf() - 1.0f;
	const Vector2 deflection = 0.01f * m * norm;

	pBullet->SetVelocity(owner->GetVelocity() + 525.0f * (view + deflection));
	pBullet->SetOrientation(owner->GetOrientation());

	//particle effect for gun fire

	CParticleDesc2D d;

	d.m_nSpriteIndex = SMOKE_SPRITE;
	d.m_vPos = pos;
	d.m_vVel = pObj->GetSpeed() * view;
	d.m_fLifeSpan = 1.5f;
	d.m_fScaleInFrac = 0.4f;
	d.m_fFadeOutFrac = 0.5f;
	d.m_fMaxScale = 1.0f;
	d.m_f4Tint = XMFLOAT4(Colors::Yellow);

	m_pParticleEngine->create(d);
} //FireProjectile


void FireBall::FireProjectileSpread(CObject* pObj, eSpriteType largefireball, eSoundType sound) {
	m_pAudio->play(sound);

	const Vector2 view = pObj->GetViewVector();
	Vector2 pos = pObj->GetPos() +
		0.5f * m_pRenderer->GetWidth(pObj->m_nSpriteIndex) * view;

	//create bullet object

	CObject* pBullet1 = new CProjectile(largefireball, pos, owner); //create bullet
	CObject* pBullet2 = new CProjectile(largefireball, pos, owner); //create bullet
	CObject* pBullet3 = new CProjectile(largefireball, pos, owner); //create bullet
	m_pObjectManager->addObject(pBullet1);
	m_pObjectManager->addObject(pBullet2);
	m_pObjectManager->addObject(pBullet3);

	const Vector2 norm1(view.y, -view.x); //normal to direction
	const Vector2 norm2(view.y * 150.0f, -view.x * 150.0f); //normal to direction modified to fly at right angle
	const Vector2 norm3(view.y * -150.0f, -view.x * -150.0f); //normal to direction modified to fly at right angle
	const float m = 2.0f * m_pRandom->randf() - 1.0f;
	const Vector2 deflection1 = 0.01f * m * norm1;
	const Vector2 deflection2 = 0.01f * m * norm2;
	const Vector2 deflection3 = 0.01f * m * norm3;

	pBullet1->SetVelocity(owner->GetVelocity() + 400.0f * (view + deflection1));
	pBullet2->SetVelocity(owner->GetVelocity() + 400.0f * (view + deflection2));
	pBullet3->SetVelocity(owner->GetVelocity() + 400.0f * (view + deflection3));

	pBullet1->SetOrientation(owner->GetOrientation());
	pBullet2->SetOrientation(owner->GetOrientation() - 100.0f); // float gives proper angle
	pBullet3->SetOrientation(owner->GetOrientation() + 100.0f);

	//particle effect for gun fire

	CParticleDesc2D d;

	d.m_nSpriteIndex = SMOKE_SPRITE;
	d.m_vPos = pos;
	d.m_vVel = pObj->GetSpeed() * view;
	d.m_fLifeSpan = 0.5f;
	d.m_fScaleInFrac = 0.4f;
	d.m_fFadeOutFrac = 0.5f;
	d.m_fMaxScale = 1.0f;
	d.m_f4Tint = XMFLOAT4(Colors::Yellow);

	m_pParticleEngine->create(d);
} //FireProjectile


/// Primary Action 
/// Launches large fireball
void FireBall::PrimaryAction() {

	Boss* bossPtr = static_cast<Boss*>(owner);
	if (bossPtr->m_bWounded && m_pStepTimer->GetTotalSeconds() > m_fAttackTimer + attackDelay)
	{
		FireProjectileSpread(owner, LARGEFIREBALL_SPRITE, FIREBALL_SOUND);
		m_fAttackTimer = m_pStepTimer->GetTotalSeconds();
	}
	else if (m_pStepTimer->GetTotalSeconds() > m_fAttackTimer + attackDelay) {
		FireProjectile(owner, LARGEFIREBALL_SPRITE, FIREBALL_SOUND);
		m_fAttackTimer = m_pStepTimer->GetTotalSeconds();
	}

}//PrimaryAction

/// Secondary Action 
/// Launches small fireball
void FireBall::SecondaryAction() {

	 if (m_pStepTimer->GetTotalSeconds() > m_fAttackTimer + attackDelay) {
		FireProjectile(owner, DEMONFIREBALL_SPRITE, FIREBALL_SOUND);
		m_fAttackTimer = m_pStepTimer->GetTotalSeconds();
	}

}//PrimaryAction
