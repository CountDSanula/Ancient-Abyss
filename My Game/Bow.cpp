/// \file Bow.cpp
/// \brief Code for the bow type weapon

#include "Bow.h"
#include "ComponentIncludes.h"
#include "GameDefines.h"
#include "Particle.h"
#include "ParticleEngine.h"
#include "ObjectManager.h"


Bow::Bow() : CWeapon() {

	attackDelay = 1.0f;
	m_nSpriteIndex = ARROW_SPRITE;

}//Bow

Bow::Bow(eSpriteType p, float delay) : CWeapon() {

	attackDelay = delay;
	m_nSpriteIndex = p;

}//bow

Bow::Bow(eSpriteType t, const Vector2& p, float delay) : CWeapon(t, p) {

	attackDelay = delay;

}//Bow



/// Create a bullet object and a flash particle effect.
/// It is assumed that the object is round and that the bullet
/// appears at the edge of the object in the direction
/// that it is facing and continues moving in that direction.
/// \param pObj Pointer to an object.
/// \param bullet Sprite type of bullet.
void Bow::FireProjectile(CObject* pObj, eSpriteType bullet, eSoundType sound) {
	m_pAudio->play(sound);

	const Vector2 view = pObj->GetViewVector();
	Vector2 pos = pObj->GetPos() +
		0.5f * m_pRenderer->GetWidth(pObj->m_nSpriteIndex) * view;

	//create bullet object

	CObject* pBullet = new CProjectile(bullet, pos, owner); //create bullet
	m_pObjectManager->addObject(pBullet);

	const Vector2 norm(view.y, -view.x); //normal to direction
	const float m = 2.0f * m_pRandom->randf() - 1.0f;
	const Vector2 deflection = 0.01f * m * norm;

	pBullet->SetVelocity(owner->GetVelocity() + 500.0f * (view + deflection));
	pBullet->SetOrientation(owner->GetOrientation());

	//particle effect for gun fire

	CParticleDesc2D d;

	d.m_nSpriteIndex = SPARK_SPRITE;
	d.m_vPos = pos;
	d.m_vVel = pObj->GetSpeed() * view;
	d.m_fLifeSpan = 0.25f;
	d.m_fScaleInFrac = 0.4f;
	d.m_fFadeOutFrac = 0.5f;
	d.m_fMaxScale = 0.5f;
	d.m_f4Tint = XMFLOAT4(Colors::Yellow);

	m_pParticleEngine->create(d);
} //FireProjectile

void Bow::FireProjectileSpread(CObject* pObj, eSpriteType largefireball, eSoundType sound) {
	m_pAudio->play(sound);

	const Vector2 view1 = pObj->GetViewVector();
	const Vector2 view2 = pObj->GetViewVector();
	const Vector2 view3 = pObj->GetViewVector();

	Vector2 pos1 = pObj->GetPos() +
		0.8f * m_pRenderer->GetWidth(pObj->m_nSpriteIndex) * view1;
	Vector2 pos2 = pObj->GetPos() +
		0.8f * m_pRenderer->GetWidth(pObj->m_nSpriteIndex) * view2;
	Vector2 pos3 = pObj->GetPos() +
		0.8f * m_pRenderer->GetWidth(pObj->m_nSpriteIndex) * view3;

	//create bullet object

	CObject* pBullet1 = new CProjectile(largefireball, pos1, owner); //create bullet
	CObject* pBullet2 = new CProjectile(largefireball, pos2, owner); //create bullet
	CObject* pBullet3 = new CProjectile(largefireball, pos3, owner); //create bullet
	m_pObjectManager->addObject(pBullet1);
	m_pObjectManager->addObject(pBullet2);
	m_pObjectManager->addObject(pBullet3);

	const Vector2 norm1(view1.y, -view1.x); //normal to direction
	const Vector2 norm2(view2.y * 150.0f, -view2.x * 150.0f); //normal to direction modified to fly at right angle
	const Vector2 norm3(view3.y * -150.0f, -view3.x * -150.0f); //normal to direction modified to fly at right angle
	const float m = 2.0f * m_pRandom->randf() - 1.0f;
	const Vector2 deflection1 = 0.01f * m * norm1;
	const Vector2 deflection2 = 0.01f * m * norm2;
	const Vector2 deflection3 = 0.01f * m * norm3;

	pBullet1->SetVelocity(owner->GetVelocity() + 400.0f * (view1 + deflection1));
	pBullet2->SetVelocity(owner->GetVelocity() + 400.0f * (view2 + deflection2));
	pBullet3->SetVelocity(owner->GetVelocity() + 400.0f * (view3 + deflection3));

	pBullet1->SetOrientation(owner->GetOrientation());
	pBullet2->SetOrientation(owner->GetOrientation()); // float gives proper angle
	pBullet3->SetOrientation(owner->GetOrientation());

	//particle effect for gun fire

	CParticleDesc2D d;

	d.m_nSpriteIndex = SMOKE_SPRITE;
	d.m_vPos = pos1;
	d.m_vVel = pObj->GetSpeed() * view1;
	d.m_fLifeSpan = 0.5f;
	d.m_fScaleInFrac = 0.4f;
	d.m_fFadeOutFrac = 0.5f;
	d.m_fMaxScale = 1.0f;
	d.m_f4Tint = XMFLOAT4(Colors::Yellow);

	m_pParticleEngine->create(d);
} //FireProjectileSpread

/// Primary Action 
/// For a bow, fire an arrow
void Bow::PrimaryAction() {

	CPlayer* playerPtr = static_cast<CPlayer*>(owner);
	Skeleton* skeletonPtr = static_cast<Skeleton*>(owner);
	if (owner->m_nSpriteIndex==PLAYER_SPRITE && playerPtr->m_bSpreadShot && m_pStepTimer->GetTotalSeconds() > m_fAttackTimer + attackDelay)
	{
		m_pPlayer->isThrowing = true;
		FireProjectileSpread(owner, CROSS_SPRITE, ARROW_SOUND);
		m_fAttackTimer = m_pStepTimer->GetTotalSeconds();
	}
	if(owner->m_nSpriteIndex == SKELETON_SPRITE && skeletonPtr->m_bSpreadShot && m_pStepTimer->GetTotalSeconds() > m_fAttackTimer + attackDelay)
	{
		FireProjectileSpread(owner, ARROW_SPRITE, ARROW_SOUND);
		m_fAttackTimer = m_pStepTimer->GetTotalSeconds();
	}
	if (m_pStepTimer->GetTotalSeconds() > m_fAttackTimer + attackDelay) {
		if (owner->m_nSpriteIndex == PLAYER_SPRITE) {
			m_pPlayer->isThrowing = true;
			FireProjectile(owner, CROSS_SPRITE, ARROW_SOUND);
		}
		else
			FireProjectile(owner, ARROW_SPRITE, ARROW_SOUND);
		m_fAttackTimer = m_pStepTimer->GetTotalSeconds();
	}

}//PrimaryAction
