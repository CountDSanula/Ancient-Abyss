/// \file Goblin.cpp
/// \breif Code for the goblin type enemy object

#include "Goblin.h"
#include "ComponentIncludes.h"
#include "GameDefines.h"
#include "Particle.h"
#include "ParticleEngine.h"
#include "ObjectManager.h"


/// Constructor For the goblin object
/// \param t sprite for the goblin to use, should always be GOBLIN_SPRITE
/// \param p the location to spawn the goblin
Goblin::Goblin(eSpriteType t, const Vector2& p) : CEnemy::CEnemy(t, p) {

	health = 1;
	m_bClipping = true; // becomes false when killed to prevent player contact damage during deathfx

}//Goblin


void Goblin::move() {

	const float t = m_pStepTimer->GetElapsedSeconds();

	m_vOldPos = m_vPos;

	const Vector2 viewvec = GetViewVector();
	m_vPos += (m_fSpeed * t * viewvec);
	//m_fRoll += m_fRotSpeed * t;


	const float r = m_pPlayer->GetBoundingSphere().Radius;
	bool bVisible = m_pTileManager->Visible(this->m_vPos, m_pPlayer->m_vPos, r);

	m_fTargetX = m_pPlayer->m_vPos.x - m_vPos.x; // Difference of x position from player and goblin
	m_fTargetY = m_pPlayer->m_vPos.y - m_vPos.y; // Difference of y position from player and goblin

	m_fDistance = sqrtf(m_fTargetX * m_fTargetX + m_fTargetY * m_fTargetY); // Calculates distance between player and goblin

	if (bVisible && m_fDistance < 450.0f && health > 0) { // if goblin sees player and is alive, move towards player and start animation

		Vector2 playerP = m_pPlayer->GetPos();
		Vector2 goblinP = m_vPos;
		float angle = (float)(atan2(playerP.y - goblinP.y, playerP.x - goblinP.x) - M_PI / 2.0f);
		SetOrientation(angle);
		SetSpeed(250.0f);
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

	

void Goblin::DeathFX() {

	t = m_pStepTimer->GetElapsedSeconds();
	total = total + t;

	if (log == 0) {
		m_nCurrentFrame = 8;
		m_pAudio->play(GOBLINDEATH_SOUND);
		log = 1;
	}

	if (total > 0.02f) {
		m_nCurrentFrame++;
		total = 0;
	}
 
	if (m_nCurrentFrame == 38) {
		srand((int)time(NULL));
		float randomNumber =(float)(rand() % 100 + 1);
		if (randomNumber <= 10) // 10 percent change to drop health potion
			m_pObjectManager->create(HEALTH_POTION_GROUND_SPRITE, Vector2(GetPos()));
		m_bDead = true;
	}


}//DeathFX



void Goblin::CollisionResponseWall() {

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


void Goblin::CollisionResponseObject(CObject* p) {
	eSpriteType t = (eSpriteType)p->m_nSpriteIndex;

	switch (t) {
	case PUNCH_SPRITE:
		health--;
		break;
	case CROSS_SPRITE:
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


void Goblin::PrimaryAction() {} //PrimaryAction


//void Goblin::equipWeapon(CWeapon* w, CObject* o) {} //equipWeapon


void Goblin::animate() {

	distance += (this->GetPos() - m_vOldPos).Length();

	if (distance > 25.0f) {

		m_nCurrentFrame++;

		if (m_nCurrentFrame == 8) {
			m_nCurrentFrame = 0;
		}

		distance = 0.0f;
	}

}//animate