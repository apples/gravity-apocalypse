#include "paddle.h"

#include "field.h"
#include "resource.h"

#include "inugami/math.h"

using Inugami::clamp;
using Inugami::sgn;
using Inugami::low;

Paddle::Paddle(Field *f, float dir, Color cIn) :
    color(cIn),
    pos{0.0, 0.0},
    vel{0.0, 0.0},
    maxHeight(128),
    owner(f), name(),
    rot(0.f),
    state(State::NORMAL), stateDur(-1),
    power(0.0),
    gDir(dir),
    force(nullptr), powerup(PowerUp::NONE), powerLevel(0)
{
    switch (color)
    {
        case Color::BLUE:
        {
            pos.x = -116.0;
            rot = 0.f;
            name = "player.1";
            break;
        }

        case Color::RED:
        {
            pos.x = 116.0;
            rot = 180.f;
            name = "player.2";
            break;
        }
    }

    rainbow.setSpritesheet(&resource->getSheet("powerups"));
    rainbow.setSprites(
    {
        {1, 0},
        {1, 1},
        {1, 2},
        {1, 3},
        {1, 4},
        {1, 5},
        {1, 6},
        {1, 7},
    });
    rainbow.setSequence(
    {
        {0, 4},
        {1, 4},
        {2, 4},
        {3, 4},
        {4, 4},
        {5, 4},
        {6, 4},
        {7, 4},
    });
    rainbow.setMode(AnimatedSprite::Mode::LOOP);
}

Paddle::~Paddle()
{}

void Paddle::tick()
{
    vel = {0,0};
    if (resource->getKeybind(name+".up"))   vel.y+=3;
    if (resource->getKeybind(name+".down")) vel.y-=3;
    vel.y = clamp(vel.y, -3.0, 3.0);

    pos += vel;

    int max = maxHeight/2;
    pos.y = clamp(int(pos.y), -max, max);

    if (--stateDur == 0) setState(State::NORMAL, -1);

    if (power < 1.0) power += 0.00390625;
    if (power > 1.0) power = 1.0;
    if (powerup == PowerUp::NONE) power = 0.0;

    updateForce();
}

void Paddle::draw(Transform mat)
{
    mat.translate(Vec3{int(pos.x), int(pos.y), 0.f});
    mat.rotate(rot, Vec3{0.f, 0.f, 1.f});
    resource->core->modelMatrix(mat);
    resource->getSheet("paddles").draw(0,int(state));

    if (int(powerup)>=0)
    {
        mat.translate(Vec3{-8.f, 0.f, 0.f});
        mat.push();
        for (int i=1; i<powerLevel; ++i)
        {
            mat.translate(Vec3{0.f, 8.f, 0.f});
            resource->core->modelMatrix(mat);
            resource->getSheet("powerups").draw(2,int(powerup));
        }
        mat.pop();
        mat.push();
        mat.rotate(180.f, Vec3{0.f, 0.f, 1.f});
        for (int i=1; i<powerLevel; ++i)
        {
            mat.translate(Vec3{0.f, 8.f, 0.f});
            resource->core->modelMatrix(mat);
            resource->getSheet("powerups").draw(2,int(powerup));
        }
        mat.pop();
        mat.scale(Vec3{power, power, 1.f});
        resource->core->modelMatrix(mat);
        resource->getSheet("powerups").draw(0,int(powerup));
        if (power == 1.0) rainbow.draw();
    }
}

void Paddle::setState(State s, unsigned int d)
{
    state = s;
    stateDur = d;
}

void Paddle::addItem(Item *in)
{
    switch (in->id)
    {
        case 0:
        {
            if (powerup != PowerUp::DRIFT)
            {
                powerLevel = 0;
                power = 0.0;
            }
            powerup = PowerUp::DRIFT;
            powerLevel = low(++powerLevel, 3);
        break;}

        case 1:
        {
            if (powerup != PowerUp::SHUNT)
            {
                powerLevel = 0;
                power = 0.0;
            }
            powerup = PowerUp::SHUNT;
            powerLevel = low(++powerLevel, 3);
        break;}

        case 2:
        {
            if (powerup != PowerUp::BOMB)
            {
                powerLevel = 0;
                power = 0.0;
            }
            powerup = PowerUp::BOMB;
            powerLevel = low(++powerLevel, 3);
        break;}

        case 3:
        {
            if (--powerLevel <= 0)
            {
                powerLevel = 0;
                powerup = PowerUp::NONE;
            }
        break;}
    }
}

void Paddle::updateForce()
{
    bool fire = resource->getKeybind(name+".fire").pressed();

    if (!force && fire && (powerup!=PowerUp::NONE))
    {
        fire = false;
        force = new Gravitation;

        switch (powerup)
        {
            case PowerUp::DRIFT:
            {
                force->pos = {pos.x, pos.y};
                force->vel = {2.5*gDir, vel.y/4.0};
                force->gTarget = power*2.5;
                force->curve = 1.0;
                force->influence = 6.0;
                force->timeEnd = 50;
                force->hold = 30;
                force->decayEnd = 10;
            break;}

            case PowerUp::SHUNT:
            {
                force->pos = {pos.x, pos.y};
                force->vel = {2.0*gDir, vel.y/4.0};
                force->gTarget = -power*5.0;
                force->curve = 1.0;
                force->influence = 2.0+powerLevel*1.0;
                force->timeEnd = 10;
                force->hold = 20;
                force->decayEnd = 10;
            break;}

            case PowerUp::BOMB:
            {
                force->pos = {pos.x, pos.y};
                force->vel = {3.0*gDir, vel.y/4.0};
                force->gTarget = power*5.0;
                force->curve = 1.0;
                force->influence = 3.0;
                force->timeEnd = 10;
                force->hold = 30;
                force->decayEnd = 10;

                meta[0] = false;
                meta[1] = false;
            break;}
        }

        owner->addForce(force);
        power = 0.0;
    }

    if (force)
    {
        switch (powerup)
        {
            case PowerUp::DRIFT:
            {
                force = nullptr;
            break;}

            case PowerUp::SHUNT:
            {
                force = nullptr;
            break;}

            case PowerUp::BOMB:
            {
                if (!meta[0])
                {
                    force->time = 2;
                    if (fire)
                    {
                        force->vel *= 0.1;
                        meta[0] = true;
                    }
                }
                else
                {
                    if (!meta[1])
                    {
                        if (force->decay >= force->decayEnd-1)
                        {
                            auto nf = new Gravitation;

                            nf->pos = force->pos;
                            nf->vel = {0.0,0.0};
                            nf->gTarget = -force->gTarget;
                            nf->curve = 1.0;
                            nf->influence = 3.0;
                            nf->timeEnd = 5;
                            nf->hold = 15;
                            nf->decayEnd = 5;

                            owner->addForce(nf);

                            force = nullptr;

                            meta[1] = true;
                        }
                    }
                }
            break;}
        }
        if (force && force->pos.x*gDir > 128.0) force=nullptr;
    }
}
