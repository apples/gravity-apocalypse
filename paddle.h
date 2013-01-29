#ifndef PADDLE_H
#define PADDLE_H

#include "simvec.h"

#include "inugami/transform.h"

#include <string>

using Inugami::Transform;
using Inugami::Vec3;

class Paddle final
{
public:
    enum class Color
    {
        BLUE,
        RED
    };

    Paddle(const std::string& in, Color cIn);
    ~Paddle();

    void tick();
    void draw(Transform mat);

    Color color;

    SimVec<double> pos;
    SimVec<double> vel;

    int maxHeight;

private:
    std::string name;
};

#endif // PADDLE_H
