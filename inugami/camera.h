/*******************************************************************************

Copyright (c) 2012 Jeramy Harrison

This software is provided 'as-is', without any express or implied warranty. In no event will the authors be held liable for any damages arising from the use of this software.

Permission is granted to anyone to use this software for any purpose, including commercial applications, and to alter it and redistribute it freely, subject to the following restrictions:

    1. The origin of this software must not be misrepresented; you must not claim that you wrote the original software. If you use this software in a product, an acknowledgment in the product documentation would be appreciated but is not required.

    2. Altered source versions must be plainly marked as such, and must not be misrepresented as being the original software.

    3. This notice may not be removed or altered from any source distribution.

*******************************************************************************/

#ifndef INUGAMI_CAMERA_H
#define INUGAMI_CAMERA_H

#include "opengl.h"

namespace Inugami {

class Camera
{
public:
    typedef ::glm::vec3 Vec3;
    typedef ::glm::mat4 Mat4;

    Camera();
    Camera(const Camera&) = default;
    virtual ~Camera();

    Camera& perspective(float fov, float ratio, float near, float far);
    Camera& ortho(float left, float right, float bottom, float top, float near, float far);

    Camera& translate(const Vec3& pos);
    Camera& rotate(float deg, const Vec3& axis);

    Camera& pitch(float deg);
    Camera& yaw(float deg);
    Camera& roll(float deg);

    Camera& pushProjection();
    Camera& pushView();
    Camera& popProjection();
    Camera& popView();

    const Mat4& getProjection() const;
    const Mat4& getView() const;

    bool cullFaces;
    bool depthTest;

private:
    Mat4 projection;
    Mat4 view;
};

} // namespace Inugami

#endif // INUGAMI_CAMERA_H
