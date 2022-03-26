/// \file Player.cpp
/// \brief Code for the game object class CPlayer.

#include "Player.h"
#include "ComponentIncludes.h"
#include "GameDefines.h"
#include "Particle.h"
#include "ParticleEngine.h"
#include "ObjectManager.h"
#include "Items.h"

CGame* CPlayer::gamePtr = nullptr;

///CPlayer object constructor
/// \param t sprite for player object
/// \param p position of the player
CPlayer::CPlayer(eSpriteType t, const Vector2& p):CObject(t, p) {


	health = 10;
	healthMax = 10;

}//CPlayer


///Move function for the player
void CPlayer::move() {
		//Get time since last frame
		const float t = m_pStepTimer->GetElapsedSeconds();
		
		if (m_fPlayerSpeed > 300.0f)
		{
			m_fStatusTimer += t;
		}

		if (m_fPlayerSpeed > 300.0f && m_fStatusTimer > 20.0f)
		{
			m_fPlayerSpeed = 300.0f;
			m_fStatusTimer = 0.0f;
		}

		//record oldpos
		m_vOldPos = m_vPos;

		if (m_bAbsoluteControls) {

			Vector2 up(0, 1);
			Vector2 down(0, -1);
			Vector2 left(-1, 0);
			Vector2 right(1, 0);
			Vector2 moveDir(0, 0);

			if (m_bStrafeForward)
				moveDir += up;
			if (m_bStrafeBack)
				moveDir += down;
			if (m_bStrafeLeft)
				moveDir += left;
			if (m_bStrafeRight)
				moveDir += right;

			moveDir.Normalize();
			m_vPos += moveDir * m_fPlayerSpeed * t;

			m_bStrafeLeft = m_bStrafeRight = m_bStrafeBack = m_bStrafeForward = false;

		}
		else {
			//get view vector and other values
			const Vector2 viewvec = GetViewVector();
			Vector2 norm(viewvec.y, -viewvec.x);
			const float delta = 200.0f * t; //possibly change constant value to speed stat

			m_vPos += m_fSpeed * t * viewvec + m_fOrthoSpeed * t * norm;
			m_fRoll += m_fRotSpeed * t;

			if (m_bStrafeRight)
				m_vPos += delta * norm;

			else if (m_bStrafeLeft)
				m_vPos -= delta * norm;

			m_bStrafeLeft = m_bStrafeRight = m_bStrafeBack = m_bStrafeForward = false;
		}

		this->animate();

		equipedWeapon->move();

		CObject::move();
	
}//move


