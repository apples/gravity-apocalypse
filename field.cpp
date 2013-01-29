#include "field.h"

#include "log.h"
#include "gravitation.h"
#include "resource.h"

#include "inugami/math.h"

#include <algorithm>

using Inugami::sgn;

static std::uniform_real_distribution<double> dist(0.0, 1.0);
static std::uniform_real_distribution<double> dist2(-1.0, 1.0);

Field::Field() :
    WIDTH(256), HEIGHT(192),
    paddles { {
        Paddle("player.1", Paddle::Color::BLUE),
        Paddle("player.2", Paddle::Color::RED)
    } },
    balls(),
    forces(),
    scores{{0,0}},
    spawnData{120, 30, 120}
{
    resetBalls();
}

Field::~Field()
{
    for (auto& f : forces) delete f;
    for (auto& b : balls) delete b;
}

void Field::tick()
{
    spawnForces();
    if (resource->core->getInterface()->mousePressed('L'_ivm))
    {
        auto iface = resource->core->getInterface();
        Gravitation *g = new Gravitation;
        auto mpos = iface->getMousePos();

        g->pos = {(mpos.x/1024.0-0.5)*WIDTH, -(mpos.y/768.0-0.5)*HEIGHT};
//        g->pos = {dist2(rng)*100.0, dist2(rng)*96.0};
//        g->vel = {dist2(rng)*1.0, dist2(rng)*1.0};
        g->vel = {0.0,0.0};
        g->gTarget = 8.0;
        g->curve = 0.0;
        g->influence = 1.0; //dist(rng)*8.0+8.0;
        g->timeEnd = 120.0;
        g->hold = 60.0;
        g->decayEnd = 120.0;

        forces.push_back(g);

    }

    for (auto& p : paddles) p.tick();

    for (auto& f : forces)
    {
        f->tick();
        if (f->dead)
        {
            delete f;
            f = nullptr;
        }
    }
    forces.erase(std::remove_if(forces.begin(), forces.end(), [](Force* in){return (in==nullptr);}), forces.end());

    for (Ball * bp : balls)
    {
        Ball &ball = *bp;
        ball.tick(std::bind(&Field::getAccel, this, std::placeholders::_1));

        if (ball.pos.y >= 76)
        {
            ball.pos.y = 76;
            ball.vel.y = std::abs(ball.vel.y)*-1;
        }

        if (ball.pos.y <= -76)
        {
            ball.pos.y = -76;
            ball.vel.y = std::abs(ball.vel.y);
        }

        if (ball.pos.x > WIDTH/2)
        {
            ++scores[0];
            resetBalls();
            break;
        }

        if (ball.pos.x < -WIDTH/2)
        {
            ++scores[1];
            resetBalls();
            break;
        }

        int dir = 1;
        for (auto& p : paddles)
        {
            int dx = int(ball.pos.x)-p.pos.x;
            int dy = int(ball.pos.y)-p.pos.y;
            if (dx >= -8 && dx <= 8 && dy >= -20 && dy <= 20)
            {
                if (sgn(ball.vel.x) != dir)
                {
                    ball.vel.x = std::abs(ball.vel.x)*dir;
                    ball.vel.y += (p.vel.y-ball.vel.y)*0.5;
                    ball.vel *= 1.1;
                    ball.hit();
                }
            }
            dir = -dir;
        }
    }
}

void Field::draw(Transform mat)
{
    for (auto& f : forces) f->draw(mat);
    for (auto& p : paddles) p.draw(mat);
    for (Ball *b : balls) b->draw(mat);
}

SimVec<double> Field::getAccel(SimVec<double> pos)
{
    SimVec<double> rval = {0.0, 0.0};
    for (Force *f : forces) rval += f->getAccel(pos);
    return rval;
}

void Field::spawnForces()
{
    if (--spawnData.cooldown <= 0)
    {
        Gravitation *g = new Gravitation;

        g->pos = {0.0,0.0};
        g->vel = {dist2(rng)*1.0, dist2(rng)*1.0};
        g->gTarget = -(dist(rng)*4.0+4.0);
        g->curve = dist(rng)*1.0;
        g->influence = 2.0;
        g->timeEnd = dist(rng)*120.0;
        g->hold = dist(rng)*30.0;
        g->decayEnd = dist(rng)*120.0;

        forces.push_back(g);

        std::uniform_int_distribution<int> disti(spawnData.minCool, spawnData.maxCool);
        spawnData.cooldown = disti(rng);
    }
}

void Field::resetBalls()
{
    for (Ball* p : balls) delete p;
    balls.clear();
    SimVec<double> dir = {dist2(rng)*1.0, dist2(rng)*1.0};
    if (dir.x != 0.0 || dir.y != 0.0) dir /= dir.length();
    balls.push_back(new Ball({0.0,0.0}, {1,0}));
}
