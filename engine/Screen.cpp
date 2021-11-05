//
// Created by Иван Ильин on 14.01.2021.
//

#include <utility>
#include <cmath>

#include <SFML/OpenGL.hpp>

#include "Screen.h"
#include "utils/Time.h"
#include "utils/Log.h"
#include "ResourceManager.h"

void Screen::open(int screenWidth, int screenHeight, const std::string &name, bool verticalSync, sf::Color background,
                  sf::Uint32 style) {
    _title = name;
    _background = background;

    sf::ContextSettings settings;
    settings.depthBits = 12;
    settings.antialiasingLevel = 1;

    _window->create(sf::VideoMode(screenWidth, screenHeight), name, style, settings);
    _window->setVerticalSyncEnabled(verticalSync);
}

void Screen::display() {
    sf::Event event{};
    while (_window->pollEvent(event)) {
        if (event.type == sf::Event::Closed) {
            _window->close();
        }
    }

    std::string title = _title + " (" + std::to_string(Time::fps()) + " fps)";
    _window->setTitle(title);

    _window->display();
}

void Screen::clear() {
    // Clear the depth buffer
    glClear(GL_DEPTH_BUFFER_BIT | GL_COLOR_BUFFER_BIT);

    _window->clear(_background);
}

void Screen::drawTriangle(const Triangle &triangle) {
    sf::Vertex tris[3] =
            {
                    sf::Vertex(sf::Vector2f(static_cast<float>(triangle[0].x()), static_cast<float>(triangle[0].y())),
                               triangle.color()),
                    sf::Vertex(sf::Vector2f(static_cast<float>(triangle[1].x()), static_cast<float>(triangle[1].y())),
                               triangle.color()),
                    sf::Vertex(sf::Vector2f(static_cast<float>(triangle[2].x()), static_cast<float>(triangle[2].y())),
                               triangle.color())
            };

    _window->draw(tris, 3, sf::Triangles);
}

void Screen::setTitle(const std::string &title) {
    _title = title;
}

bool Screen::isOpen() {
    return _window->isOpen();
}

void Screen::close() {
    _window->close();
}

void Screen::setMouseCursorVisible(bool visible) {
    _window->setMouseCursorVisible(visible);
}

void Screen::drawTetragon(const Vec2D &p1, const Vec2D &p2, const Vec2D &p3, const Vec2D &p4, sf::Color color) {
    sf::ConvexShape polygon;
    polygon.setPointCount(4);
    polygon.setPoint(0, sf::Vector2f(static_cast<float>(p1.x()), static_cast<float>(p1.y())));
    polygon.setPoint(1, sf::Vector2f(static_cast<float>(p2.x()), static_cast<float>(p2.y())));
    polygon.setPoint(2, sf::Vector2f(static_cast<float>(p3.x()), static_cast<float>(p3.y())));
    polygon.setPoint(3, sf::Vector2f(static_cast<float>(p4.x()), static_cast<float>(p4.y())));
    polygon.setFillColor(color);

    _window->draw(polygon);
}

void Screen::drawText(const std::string &string, const Vec2D &position, int size, sf::Color color) {
    sf::Text text;

    text.setFont(*ResourceManager::loadFont(Consts::MEDIUM_FONT));

    text.setCharacterSize(size);
    text.setFillColor(color);
    text.setStyle(sf::Text::Italic);
    text.setPosition(static_cast<float>(position.x()), static_cast<float>(position.y()));

    text.setString(string);

    _window->draw(text);
}

void Screen::drawSprite(const sf::Sprite &sprite) {
    _window->draw(sprite);
}

void Screen::drawText(const sf::Text &text) {
    _window->draw(text);
}

// OpenGL functions
void Screen::glDrawMesh(GLfloat *geometry, GLfloat *view, GLfloat *model, size_t count) {

    if (!sf::Shader::isAvailable())
    {
        Log::log("Shaders are not available!");
    }

    glEnable(GL_CULL_FACE); // enable culling face
    glCullFace(GL_BACK); // cull faces from back
    glFrontFace(GL_CCW); // vertex order (counter clock wise)

    // Enable Z-buffer read and write
    glEnable(GL_DEPTH_TEST);
    glDepthMask(GL_TRUE);
    glClearDepth(1.f);

    // Disable lighting
    glDisable(GL_LIGHTING);

    // enable alpha channel:
    glEnable(GL_BLEND);
    glBlendFunc(GL_SRC_ALPHA, GL_ONE_MINUS_SRC_ALPHA);

    // Configure the viewport (the same size as the window)
    glViewport(0, 0, _window->getSize().x, _window->getSize().y);

    // Setup a perspective projection
    glMatrixMode(GL_PROJECTION);
    glLoadIdentity();
    GLfloat ratio = static_cast<float>(_window->getSize().x) / _window->getSize().y;
    glFrustum(-ratio, ratio, -1.f, 1.f, 1.0f, 500.f);

    // Enable position and texture coordinates vertex components
    glEnableClientState(GL_VERTEX_ARRAY);
    glEnableClientState(GL_COLOR_ARRAY);
    glVertexPointer(3, GL_FLOAT, 7 * sizeof(GLfloat), geometry);
    glColorPointer(4, GL_FLOAT, 7 * sizeof(GLfloat), geometry + 3);

    // Disable normal and color vertex components
    glDisableClientState(GL_NORMAL_ARRAY);
    glDisableClientState(GL_TEXTURE_COORD_ARRAY);

    // Apply some transformations
    glMatrixMode(GL_MODELVIEW);
    glLoadIdentity();

    glLoadMatrixf(view);
    glMultMatrixf(model);

    // Draw the mesh
    glDrawArrays(GL_TRIANGLES, 0, count);

    sf::Shader::bind(NULL);
}

GLfloat *Screen::glMeshToGLfloatArray(std::shared_ptr<Mesh> mesh) {
    std::vector<Triangle> const &triangles = mesh->triangles();

    auto *geometry = new GLfloat[7 * 3 * triangles.size()];

    for (size_t i = 0; i < triangles.size(); i++) {

        int stride = 21 * i;

        Triangle triangle = triangles[i];

        for (int k = 0; k < 3; k++) {
            float dot = 0.5;

            sf::Color color = triangle.color();
            GLfloat ambientColor[4] = {
                color.r * (0.3f * std::fabs(dot) + 0.7f) / 255.0f,
                color.g * (0.3f * std::fabs(dot) + 0.7f) / 255.0f,
                color.b * (0.3f * std::fabs(dot) + 0.7f) / 255.0f,
                color.a / 255.0f
            };

            geometry[stride + 7 * k + 0] = static_cast<GLfloat>(triangle[k].x());
            geometry[stride + 7 * k + 1] = static_cast<GLfloat>(triangle[k].y());
            geometry[stride + 7 * k + 2] = static_cast<GLfloat>(triangle[k].z());

            geometry[stride + 7 * k + 3] = ambientColor[0];
            geometry[stride + 7 * k + 4] = ambientColor[1];
            geometry[stride + 7 * k + 5] = ambientColor[2];
            geometry[stride + 7 * k + 6] = ambientColor[3];
        }
    }
    return geometry;
}
