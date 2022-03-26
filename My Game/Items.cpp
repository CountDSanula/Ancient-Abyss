/// \file Items.cpp
/// \Code for the item objects CItems

#include "Items.h"
#include "ComponentIncludes.h"
#include "GameDefines.h"
#include "Particle.h"
#include "ParticleEngine.h"
#include "ObjectManager.h"
#include "Game.h"
//#include "Common.h"

CGame* CItems::gamePtr = nullptr;

CItems::CItems() : CItems((eSpriteType)0, Vector2::Zero) {} //CItems

/// CItem object constructor
/// \param t sprite for item object
/// \param p position of the item
CItems::CItems(eSpriteType t, const Vector2& p) : CObject::CObject(t, p) {

	health = 3;

}//CItems


bool CItems::UseHealthPotion(CObject* p)
{
	if (p!= nullptr)
	{
		CPlayer* x = static_cast<CPlayer*>(p);
		if (x->healthMax > x->health + 3)
		{
			x->health += 3;
			m_pObjectManager->m_nPlayerHealth += 3;

		}
		else
		{
			x->health = x->healthMax;
			m_pObjectManager->m_nPlayerHealth = x->healthMax;
		}
	}
	return true; // item removed from inventory
}//UseHealthPotion


bool CItems::UseSpeedPotion(CObject* p)
{
	const float t = m_pStepTimer->GetElapsedSeconds();
	float m_fPotionTimer = 0;
	m_fPotionTimer += m_fPotionTimer;
	if (p != nullptr)
	{
		CPlayer* x = static_cast<CPlayer*>(p);
		if (x->m_fPlayerSpeed == 300.0f)
		{
			x->m_fPlayerSpeed = 400.0f;
			m_pAudio->play(UMPH_SOUND);
		}
		if (m_fPotionTimer >= 5.0f)
		{
			x->m_fPlayerSpeed = 300.0f;
			m_pAudio->play(UMPH_SOUND);
		}
	}
	return true; // item removed from inventory
}//UseAttackPotion


bool CItems::UseAttackSpeedPotion(CObject* p)
{
	if (p != nullptr)
	{
		CPlayer* x = static_cast<CPlayer*>(p);
		if (x->healthMax > x->health + 10)
		{
			x->health += 10;
		}
		else
		{
			x->health = x->healthMax;
		}
	}
	return true; // item removed from inventory
}//UseAttackPotion

void CItems::CollisionResponseObject(CObject* p) {
	eSpriteType t = (eSpriteType)p->m_nSpriteIndex;

	switch (t) {
	case PLAYER_SPRITE:
		break;
	}//switch(t)
}//CollisionResponseObject