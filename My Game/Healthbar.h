#pragma once

/// \file Healthbar.h
/// \brief Interface for the healthbar object

#pragma once

#include "Renderer.h"
#include "Object.h"
#include "Player.h"

class cGame;

class CHealthbar : public CObject {

protected:


public:
	CHealthbar(); ///< Default Constructor.

	void playerHealthbar();
	void bossHealthbar();
	void move();
	

	
};//CHealthbar