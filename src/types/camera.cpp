#include "hilma/types/camera.h"

#include "hilma/ops/math.h"

namespace hilma {

Camera::Camera( const glm::vec3& _lookfrom, const glm::vec3& _lookat, const glm::vec3& _vup,
                float _vfov, // vertical field-of-view in degrees
                float _aspect_ratio, float _aperture, float _focus_dist) {

    origin = _lookfrom;
    lens_radius = _aperture * 0.5f;

    float theta = glm::radians(_vfov);
    float half_height = tan(theta/2.0f);
    float half_width = _aspect_ratio * half_height;

    w = glm::normalize(_lookfrom - _lookat);
    u = glm::normalize(glm::cross(_vup, w));
    v = glm::cross(w, u);
    lower_left_corner = origin
                        - half_width * _focus_dist * u
                        - half_height * _focus_dist * v
                        - _focus_dist * w;

    horizontal = 2.0f * half_width * _focus_dist * u;
    vertical = 2.0f * half_height * _focus_dist * v;
}

Ray Camera::getRay(float s, float t) const {
    // float a = randomf(0.0f, TAU);
    // float r = randomf();
    // glm::vec3 rd = lens_radius * glm::vec3(r*cos(a), r*sin(a), 0.0);
    glm::vec3 rd = lens_radius * random_in_unit_disk();

    glm::vec3 offset = u * rd.x + v * rd.y;

    return Ray(
        origin + offset,
        lower_left_corner + s * horizontal + t * vertical - origin - offset
    );
}

}