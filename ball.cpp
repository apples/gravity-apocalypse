#include "ball.h"

#include "resource.h"

using Inugami::Vec3;

Ball::Ball() :
    pos{0.f, 0.f},
    vel{0.f, 0.f},
    sprite(),
    paddleHits(1)
{
    setupSprites();
}

Ball::Ball(const SimVec<double>& p, const SimVec<double>& v) :
    pos(p),
    vel(v),
    sprite(),
    paddleHits(1)
{
    setupSprites();
}

Ball::~Ball()
{}

void Ball::tick(const AccelFunc& accel)
{
    SimVec<double> vels[4];
    vels[0] = vel+accel(pos);
    vels[1] = vels[0]+accel(pos+vels[0]*0.5);
    vels[2] = vels[1]+accel(pos+vels[1]*0.5);
    vels[3] = vels[2]+accel(pos+vels[2]);

    vel = (vels[0] + vels[1]*2.0 + vels[2]*2.0 + vels[3])/6.0;
    pos += vel;
}

void Ball::draw(Transform mat)
{
    mat.translate(Vec3(int(pos.x), int(pos.y), 0.f));
    resource->core->modelMatrix(mat);
    sprite.draw();
}

void Ball::hit()
{
    ++paddleHits;
}

void Ball::setupSprites()
{
    sprite.setSpritesheet(&resource->getSheet("ball"));
    sprite.setSprites(
    {
        {0, 0},
        {0, 1},
        {0, 2},
        {0, 3},
        {0, 4},
        {0, 5},
        {0, 6},
    });
    sprite.setSequence(
    {
        {0, 4},
        {1, 4},
        {2, 4},
        {3, 4},
        {4, 4},
        {5, 4},
        {6, 4},
    });
    sprite.setMode(AnimatedSprite::Mode::LOOP);
}
