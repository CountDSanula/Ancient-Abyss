/// \file Lamp.h
/// \brief Interface for the lammp object

#pragma once

#include "Object.h"

class Lamp : public CObject {

private:
	bool isLit = true; ///< Stores if the lamp is lit or not.

public:
	Lamp(eSpriteType t, const Vector2& p, bool l = true);

	void move();
	void animate();

};//Lamp

