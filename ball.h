#ifndef BALL_H
#define BALL_H

#include "simvec.h"

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

    void hit();

    SimVec<double> pos, vel, acc;

private:
    void setupSprites();

    AnimatedSprite sprite;
    int paddleHits;
};

#endif // BALL_H
