//
// Created by Иван Ильин on 13.01.2021.
//

#include <utility>

#include "Mesh.h"
#include "ResourceManager.h"
#include "Screen.h"

using namespace std;

Mesh &Mesh::operator*=(const Matrix4x4 &matrix4X4) {
    std::vector<Triangle> newTriangles;
    newTriangles.reserve(_tris.size());
    for (auto &t : _tris) {
        newTriangles.emplace_back(t * matrix4X4);
    }
    setTriangles(std::move(newTriangles));

    return *this;
}

void Mesh::loadObj(const std::string &filename, const Vec3D &scale) {

    auto objects = ResourceManager::loadObjects(filename);
    for (auto &obj : objects) {
        for (auto &tri : obj->triangles()) {
            _tris.push_back(tri);
        }
    }
    this->scale(scale);
}

Mesh::Mesh(ObjectNameTag nameTag, const std::string &filename, const Vec3D &scale) : Object(std::move(nameTag)) {
    loadObj(filename, scale);
}

Mesh::Mesh(ObjectNameTag nameTag, const vector<Triangle> &tries) : Object(std::move(nameTag)), _tris(tries) {
}

Mesh Mesh::Obj(ObjectNameTag nameTag, const std::string &filename) {
    return Mesh(std::move(nameTag), filename);
}

void Mesh::setColor(const sf::Color &c) {
    _color = c;

    // change color of all mesh triangles:
    std::vector<Triangle> newTriangles;
    newTriangles.reserve(_tris.size());
    for (auto &t : _tris) {
        newTriangles.emplace_back(Triangle(t[0], t[1], t[2], c));
    }
    setTriangles(std::move(newTriangles));
}

Mesh
Mesh::LineTo(ObjectNameTag nameTag, const Vec3D &from, const Vec3D &to, double line_width, const sf::Color &color) {

    Mesh line(std::move(nameTag));

    Vec3D v1 = (to - from).normalized();
    Vec3D v2 = from.cross(from + Vec3D{1, 0, 0}).normalized();
    Vec3D v3 = v1.cross(v2).normalized();

    // from plane
    Vec4D p1 = (from - v2 * line_width / 2.0 - v3 * line_width / 2.0).makePoint4D();
    Vec4D p2 = (from - v2 * line_width / 2.0 + v3 * line_width / 2.0).makePoint4D();
    Vec4D p3 = (from + v2 * line_width / 2.0 + v3 * line_width / 2.0).makePoint4D();
    Vec4D p4 = (from + v2 * line_width / 2.0 - v3 * line_width / 2.0).makePoint4D();
    // to plane
    Vec4D p5 = (to - v2 * line_width / 2.0 - v3 * line_width / 2.0).makePoint4D();
    Vec4D p6 = (to - v2 * line_width / 2.0 + v3 * line_width / 2.0).makePoint4D();
    Vec4D p7 = (to + v2 * line_width / 2.0 + v3 * line_width / 2.0).makePoint4D();
    Vec4D p8 = (to + v2 * line_width / 2.0 - v3 * line_width / 2.0).makePoint4D();


    line._tris = std::move(std::vector<Triangle>{
            {p2, p4, p1},
            {p2, p3, p4},
            {p1, p6, p2},
            {p1, p5, p6},
            {p2, p6, p7},
            {p2, p7, p3},
            {p6, p5, p8},
            {p6, p8, p7},
            {p4, p3, p7},
            {p4, p7, p8},
            {p1, p8, p5},
            {p1, p4, p8}
    });
    line.setColor(color);

    return line;
}

void Mesh::setTriangles(const vector<Triangle> &t) {
    _tris.clear();
    for (auto &tri : t) {
        _tris.push_back(tri);
    }
}

void Mesh::setTriangles(vector<Triangle>&& t) {
    _tris = std::move(t);
}

Mesh::~Mesh() {
    _tris.clear();
    delete[] _geometry;
}

GLfloat *Mesh::glFloatArray() const {
    if(_geometry != nullptr) {
        return _geometry;
    }
    _geometry = new GLfloat[7 * 3 * _tris.size()];

    for (size_t i = 0; i < _tris.size(); i++) {

        unsigned stride = 21 * i;

        Triangle triangle = _tris[i];
        Vec3D norm = (model()*triangle.norm()).normalized();
        float dot = static_cast<float>(norm.dot(Vec3D(0, 1, 2).normalized()));

        for (int k = 0; k < 3; k++) {
            sf::Color color = triangle.color();
            GLfloat ambientColor[4] = {
                    static_cast<float>(color.r) * (0.3f * std::fabs(dot) + 0.7f) / 255.0f,
                    static_cast<float>(color.g) * (0.3f * std::fabs(dot) + 0.7f) / 255.0f,
                    static_cast<float>(color.b) * (0.3f * std::fabs(dot) + 0.7f) / 255.0f,
                    static_cast<float>(color.a) / 255.0f
            };

            _geometry[stride + 7 * k + 0] = static_cast<GLfloat>(triangle[k].x());
            _geometry[stride + 7 * k + 1] = static_cast<GLfloat>(triangle[k].y());
            _geometry[stride + 7 * k + 2] = static_cast<GLfloat>(triangle[k].z());

            _geometry[stride + 7 * k + 3] = ambientColor[0];
            _geometry[stride + 7 * k + 4] = ambientColor[1];
            _geometry[stride + 7 * k + 5] = ambientColor[2];
            _geometry[stride + 7 * k + 6] = ambientColor[3];
        }
    }

    return _geometry;
}
