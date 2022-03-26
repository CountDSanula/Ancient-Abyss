/// \file Skeleton.cpp
/// \brief Code for the skeleton type enemy

#include "Skeleton.h"
#include "ComponentIncludes.h"
#include "GameDefines.h"
#include "Particle.h"
#include "ParticleEngine.h"
#include "ObjectManager.h"

CGame* Skeleton::gamePtr = nullptr;

/// Constructor For the skeleton object
/// \param t sprite for the goblin to use, should always be SKELETON_SPRITE
/// \param p the location to spawn the goblin
Skeleton::Skeleton(eSpriteType t, const Vector2& p) : CEnemy::CEnemy(t, p) {

	health = 1;
	m_bClipping=true; // becomes false when killed to prevent player contact damage during deathfx
	m_bSpreadShot = false;

}//Goblin


void Skeleton::move() {

	const float t = m_pStepTimer->GetElapsedSeconds();

	m_vOldPos = m_vPos;

	const Vector2 viewvec = GetViewVector();
	m_vPos += (m_fSpeed * t * viewvec);
	//m_fRoll += m_fRotSpeed * t;


	const float r = m_pPlayer->GetBoundingSphere().Radius;
	bool bVisible = m_pTileManager->Visible(this->m_vPos, m_pPlayer->m_vPos, r);

	if (bVisible && health > 0) { // if skeleton sees player, <DO STUFF>

		Vector2 playerPos = m_pPlayer->GetPos();
		Vector2 toPlayer = playerPos - m_vPos;
		float playerDistance = toPlayer.Length();
		float angle = (atan2(toPlayer.y, toPlayer.x) - XM_PI / 2.0f);
		SetOrientation(angle); //possibly change to quarternion
		//RotateToFacePlayer();
		
		
	    if (playerDistance < 10) {//if player is very close, rush the player
			SetSpeed(300.0f);
		}
		else { //otherwise stay still and shoot the player
			PrimaryAction();
		}
		
		SetSpeed(150.0f);
		animate();

	}
	else { //if player is not visible
		SetSpeed(0.0f);
	}

	if (health <= 0) {
		m_bClipping = false;
		DeathFX();
	}

	if (gamePtr->GetCurLevel() >= 3) { // skeletons past level 3 will use spreadshot
		m_bSpreadShot = true;
	}

	//Update Bounding Boxes
	CObject::move();

}//move


void Skeleton::DeathFX() {

	deathTimer = m_pStepTimer->GetElapsedSeconds();
	total = total + deathTimer;


	if (log == 0) {
		m_nCurrentFrame = 28;
		m_pAudio->play(FALLINGBONES_SOUND);
		log = 1;
	}

	if (total > 0.03f) {
		m_nCurrentFrame++;
		total = 0;
	}

	if (m_nCurrentFrame == 57) {
		m_bDead = true;
	}
	
}//DeathFX


void Skeleton::CollisionResponseWall() {

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


void Skeleton::CollisionResponseObject(CObject* p) {
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


//Skeleton's Primary Action
//Shoot bow
void Skeleton::PrimaryAction() {

	equipedWeapon->PrimaryAction();

} //PrimaryAction


//void Goblin::equipWeapon(CWeapon* w, CObject* o) {} //equipWeapon


void Skeleton::animate() {

	distance += (this->GetPos() - m_vOldPos).Length();

	if (distance > 10.0f) {

		m_nCurrentFrame++;

		if (m_nCurrentFrame == 27) {
			m_nCurrentFrame = 3;
		}

		distance = 0.0f;
	}

}//animate
