/// \file Demon.cpp
/// \breif Code for the goblin type enemy object

#include "Demon.h"
#include "ComponentIncludes.h"
#include "GameDefines.h"
#include "Particle.h"
#include "ParticleEngine.h"
#include "ObjectManager.h"


/// Constructor For the Demon object
/// \param t sprite for the demon to use, should always be DEMON_SPRITE
/// \param p the location to spawn the goblin
Demon::Demon(eSpriteType t, const Vector2& p) : CEnemy::CEnemy(t, p) {

	health = 3;
	m_bClipping = true; // becomes false when killed to prevent player contact damage during deathfx

}//Goblin


void Demon::move() {

	const float t = m_pStepTimer->GetElapsedSeconds();

	m_vOldPos = m_vPos;

	const Vector2 viewvec = GetViewVector();
	m_vPos += (m_fSpeed * t * viewvec);
	//m_fRoll += m_fRotSpeed * t; 


	const float r = m_pPlayer->GetBoundingSphere().Radius;
	bool bVisible = m_pTileManager->Visible(this->m_vPos, m_pPlayer->m_vPos, r);

	if (bVisible && health > 0) { // if demon sees player, move towards player and start animation
		
		Vector2 playerP = m_pPlayer->GetPos();
		Vector2 demonP = m_vPos;
		float angle = (float)(atan2(playerP.y - demonP.y, playerP.x - demonP.x) - M_PI / 2.0f);
		SetOrientation(angle);
		SetSpeed(150.0f);
		PrimaryAction();
		animate();

	}
	else {
		SetSpeed(0.0f);
	}

	if (health <= 0) {
		m_bClipping = false;
		DeathFX();
	}

	//Update Bounding Boxes
	CObject::move();

}//move


void Demon::DeathFX() {
	t = m_pStepTimer->GetElapsedSeconds();
	total = total + t;

	if (log == 0) {
		m_nCurrentFrame = 30;
		m_pAudio->play(DEMONDEATH_SOUND);
		log = 1;
	}

	if (total > 0.04f) {
		m_nCurrentFrame++;
		total = 0;
	}

	if (m_nCurrentFrame == 46)
		m_bDead = true;
	//m_pAudio->play(GOBLINDEATH_SOUND);
}//DeathFX



void Demon::CollisionResponseWall() {

	int c = m_pTileManager->CollideDirection(this->GetBoundingSphere(), this);

	switch (c) {
	case 1:
	case 2:
		m_vPos.y = m_vOldPos.y;
		break;
	case 3:
	case 4:
		m_vPos.x = m_vOldPos.x;
		break;
	case 5:
		break;
	default:
		m_vPos = m_vOldPos;
		break;
	}//switch

} //CollisionResponseWall


void Demon::CollisionResponseObject(CObject* p) {
	eSpriteType t = (eSpriteType)p->m_nSpriteIndex;

	switch (t) {
	case PUNCH_SPRITE:
		health--;
		break;

	case CROSS_SPRITE :
		health--;
		break;

	case PLAYER_SPRITE:
	case GOBLIN_SPRITE:
	case SKELETON_SPRITE:
	case DEMON_SPRITE:
	case BOSS_SPRITE:
		//CollisionResponseWall();
		EntityCollision(p);
		break;
	}//switch(t)

} //CollisionResponseObject


void Demon::PrimaryAction() {

	equipedWeapon->SecondaryAction();


} //PrimaryAction


//void Demon::equipWeapon(CWeapon* w, CObject* o) {} //equipWeapon


void Demon::animate() {

	distance += (this->GetPos() - m_vOldPos).Length();

	if (distance > 15.0f) {

		m_nCurrentFrame++;

		if (m_nCurrentFrame == 23) {
			m_nCurrentFrame = 16;
		}

		distance = 0.0f;
	}

}//animate