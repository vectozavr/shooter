//
// Created by Иван Ильин on 13.01.2021.
//

#include <string>
#include <fstream>
#include <sstream>
#include <utility>
#include "Mesh.h"
#include "utils/Log.h"

using namespace std;

Mesh &Mesh::operator*=(const Matrix4x4 &matrix4X4) {
    std::vector<Triangle> newTriangles;
    for(auto &t : _tris) {
        newTriangles.emplace_back(t * matrix4X4);
    }
    setTriangles(newTriangles);

    return *this;
}

Mesh &Mesh::loadObj(const std::string& filename, const Vec3D& scale) {

    auto objects = Mesh::LoadObjects(filename, scale);
    for(auto& obj : objects) {
        for (auto &tri : obj->triangles()) {
            _tris.push_back(tri);
        }
    }
    return *this;
}

Mesh::Mesh(const std::string& filename, const Vec3D& scale){
    loadObj(filename, scale);
}

Mesh::Mesh(const vector<Triangle> &tries) : _tris(tries) {
}

Mesh Mesh::Obj(const std::string& filename) {
    return Mesh(filename);
}

void Mesh::rotate(const Vec3D &r) {
    Object::rotate(r);
    *this *= Matrix4x4::Rotation(r);
}

void Mesh::rotate(const Vec3D &v, double r) {
    Object::rotate(v, r);
    *this *= Matrix4x4::Rotation(v, r);
}

void Mesh::scale(const Vec3D &s) {
    Object::scale(s);
    *this *= Matrix4x4::Scale(s);
}

void Mesh::rotateRelativePoint(const Vec3D &s, const Vec3D &r) {
    Object::rotateRelativePoint(s, r);
    *this *= Matrix4x4::Rotation(r);
}

void Mesh::rotateRelativePoint(const Vec3D &s, const Vec3D &v, double r) {
    Object::rotateRelativePoint(s, v, r);
    *this *= Matrix4x4::Rotation(v, r);
}

void Mesh::setColor(const sf::Color& c) {
    _color = c;

    // change color of all mesh triangles:
    std::vector<Triangle> newTriangles;
    for(auto &t : _tris) {
        newTriangles.emplace_back(Triangle(t[0], t[1], t[2], c));
    }
    setTriangles(newTriangles);
}

std::vector<std::shared_ptr<Mesh>> Mesh::LoadObjects(const string &filename, const Vec3D &scale) {
    std::vector<std::shared_ptr<Mesh>> objects;
    map<string, sf::Color> maters;

    ifstream file(filename);
    if (!file.is_open())
    {
        Log::log("Mesh::LoadObjects(): cannot load file from " + filename);
        return objects;
    }

    vector<Point4D> verts;
    std::vector<Triangle> tris;
    sf::Color currentColor = sf::Color(255, 245, 194, 255);

    while (!file.eof())
    {
        char line[128];
        file.getline(line, 128);

        stringstream s;
        s << line;

        char junk;
        if(line[0] == 'o') {
            if(!tris.empty()) {
                objects.push_back(make_shared<Mesh>(tris));
                objects.back()->scale(scale);
            }
            tris.clear();
        }
        if (line[0] == 'v')
        {
            double x, y, z;
            s >> junk >> x >> y >> z;
            verts.emplace_back(x, y, z, 1);
        }
        if(line[0] == 'g') {
            string matInfo;
            s >> junk >> matInfo;
            string colorName = matInfo.substr(matInfo.size()-3, 3);
            currentColor = maters[matInfo.substr(matInfo.size()-3, 3)];
        }
        if (line[0] == 'f')
        {
            int f[3];
            s >> junk >> f[0] >> f[1] >> f[2];
            tris.emplace_back(verts[f[0] - 1], verts[f[1] - 1], verts[f[2] - 1], currentColor);
        }
        if(line[0] == 'm')
        {
            int color[4];
            string matName;

            s >> junk >> matName >> color[0] >> color[1] >> color[2] >> color[3];
            maters.insert({matName, sf::Color(color[0],color[1],color[2], color[3])});
        }
    }

    if(!tris.empty()) {
        objects.push_back(make_shared<Mesh>(tris));
        objects.back()->scale(scale);
    }

    file.close();

    return objects;
}

Mesh Mesh::LineTo(const Vec3D& from, const Vec3D& to, double line_width, const sf::Color& color) {

    Mesh line;

    Vec3D v1 = (to - from).normalized();
    Vec3D v2 = from.cross(from + Vec3D{1, 0, 0}).normalized();
    Vec3D v3 = v1.cross(v2).normalized();

    // from plane
    Point4D p1 = (from - v2 * line_width/2.0 - v3 * line_width/2.0).makePoint4D();
    Point4D p2 = (from - v2 * line_width/2.0 + v3 * line_width/2.0).makePoint4D();
    Point4D p3 = (from + v2 * line_width/2.0 + v3 * line_width/2.0).makePoint4D();
    Point4D p4 = (from + v2 * line_width/2.0 - v3 * line_width/2.0).makePoint4D();
    // to plane
    Point4D p5 = (to - v2 * line_width/2.0 - v3 * line_width/2.0).makePoint4D();
    Point4D p6 = (to - v2 * line_width/2.0 + v3 * line_width/2.0).makePoint4D();
    Point4D p7 = (to + v2 * line_width/2.0 + v3 * line_width/2.0).makePoint4D();
    Point4D p8 = (to + v2 * line_width/2.0 - v3 * line_width/2.0).makePoint4D();


    line._tris = std::move(std::vector<Triangle>{
            { p2, p4, p1 },
            { p2, p3, p4 },
            { p1, p6, p2 },
            { p1, p5, p6 },
            { p2, p6, p7 },
            { p2, p7, p3 },
            { p6, p5, p8 },
            { p6, p8, p7 },
            { p4, p3, p7 },
            { p4, p7, p8 },
            { p1, p8, p5 },
            { p1, p4, p8 }
    });
    line.setColor(color);

    return line;
}

Mesh::Mesh(const Mesh &mesh) : _tris(mesh._tris), _color(mesh._color), _visible(mesh._visible) {

}

void Mesh::setTriangles(const vector<Triangle> &t) {
    _tris.clear();
    for (auto & tri : t) {
        _tris.push_back(tri);
    }
}

Mesh::~Mesh() {
    _tris.clear();
}
