#include "Scene.hpp"

void Scene::buildBVH() {
    printf(" - Generating BVH...\n\n");
    this->bvh = new BVHAccel(objects, 1, BVHAccel::SplitMethod::NAIVE);
}

Intersection Scene::intersect(const Ray &ray) const {
    return this->bvh->Intersect(ray);
}

void Scene::sampleLight(Intersection &pos, float &pdf) const {
    float emit_area_sum = 0;
    for (uint32_t k = 0; k < objects.size(); ++k) {
        if (objects[k]->hasEmit()) {
            emit_area_sum += objects[k]->getArea();
        }
    }
    float p = get_random_float() * emit_area_sum;
    emit_area_sum = 0;
    for (uint32_t k = 0; k < objects.size(); ++k) {
        if (objects[k]->hasEmit()) {
            emit_area_sum += objects[k]->getArea();
            if (p <= emit_area_sum) {
                objects[k]->Sample(pos, pdf);
                break;
            }
        }
    }
}

bool Scene::trace(
        const Ray &ray,
        const std::vector<Object *> &objects,
        float &tNear, uint32_t &index, Object **hitObject) {
    *hitObject = nullptr;
    for (uint32_t k = 0; k < objects.size(); ++k) {
        float tNearK = kInfinity;
        uint32_t indexK;
        Vector2f uvK;
        if (objects[k]->intersect(ray, tNearK, indexK) && tNearK < tNear) {
            *hitObject = objects[k];
            tNear = tNearK;
            index = indexK;
        }
    }


    return (*hitObject != nullptr);
}

// Implementation of Path Tracing
Vector3f Scene::castRay(const Ray &ray, int depth) const {
    if (depth > maxDepth) return {};

    Vector3f light_dir(0, 0, 0), light_indir(0, 0, 0);
    Intersection p = intersect(ray);
    // no intersection or intersect with light, just return
    if (!p.happened) return {};
    else if (p.m->hasEmission()) return p.m->getEmission();

    Vector3f loc_p = p.coords;
    Vector3f wo = ray.direction;
    Vector3f n = p.normal;
    Material *m = p.m;

    // calculate direct illumination
    float pdf_light;
    Intersection x;
    sampleLight(x, pdf_light);

    Vector3f loc_x = x.coords;
    Vector3f ws = loc_x - loc_p;
    Vector3f nn = x.normal;
    Vector3f emit = x.emit;

    Ray test_ray(loc_p, ws.normalized());
    Intersection test_inter = intersect(test_ray);

    float dis = dotProduct(ws, ws);
    Vector3f ws_norm = ws.normalized();
    if (test_inter.distance > ws.norm() - 0.05) {
        light_dir = emit * m->eval(wo, ws_norm, n)
                    * dotProduct(ws_norm, n) * dotProduct(-ws_norm, nn)
                    / dis / pdf_light;
    }

    // enable russian roulette
    float guess = get_random_float();
    if (guess > RussianRoulette) return light_dir;

    // calculate indirect illumination
    Vector3f wi = m->sample(wo, n).normalized();
    float pdf = m->pdf(wo, wi, n);

    Ray trace_ray(loc_p, wi);
    Intersection q = intersect(trace_ray);
    if (!q.happened || q.m->hasEmission()) return light_dir;

    // make sure this is an object
    light_indir = castRay(Ray(loc_p, wi), depth + 1) * m->eval(wo, wi, n)
                  * std::fmax(dotProduct(wi, n), 0.0f)
                  / std::fmax(pdf, EPSILON) / RussianRoulette;
    return light_dir + light_indir;
}