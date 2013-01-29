#ifndef FORCE_H
#define FORCE_H

#include "simvec.h"

#include "inugami/transform.h"

using Inugami::Transform;

class Force
{
public:
    Force();
    virtual ~Force();

    virtual void tick() = 0;
    virtual void draw(Transform) = 0;

    virtual SimVec<double> getAccel(const SimVec<double>& pos) = 0;

    bool dead;

};

#endif // FORCE_H
