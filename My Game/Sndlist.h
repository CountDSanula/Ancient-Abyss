/// \file SndList.h
/// \brief Enumerated type for sounds.

#pragma once

#include "Sound.h"

/// \brief Game sound enumerated type. 
///
/// These are the sounds used in actual gameplay. 
/// The sounds must be listed here in the same order that
/// they are in the sound settings XML file.

enum eSoundType{ 
	//from blank game
  UMPH_SOUND,
  //from top down tile based game
  ARROW_SOUND, ARROWHIT_SOUND, FOOTSTEP_SOUND, GOBLINDEATH_SOUND, SKELETONDEATH_SOUND, LEVEL1_SOUND, ITEMPICKUP_SOUND,
  FALLINGBONES_SOUND, DEMONDEATH_SOUND, FIREBALL_SOUND, DIALOGUE0_SOUND, DIALOGUE1_SOUND, DIALOGUE2_SOUND, DIALOGUE3_SOUND,
  BOSS_DEATH, INTRO_SOUND, DEMONLAUGH_SOUND,
  
  // player sounds
  PLAYERGRUNT1_SOUND, PLAYERGRUNT2_SOUND, PLAYERGRUNT3_SOUND, PLAYERGRUNT4_SOUND, PLAYERGRUNT5_SOUND,

  // sword sounds

  SWORDHIT_SOUND, SWORDWOOSH_SOUND,

  WINSCREEN_SOUND,

}; //eSoundType