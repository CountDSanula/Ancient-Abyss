/// \file Goblin.h
/// \brief An interface for the goblin type enemy object



#pragma once

#include "Enemy.h"

class Boss : public CEnemy {
private:
	enum eStateType {
		ATTACK_STATE, SPELL_STATE, LUNGE_STATE
	};

	eStateType m_eState = ATTACK_STATE;

	Vector2 m_vPlayerPos = Vector2::Zero; ///< player current position.
	float m_fPlayerDist = FLT_MAX; ///< Euclidean distance to plane.
	float m_fPlayerDistX = FLT_MAX; ///< Horizontal distance to plane.
	float m_fPlayerDistY = FLT_MAX; ///< Vertical distance to plane.
	bool alertSound = false;
	float m_fSpellTime = 0.0f;
	float m_fSpellDelayTime = 0.0f;
	float m_fFrameTimer = 0.0f;

	float m_fLastAiTime = 0.0f; ///< Last time AI was used
	float m_fAiDelayTime = 0.0f; ///< Time until AI next use.

	bool m_bFrameReset = false;
	bool m_bDeath = false;

	void Think();

	void SetState(eStateType state);

	void AttackAi();

	void SpellAi();

	void LungeAi();

	void DeathFX(const float time); ///< Death special effects.

	void CollisionResponseWall(); ///< Collision response for hitting a wall.
	void CollisionResponseObject(CObject* p); ///< Collision response for hitting another object.
	void RandomBossAudio(); ///< Outputs audio for random dialogue of boss
	void PrimaryAction();

	//void equipWeapon(CWeapon* w, CObject* o); ///< Equip a weapon to this enemy

	void animate();

	void animateWounded();

public:
	Boss(eSpriteType t, const Vector2& p); ///< Constructor for boss

	void move(); ///< Move the enemy

	bool m_bGameWin = false; // win state for game is killing the boss
	bool m_bClipping;
	bool m_bWounded = false;
	bool m_bGoblinSpawn = false;
	bool m_bSkeletonSpawn = false;
	bool m_bDemonSpawn = false;
};