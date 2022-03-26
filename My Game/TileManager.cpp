/// \file TileManager.cpp
/// \brief Code for the tile manager CTileManager.



#include "TileManager.h"
#include "Renderer.h"
#include "Abort.h"

#define STBI_ASSERT(x)
#define STB_IMAGE_IMPLEMENTATION
#include "stb_image.h"

/// \param tilesize Width and height of square tile in pixels.
CTileManager::CTileManager(size_t tilesize):
  m_fTileSize((float)tilesize),
  m_vTileRadius(Vector2(m_fTileSize, m_fTileSize)/2){
} //constructor


/// Delete the memory used for storing the map.
CTileManager::~CTileManager(){
  for(int i=0; i<m_nHeight; i++)
    delete [] m_chMap[i];

  delete [] m_chMap;
} //destructor


/// Returns if the given tile is a wall or not
/// \param c the character representing the tile being checked
bool CTileManager::isWall(char c) {

	if (c == 'F' || c=='O') {
		return false;
	}
	else {
		return true;
	}

}


/// Make the AABBs for the walls. Care is taken to use the
/// longest horizontal and vertical AABBs possible so that
/// there aren't so many of them.
void CTileManager::MakeBoundingBoxes(){
  bool** used = new bool*[m_nHeight]; //which walls have been used in an AABB already

  for(int i=0; i<m_nHeight; i++){
    used[i] = new bool[m_nWidth];
  
    for(int j=0; j<m_nWidth; j++)
      used[i][j] = false;
  } //for

  m_vecWalls.clear(); //no walls yet

  BoundingBox aabb; //current bounding box
  const Vector3 vTileExtents = 0.5f*Vector3(m_fTileSize, m_fTileSize, m_fTileSize);

  //horizontal walls with more than one tile

  const Vector2 vstart = m_vTileRadius + Vector2(0, m_fTileSize*(m_nHeight - 1)); //start position
  Vector2 pos = vstart; //current position
  
  for(int i=0; i<m_nHeight; i++){
    int j = 0;
    pos.x = vstart.x;

    while(j < m_nWidth){
      while(j < m_nWidth && !isWall(m_chMap[i][j])){ 
        j++; pos.x += m_fTileSize;
      } //while

      if(j < m_nWidth){
        aabb.Center = Vector3(pos.x, pos.y, 0);
        aabb.Extents = vTileExtents;
        used[i][j] = true;
        j++; pos.x += m_fTileSize;
      } //if

      bool bSingleTile = true;

      while(j < m_nWidth && isWall(m_chMap[i][j])){
        BoundingBox b;
        b.Center = Vector3(pos.x, pos.y, 0);
        b.Extents = vTileExtents;
        BoundingBox::CreateMerged(aabb, aabb, b);
        used[i][j] = true;
        bSingleTile = false;
        j++; pos.x += m_fTileSize;
      } //while

      if(!bSingleTile)
        m_vecWalls.push_back(aabb);
    } //while

    pos.y -= m_fTileSize;
  } //for

  //vertical walls, the single tiles get caught here

  pos = vstart; //current position
  
  for(int j=0; j<m_nWidth; j++){
    int i = 0;
    pos.y = vstart.y;

    while(i < m_nHeight){
      while(i < m_nHeight && !isWall(m_chMap[i][j])){
        i++; pos.y -= m_fTileSize;
      } //while

      if(i < m_nHeight){
        aabb.Center = Vector3(pos.x, pos.y, 0);
        aabb.Extents = vTileExtents;
        used[i][j] = true;
        i++; pos.y -= m_fTileSize;
      } //if
      
      bool bSingleTile = true;

      while(i < m_nHeight && isWall(m_chMap[i][j])){
        BoundingBox b;
        b.Center = Vector3(pos.x, pos.y, 0);
        b.Extents = vTileExtents;
        BoundingBox::CreateMerged(aabb, aabb, b);
        used[i][j] = true;
        bSingleTile = false;
        i++; pos.y -= m_fTileSize;
      } //while
      
      if(!bSingleTile)
        m_vecWalls.push_back(aabb);
    } //while

    pos.x += m_fTileSize;
  } //for

  //orphaned single tiles that are not on an edge

  pos = vstart + Vector2(m_fTileSize, -m_fTileSize);
  
  for(int i=1; i<m_nHeight-1; i++){
    for(int j=1; j<m_nWidth-1; j++){
      if(isWall(m_chMap[i][j]) && 
        (!isWall(m_chMap[i - 1][j]) && !isWall(m_chMap[i + 1][j]) &&
         !isWall(m_chMap[i][j - 1]) &&!isWall(m_chMap[i][j + 1])))
      {    
        BoundingBox b;
        b.Center = Vector3(pos.x, pos.y, 0);
        b.Extents = vTileExtents;
        m_vecWalls.push_back(b);
      } //if

      pos.x += m_fTileSize;
    } //for
    
    pos.x = vstart.x + m_fTileSize;
    pos.y -= m_fTileSize;
  } //for
  
  //clean up and exit
  
  for(int i=0; i<m_nHeight; i++)
    delete [] used[i];

  delete [] used;
} //MakeBoundingBoxes


