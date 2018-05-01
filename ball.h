#ifndef BALL_H
#define BALL_H

#include "simvec.h"
#include "paddle.h"

#include "inugami/animatedsprite.h"
#include "inugami/transform.h"

#include <functional>

using Inugami::AnimatedSprite;
using Inugami::Transform;

class Ball
{
public:
    typedef std::function<SimVec<double>(SimVec<double>)> AccelFunc;

    Ball();
    Ball(const SimVec<double>& p, const SimVec<double>& v);
    virtual ~Ball();

    void tick(const AccelFunc& accel);
    void draw(Transform mat);

    void hit(Paddle *p);

    SimVec<double> pos, vel;

    Paddle *owner;

private:
    void setupSprites();

    AnimatedSprite sprite;
    int paddleHits;

    float size;

};

#endif // BALL_H
