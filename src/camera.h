#ifndef CAMERA_H
#define CAMERA_H

#include "ray.h"

typedef struct camera {
    vec3 origin;
    vec3 lower_left_corner;
    vec3 horizontal;
    vec3 vertical;
    vec3 u, v, w;
    float lens_radius;
} camera;

ray camera_cast_ray(camera *cam, float u, float v);
void camera_pos(camera *cam, vec3 lookfrom, vec3 lookat, vec3 vup, float vfov, float aspect, float aperture, float focus_dist);

#endif