/// Delete the old map (if any), allocate the right sized
/// chunk of memory for the new map, and read it from a
/// text file.
/// \param filename Name of the map file.
void CTileManager::LoadMap(char* filename){

	m_vecTurrets.clear(); //clear turrets from previous level
	m_vecGoblins.clear(); //clear goblins from previous level
	m_vecSkeletons.clear(); //clear skeletons from previous level
	m_vecDemons.clear(); //clear demons from previous level
	m_vecBoss.clear(); //clear boss from previous level
	m_vecStairs.clear(); // clear stairs from previous level

  if(m_chMap != nullptr){ //unload any previous maps
    for(int i=0; i<m_nHeight; i++)
      delete [] m_chMap[i];

    delete [] m_chMap;
  } //if

  FILE *input; //input file handle

  fopen_s(&input, filename, "rb"); //open the map file
  if(input == nullptr) //abort if it's missing
    ABORT("Map &s not found.", filename);

  //read map file into a character buffer 

  fseek(input, 0, SEEK_END); //seek to end of map file
  const int n = ftell(input); //get file size in bytes
  rewind(input); //seek to start of file

  char *buffer = new char[n + 1]; //temporary character buffer
  fread(buffer, n, 1, input); //read the whole thing in a chunk
  fclose(input); //close the map file, we're done with it

  //get map width and height into m_nWidth and m_nHeight

  m_nWidth = -1; 
  m_nHeight = 0;
  int w = 0; //width of current row

  for(int i=0; i<n; i++){
    if(buffer[i] != '\n')
      w++; //skip characters until the end of line
    else{
      if(w == 0)ABORT("Panic!");
      if(w != m_nWidth && m_nWidth >= 0 && w != 0) //not the same length as the previous one
        ABORT("Line %d of map is not the same length as the previous one.", m_nHeight);
      m_nWidth = w; w = 0; m_nHeight++; //next line
    } //else
  } //for

  m_nWidth--;

  //allocate space for the map 
  
  m_chMap = new char*[m_nHeight];

  for(int i=0; i<m_nHeight; i++)
    m_chMap[i] = new char[m_nWidth];

  //load the map information from the buffer to the map

  int index = 0; //index into character buffer
  
  for(int i=0; i<m_nHeight; i++){
    for(int j=0; j<m_nWidth; j++){
      m_chMap[i][j] = buffer[index]; //load character into map
      index++; //next index
    } //for
    index += 2; //skip end of line character
  } //for

  m_vWorldSize = Vector2((float)m_nWidth, (float)m_nHeight)*m_fTileSize;
  MakeBoundingBoxes();

  delete [] buffer; //clean up
} //LoadMap


