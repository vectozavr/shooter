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

Mesh Mesh::operator*(const Matrix4x4 &matrix4X4) const {
    return Mesh(*this) *= matrix4X4;
}

Mesh &Mesh::operator*=(const Matrix4x4 &matrix4X4) {
    for (auto& t : tris)
        t *= matrix4X4;

    return *this;
}

Mesh &Mesh::loadObj(const std::string& filename, const std::string &materials, const Point4D& scale) {

    auto objects = Mesh::LoadObjects(filename, materials, scale);
    for(auto& obj : objects) {
        for (auto &tri : obj->triangles()) {
            tris.push_back(tri);
        }
    }
    return *this;
}

Mesh::Mesh(const std::string& filename, const std::string &materials, const Point4D& scale){
    loadObj(filename, materials, scale);
}

Mesh::Mesh(const vector<Triangle> &tries){
    tris = tries;
}

Mesh::Mesh(const Mesh& mesh) : Animatable(mesh) {
    *this = mesh;
}

Mesh Mesh::Obj(const std::string& filename) {
    return Mesh(filename);
}

void Mesh::rotate(const Point4D &r) {
    p_angle = p_angle + r;
    *this *= Matrix4x4::Rotation(r);

    for(auto attached : v_attached)
        attached->rotateRelativePoint(position(), r);
}

void Mesh::rotate(const Point4D &v, double r) {
    *this *= Matrix4x4::Rotation(v, r);

    for(auto attached : v_attached)
        attached->rotateRelativePoint(position(), v, r);
}

void Mesh::scale(const Point4D &s) {
    *this *= Matrix4x4::Scale(s);

    // TODO: scale attached objects
}

void Mesh::translate(const Point4D &t) {
    p_position = p_position + t;

    for(auto attached : v_attached)
        attached->translate(t);
}

Mesh &Mesh::operator=(const Mesh &mesh) {
    tris = mesh.tris;
    p_position = mesh.p_position;
    c_color = mesh.c_color;
    return *this;
}

void Mesh::rotateRelativePoint(const Point4D &s, const Point4D &r) {
    p_angle = p_angle + r;

    // Translate XYZ by vector r1
    Point4D r1 = p_position - s;
    *this *= Matrix4x4::Translation(r1);

    // In translated coordinate system we rotate mesh and position
    Matrix4x4 rotationMatrix = Matrix4x4::Rotation(r);
    Point4D r2 = rotationMatrix*r1;
    *this *= rotationMatrix;

    // After rotation we translate XYZ by vector -r2 and recalculate position
    *this *= Matrix4x4::Translation(-r2);
    p_position = s + r2;

    if(v_attached.empty())
        return;
    for(auto attached : v_attached)
        attached->rotateRelativePoint(s, r);
}

void Mesh::rotateRelativePoint(const Point4D &s, const Point4D &v, double r) {
    // Translate XYZ by vector r1
    Point4D r1 = p_position - s;
    *this *= Matrix4x4::Translation(r1);

    // In translated coordinate system we rotate mesh and position
    Matrix4x4 rotationMatrix = Matrix4x4::Rotation(v, r);
    Point4D r2 = rotationMatrix*r1;
    *this *= rotationMatrix;

    // After rotation we translate XYZ by vector -r2 and recalculate position
    *this *= Matrix4x4::Translation(-r2);
    p_position = s + r2;

    for(auto attached : v_attached)
        attached->rotateRelativePoint(s, v, r);
}


void Mesh::translateToPoint(const Point4D &point) {
    translate(point - p_position);
}

void Mesh::setColor(sf::Color c) {
    c_color = c;
    for (auto& t : tris)
        t.color = c_color;
}

std::vector<std::shared_ptr<Mesh>>
Mesh::LoadObjects(const string &filename, const string &materials, const Point4D &scale) {
    std::vector<std::shared_ptr<Mesh>> objects;
    map<string, sf::Color> maters;

    ifstream file(filename);
    if (!file.is_open())
    {
        Log::log("Mesh::LoadObjects(): cannot load file from " + filename);
        return objects;
    }

    if(!materials.empty()) {
        ifstream mat(materials);

        if (!mat.is_open())
        {
            Log::log("Mesh::LoadObjects(): cannot load mat from " + materials);
            return objects;
        } else {
            while (!mat.eof())
            {
                char line[128];
                mat.getline(line, 128);

                stringstream s;
                s << line;

                int color[4];
                string matName;

                s >> matName >> color[0] >> color[1] >> color[2] >> color[3];
                maters.insert({matName, sf::Color(color[0],color[1],color[2], color[3])});
            }
            mat.close();
        }
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
            tris.emplace_back(verts[f[0] - 1], verts[f[1] - 1], verts[f[2] - 1] );
            tris.back().color = currentColor;
        }
    }

    if(!tris.empty()) {
        objects.push_back(make_shared<Mesh>(tris));
        objects.back()->scale(scale);
    }

    file.close();

    return objects;
}

Mesh Mesh::LineTo(const Point4D& from, const Point4D& to, double line_width, sf::Color color) {
    Mesh line;

    Point4D v1 = (to - from).normalized();
    Point4D v2 = from.cross3D(from + Point4D{1, 0, 0}).normalized();
    Point4D v3 = v1.cross3D(v2).normalized();

    // from plane
    Point4D p1 = from - v2 * line_width/2.0 - v3 * line_width/2.0;
    Point4D p2 = from - v2 * line_width/2.0 + v3 * line_width/2.0;
    Point4D p3 = from + v2 * line_width/2.0 + v3 * line_width/2.0;
    Point4D p4 = from + v2 * line_width/2.0 - v3 * line_width/2.0;
    // to plane
    Point4D p5 = to - v2 * line_width/2.0 - v3 * line_width/2.0;
    Point4D p6 = to - v2 * line_width/2.0 + v3 * line_width/2.0;
    Point4D p7 = to + v2 * line_width/2.0 + v3 * line_width/2.0;
    Point4D p8 = to + v2 * line_width/2.0 - v3 * line_width/2.0;

    line.tris = {
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
    };

    line.setColor(color);

    return line;
}
