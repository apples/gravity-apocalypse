#include "field.h"

#include "log.h"
#include "gravitation.h"
#include "resource.h"

#include "inugami/math.h"

#include <algorithm>

using Inugami::sgn;

static std::uniform_real_distribution<double> dist(0.0, 1.0);
static std::uniform_real_distribution<double> dist2(-1.0, 1.0);

template <typename T>
static void eraseNullptr(T& c)
{
    c.erase(
        std::remove_if(c.begin(), c.end(),
            [](const typename T::value_type& in){return (in==nullptr);}),
        c.end())
    ;
}

Field::Field() :
    WIDTH(256), HEIGHT(192),
    paddles { {
        Paddle(this, 1.f, Paddle::Color::BLUE),
        Paddle(this, -1.f, Paddle::Color::RED)
    } },
    balls(),
    forces(),
    scores{{0,0}},
    spawnData{120, 30, 240},
    rng(),
    leftMost(nullptr), rightMost(nullptr)
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
    updateForces();

    for (auto& p : paddles) p.tick();

    updateBalls();
    updateItems();
}

void Field::draw(Transform mat)
{
    for (Force *f : forces) f->draw(mat);
    for (auto& p : paddles) p.draw(mat);
    for (Ball *b : balls) b->draw(mat);
    for (Item *i : items) i->draw(mat);
}

SimVec<double> Field::getAccel(SimVec<double> pos)
{
    SimVec<double> rval = {0.0, 0.0};
    for (Force *f : forces) rval += f->getAccel(pos);
    return rval;
}

void Field::addForce(Force *f)
{
    forces.push_back(f);
}

void Field::updateBalls()
{
    for (Ball *& bp : balls)
    {
        bp->tick(std::bind(&Field::getAccel, this, std::placeholders::_1));

        if (bp->pos.y >= 76)
        {
            bp->pos.y = 76;
            bp->vel.y = std::abs(bp->vel.y)*-1;
        }

        if (bp->pos.y <= -76)
        {
            bp->pos.y = -76;
            bp->vel.y = std::abs(bp->vel.y);
        }

        if (bp->pos.x > WIDTH/2)
        {
            ++scores[0];
            bp = nullptr;
            spawnBall();
            break;
        }

        if (bp->pos.x < -WIDTH/2)
        {
            ++scores[1];
            bp = nullptr;
            spawnBall();
            break;
        }

        int dir = 1;
        for (auto& p : paddles)
        {
            int dx = int(bp->pos.x)-p.pos.x;
            int dy = int(bp->pos.y)-p.pos.y;
            if (dx >= -8 && dx <= 8 && dy >= -20 && dy <= 20)
            {
                if (sgn(bp->vel.x) != dir)
                {
                    bp->pos.x = p.pos.x+8.0*dir;
                    bp->vel.x = std::abs(bp->vel.x)*dir;
                    bp->vel.y += (p.vel.y-bp->vel.y)*0.5;
                    bp->vel *= 1.1;
                    bp->hit(&p);
                }
            }
            dir = -dir;
        }

        if (bp->vel.x < 0.0)
        {
            if (!leftMost || bp->pos.x < leftMost->pos.x)
            {
                leftMost = bp;
            }
        }

        if (bp->vel.x > 0.0)
        {
            if (!rightMost || bp->pos.x > rightMost->pos.x)
            {
                rightMost = bp;
            }
        }
    }

    eraseNullptr(balls);
}

void Field::updateItems()
{
    for (Item *& ip : items)
    {
        ip->tick(std::bind(&Field::getAccel, this, std::placeholders::_1));

        if (ip->pos.y >= 76)
        {
            ip->pos.y = 76;
            ip->vel.y = std::abs(ip->vel.y)*-1;
        }

        if (ip->pos.y <= -76)
        {
            ip->pos.y = -76;
            ip->vel.y = std::abs(ip->vel.y);
        }

        if (ip->pos.x > WIDTH/2)
        {
            ip = nullptr;
            break;
        }

        if (ip->pos.x < -WIDTH/2)
        {
            ip = nullptr;
            break;
        }

        for (auto& p : paddles)
        {
            int dx = int(ip->pos.x)-p.pos.x;
            int dy = int(ip->pos.y)-p.pos.y;
            if (dx >= -8 && dx <= 8 && dy >= -20 && dy <= 20)
            {
                p.addItem(ip);
                ip = nullptr;
                break;
            }
        }

        if (!ip) continue;

        for (Ball *bp : balls)
        {
            if (!bp->owner) continue;
            int dx = int(ip->pos.x)-bp->pos.x;
            int dy = int(ip->pos.y)-bp->pos.y;
            if (dx >= -8 && dx <= 8 && dy >= -8 && dy <= 8)
            {
                ip->vel += {sgn(bp->owner->pos.x)*0.1f, 0.f};
                break;
            }
        }
    }
    eraseNullptr(items);
}

void Field::updateForces()
{
    for (auto& f : forces)
    {
        f->tick();
        if (f->dead)
        {
            delete f;
            f = nullptr;
        }
    }
    eraseNullptr(forces);
}

void Field::spawnForces()
{
    static double ts = 1.0;
    //ts += 0.01;

    if (--spawnData.cooldown <= 0)
    {
        Gravitation *g = new Gravitation;

        g->pos = {0.0,0.0};
        g->vel = {dist2(rng)*1.0, dist2(rng)*1.0};
        g->gTarget = (dist(rng)*4.0+2.0)*ts;
        g->curve = dist(rng)*1.0;
        g->influence = 3.0;
        g->timeEnd = dist(rng)*120.0;
        g->hold = dist(rng)*30.0;
        g->decayEnd = dist(rng)*120.0;

        addForce(g);

        std::uniform_int_distribution<int> distcd(spawnData.minCool, spawnData.maxCool);
        spawnData.cooldown = distcd(rng);

        if (dist(rng)>0.5)
        {
            std::uniform_int_distribution<int> disti(0, 3);

            SimVec<double> dir = {dist2(rng)*1.0, dist2(rng)*1.0};
            if (dir.length() != 0.0) dir /= dir.length();
            items.push_back(new Item({0.0,0.0}, dir, disti(rng)));
        }
    }
}

void Field::resetBalls()
{
    for (Ball* p : balls) delete p;
    balls.clear();
    for (int i=0; i<2; ++i)
    {
        spawnBall();
    }
}

void Field::spawnBall()
{
    SimVec<double> dir = {dist2(rng)*1.0, dist2(rng)*1.0};
    if (dir.length() != 0.0) dir /= dir.length();
    balls.push_back(new Ball({0.0,0.0}, dir));
}
