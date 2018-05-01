#include "item.h"

#include "resource.h"

Item::Item() :
    pos{0.f, 0.f},
    vel{0.f, 0.f},
    id(0),
    size(0.f)
{
    //ctor
}

Item::Item(const SimVec<double>& p, const SimVec<double>& v, int i) :
    pos(p),
    vel(v),
    id(i),
    size(0.f)
{
    //ctor
}

Item::~Item()
{
    //dtor
}

void Item::tick(const AccelFunc& accel)
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

void Item::draw(Transform mat)
{
    mat.translate(Vec3{int(pos.x), int(pos.y), 0.f});
    mat.scale(Vec3{size, size, 1.f});
    resource->core->modelMatrix(mat);
    resource->getSheet("powerups").draw(0,id);
}
