/// \file Renderer.cpp
/// \brief Code for the renderer class CRenderer.

#include "Renderer.h"
#include "ComponentIncludes.h"
#include "Items.h"

#include "Abort.h"

CRenderer::CRenderer():
  CSpriteRenderer(Batched2D){
} //constructor

/// Load the specific images needed for this game.
/// This is where eSpriteType values from GameDefines.h get
/// tied to the names of sprite tags in gamesettings.xml. Those
/// sprite tags contain the name of the corresponding image file.
/// If the image tag or the image file are missing, then
/// the game should abort from deeper in the Engine code,
/// leaving you with a dialog box that tells you what
/// went wrong.
void CRenderer::LoadImages(){  
  BeginResourceUpload();

  //from blank game
  Load(TEXTWHEEL_SPRITE, "textwheel"); 
  Load(LOADGAME_SPRITE, "loading");

  //from top down tile based game
  Load(BACKGROUND_SPRITE, "background");
  Load(TILE_SPRITE, "tile");
  Load(CHEST_SPRITE, "chest");
  Load(STAIR_SPRITE, "stairs");

  Load(GREENLINE_SPRITE, "greenline");
  Load(REDLINE_SPRITE, "redline");
  Load(WHITELINE_SPRITE, "whiteline");

  Load(SMOKE_SPRITE, "smoke");
  Load(SPARK_SPRITE, "spark");

  Load(HEALTHOUTLINE_SPRITE, "healthoutline");
  Load(HEALTHBAR_SPRITE, "healthbar");
  Load(BOSSHEALTH_SPRITE, "bosshealthbar");


  Load(PLAYER_SPRITE, "player");
  Load(TURRET_SPRITE, "turret");
  Load(GOBLIN_SPRITE, "goblin");
  Load(SKELETON_SPRITE, "skeleton");
  Load(DEMON_SPRITE, "demon");
  Load(BOSS_SPRITE, "boss");

  Load(STARTTEXT_SPRITE, "starttext");
  Load(CREDITSTEXT_SPRITE, "creditstext");
  Load(TUTORIAL_1_SPRITE, "tutorailtext1");
  Load(TUTORIAL_2_SPRITE, "tutorailtext2");
  Load(TUTORIAL_3_SPRITE, "tutorailtext3");
  Load(TUTORIAL_4_SPRITE, "tutorailtext4");
  Load(TUTORIAL_5_SPRITE, "tutorailtext5");


  Load(SWORDGROUND_SPRITE, "swordground");
  Load(SWORDINVENTORY_SPRITE, "swordinventory");
  Load(CROSSGROUND_SPRITE, "crossground");
  Load(CROSSINVENTORY_SPRITE, "crossinventory");
  Load(LAMP_SPRITE, "lamp");

  Load(SWORD_SPRITE, "sword");
  Load(CROSS_SPRITE, "cross");

  Load(WINSCREEN_SPRITE, "winscreen");
  Load(DEATHSCREEN_SPRITE, "death");
  Load(PUNCH_SPRITE, "punch");
  Load(ARROW_SPRITE, "arrow");
  Load(BULLET2_SPRITE, "bullet2");
  Load(LARGEFIREBALL_SPRITE, "largefireball");
  Load(DEMONFIREBALL_SPRITE, "demonfireball");
  Load(HEALTH_POTION_GROUND_SPRITE, "healthpotionground");
  Load(SPEED_POTION_GROUND_SPRITE, "speedpotionground");
  Load(HEALTH_POTION_INVENTORY_SPRITE, "healthpotioninventory");
  Load(SPEED_POTION_INVENTORY_SPRITE, "speedpotioninventory");

  EndResourceUpload();
} //LoadImages

/// Draw an axially aligned bounding box using the green line sprite.
/// \param aabb An axially aligned bounding box.
void CRenderer::DrawBoundingBox(const BoundingBox& aabb) {
	const Vector3 center = aabb.Center;
	const Vector3 extent = aabb.Extents;

	const Vector2 topleft(center.x - extent.x, center.y + extent.y);
	const Vector2 topright(center.x + extent.x, center.y + extent.y);
	const Vector2 bottomleft(center.x - extent.x, center.y - extent.y);
	const Vector2 bottomright(center.x + extent.x, center.y - extent.y);

	DrawLine(GREENLINE_SPRITE, topleft, topright);
	DrawLine(GREENLINE_SPRITE, bottomleft, bottomright);
	DrawLine(GREENLINE_SPRITE, topleft, bottomleft);
	DrawLine(GREENLINE_SPRITE, topright, bottomright);
} //DrawBoundingBox
