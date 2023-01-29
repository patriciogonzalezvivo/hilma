#include "hilma/ops/raytrace.h"

#include <cstdlib>
#include <stdlib.h>
#include <algorithm>

#include "glm/glm.hpp"

#include "hilma/ops/math.h"
#include "hilma/ops/text.h"
#include "hilma/ops/intersection.h"

#include <thread>
#include <mutex>
#include <atomic>
#include <condition_variable>

namespace hilma {

Hittable::Hittable( const std::vector<Line>& _lines, int _branches) {
    for (size_t i = 0; i < _lines.size(); i++) {
        expand(_lines[i][0]);
        expand(_lines[i][1]);
    }
    expand(0.001f);

    if (_branches > 0 && _lines.size() > 1) {
        leaf = false;

        // int axis = rand() % 3;
        // auto comparator = (axis == 0) ? Line::compareX
        //                 : (axis == 1) ? Line::compareY
        //                 : Line::compareZ;

        float width = getWidth();
        float height = getHeight();
        float depth = getDepth();
        auto comparator = (width > std::max(height, depth) ) ? Line::compareX
                        : (height > std::max(width, depth) ) ? Line::compareY
                        : Line::compareZ;

        std::vector<Line> lns = _lines;
        std::sort(lns.begin(), lns.end(), comparator);

        std::size_t const half_size = lns.size() / 2;
        left = std::make_shared<Hittable>(std::vector<Line>(lns.begin(), lns.begin() + half_size), _branches-1);
        right = std::make_shared<Hittable>(std::vector<Line>(lns.begin() + half_size, lns.end()), _branches-1);
    }
    else {
        leaf = true;
        lines = _lines;
    }
}

Hittable::Hittable( const std::vector<Triangle>& _triangles, int _branches) {
    for (size_t i = 0; i < _triangles.size(); i++) {
        expand(_triangles[i][0]);
        expand(_triangles[i][1]);
        expand(_triangles[i][2]);
    }
    expand(0.001f);

    if (_branches > 0 && _triangles.size() > 1) {
        leaf = false;

        // int axis = rand() % 3;
        // auto comparator = (axis == 0) ? Triangle::compareX
        //                 : (axis == 1) ? Triengle::compareY
        //                 : Triangle::compareZ;

        float width = getWidth();
        float height = getHeight();
        float depth = getDepth();
        auto comparator = (width > std::max(height, depth) ) ? Triangle::compareX
                        : (height > std::max(width, depth) ) ? Triangle::compareY
                        : Triangle::compareZ;

        std::vector<Triangle> tris = _triangles;
        std::sort(tris.begin(), tris.end(), comparator);

        std::size_t const half_size = tris.size() / 2;
        left = std::make_shared<Hittable>(std::vector<Triangle>(tris.begin(), tris.begin() + half_size), _branches-1);
        right = std::make_shared<Hittable>(std::vector<Triangle>(tris.begin() + half_size, tris.end()), _branches-1);
    }
    else {
        leaf = true;
        triangles = _triangles;
    }
}

int Hittable::getTotalLines() {
    if (leaf)
        return lines.size();
    else
        return left->getTotalLines() + right->getTotalLines();
}

int Hittable::getTotalTriangles() {
    if (leaf)
        return triangles.size();
    else
        return left->getTotalTriangles() + right->getTotalTriangles();
}

Mesh Hittable::getMesh() {
    Mesh mesh;
    if (leaf) {
        if (triangles.size() > 0)
            mesh.addTriangles(&triangles[0], triangles.size());
        
        if (lines.size() > 0)
            mesh.addEdges(&lines[0], lines.size());
    }
    else {
        mesh.append( left->getMesh() );
        mesh.append( right->getMesh() );
    }
    return mesh;
}

// Ray / Line

bool hit(const Ray& _ray, float _minDistance, float _maxDistance, const std::vector<Line>& _lines, HitRecord& _rec) {
    Line ray = Line(_ray.getOrigin(), _ray.getAt(std::min(100.0f, _maxDistance)));

    HitRecord tmp_rec;
    bool hit_anything = false;
    float closest_so_far = _maxDistance;

    glm::vec3 ip;
    for (size_t i = 0; i < _lines.size(); i++) {
        if (intersection(ray, _lines[i], ip, 0.005)) {
            tmp_rec.distance = glm::length(_ray.getOrigin() - ip);
            if (tmp_rec.distance < closest_so_far) {
                closest_so_far = tmp_rec.distance;
                tmp_rec.line = std::make_shared<Line>(_lines[i]);
                tmp_rec.frontFace = true;
                _rec = tmp_rec;
                hit_anything = true;
            }
        }
    }

    return hit_anything;
}

// RAY / TRIANGLE 
bool hit(const Ray& _ray, float _minDistance, float _maxDistance, const std::vector<Triangle>& _triangles, HitRecord& _rec) {

    bool hit_anything = false;
    float closest_so_far = _maxDistance;

    HitRecord tmp_rec;
    for (size_t i = 0; i < _triangles.size(); i++) {
        float distance, u, v;
        if ( intersection(_ray, _triangles[i], distance, u, v) ) {
            if (distance > _minDistance && distance < closest_so_far ) {
                hit_anything = true;
                closest_so_far = distance;

                _rec.distance = distance;
                _rec.position = _ray.getAt(distance);
                _rec.barycentric = glm::vec3((1.0f - u - v), u, v);
                _rec.normal = _triangles[i].getNormal();

                _rec.frontFace = glm::dot(_ray.getDirection(), _rec.normal) < 0;
                _rec.normal = _rec.frontFace ? _rec.normal :-_rec.normal;

                _rec.triangle = std::make_shared<Triangle>(_triangles[i]);
            }
        }
    }

    return hit_anything;
}


// RAY HITTABLE
//
bool hit(const Ray& _ray, float _minDistance, float _maxDistance, const std::vector<Hittable>& _hittables, HitRecord& _rec) {

    bool hit_anything = false;
    float closest_so_far = _maxDistance;

    HitRecord tmp_rec;
    // foreach hittable
    for (size_t i = 0; i < _hittables.size(); i++) {
        if ( _hittables[i].hit( _ray, _minDistance, _maxDistance, tmp_rec) ) {
            if (tmp_rec.distance < closest_so_far) {
                hit_anything = true;
                closest_so_far = tmp_rec.distance;
                _rec = tmp_rec;
            }
        }
    }

    return hit_anything;
}

bool Hittable::hit(const Ray& _ray, float _minDistance, float _maxDistance, HitRecord& _rec) const {
    if ( !intersection(_ray, (BoundingBox)*this, _minDistance, _maxDistance) )
        return false;

    bool hit_anything = false;

    if (!leaf) {
        bool hit_left = left->hit(_ray, _minDistance, _maxDistance, _rec);
        bool hit_right = right->hit(_ray, _minDistance, hit_left ? _rec.distance : _maxDistance, _rec);

        return hit_left || hit_right;
    }
    else {
        HitRecord tmp_rec;
        float closest_so_far = _maxDistance;
        if ( hilma::hit(_ray, _minDistance, closest_so_far, triangles, tmp_rec) ) {
            if ( tmp_rec.distance < closest_so_far ) {
                hit_anything = true;
                closest_so_far = tmp_rec.distance;
                _rec = tmp_rec;
            }
        }

        if ( hilma::hit(_ray, _minDistance, closest_so_far, lines, tmp_rec) ) {
            if (tmp_rec.distance < closest_so_far) {
                hit_anything = true;
                closest_so_far = tmp_rec.distance;
                _rec = tmp_rec;
            }
        }
    }

    return hit_anything;
}

glm::vec3 default_rayColor(const Ray& _ray, const std::vector<Hittable>& _hittables, int _depth) {
    if (_depth <= 0)
        return glm::vec3(0.0f);

    HitRecord rec;
    if ( hit(_ray, 0.001, 1000.0, _hittables, rec) ) {

        if (!rec.frontFace)
            return glm::vec3(0.0f);

        if (rec.line != nullptr)
            return glm::vec3(2.0f);

        glm::vec3 diffuse = glm::vec3(1.0f);
        glm::vec3 emissive = glm::vec3(0.0f);
        glm::vec3 normal = rec.normal;
        float opacity = 1.0f;
        float metallic = 0.0f;
        float roughtness = 1.0f;

        if (rec.triangle != nullptr) {
            normal = rec.triangle->getNormal(rec.barycentric);
            diffuse = rec.triangle->getColor(rec.barycentric);

            if (rec.triangle->material != nullptr) {
                bool haveUV = rec.triangle->haveTexCoords();
                glm::vec2 uv;

                if (haveUV) uv = rec.triangle->getTexCoord(rec.barycentric);
                // uv.x = 1.0f - uv.x;

                if ( rec.triangle->material->haveProperty("emissive") )
                    if ( haveUV ) emissive = rec.triangle->material->getColor("emissive", uv);
                    else emissive = rec.triangle->material->getColor("emissive");
                
                if ( rec.triangle->material->haveProperty("roughness") )
                    roughtness = rec.triangle->material->getValue("roughness", uv);

                if ( rec.triangle->material->haveProperty("metallic") )
                    metallic = rec.triangle->material->getValue("metallic", uv);

                if ( rec.triangle->material->haveProperty("opacity") )
                    opacity = rec.triangle->material->getValue("opacity", uv);
            }
        }

        glm::vec3 dir = glm::normalize(_ray.getDirection());
        glm::vec3 reflected = glm::reflect(dir, normal);
        glm::vec3 target = glm::mix(normal, reflected, metallic);
        target += random_unit_vector() * roughtness;

        Ray scattered(rec.position, target);
        return emissive + diffuse * default_rayColor( scattered, _hittables, _depth-1 );
    }

    return glm::vec3(0.0f);
    // glm::vec3 unit_direction = normalize(_ray.getDirection() );
    // float t = 0.5f * (unit_direction.y + 1.0f);
    // return glm::mix(glm::vec3(1.0f), glm::vec3(0.5f, 0.7f, 1.0f), t);
}

glm::vec3 albedo_rayColor(const Ray& _ray, const std::vector<Hittable>& _hittables, int _depth) {
    HitRecord rec;
    if ( hit(_ray, 0.001, 1000.0, _hittables, rec) ) {
        glm::vec3 diffuse = glm::vec3(1.0f);

        if (rec.triangle != nullptr)
            diffuse = rec.triangle->getColor(rec.barycentric);
        
        return diffuse;
    }
    return glm::vec3(0.0f);
}

glm::vec3 normal_rayColor(const Ray& _ray, const std::vector<Hittable>& _hittables, int _depth) {
    HitRecord rec;
    if ( hit(_ray, 0.001, 1000.0, _hittables, rec) )
        if (rec.triangle != nullptr)
            return rec.triangle->getNormal(rec.barycentric);
    
    return glm::vec3(0.0f);
}

void raytrace(  Image& _image, const Camera& _cam, const std::vector<Hittable>& _scene, int _samplesPerPixel, int _maxDepth, 
                std::function<glm::vec3(const Ray&, const std::vector<Hittable>&, int)> _rayColor) {

    const int image_width = _image.getWidth();
    const int image_height = _image.getHeight();
    const float over_samples = 1.0f/_samplesPerPixel;

    const int totalPixels = image_width * image_height;

    std::cout << std::endl;

    for (int y = 0; y < image_height; ++y) {
        for (int x = 0; x < image_width; ++x) {
            int i = y * image_width + x;

            glm::vec3 pixel_color(0.0f, 0.0f, 0.0f);
            for (int s = 0; s < _samplesPerPixel; ++s) {
                float u = (x + randomf()) / (image_width-1);
                float v = (y + randomf()) / (image_height-1);

                Ray ray = _cam.getRay(u, v);
                pixel_color += _rayColor(ray, _scene, _maxDepth);
            }

            pixel_color = pixel_color * over_samples;
            pixel_color = sqrt(pixel_color);
            _image.setColor( _image.getIndex(x, y) , pixel_color);
            
            printProgressBar("RayTracing -", i / float(totalPixels), 100 );
        }
    }
}

std::mutex writeM;

struct BlockJob {
    int rowStart;
    int rowEnd;
    int colSize;
    int spp;
    std::vector<int> index;
    std::vector<glm::vec3> colors;
};

void raytrace_thread(BlockJob _job, std::vector<BlockJob>& imageBlocks, int _ny, 
                    const Camera& _cam, const std::vector<Hittable>& _scene, int _maxDepth,
                    std::mutex& mutex, std::condition_variable& cv, std::atomic<int>& _completedThreads,
                    std::function<glm::vec3(const Ray&, const std::vector<Hittable>&, int)> _rayColor) {
    
    for (int j = _job.rowStart; j < _job.rowEnd; ++j) {
        for (int i = 0; i < _job.colSize; ++i) {

            glm::vec3 pixel_color(0.0f, 0.0f, 0.0f);
            for (int s = 0; s < _job.spp; ++s) {
                float u = float(i + randomf()) / float(_job.colSize);
                float v = float(j + randomf()) / float(_ny);

                Ray ray = _cam.getRay(u, v);
                pixel_color += _rayColor(ray, _scene, _maxDepth);
            }

            pixel_color /= float(_job.spp);
            pixel_color = sqrt(pixel_color);

            _job.index.push_back( (j * _job.colSize + i) * 3);
            _job.colors.push_back(pixel_color);
        }
    }

    {
        std::lock_guard<std::mutex> lock(mutex);
        imageBlocks.push_back(_job);
        _completedThreads++;
        cv.notify_one();
    }
}

void raytrace_multithread(Image& _image, const Camera& _cam, const std::vector<Hittable>& _scene, int _samples, int _maxDepth, 
                            std::function<glm::vec3(const Ray&, const std::vector<Hittable>&, int)> _rayColor) {

    int nx = _image.getWidth();
    int ny = _image.getHeight();
    int ns = _samples;

    const int nThreads = std::thread::hardware_concurrency();
    int rowsPerThread = ny / nThreads;
    int leftOver = ny % nThreads;

    std::mutex mutex;
    std::condition_variable cvResults;
    std::vector<BlockJob> imageBlocks;
    std::atomic<int> completedThreads = { 0 };
    std::vector<std::thread> threads;

    for (int i = 0; i < nThreads; ++i) {
        BlockJob job;
        job.rowStart = i * rowsPerThread;
        job.rowEnd = job.rowStart + rowsPerThread;
        if (i == nThreads - 1)
            job.rowEnd = job.rowStart + rowsPerThread + leftOver;

        job.colSize = nx;
        job.spp = ns;

        std::thread t(  [   job, &imageBlocks, ny, 
                            _cam, _scene, _maxDepth,
                            &mutex, &cvResults, &completedThreads,
                            _rayColor]() {
            raytrace_thread(job, imageBlocks, ny, 
                            _cam, _scene, _maxDepth,
                            mutex, cvResults, completedThreads,
                            _rayColor);
        });
        threads.push_back(std::move(t));
    }

    // launched jobs. need to build image.
    // wait for number of jobs = pixel count
    {
        std::unique_lock<std::mutex> lock(mutex);
        cvResults.wait(lock, [&completedThreads, &nThreads] {
            printProgressBar("RayTracing -", completedThreads / float(nThreads), 100 );
            return completedThreads == nThreads;
        });
    }

    for (std::thread& t : threads)
        t.join();

    for (BlockJob job : imageBlocks) {
        int index = job.rowStart;
        int colorIndex = 0;
        for (glm::vec3& col : job.colors) {
            // glm::vec2 pos = job.index[colorIndex];
            _image.setColor( job.index[colorIndex] , col);
            ++colorIndex;
        }
    }

}

}