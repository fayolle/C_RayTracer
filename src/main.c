#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <math.h>

#include "vec3.h"
#include "ray.h"
#include "hitable.h"
#include "camera.h"
#include "material.h"


vec3 color(ray r, struct sphere *spheres, int nsph, int depth) {
    hit_record rec;
    if (world_hit(spheres, nsph, &r, 0.001, MAXFLOAT, &rec)) {
        ray scattered;
        vec3 attenuation;
        if (depth < 8 && rec.mat->scatter(rec.mat, r, &rec, &attenuation, &scattered)) {
            return vec3_multiply_vec(attenuation, color(scattered, spheres, nsph, depth + 1));
        }
        else {
            return (vec3) { .x = 0, .y = 0, .z = 0 };
        }
    }
    else {
        //vec3 unit_direction = unit_vector(r.B);
        //float t = 0.5f * (unit_direction.y + 1.0);
        vec3 white = { .x = 1.0f,.y = 1.0f,.z = 1.0f };
        //vec3 blue = { .x = 0.5f, .y = 0.7f, .z = 1.0f };
        //vec3 v_ = vec3_add_vec(vec3_multiply_float(white, 1.0f - t),
        //                      vec3_multiply_float(blue, t));
        //return v_;
        return white;
    }
}


static double unitrand() {
    return (float)(rand()) / (float)(RAND_MAX);
}


int
main(int argc, char **argv) {
    int width = 1200, height = 800;

    int i, j, s;
    int ns = 100;

    int ir, ig, ib;


    struct material lamb1 = { .albedo.x = 0.1,.albedo.y = 0.2,.albedo.z = 0.5,.scatter = lambertian_scatter };
    struct material lamb2 = { .albedo.x = 0.5,.albedo.y = 0.5,.albedo.z = 0.5,.scatter = lambertian_scatter };
    struct material metal1 = { .albedo.x = 0.8,.albedo.y = 0.6,.albedo.z = 0.2,.scatter = metal_scatter };
    struct material dielectric = { .ref_idx = 1.5,.scatter = dielectric_scatter };


    struct sphere world[] = {
      {.center.x = 0,.center.y = 0,.center.z = -1,.radius = 0.5,.mat = &lamb1 },
      {.center.x = 0,.center.y = -100.5,.center.z = -1,.radius = 100,.mat = &lamb2 },
      {.center.x = 1,.center.y = 0,.center.z = -1,.radius = 0.5,.mat = &metal1 },
      {.center.x = -1,.center.y = 0,.center.z = -1,.radius = 0.5,.mat = &dielectric }//,
      //{ .center.x = -1, .center.y = 0, .center.z = -1, .radius = 0.45, .mat = &dielectric }
    };


    vec3 lookfrom = (vec3) { .x = 0, .y = 1, .z = 20 };//{ .x = 13, .y = 2, .z = 3 };
    vec3 lookat = (vec3) { .x = 0, .y = 0, .z = 0 };
    float dist_to_focus = 10.0;
    float aperture = 0.1;
    camera cam;
    camera_pos(
        &cam,
        lookfrom,
        lookat,
        (vec3) {
        .x = 0, .y = 1, .z = 0
    },  /* up */
        20,                       /* field of view */
            (float)width / (float)height, /* aspect ratio */
            aperture,
            dist_to_focus
            );

    printf("P3\n%d %d\n255\n", width, height);

    for (i = 0; i < height; i++) {
        for (j = 0; j < width; j++) {
            vec3 col = { .x = 0,.y = 0,.z = 0 };
            for (s = 0; s < ns; s++) {
                float u = (j + unitrand()) / (float)width;
                float v = ((height - i) + unitrand()) / (float)height;
                ray r = camera_cast_ray(&cam, u, v);
                col = vec3_add_vec(col, color(r, world, sizeof(world) / sizeof(struct sphere), 0));
            }
            col = vec3_divide_float(col, ns);
            col = (vec3) { .x = sqrtf(col.x), .y = sqrtf(col.y), .z = sqrtf(col.z) };
            col = vec3_multiply_float(col, 255.99);

            ir = (int)(col.x);
            ig = (int)(col.y);
            ib = (int)(col.z);

            printf("%d %d %d\n", ir, ig, ib);
        }
    }

    return 0;
}