/// Load in a map from an image file
void CTileManager::LoadMapFromImageFile(char* filename) {

	m_vecTurrets.clear(); //clear turrets from previous level
	m_vecGoblins.clear(); //clear goblins from previous level
	m_vecSkeletons.clear(); //clear skeletons from previous level
	m_vecDemons.clear(); //clear demons from previous level
	m_vecBoss.clear(); //clear boss from previous level
	m_vecStairs.clear(); // clear stairs from previous level
	m_vecLamps.clear(); // clear lamps from previous level

	if (m_chMap != nullptr) { //unload any previous maps
		for (int i = 0; i < m_nHeight; i++) {
			delete[] m_chMap[i];
		}
		delete[] m_chMap;
	}

	//read map file into a byte buffer
	int channels = 0;
	unsigned char* buffer = stbi_load(filename, &m_nWidth, &m_nHeight, &channels, 0);

	//allocate space for the map
	m_chMap = new char* [m_nHeight];
	for (int i = 0; i < m_nHeight; i++) {
		m_chMap[i] = new char[m_nWidth];
	}

	//load the map information from the buffer to the map
	int index = 0;

	for (int i = 0; i < m_nHeight; i++) {

		for (int j = 0; j < m_nWidth; j++) {

			//if #BCBCBC Generic Wall
			if (buffer[index] == 188 && buffer[index + 1] == 188 && buffer[index + 2] == 188)
				m_chMap[i][j] = 'W';

			//if #010101 Bottom Wall
			else if (buffer[index] == 1 && buffer[index + 1] == 1 && buffer[index + 2] == 1)
				m_chMap[i][j] = 'B';

			//if #020202 Right Wall
			else if (buffer[index] == 2 && buffer[index + 1] == 2 && buffer[index + 2] == 2)
				m_chMap[i][j] = 'R';

			//if #030303 Left Wall
			else if (buffer[index] == 3 && buffer[index + 1] == 3 && buffer[index + 2] == 3)
				m_chMap[i][j] = 'L';

			//if #040404 Top Wall
			else if (buffer[index] == 4 && buffer[index + 1] == 4 && buffer[index + 2] == 4)
				m_chMap[i][j] = 'T';

			//if #050505 Top Left Corner Concave
			else if (buffer[index] == 5 && buffer[index + 1] == 5 && buffer[index + 2] == 5)
				m_chMap[i][j] = '{';

			//if #060606 Top Right Corner Concave
			else if (buffer[index] == 6 && buffer[index + 1] == 6 && buffer[index + 2] == 6)
				m_chMap[i][j] = '}';

			//if #070707 Bottom Left Corner Concave
			else if (buffer[index] == 7 && buffer[index + 1] == 7 && buffer[index + 2] == 7)
				m_chMap[i][j] = '[';

			//if #080808 Bottom Right Corner Concave
			else if (buffer[index] == 8 && buffer[index + 1] == 8 && buffer[index + 2] == 8)
				m_chMap[i][j] = ']';

			//if #090909 Bottom Right Corner Convex
			else if (buffer[index] == 9 && buffer[index + 1] == 9 && buffer[index + 2] == 9)
				m_chMap[i][j] = '.';

			//if #0a0a0a Bottom Left Corner Convex
			else if (buffer[index] == 10 && buffer[index + 1] == 10 && buffer[index + 2] == 10)
				m_chMap[i][j] = ',';

			//if #0b0b0b Top Right Corner Convex
			else if (buffer[index] == 11 && buffer[index + 1] == 11 && buffer[index + 2] == 11)
				m_chMap[i][j] = '>';

			//if #0c0c0c Top Left Corner Convex
			else if (buffer[index] == 12 && buffer[index + 1] == 12 && buffer[index + 2] == 12)
				m_chMap[i][j] = '<';

			//if #808080 (HTML Gray) Outside floor (Abyss)
			else if (buffer[index] == 128 && buffer[index + 1] == 128 && buffer[index + 2] == 128)
				m_chMap[i][j] = 'O';

			//if #FFFFFF Floor
			else if (buffer[index] == 255 && buffer[index + 1] == 255 && buffer[index + 2] == 255)
				m_chMap[i][j] = 'F';

			else
				m_chMap[i][j] = 'F';

			//if #0000FF(Blue) add stairs
			if (buffer[index] == 0 && buffer[index + 1] == 0 && buffer[index + 2] == 255)
				m_vecStairs.push_back(Vector2((float)j, m_nHeight - (float)i) * m_fTileSize);
			//This causes severe lag for some reason


			//if #00FF00(Green) add goblin
			if (buffer[index] == 0 && buffer[index + 1] == 255 && buffer[index + 2] == 0)
				m_vecGoblins.push_back(Vector2((float)j, m_nHeight - (float)i) * m_fTileSize);

			//if #FFFF00(Yellow) add skeleton
			if (buffer[index] == 255 && buffer[index + 1] == 255 && buffer[index + 2] == 0)
				m_vecSkeletons.push_back(Vector2((float)j, m_nHeight - (float)i) * m_fTileSize);

			//if #FF0000(Red) add demon
			if (buffer[index] == 255 && buffer[index + 1] == 0 && buffer[index + 2] == 0)
				m_vecDemons.push_back(Vector2((float)j, m_nHeight - (float)i) * m_fTileSize);

			//if #FF00FF(Magenta) add boss
			if (buffer[index] == 255 && buffer[index + 1] == 0 && buffer[index + 2] == 255)
				m_vecBoss.push_back(Vector2((float)j, m_nHeight - (float)i) * m_fTileSize);

			//if #FF7700(Orange) add Lamp
			if (buffer[index] == 255 && buffer[index + 1] == 119 && buffer[index + 2] == 0)
				m_vecLamps.push_back(Vector2((float)j, m_nHeight - (float)i) * m_fTileSize);

			//if #00FFFF(Cyan) add player (SHOULD ONLY BE ONE!!!!!!)
			if (buffer[index] == 0 && buffer[index + 1] == 255 && buffer[index + 2] == 255)
				m_vPlayerPos = Vector2((float)j, m_nHeight - (float)i) * m_fTileSize;


			index += channels;

		}//inner for

	}//outer for

	m_vWorldSize = Vector2((float)m_nWidth, (float)m_nHeight) * m_fTileSize;
	MakeBoundingBoxes();

	stbi_image_free(buffer);

} //LoadMapFromImageFile


