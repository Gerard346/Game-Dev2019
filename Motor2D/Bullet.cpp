#include "j1App.h"
#include"p2Log.h"
#include"p2Defs.h"
#include "j1Audio.h"
#include "j1Render.h"
#include "j1Animation.h"
#include "j1Map.h"
#include "j1Module.h"
#include "EntityManager.h"
#include "Bullet.h"

Bullet::Bullet()
{
}

Bullet::Bullet(const Bullet* copy) :BaseEntity(copy)
{
}

Bullet::~Bullet()
{
}