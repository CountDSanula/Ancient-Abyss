/// \file Fist.cpp
/// \brief Code for the bow type weapon

#include "Fist.h"
#include "ComponentIncludes.h"
#include "GameDefines.h"
#include "Particle.h"
#include "ParticleEngine.h"
#include "ObjectManager.h"


Fist::Fist() : CWeapon() {

	attackDelay = 0.5f;

}

Fist::Fist(eSpriteType t, const Vector2& p) : CWeapon(t, p) {

	attackDelay = 0.5f;

}//Bow


/// Create a bullet object and a flash particle effect.
/// It is assumed that the object is round and that the bullet
/// appears at the edge of the object in the direction
/// that it is facing and continues moving in that direction.
/// \param pObj Pointer to an object.
/// \param bullet Sprite type of bullet.
void Fist::Punch(CObject* pObj, eSpriteType bullet, eSoundType sound) {
	m_pAudio->play(sound);

	const Vector2 view = pObj->GetViewVector();
	Vector2 pos = pObj->GetPos() +
		0.6f * m_pRenderer->GetWidth(pObj->m_nSpriteIndex) * view;

	//create bullet object

	CObject* pBullet = new CProjectile(bullet, pos, owner); //create bullet
	m_pObjectManager->addObject(pBullet);

	const Vector2 norm(view.y, -view.x); //normal to direction
	const float m = 2.0f * m_pRandom->randf() - 1.0f;
	const Vector2 deflection = 0.01f * m * norm;

	pBullet->SetVelocity(owner->GetVelocity() + 5.0f * (view + deflection));
	pBullet->SetOrientation(owner->GetOrientation());


	//particle effect for gun fire

	CParticleDesc2D d;

	d.m_nSpriteIndex = SPARK_SPRITE;
	d.m_vPos = pos;
	d.m_vVel = pObj->GetSpeed() * view;
	d.m_fLifeSpan = 0.05f;
	d.m_fScaleInFrac = 0.4f;
	d.m_fFadeOutFrac = 0.0f;
	d.m_fMaxScale = 0.25f;
	d.m_f4Tint = XMFLOAT4(Colors::Red);

	m_pParticleEngine->create(d);
} //FireProjectile

/// Primary Action 
/// For a bow, fire an arrow
void Fist::PrimaryAction() {

	if (m_pStepTimer->GetTotalSeconds() > m_fAttackTimer + attackDelay) {
		Punch(owner, PUNCH_SPRITE, ARROW_SOUND);
		m_fAttackTimer = m_pStepTimer->GetTotalSeconds();
	}


}//PrimaryAction
