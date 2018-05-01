#include "gravitation.h"

#include "resource.h"
#include "log.h"

#include "inugami/math.h"

using Inugami::Vec3;
using Inugami::clamp;

Gravitation::Gravitation() :
    Force(),
    pos{0,0}, vel{0,0},
    gTarget(0),
    curve(0),
    influence(0),
    time(0), timeEnd(0), hold(0), decay(0), decayEnd(0),
    rot(0.0)
{}
//
//Gravitation::Gravitation(double gForce, double curveIn, double influenceIn, int startTime, int endTime, int holdIn, int decayIn) :
//    Force(),
//    pos{0,0}, vel{0,0},
//    gTarget(gForce),
//    curve(curveIn),
//    influence(influenceIn),
//    time(startTime), timeEnd(endTime), hold(holdIn), decay(0), decayEnd(decayIn),
//    rot(0.0)
//{}

Gravitation::~Gravitation()
{}

void Gravitation::tick()
{
    double t = 0.0;
    if (timeEnd != 0) t = time/double(timeEnd);
    if (decayEnd != 0) t -= decay/double(decayEnd);

    gCurrent = gTarget*calcFactor(curve, t);

    if (time >= timeEnd)
    {
        if (hold <= 0)
        {
            if (decay >= decayEnd) dead = true;
            else ++decay;
        }
        else --hold;
    }
    else ++time;

    pos += vel;
}

void Gravitation::draw(Transform mat)
{
    if (timeEnd != 0) rot += time/double(timeEnd);
    if (decayEnd != 0) rot += decay/double(decayEnd);

    mat.translate(Vec3(pos.x, pos.y, 0.f));
    mat.scale(Vec3(gCurrent/4.0, gCurrent/4.0, 1.f));
    mat.rotate(rot*5.0*influence, Vec3(0.f, 0.f, -1.f));
    resource->core->modelMatrix(mat);
    if (gCurrent>0.0) resource->getSheet("forces").draw(0,0);
    else resource->getSheet("forces").draw(0,1);
}

SimVec<double> Gravitation::getAccel(const SimVec<double>& inPos)
{
    SimVec<double> rval = {0.0, 0.0};
    rval.x = pos.x-inPos.x;
    rval.y = pos.y-inPos.y;
    double dist = rval.length();
    if (dist < gCurrent) return {0.0, 0.0};
    rval /= dist;
    dist /= influence;
    rval *= gCurrent/(dist*dist+1.0);
    return rval;
}

double Gravitation::calcFactor(double c, double t) //static
{
    static const double PI = 4.0*std::atan(1);
    return  ((-c*t/2.0) - (c*std::sin(2.0*PI*t)/(4.0*PI)) + t) / (1.0 - c/2.0);
}
