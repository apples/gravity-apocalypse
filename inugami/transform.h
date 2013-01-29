#ifndef INUGAMI_TRANSFORM_H
#define INUGAMI_TRANSFORM_H

#include "opengl.h"

#include <vector>

namespace Inugami {

typedef ::glm::vec3 Vec3;
typedef ::glm::mat4 Mat4;

class Transform
{
    typedef std::vector<Mat4> Stack;

public:
    Transform();
    Transform(const Transform& in);
    virtual ~Transform();

    operator Mat4() const;

    Transform& translate(const Vec3& pos);
    Transform& scale(const Vec3& vec);
    Transform& rotate(float deg, const Vec3& axis);

    Transform& push();
    Transform& pop();
    Transform& reset();

    Mat4 toMat4() const;

private:
    Stack stack;
};

} // namespace Inugami

#endif // INUGAMI_TRANSFORM_H
