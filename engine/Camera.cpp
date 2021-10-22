//
// Created by Иван Ильин on 14.01.2021.
//

#include "Camera.h"
#include "utils/Log.h"

#include <cmath>

std::vector<std::shared_ptr<Triangle>> Camera::project(std::shared_ptr<Mesh> mesh) {

    if(!_ready) {
        Log::log("Camera::project(): cannot project _tris without camera initialization ( Camera::init() ) ");
        return _triangles;
    }

    if(!mesh->isVisible())
        return this->_triangles;

    // Model transform matrix: translate _tris in the origin of body.
    Matrix4x4 M = Matrix4x4::Translation(mesh->position());
    Matrix4x4 VM = _V * M;

    // We don't want to waste time re-allocating memory every time
    std::vector<Triangle> clippedTriangles, tempBuffer;

    for(auto& t : mesh->triangles()) {


        double dot = t.norm().dot((mesh->position() + Vec3D(t[0]) - position()).normalized());

        if(dot > 0)
            continue;

        // It needs to be cleared because it's reused through iterations. Usually it doesn't free memory.
        clippedTriangles.clear();
        tempBuffer.clear();

        // In the beginning we need to to translate triangle from world coordinate to our camera system:
        // After that we apply clipping for all planes from _clipPlanes
        clippedTriangles.emplace_back(t * VM);
        for(auto& plane : _clipPlanes)
        {
            while(!clippedTriangles.empty())
            {
                std::vector<Triangle> clipResult = plane.clip(clippedTriangles.back());
                clippedTriangles.pop_back();
                for(auto & i : clipResult)
                    tempBuffer.emplace_back(i);
            }
            clippedTriangles.swap(tempBuffer);
        }

        for(auto& clipped : clippedTriangles) {
            sf::Color color = clipped.color();
            sf::Color ambientColor = sf::Color((sf::Uint8)(color.r * (0.3 * std::abs(dot) + 0.7)),
                                               (sf::Uint8)(color.g * (0.3 * std::abs(dot) + 0.7)),
                                               (sf::Uint8)(color.b * (0.3 * std::abs(dot) + 0.7)),
                                               (sf::Uint8)color.a);

            // Finally its time to project our clipped colored drawTriangle from 3D -> 2D
            // and transform it's coordinate to screen space (in pixels):
            Triangle clippedProjected = clipped * _SP;

            Triangle clippedProjectedNormalized = Triangle(clippedProjected[0] / clippedProjected[0].w(),
                                                           clippedProjected[1] / clippedProjected[1].w(),
                                                           clippedProjected[2] / clippedProjected[2].w(),
                                                           ambientColor);

            _triangles.emplace_back(std::make_shared<Triangle>(clippedProjectedNormalized));
        }
    }

    return this->_triangles;
}

void Camera::init(int width, int height, double fov, double ZNear, double ZFar) {
    // We need to init camera only after creation or changing width, height, fov, ZNear or ZFar.
    // Because here we calculate matrix that does not change during the motion of _objects or camera
    _aspect = (double)width / (double)height;
    _P = Matrix4x4::Projection(fov, _aspect, ZNear, ZFar);
    _S = Matrix4x4::ScreenSpace(width, height);

    _SP = _S * _P; // screen-space-projections matrix

    // This is planes for clipping _tris.
    // Motivation: we are not interest in _tris that we cannot see.
    _clipPlanes.emplace_back(Plane(Vec3D{0, 0, 1}, Vec3D{0, 0, ZNear})); // near plane
    _clipPlanes.emplace_back(Plane(Vec3D{0, 0, -1}, Vec3D{0, 0, ZFar})); // far plane

    double thetta1 = Consts::PI*fov*0.5/180.0;
    double thetta2 = atan(_aspect * tan(thetta1));
    _clipPlanes.emplace_back(Plane(Vec3D{-cos(thetta2), 0, sin(thetta2)}, Vec3D{0, 0, 0})); // left plane
    _clipPlanes.emplace_back(Plane(Vec3D{cos(thetta2), 0, sin(thetta2)}, Vec3D{0, 0, 0})); // right plane
    _clipPlanes.emplace_back(Plane(Vec3D{0, cos(thetta1), sin(thetta1)}, Vec3D{0, 0, 0})); // down plane
    _clipPlanes.emplace_back(Plane(Vec3D{0, -cos(thetta1), sin(thetta1)}, Vec3D{0, 0, 0})); // up plane

    _ready = true;
    Log::log("Camera::init(): camera successfully initialized.");
}

std::vector<std::shared_ptr<Triangle>> Camera::sorted() {

    // Sort _tris from _back to front
    // This is some replacement for Z-buffer
    std::sort(_triangles.begin(), _triangles.end(), [](std::shared_ptr<Triangle> &t1, std::shared_ptr<Triangle> &t2)
    {
        std::vector<double> v_z1({(*t1)[0].z(), (*t1)[1].z(), (*t1)[2].z()});
        std::vector<double> v_z2({(*t2)[0].z(), (*t2)[1].z(), (*t2)[2].z()});

        std::sort(v_z1.begin(), v_z1.end());
        std::sort(v_z2.begin(), v_z2.end());

        double z1 = v_z1[0] + v_z1[1] + v_z1[2];
        double z2 = v_z2[0] + v_z2[1] + v_z2[2];

        return z1 > z2;
    });

    return _triangles;
}

void Camera::clear() {
    // Cleaning all _tris and recalculation of View matrix
    // That is like preparation for new camera shot: we need to set
    // the position of camera and insert new cartridge for photo.
    _triangles.clear();
    _V = Matrix4x4::View(left(), up(), lookAt(), position());
}

// OpenGL function
GLfloat *Camera::view() const {
    auto* v = (GLfloat*)malloc(4*4*sizeof(GLfloat));

    v[0] = -(GLfloat)left().x();
    v[4] = -(GLfloat)left().y();
    v[8] = -(GLfloat)left().z();
    v[12] = (GLfloat)position().dot(left());

    v[1] = (GLfloat)up().x();
    v[5] = (GLfloat)up().y();
    v[9] = (GLfloat)up().z();
    v[13] = -(GLfloat)position().dot(up());

    v[2] = -(GLfloat)lookAt().x();
    v[6] = -(GLfloat)lookAt().y();
    v[10] = -(GLfloat)lookAt().z();
    v[14] = (GLfloat)position().dot(lookAt());

    v[3] = (GLfloat)0.0f;
    v[7] = (GLfloat)0.0f;
    v[11] = (GLfloat)0.0f;
    v[15] = (GLfloat)1.0f;

    return v;
}
