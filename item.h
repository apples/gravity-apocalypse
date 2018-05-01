#ifndef ITEM_H
#define ITEM_H

#include "simvec.h"

#include "inugami/transform.h"

#include <functional>

using Inugami::Transform;
using Inugami::Vec3;

class Item
{
public:
    typedef std::function<SimVec<double>(SimVec<double>)> AccelFunc;

    Item();
    Item(const SimVec<double>& p, const SimVec<double>& v, int i);
    virtual ~Item();

    void tick(const AccelFunc& accel);
    void draw(Transform mat);

    SimVec<double> pos, vel;

    int id;

private:
    float size;
};

#endif // ITEM_H
