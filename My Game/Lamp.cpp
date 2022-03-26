/// \file Lamp.cpp
/// \brief Code for the lamp object.

#include "Lamp.h"
#include "Player.h"
#include "ComponentIncludes.h"
#include "GameDefines.h"
#include "Particle.h"
#include "ParticleEngine.h"

Lamp::Lamp(eSpriteType t, const Vector2& p, bool l) : CObject(t,p) {
	m_vPos.y -= 16;
	m_vPos.x += 16;
	isLit = l;
}//Lamp

void Lamp::move() {
	if (!isLit) {
		if (m_pPlayer->GetPos().y >= m_vPos.y) {
			isLit = true;
			//Maybe play sound here?
			m_pAudio->play(FIREBALL_SOUND);
		}
	}

	animate();

}//move

void Lamp::animate() {

	if (isLit) {
		m_nCurrentFrame++;
		if (m_nCurrentFrame > 4)
		m_nCurrentFrame = 1;
	}
	else
		m_nCurrentFrame = 0;

}