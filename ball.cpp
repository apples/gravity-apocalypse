#include "ball.h"

#include "resource.h"

using Inugami::Vec3;

Ball::Ball() :
    pos{0.f, 0.f},
    vel{0.f, 0.f},
    sprite(),
    paddleHits(1),
    size(0.f),
    owner(nullptr)
{
    setupSprites();
}

Ball::Ball(const SimVec<double>& p, const SimVec<double>& v) :
    pos(p),
    vel(v),
    sprite(),
    paddleHits(1),
    size(0.f),
    owner(nullptr)
{
    setupSprites();
}

Ball::~Ball()
{}

void Ball::tick(const AccelFunc& accel)
{
    if (size < 1.f)
    {
        size += 0.025;
        return;
    }

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
    mat.translate(Vec3{int(pos.x), int(pos.y), 0.f});
    mat.scale(Vec3{size, size, 1.f});
    resource->core->modelMatrix(mat);
    if (owner) resource->getSheet("ball.2x2").draw(1,int(owner->color));
    sprite.draw();
}

void Ball::hit(Paddle *p)
{
    owner=p;
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
