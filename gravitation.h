#ifndef GRAVITATION_H
#define GRAVITATION_H

#include "force.h"

class Gravitation : public Force
{
public:
    Gravitation();
    Gravitation(double gForce, double curveIn, double influenceIn, int startTime, int endTime, int holdIn, int decayIn);
    virtual ~Gravitation();

    virtual void tick();
    virtual void draw(Transform mat);

    virtual SimVec<double> getAccel(const SimVec<double>& pos);

    SimVec<double> pos, vel;

    double gCurrent, gTarget;
    double curve;
    double influence;
    int time, timeEnd, hold, decay, decayEnd;

private:
    static double calcFactor(double c, double t);

    double rot;
};

#endif // GRAVITATION_H