/// This is for debug purposes so that you can verify that
/// the collision shapes are in the right places.
/// \param t Line sprite to be stretched to draw the line.
void CTileManager::DrawBoundingBoxes(eSpriteType t){
  for(auto&  p: m_vecWalls)
    m_pRenderer->DrawBoundingBox(p);
} //DrawBoundingBoxes


/// Draw order is top-down, left-to-right so that the image
/// agrees with the map text file viewed in NotePad.
/// \param t Sprite type for a 3-frame sprite: 0 is floor, 1 is wall, 2 is an error tile.
void CTileManager::Draw(eSpriteType t){
  CSpriteDesc2D desc; //sprite descriptor for tile
  desc.m_nSpriteIndex = t; //sprite index for tile

  const int w = (int)ceil(m_nWinWidth/m_fTileSize) + 2; //width of window in tiles, with 2 extra
  const int h = (int)ceil(m_nWinHeight/m_fTileSize) + 2; //height of window in tiles, with 2 extra

  const Vector2 campos = m_pRenderer->GetCameraPos(); //camera position
  const Vector2 origin = campos + 0.5f*m_nWinWidth*Vector2(-1.0f, 1.0f); //position of top left corner of window

  const int top = max(0, m_nHeight - (int)round(origin.y/m_fTileSize) + 1); //index of top tile
  const int bottom = min(top + h + 1, m_nHeight - 1); //index of bottom tile

  const int left = max(0, (int)round(origin.x/m_fTileSize) - 1); //index of left tile
  const int right = min(left + w, m_nWidth - 1); //index of right tile

  for(int i=top; i<=bottom; i++) //for each column
    for(int j=left; j<=right; j++){ //for each row
      desc.m_vPos.x = (j + 0.5f)*m_fTileSize; //horizontal component of tile position
      desc.m_vPos.y = (m_nHeight - 1 - i + 0.5f)*m_fTileSize; //vertical component of tile position

      switch(m_chMap[i][j]){ //select which frame of the tile sprite is to be drawn
		//add cases for more tiles
	  case '{': desc.m_nCurrentFrame = 0; break; //top left corner concave
	  case 'T': desc.m_nCurrentFrame = 1; break; //top wall
	  case '}': desc.m_nCurrentFrame = 2; break; //top right corner concave
	  case '.': desc.m_nCurrentFrame = 3; break; //bottom right corner convex
	  case ',': desc.m_nCurrentFrame = 4; break; //bottom left corner convex
	  case 'L': desc.m_nCurrentFrame = 6; break; //left wall
      case 'F': desc.m_nCurrentFrame = 7; break; //floor
	  case 'R': desc.m_nCurrentFrame = 8; break; //right wall
	  case '>': desc.m_nCurrentFrame = 9; break; //top right corner convex
	  case '<': desc.m_nCurrentFrame = 10; break; //top left corner convex
	  case '[': desc.m_nCurrentFrame = 12; break; //bottom left corner concave
	  case 'B': desc.m_nCurrentFrame = 13; break; //bottom wall
	  case ']': desc.m_nCurrentFrame = 14; break; //bottom right corner concave
      case 'W': desc.m_nCurrentFrame = 32; break; //wall
	  case 'C': desc.m_nCurrentFrame = 34; break; // chest
	  case 'O': desc.m_nCurrentFrame = 51; break; //outside floor abyss
      default:  desc.m_nCurrentFrame = 99; break; //error tile

      } //switch

      m_pRenderer->Draw(desc); //finally we can draw a tile
    } //for
} //Draw


