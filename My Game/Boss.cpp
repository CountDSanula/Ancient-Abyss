/// \file Goblin.cpp
/// \breif Code for the goblin type enemy object



#include "Boss.h"
#include "ComponentIncludes.h"
#include "GameDefines.h"
#include "Particle.h"
#include "ParticleEngine.h"
#include "ObjectManager.h"

/// Constructor For the goblin object
/// \param t sprite for the goblin to use, should always be GOBLIN_SPRITE
/// \param p the location to spawn the goblin
Boss::Boss(eSpriteType t, const Vector2& p) : CEnemy::CEnemy(t, p) {

	health = 30;
	m_bClipping = true;

}//Goblin


void Boss::move() {
	if(health == 30)
		m_pBossHealthbar->m_nCurrentFrame = health;
		
	if (m_pBossHealthbar->m_nCurrentFrame <= 0)
		m_pBossHealthbar->m_nCurrentFrame = 0;
	else {

		m_pBossHealthbar->m_nCurrentFrame = health;

	}

	const float t = m_pStepTimer->GetElapsedSeconds();

	m_vOldPos = m_vPos;

	const Vector2 viewvec = GetViewVector();
	m_vPos += (m_fSpeed * t * viewvec);
	//m_fRoll += m_fRotSpeed * t;


	const float r = m_pPlayer->GetBoundingSphere().Radius;
	bool bVisible = m_pTileManager->Visible(this->m_vPos, m_pPlayer->m_vPos, r);

	if (bVisible&&m_eState!=LUNGE_STATE && health>0) { // if boss sees player, move towards player and start animation unless lunging

		Vector2 playerP = m_pPlayer->GetPos();
		Vector2 bossP = m_vPos;
		float angle = (float)(atan2(playerP.y - bossP.y, playerP.x - bossP.x) - M_PI / 2.0f);
		SetOrientation(angle);
	}

	if (health <= 25 && !m_bGoblinSpawn)
	{
		m_pAudio->play(DIALOGUE0_SOUND);
		m_pObjectManager->create(GOBLIN_SPRITE, Vector2(250.0f, 650.0f));
		//m_pObjectManager->create(GOBLIN_SPRITE, Vector2(250.0f, 650.0f));
		m_bGoblinSpawn = true;
	}

	if (health <= 15 && !m_bSkeletonSpawn)
	{
		m_pAudio->play(DIALOGUE3_SOUND);
		m_pObjectManager->create(SKELETON_SPRITE, Vector2(250.0f, 650.0f));
		//m_pObjectManager->create(SKELETON_SPRITE, Vector2(250.0f, 650.0f));
		m_bSkeletonSpawn = true;
	}

	if (health <= 10 && !m_bDemonSpawn)
	{
		m_pObjectManager->create(DEMON_SPRITE, Vector2(250.0f, 650.0f));
		//m_pObjectManager->create(DEMON_SPRITE, Vector2(250.0f, 650.0f));
		m_bDemonSpawn = true;
		m_bWounded = true;
	}

	if (!m_bWounded && health > 0)
		animate();

	else if (m_bWounded && health > 0)
	{
		animateWounded();
	}

	else if (health <= 0)
	{
		m_bClipping = false;
		DeathFX(t);
	}

	//Update Bounding Boxes
	CObject::move();

	Think();

}//move


void Boss::Think() {
	if (m_pPlayer == nullptr)
		ABORT("Boss object's player pointer is NULL.");

	if (m_pStepTimer->GetTotalSeconds() > m_fLastAiTime + m_fAiDelayTime) {
		m_fLastAiTime = m_pStepTimer->GetTotalSeconds();
		m_vPlayerPos = m_pPlayer->GetPos();
		m_fPlayerDistX = m_vPos.x - m_vPlayerPos.x;
		m_fPlayerDistY = fabs(m_vPos.y - m_vPlayerPos.y);
		m_fPlayerDist = (m_vPos - m_vPlayerPos).Length();

		//choose state-based behavior
		switch (m_eState) { //behavior depends on state
		case ATTACK_STATE: AttackAi(); break;
		case SPELL_STATE: SpellAi(); break;
		case LUNGE_STATE: LungeAi(); break;
		default: break;
		} //switch
	} //if
} //Think


/// Set the current state.
/// Change the current state, taking whatever actions are necessary on entering
/// the new state.
/// \param state New state

void Boss::SetState(eStateType state) {
	//m_pAudio->play(CAW_SOUND, Vector2(Wrap(m_vPos.x), m_vPos.y)); //caw, caw, caw
	m_eState = state; //change state

	switch (m_eState) { //change behavior settings
	case ATTACK_STATE:
		m_fAiDelayTime = 0.4f + 0.2f * m_pRandom->randf();
		m_fSpellDelayTime = 8 + 5 * m_pRandom->randf();
		break;

	case SPELL_STATE:
		m_fAiDelayTime = 0.25f + 0.4f * m_pRandom->randf();
		m_fSpellDelayTime = 3 + 2 * m_pRandom->randf();
		break;

	case LUNGE_STATE:
		m_fAiDelayTime = 0.1f + 0.1f * m_pRandom->randf();
		m_fSpellDelayTime = 1 + 1 * m_pRandom->randf();
		break;
	} //switch
} //SetState