/// Respond to a collision with a wall
void CPlayer::CollisionResponseWall(){
	
	int c = m_pTileManager->CollideDirection(this->GetBoundingSphere(),this);

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


void CPlayer::RandomGrunt() {

	srand((int)time(NULL));
	float randomNumber = (float)(rand() % 5 + 1);

	if (randomNumber == 1)
		m_pAudio->play(PLAYERGRUNT1_SOUND);
	if (randomNumber == 2)
		m_pAudio->play(PLAYERGRUNT2_SOUND);
	if (randomNumber == 3)
		m_pAudio->play(PLAYERGRUNT3_SOUND);
	if (randomNumber == 4)
		m_pAudio->play(PLAYERGRUNT4_SOUND);
	if (randomNumber == 5)
		m_pAudio->play(PLAYERGRUNT5_SOUND);

}


void CPlayer::ContactDamage() {
	if (m_pStepTimer->GetTotalSeconds() > m_fHitTimer + m_fHitDelay) { // if 0.5 seconds pass, do contact damage
		m_fHitTimer = m_pStepTimer->GetTotalSeconds();
		RandomGrunt();
		health--;	
		m_pObjectManager->m_nPlayerHealth--;
		m_pHealthbar->m_nCurrentFrame = m_pObjectManager->m_nPlayerHealth;
	}


	if (health <= 0) {
		CCommon::m_bPlayerDeath = true; // player died from contact damage
		m_pObjectManager->m_nPlayerHealth = healthMax;
	}
}

/// Respond to collision with another object
void CPlayer::CollisionResponseObject(CObject* p) {
	eSpriteType t = (eSpriteType)p->m_nSpriteIndex;
	Goblin* goblin = static_cast<Goblin*>(p); // all used to check if enemy in deathfx, if so do not take contact damage
	Skeleton* skeleton = static_cast<Skeleton*>(p);
	Demon* demon = static_cast<Demon*>(p);
	Boss* boss = static_cast<Boss*>(p);

	switch (t) {
	case BOSS_SPRITE:
		if (boss->m_bClipping) {
			ContactDamage();
			CollisionResponseWall();
		}
		break;

	case GOBLIN_SPRITE:
		if (goblin->m_bClipping) {
			ContactDamage();
			//CollisionResponseWall();
			EntityCollision(p);
		}
		break;

	case SKELETON_SPRITE:
		if (skeleton->m_bClipping) {
			ContactDamage();
			CollisionResponseWall();
		}
		break;

	case DEMON_SPRITE:
		if (demon->m_bClipping) {
			ContactDamage();
			CollisionResponseWall();
		}
		break;

	case TURRET_SPRITE:
		CollisionResponseWall();
		break;

	case ARROW_SPRITE:
		if (p->getOwner() != this) {
			RandomGrunt();
			health--;
			m_pObjectManager->m_nPlayerHealth--;
			m_pHealthbar->m_nCurrentFrame = m_pObjectManager->m_nPlayerHealth;
			if (health <= 0) {
				CCommon::m_bPlayerDeath = true; // player died from arrows
				m_pObjectManager->m_nPlayerHealth = healthMax;
			}
		}
		break;

	case LARGEFIREBALL_SPRITE:
	case DEMONFIREBALL_SPRITE:
		//m_pAudio->play(CLANG_SOUND);
		health -= 3;
		m_pObjectManager->m_nPlayerHealth--;
		if (health <= 0) {
			CCommon::m_bPlayerDeath = true; // player died from fireballs
			m_pObjectManager->m_nPlayerHealth = healthMax;
		}
		break;

	case HEALTH_POTION_GROUND_SPRITE:
		if (m_pKeyboard->TriggerDown('E')) {
			m_pAudio->play(ITEMPICKUP_SOUND);
			gamePtr->InsertItem(1); // inserts health potion item into inventory shouldn't this go to player?
			p->kill();
		}
		break;

	case SPEED_POTION_GROUND_SPRITE:
		if (m_pKeyboard->TriggerDown('E')) {
			m_pAudio->play(ITEMPICKUP_SOUND);
			gamePtr->InsertItem(2);
			p->kill();
		}
		break;

	case CHEST_SPRITE:
		if (p->m_nCurrentFrame == 0 && m_pKeyboard->TriggerDown('E')) {
			p->m_nCurrentFrame = 1;

			srand((int)time(NULL));
			float randomNumber1 = (float)(rand() % 100 + 1);
			float randomNumber2 = (float)(rand() % 100 + 1);
			if (randomNumber1 <=75) // 75 percent change to drop health potion
				m_pObjectManager->create(HEALTH_POTION_GROUND_SPRITE, p->GetPos() + Vector2(-30.0f, 40.0f));
			if (randomNumber2 <=50) // 50 percent change to drop speed boost potion
				m_pObjectManager->create(SPEED_POTION_GROUND_SPRITE, p->GetPos() + Vector2(30.0f, 40.0f));
		}
		//p->m_nCurrentFrame=1;
		//CollisionResponseWall();
		//p->kill();
		break;

	case SWORDGROUND_SPRITE:
		if (m_pKeyboard->TriggerDown('E')) {
			m_pAudio->play(ITEMPICKUP_SOUND);
			gamePtr->InsertWeapon(1);
			m_pObjectManager->m_bPlayerFist = false;
			m_pObjectManager->m_bPlayerSword = true;
			Sword* s = new Sword(SWORD_SPRITE, m_vPos);
			equipWeapon(s, this);
			m_pObjectManager->addObject(s);
			p->kill();
		}
		break;

	case CROSSGROUND_SPRITE:
		if (m_pKeyboard->TriggerDown('E')) {
			m_pAudio->play(ITEMPICKUP_SOUND);
			gamePtr->InsertWeapon(2);
			m_pObjectManager->m_bPlayerBow = true;
			m_pObjectManager->m_bPlayerFireBall = false;
			equipWeapon2(new Bow(CROSS_SPRITE, 0.7f), this);
			p->kill();
		}
		break;

	case STAIR_SPRITE:
		if (m_pKeyboard->TriggerDown('E'))
			m_bNextLevel = true;
		
		break;
	}//switch(t)

}//CollisionResponseObject


/// Perform the player's primary action
/// [On Left Click]
/// Attack with equiped weapon
void CPlayer::PrimaryAction() {
	//isSwinging = true;
	equipedWeapon->PrimaryAction();
}//PrimaryAction


void CPlayer::SecondaryAction(){
	//isThrowing = true;
	equipedWeapon2->PrimaryAction();
}//SecondaryAction

/// Equip a weapon to the player
void CPlayer::equipWeapon(CWeapon* w, CObject* o) {
	equipedWeapon = w;
	equipedWeapon->setOwner(o);
}//equipWeapon

void CPlayer::equipWeapon2(CWeapon* w, CObject* o) {
	equipedWeapon2 = w;
	equipedWeapon2->setOwner(o);
}//equipWeapon


//Animate the player sprite
void CPlayer::animate() {

	distance += (this->GetPos() - m_vOldPos).Length();

	if (((this->GetPos() - m_vOldPos).Length() == 0.0f) && !isSwinging && !isThrowing)
		m_nCurrentFrame = 0;

	if (distance > 25.0f) {

		m_nCurrentFrame++;


		switch (m_nCurrentFrame) {
		case 2:
		case 6:
			m_pAudio->play(FOOTSTEP_SOUND, 0.5f);
			break;
		case 8:
			m_nCurrentFrame = 0;
			break;
		default:
			break;
		}//swtich

		distance = 0.0f;
	}//if(distance > f)

	if (isThrowing) {

		//m_fAnimTimer += m_pStepTimer->GetElapsedSeconds();
		
		if (m_nCurrentFrame <= 7)
			m_nCurrentFrame = 8;
		else if (m_nCurrentFrame < 10)
			m_nCurrentFrame++;
		else {
			m_nCurrentFrame = 0;
			isThrowing = false;
			//m_fAnimTimer = 0.0f;
		}

	}

	if (isSwinging) {

		//m_fAnimTimer += m_pStepTimer->GetElapsedSeconds();

		if (m_nCurrentFrame <= 10)
			m_nCurrentFrame = 11;
		else if (m_nCurrentFrame < 16)
			m_nCurrentFrame++;
		else{
			m_nCurrentFrame = 0;
			isSwinging = false;
			
		}

	}//if(isSwinging)


}//animate