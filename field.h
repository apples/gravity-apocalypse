#ifndef FIELD_H
#define FIELD_H

#include "ball.h"
#include "force.h"
#include "paddle.h"
#include "simvec.h"

#include "inugami/transform.h"

#include <array>
#include <vector>
#include <random>

class Field
{
public:
    Field();
    virtual ~Field();

    void tick();
    void draw(Transform mat);

    SimVec<double> getAccel(SimVec<double> pos);

    const int WIDTH;
    const int HEIGHT;

    std::array<Paddle, 2> paddles;

    std::vector<Ball*> balls;
    std::vector<Force*> forces;

    std::array<int, 2> scores;

private:
    void doCollision();
    void spawnForces();
    void resetBalls();

    struct
    {
        int cooldown;
        int minCool, maxCool;
    } spawnData;

    std::mt19937 rng;
};

#endif // FIELD_H
