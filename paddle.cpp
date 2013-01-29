#include "paddle.h"

#include "log.h"
#include "resource.h"

#include "inugami/math.h"

using Inugami::clamp;
using Inugami::sgn;

Paddle::Paddle(const std::string& in, Color cIn) :
    color(cIn),
    pos{0.0, 0.0},
    vel{0.0, 0.0},
    maxHeight(128),
    name(in)
{
    switch (color)
    {
        case Color::BLUE:
        {
            pos.x = -116.0;
            break;
        }

        case Color::RED:
        {
            pos.x = 116.0;
            break;
        }
    }
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
}

void Paddle::draw(Transform mat)
{
    mat.translate(Vec3{int(pos.x), int(pos.y), 0.f});
    resource->core->modelMatrix(mat);
    resource->getSheet("paddles").draw(0,0);
}
