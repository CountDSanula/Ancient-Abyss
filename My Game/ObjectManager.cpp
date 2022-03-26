/// \file ObjectManager.cpp
/// \brief Code for the the object manager class CObjectManager.

#include "ObjectManager.h"
#include "ComponentIncludes.h"
#include "ParticleEngine.h"

CGame* CObjectManager::gamePtr = nullptr;

/// Create the tile manager.
CObjectManager::CObjectManager() {
	m_pTileManager = new CTileManager(32);
} //constructor


/// Destruct all of the objects in the object list.
CObjectManager::~CObjectManager() {
	delete m_pTileManager;

	for (auto const& p : m_stdObjectList) //for each object
		delete p; //delete object
} //destructor


/// Load a map from a text file into the tile manager.
/// You can specify one of three maps whose names are hard-coded.
/// \param level Current level.
void CObjectManager::LoadMap(int level) {
	switch (level) {
	case 0: m_pTileManager->LoadMapFromImageFile("Media\\Maps\\dungeontutorial.png"); break;
	case 1: m_pTileManager->LoadMapFromImageFile("Media\\Maps\\dungeonsewers.png"); break;
	case 2: m_pTileManager->LoadMapFromImageFile("Media\\Maps\\dungeonentrance.png"); break;
	case 3: m_pTileManager->LoadMapFromImageFile("Media\\Maps\\dungeonlab.png"); break;
	case 4: m_pTileManager->LoadMapFromImageFile("Media\\Maps\\dungeonbanquet.png"); break;
	case 5: m_pTileManager->LoadMapFromImageFile("Media\\Maps\\dungeonhall.png"); break;
	case 6: m_pTileManager->LoadMapFromImageFile("Media\\Maps\\dungeonboss.png"); break;
	} //switch

	vector<Vector2> goblins = m_pTileManager->GetGoblins();
	vector<Vector2> skeletons = m_pTileManager->GetSkeletons();
	vector<Vector2> demons = m_pTileManager->GetDemons();
	vector<Vector2> boss = m_pTileManager->GetBoss();
	vector<Vector2> stair = m_pTileManager->GetStair();
	vector<Vector2> lamps = m_pTileManager->GetLamps();
	Vector2 plr = m_pTileManager->GetPlayerPos();

	for (auto p : goblins)
		create(GOBLIN_SPRITE, p);

	for (auto p : skeletons)
		create(SKELETON_SPRITE, p);

	for (auto p : demons)
		create(DEMON_SPRITE, p);

	for (auto p : boss)
		create(BOSS_SPRITE, p);

	for (auto p : stair)
		create(STAIR_SPRITE, p);

	for (auto p : lamps)
		create(LAMP_SPRITE, p);

	createPlayer(plr);


	///EXPAND HERE///
	//to add other enemy types;

} //LoadMap


/// Create the player object
CPlayer* CObjectManager::createPlayer(Vector2 pos) {
	CPlayer* p;
	p = new CPlayer(PLAYER_SPRITE,pos);

	if (gamePtr->GetCurLevel() == 0) { // starts with player only having fists on both left and right click
		p->equipWeapon(new Fist(), p);
		p->equipWeapon2(new Fist(), p);
	}

	else {
		if (m_bPlayerFist) // if player doesn't have any weapons give fist for first slot
			p->equipWeapon(new Fist(), p);
		else if (m_bPlayerSword) { // if player picked up sword give sword in first slot
			CWeapon* s;
			s = new Sword(SWORD_SPRITE, p->GetPos());
			p->equipWeapon(s, p);
			m_stdObjectList.push_back(s);
		}
		if (m_bPlayerBow) // if player picked up bow give bow in second slot
			p->equipWeapon2(new Bow(), p);
		else if (m_bPlayerFireBall) // if player picked up fireball spell give fireball spell in second slot
			p->equipWeapon2(new FireBall(), p);
		else // if player doesn't have any weapons give fist in second slot
			p->equipWeapon2(new Fist(), p);
	}

	m_stdObjectList.push_back(p);
	m_pPlayer = p;
	m_pPlayer->health = m_nPlayerHealth; // keeps player health consistent between levels
	m_pPlayer->m_bSpreadShot = m_bPlayerSpreadShot;
	return p;
}


/// Create an object and put a pointer to it on the object list.
/// \param t Sprite type.
/// \param v Initial position..
/// \return Pointer to the object created.
CObject* CObjectManager::create(eSpriteType t, const Vector2& v) {
	CObject* p;

	//Extend this switch for new types of objects
	switch (t) {

	case TURRET_SPRITE:
		p = new CEnemy(t, v);
		p->equipWeapon(new Bow(), p);
		break;

	case GOBLIN_SPRITE:
		p = new Goblin(t, v);
		//p->equipWeapon(new CWeapon(), p);
		break;

	case DEMON_SPRITE:
		p = new Demon(t, v);
		p->equipWeapon(new FireBall(), p);
		break;

	case SKELETON_SPRITE:
		p = new Skeleton(t, v);
		p->equipWeapon(new Bow(), p);
		break;

	case BOSS_SPRITE:
		p = new Boss(t, v);
		p->equipWeapon(new FireBall(), p);
		break;

	case HEALTH_POTION_GROUND_SPRITE:
		p = new CItems(t, v);
		break;

	case SPEED_POTION_GROUND_SPRITE:
		p = new CItems(t, v);
		break;

	case LAMP_SPRITE:
		if (gamePtr->GetCurLevel() == 5) ///CHANGE THIS TO PRE BOSS LEVEL
			p = new Lamp(t, v, false);
		else
			p = new Lamp(t, v);
		break;

	default:
		p = new CObject(t, v);
	}//switch

	m_stdObjectList.push_back(p);
	return p;
} //create