/// Check whether a circle is visible from a point, that is, either the left
/// or the right side of the object (from the perspective of the point)
/// has no walls between it and the point. This gives some weird behavior
/// when the circle is partially hidden by a block, but it doesn't seem
/// particularly unnatural in practice. It'll do.
/// \param p0 A point.
/// \param p1 Center of circle.
/// \param radius Radius of circle.
/// \return true If the circle is visible from the point.
bool CTileManager::Visible(const Vector2& p0, const Vector2& p1, float radius){
  bool visible = true;

  for(auto i=m_vecWalls.begin(); i!=m_vecWalls.end() && visible; i++){
    Vector2 direction = p0 - p1;
    direction.Normalize();
    const Vector2 norm = Vector2(-direction.y, direction.x);

    const float delta = min(radius, 16.0f);

    //left-hand triangle
    const Vector3 v0(p0);
    const Vector3 v1(p1 + radius*norm);
    const Vector3 v2(p1 + (radius - delta)*norm);
    
    //right-hand triangle (or perhaps it's the other way round)
    const Vector3 v3(p1 - radius*norm);
    const Vector3 v4(p1 - (radius - delta)*norm);

    visible = !(*i).Intersects(v0, v1, v2) || !(*i).Intersects(v0, v3, v4);
  } //for

  return visible;
} //Visible


/// \brief Template for bounding shape collisions.
///
/// Determine whether a bounding shape intersects a wall.
/// \param s A bounding shape.
/// \return true if the bounding shape intersects a wall.
template<class t> bool CTileManager::CollideWithWall(const t& s){
  bool hit = false;

  for(auto i=m_vecWalls.begin(); i!=m_vecWalls.end() && !hit; i++)
    hit = s.Intersects(*i);

  return hit;
} //CollideWithWall


///Determines the direction of the collision
template<class t> int CTileManager::CollideDirection(const t& o, CObject* obj) {
	Vector2 objPos = obj->GetPos();
	BoundingBox wall;
	bool hit = false;
	
	for (auto i = m_vecWalls.begin(); i != m_vecWalls.end(); i++) {  //this is the corner slidy thing
		if (o.Intersects(*i)) {
			if (hit)
				return 6;
			hit = true;
			wall = *i;
		}//if
	}//for

	if (hit == false)
		return 5;

	Vector2 wallPos = Vector2(wall.Center.x,wall.Center.y);

	float Xdiff = objPos.x - wallPos.x;
	float Ydiff = objPos.y - wallPos.y;

	float wHeight = wall.Extents.y + obj->GetBoundingSphere().Radius;
	float wWidth = wall.Extents.x + obj->GetBoundingSphere().Radius;
	
	if (wHeight - 5 <= Ydiff && Ydiff <= wHeight && -wWidth <= Xdiff && Xdiff <= wWidth) //collision with top side
		return 1;
	else if (-(wHeight - 5) >= Ydiff && Ydiff >= -wHeight && -wWidth <= Xdiff && Xdiff <= wWidth) //colision with bottom side
		return 2;
	else if (wWidth - 5 <= Xdiff && Xdiff <= wWidth && -wHeight <= Ydiff && Ydiff <= wHeight) //colllision with right side
		return 3;
	else if (-(wWidth - 5) >= Xdiff && Xdiff >= -wWidth && -wHeight <= Ydiff && Ydiff <= wHeight) //colision with left side
		return 4;
	else
		return 6;


}//CollideDirection


//Enforce bounding shapes that can be used by function CollideWithWall.
template bool CTileManager::CollideWithWall<BoundingBox>(const BoundingBox&);
template bool CTileManager::CollideWithWall<BoundingSphere>(const BoundingSphere&);

template int CTileManager::CollideDirection<BoundingBox>(const BoundingBox&, CObject*);
template int CTileManager::CollideDirection<BoundingSphere>(const BoundingSphere&, CObject*);
