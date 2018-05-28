#ifndef PROJECT_PROJECTGAMEENGINE_TILES_SQUARETILE
#define PROJECT_PROJECTGAMEENGINE_TILES_SQUARETILE 1
#pragma once

#include "BaseTile.h"

class QuadRenderObject;

class SquareTile : public BaseTile
{
public:

  SquareTile( uint32 resourceId );

  virtual uint8               GetNumRotations(){ return (uint8)(m_Rotation.y / 90); }
};

#endif /// PROJECT_PROJECTGAMEENGINE_TILES_SQUARETILE