/*
void CObjectManager::PickUpSword(Vector2 pos) {
	CWeapon* s;
	s = new Sword(SWORD_SPRITE, pos);
	m_stdObjectList.push_back(s);
}
*/

/// Adds an object to stdObjectList
/// \param p the object to be added
void CObjectManager::addObject(CObject* p) {
	m_stdObjectList.push_back(p);
}//addObject


/// Delete all of the objects managed by the object manager. 
/// This involves deleting all of the CObject instances pointed
/// to by the object list, then clearing the object list itself.
void CObjectManager::clear() {
	for (auto const& p : m_stdObjectList) //for each object
		delete p; //delete object

	m_stdObjectList.clear(); //clear the object list
} //clear


/// Draw the tiled background and all of the objects.
void CObjectManager::draw() {
	//draw tiled background

	m_pHealthbar->m_nCurrentFrame = m_nPlayerHealth; // set health bar equal to its global frame counter to keep for level transitions
	m_pTileManager->Draw(TILE_SPRITE);

	if (m_bDrawAABBs) {

		m_pTileManager->DrawBoundingBoxes(GREENLINE_SPRITE);

		for (auto const& p : m_stdObjectList)
			m_pRenderer->DrawBoundingBox(p->GetBoundingBox());

	}//if(m_bDrawAABBs)

	m_pParticleEngine->Draw();

	//draw objects
	for (auto const& p : m_stdObjectList) //for each object
		if (p->m_nSpriteIndex != ((PLAYER_SPRITE) )){

			m_pRenderer->Draw(*(CSpriteDesc2D*)p);

		}
	//m_pRenderer->Draw(*(CSpriteDesc2D*));
	m_pRenderer->Draw(*(CSpriteDesc2D*)m_pPlayer);
	m_pRenderer->Draw(*(CSpriteDesc2D*)m_pHealthoutline);
	m_pRenderer->Draw(*(CSpriteDesc2D*)m_pHealthbar);

	
	

} //draw


/// Move all of the objects and perform 
/// broad phase collision detection and response.
void CObjectManager::move() {
	for (auto const& p : m_stdObjectList) { //for each object
		const Vector2 oldpos = p->m_vPos; //its old position


		if (p->m_nSpriteIndex == HEALTHBAR_SPRITE)
			p->m_vPos = m_pPlayer->GetPos() + healthoffset;
		if (p->m_nSpriteIndex == HEALTHOUTLINE_SPRITE)
			p->m_vPos = m_pPlayer->GetPos() + healthoffset;
		if (p->m_nSpriteIndex == BOSSHEALTH_SPRITE)
			p->m_vPos = m_pPlayer->GetPos() + bosshealthoffset;

		p->move(); //move it

		//object to wall collision detection and response using
		//bounding spheres for the objects and AABBs for the walls.

		if (m_pTileManager->CollideWithWall(p->GetBoundingSphere()))
				p->CollisionResponseWall();

	} //for

	//now do object-object collision detection and response and
	//remove any dead objects from the object list.

	BroadPhase(); //broad phase collision detection and response

	CullDeadObjects(); //remove dead objects from object list
} //move


/// This is a "bring out yer dead" Monty Python type of thing.
/// Iterate through the objects and check whether their "is dead"
/// flag has been set. If so, then delete its pointer from
/// the object list and destruct the object.
void CObjectManager::CullDeadObjects() {
	for (auto i = m_stdObjectList.begin(); i != m_stdObjectList.end();) {
		if ((*i)->IsDead()) { //"He's dead, Dave." --- Holly, Red Dwarf
			delete* i; //delete object
			i = m_stdObjectList.erase(i); //remove from object list and advance to next object
		} //if

		else ++i; //advance to next object
	} //for
} //CullDeadObjects


/// Perform collision detection and response for all pairs
/// of objects in the object list, making sure that each
/// pair is processed only once.
void CObjectManager::BroadPhase() {
	for (auto i = m_stdObjectList.begin(); i != m_stdObjectList.end(); i++) {
		for (auto j = next(i); j != m_stdObjectList.end(); j++)
			NarrowPhase(*i, *j);
	} //for
} //BroadPhase


/// Perform collision detection and response for a pair of objects.
/// Call each object's collison response for getting hit by the other object
/// \param p0 Pointer to the first object.
/// \param p1 Pointer to the second object.
void CObjectManager::NarrowPhase(CObject* p0, CObject* p1) {

	if (p0->m_Sphere.Intersects(p1->m_Sphere)) { //bounding spheres intersect
		p0->CollisionResponseObject(p1);
		p1->CollisionResponseObject(p0);
	} //if

} //NarrowPhase
