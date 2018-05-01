#ifndef PADDLE_H
#define PADDLE_H

#include "simvec.h"
#include "gravitation.h"
#include "item.h"

#include "inugami/animatedsprite.h"
#include "inugami/transform.h"

#include <string>

using Inugami::AnimatedSprite;
using Inugami::Transform;
using Inugami::Vec3;

class Field;

class Field;

class Paddle final
{
public:
    enum class State
    {
        SAD     =0,
        HURT    =1,
        HAPPY   =2,
        SMUG    =3,
        NORMAL  =4
    };

    enum class Color
    {
        BLUE    =0,
        RED     =1
    };

    enum class PowerUp
    {
        NONE    =-1,
        DRIFT   =0,
        SHUNT   =1,
        BOMB    =2
    };

    Paddle(Field *f, float dir, Color cIn);
    ~Paddle();

    void tick();
    void draw(Transform mat);

    void setState(State s, unsigned int d);
    void addItem(Item *in);

    Color color;

    SimVec<double> pos;
    SimVec<double> vel;

    int maxHeight;


private:
    void updateForce();

    Field *owner;

    std::string name;
    float rot;
    State state;
    unsigned int stateDur;

    double power;
    float gDir;

    Gravitation *force;
    PowerUp powerup;
    int powerLevel;
    bool meta[8];

    AnimatedSprite rainbow;
};

#endif // PADDLE_H
