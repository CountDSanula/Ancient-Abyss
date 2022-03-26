/// \file TileManager.h
/// \brief Interface for the tile manager CTileManager.

#pragma once

#include <vector>

using namespace std;

#include "Common.h"
#include "Settings.h"
#include "Sprite.h"
#include "GameDefines.h"
#include "Object.h"
#include "Player.h"

/// \brief The tile manager.
///
/// The tile manager is responsible for the
/// tile-based background.

class CTileManager: public CCommon, public CSettings{
  private:
    int m_nWidth = 0; ///< Number of tiles wide.
    int m_nHeight = 0; ///< Number of tiles high.

    float m_fTileSize; ///< Tile width and height.
    Vector2 m_vTileRadius; ///< Tile radius.

    char** m_chMap = nullptr; ///< The level map.

    vector<BoundingBox> m_vecWalls; ///< AABBs for the walls.

	vector<Vector2> m_vecTurrets; ///<Positions of turrets
	vector<Vector2> m_vecGoblins; ///<Positions of goblins
	vector<Vector2> m_vecSkeletons; ///<Positions of skeletons
	vector<Vector2> m_vecDemons; ///<Positions of demons
	vector<Vector2> m_vecBoss; ///<Positions of boss
	vector<Vector2> m_vecStairs; ///<Positions of stair
	vector<Vector2> m_vecLamps; ///<Positions of lamps
	Vector2 m_vPlayerPos; ///<Position of player

	bool isWall(char c); ///< Function that returns if a tile is a wall or not.

    void MakeBoundingBoxes(); ///< Make bounding boxes for walls.

  public:
    CTileManager(size_t tilesize); ///< Constructor.
    ~CTileManager(); ///< Destructor.

    void LoadMap(char* filename); ///< Load a map.
	void LoadMapFromImageFile(char* filename); ///< Load map.
    void Draw(eSpriteType t); ///< Draw the map with a given tile.
    void DrawBoundingBoxes(eSpriteType t); ///< Draw the bounding boxes.

	vector<Vector2> GetTurrets() { return m_vecTurrets; }; ///< Get turrets.
	vector<Vector2> GetGoblins() { return m_vecGoblins; }; ///< Get goblins.
	vector<Vector2> GetSkeletons() { return m_vecSkeletons; }; ///< Get skeletons.
	vector<Vector2> GetDemons() { return m_vecDemons; }; ///< Get demons.
	vector<Vector2> GetBoss() { return m_vecBoss; }; ///< Get boss.
	vector<Vector2> GetStair() { return m_vecStairs; }; ///< Get stair.
	vector<Vector2> GetLamps() { return m_vecLamps; }; ///< Get lamps.
	Vector2 GetPlayerPos() { return m_vPlayerPos; }; ///< Get player.

    bool Visible(const Vector2& v0, const Vector2& v1, float radius); ///< Check visibility.

    template<class t> bool CollideWithWall(const t& s); ///< Check object collision with a wall.
	template<class t> int CollideDirection(const t& o, CObject* obj); ///< Determines the direction of the colision between the wall and the object.
}; //CTileManager