void Boss::AttackAi() {
	if (health > 0)
	{
		SetSpeed(225.0f);
	}
	if (m_pStepTimer->GetTotalSeconds() > m_fSpellTime + m_fSpellDelayTime) {

		m_fSpellTime = m_pStepTimer->GetTotalSeconds();
		m_fSpellDelayTime = 4 + 1 * m_pRandom->randf(); // was 15 5
		SetState(LUNGE_STATE);
	} //if
} //CruisingAi

/// AI for crows in the avoiding state.
/// These crows are trying to get away from the plane.

void Boss::SpellAi() {
	SetSpeed(0.0f);
	if (!m_bDeath)
	{
		PrimaryAction();
	}
	if (m_pStepTimer->GetTotalSeconds() > m_fSpellTime + m_fSpellDelayTime) {
		m_fSpellTime = m_pStepTimer->GetTotalSeconds();

		m_fSpellDelayTime = 3 + 2 * m_pRandom->randf();

		//m_pAudio->play(UMPH_SOUND);
		SetState(ATTACK_STATE); //then back to cruising

	} //if

} //AvoidingAi

void Boss::LungeAi() {
	SetSpeed(500.0f);
	if (m_vPos == m_vOldPos)
	{
		SetState(SPELL_STATE);
	}
	if (m_pStepTimer->GetTotalSeconds() > m_fSpellTime + m_fSpellDelayTime) {
		m_fSpellTime = m_pStepTimer->GetTotalSeconds();
		//m_fSpellTime = t;
		//m_fDesiredHeight = m_fMinAltitude + m_fDeltaAltitude * m_pRandom->randf();
		//m_vVel.y = 0.0f;

		m_fSpellDelayTime = 1 + 1 * m_pRandom->randf();

		SetState(SPELL_STATE); //then back to cruising
	}
}


void Boss::DeathFX(const float time) {
	m_fFrameTimer = m_fFrameTimer + time;
	m_pAudio->play(BOSS_DEATH);
	if (m_eState == SPELL_STATE) // add lunge state maybe?
	{
		SetSpeed(0.0f);
	}

	else if (m_eState == ATTACK_STATE)
	{
		SetSpeed(25.0f);
	}

	if (!m_bDeath)
	{
		m_nCurrentFrame = 17;
		m_bDeath = true;
	}

	if (m_fFrameTimer > 0.38f)
	{
		//if (m_nCurrentFrame < 30)
		//{
		m_nCurrentFrame++;
		//}
		m_fFrameTimer = 0.0f;
	}

	if (m_nCurrentFrame == 31)
	{
		m_nCurrentFrame = 0;
		CCommon::m_bGameWin = true;
		m_bDead = true;
	}
}//DeathFX


void Boss::CollisionResponseWall() {

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

void Boss::RandomBossAudio() {

	srand((int)time(NULL));
	float randomNumber = (float)(rand() % 4 + 1);

	if (randomNumber == 1)
		m_pAudio->play(DIALOGUE0_SOUND);
	if (randomNumber == 2)
		m_pAudio->play(DIALOGUE1_SOUND);
	if (randomNumber == 3)
		m_pAudio->play(DIALOGUE2_SOUND);
	if (randomNumber == 4)
		m_pAudio->play(DIALOGUE3_SOUND);
		

}


void Boss::CollisionResponseObject(CObject* p) {
	eSpriteType t = (eSpriteType)p->m_nSpriteIndex;

	switch (t) {
	case PUNCH_SPRITE:
		health--;
	case CROSS_SPRITE:
		health--;

		if (health % 5 == 0 && health != 0)
			RandomBossAudio();

		break;

	case PLAYER_SPRITE:
	case GOBLIN_SPRITE:
	case SKELETON_SPRITE:
	case DEMON_SPRITE:
		//CollisionResponseWall();
		EntityCollision(p);
		break;

	}//switch(t)


} //CollisionResponseObject


void Boss::PrimaryAction() {

	equipedWeapon->PrimaryAction();

} //PrimaryAction



void Boss::animate() {

	distance += (this->GetPos() - m_vOldPos).Length();

	if (distance > 25.0f) {

		m_nCurrentFrame++;

		if (m_nCurrentFrame == 8) {
			m_nCurrentFrame = 0;
		}

		distance = 0.0f;
	}

}//animate

void Boss::animateWounded() {

	distance += (this->GetPos() - m_vOldPos).Length();

	if (distance > 25.0f && !m_bDeath) {

		if (!m_bFrameReset)
		{
			m_nCurrentFrame = 8;
			m_bFrameReset = true;
		}

		m_nCurrentFrame++;

		if (m_nCurrentFrame == 16) {
			m_nCurrentFrame = 8;
		}

		distance = 0.0f;
	}

	/*
	else if (m_bDeath) {
		if (!m_bFrameReset)
		{
			m_nCurrentFrame = 8;
			m_bFrameReset = true;
		}

		m_nCurrentFrame++;

		if (m_nCurrentFrame == 16) {
			m_nCurrentFrame = 8;
		}
	}
	*/
}//animate