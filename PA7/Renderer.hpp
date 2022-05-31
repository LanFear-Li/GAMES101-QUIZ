#ifndef RAYTRACING_RENDERER_H
#define RAYTRACING_RENDERER_H

#include <fstream>

#include "Scene.hpp"

struct hit_payload {
    float tNear;
    uint32_t index;
    Vector2f uv;
    Object *hit_obj;
};

class Renderer {
public:
    void Render(const Scene &scene);

private:
};

#endif //RAYTRACING_RENDERER_H