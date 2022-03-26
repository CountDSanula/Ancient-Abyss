/// \file Projectile.cpp
/// \brief Code for the projectile object CProjectile

#include "Projectile.h"
#include "ComponentIncludes.h"
#include "GameDefines.h"
#include "Particle.h"
#include "ParticleEngine.h"
#include "ObjectManager.h"

/// Create and initialize a projectile
/// \param t Type of sprite.
/// \param p Initial position of object.
/// \param o Owner of the projectile.
CProjectile::CProjectile(eSpriteType t, const Vector2& p, CObject* o) : CWeapon(t,p) {
	owner = o;
}//CProjectile


/// Move the projectile and call object move to update bounding shapes
void CProjectile::move() {

	const float t = m_pStepTimer->GetElapsedSeconds();

	m_vOldPos = m_vPos;
	m_vPos += m_vVelocity * t;

	FistLifeTime();
	CObject::move();

}//move

void CProjectile::FistLifeTime() {
	if (m_pStepTimer->GetTotalSeconds() > m_fHitBoxLife&& m_nSpriteIndex == PUNCH_SPRITE) {
		m_fHitBoxLife = m_pStepTimer->GetTotalSeconds();
		kill();
	}
}

/// Respond to hitting a wall
void CProjectile::CollisionResponseWall() {
	m_pAudio->play(ARROWHIT_SOUND, 0.1f);
	kill();
} //CollisionResponse

/// Respond to hitting another object
void CProjectile::CollisionResponseObject(CObject* p) {

	switch (p->m_nSpriteIndex) {
	case ARROW_SPRITE:
	case CROSS_SPRITE:
	case HEALTHBAR_SPRITE:
	case HEALTHOUTLINE_SPRITE:
	case HEALTH_POTION_GROUND_SPRITE:
	case STAIR_SPRITE:
	case SWORD_SPRITE:
		break; //do nothing
	case LARGEFIREBALL_SPRITE:
		if (m_nSpriteIndex == LARGEFIREBALL_SPRITE);
		else
			kill();
		break;
	case GOBLIN_SPRITE:
	case SKELETON_SPRITE:
	case DEMON_SPRITE:
	case BOSS_SPRITE:
		if (m_nSpriteIndex == CROSS_SPRITE) {
			m_pAudio->play(ARROWHIT_SOUND, 0.1f);
			kill();
		}
		break;
	default:
		m_pAudio->play(ARROWHIT_SOUND, 0.1f);
		kill();
		break;
	}//switch

}//CollisionResponseObject