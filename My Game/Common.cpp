/// \file Common.cpp
/// \brief Code for the class CCommon.
///
/// This file contains declarations and initial values
/// for CCommon's static member variables.

#include "Common.h"

CGame* CCommon::m_gGame = nullptr;
CRenderer* CCommon::m_pRenderer = nullptr;
CObjectManager* CCommon::m_pObjectManager = nullptr;
CParticleEngine2D* CCommon::m_pParticleEngine = nullptr;
CTileManager* CCommon::m_pTileManager = nullptr;

Vector2 CCommon::m_vWorldSize = Vector2::Zero;
bool CCommon::m_bDrawAABBs = false;
CPlayer* CCommon::m_pPlayer = nullptr;
CItems* CCommon::m_pItems = nullptr;
Skeleton* CCommon::m_pSkeleton = nullptr;
CObject* CCommon::m_pLoadingscreen = nullptr;
CObject* CCommon::m_pHealthbar = nullptr;
CObject* CCommon::m_pBossHealthbar = nullptr;
CObject* CCommon::m_pHealthoutline = nullptr;
bool CCommon::m_bPlayerDeath = false;
bool CCommon::m_bNextLevel = false;
bool CCommon::m_bGameWin= false;

bool CCommon::m_bAbsoluteControls = true;

//eSoundType CCommon::m_eDownSnd = CLANG_SOUND;
//eSoundType CCommon::m_eUpSnd = UMPH_SOUND;
