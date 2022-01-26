#include "lab_m1/Tema1/object2D.h"

#include <vector>
#include <iostream>

#include "core/engine.h"
#include "utils/gl_utils.h"


Mesh* object2D::CreateSquare(
    const std::string &name,
    glm::vec3 leftBottomCorner,
    float length,
    glm::vec3 color,
    bool fill)
{
    glm::vec3 corner = leftBottomCorner;

    std::vector<VertexFormat> vertices =
    {
        VertexFormat(corner, color),
        VertexFormat(corner + glm::vec3(length, 0, 0), color),
        VertexFormat(corner + glm::vec3(length, length, 0), color),
        VertexFormat(corner + glm::vec3(0, length, 0), color)
    };

    Mesh* square = new Mesh(name);
    std::vector<unsigned int> indices = { 0, 1, 2, 3 };

    if (!fill) {
        square->SetDrawMode(GL_LINE_LOOP);
    } else {
        // Draw 2 triangles. Add the remaining 2 indices
        indices.push_back(0);
        indices.push_back(2);
    }

    square->InitFromData(vertices, indices);
    return square;
}

Mesh* object2D::CreateCircle(
    const std::string& name,
    unsigned int nrOfVertices,
    float radius,
    glm::vec3 color,
    bool fill)
{
    float theta = 2.0f * M_PI / nrOfVertices;
    nrOfVertices += 2;

    Mesh* circle = new Mesh(name);
    std::vector<VertexFormat> vertices;
    std::vector<unsigned int> indices;
    
    float x, y;
    x = y = 0;

    vertices.push_back(VertexFormat(glm::vec3(0, 0, 0), color));
    indices.push_back(0);

    for (int i = 1; i < nrOfVertices ; i++) {
        indices.push_back(i);
        x = radius * cos(i * theta);
        y = radius * sin(i * theta);
        vertices.push_back(VertexFormat(glm::vec3(x, y, 0), color));
    }

   
    if (!fill) {
        circle->SetDrawMode(GL_LINE_LOOP);
    }
    else {
        // Draw 2 triangles. Add the remaining 2 indices
       circle->SetDrawMode(GL_TRIANGLE_FAN);
    }

    circle->InitFromData(vertices, indices);
    return circle;
}

Mesh* object2D::CreateTriangle(
    const std::string& name,
    glm::vec3 leftBottomCorner,
    float length,
    glm::vec3 color,
    bool fill)
{
    glm::vec3 corner = leftBottomCorner;

    std::vector<VertexFormat> vertices =
    {
        VertexFormat(glm::vec3(0, 0, 0), color),
        VertexFormat(glm::vec3(length / 2, length, 0), color),
        VertexFormat(glm::vec3(length, 0, 0), color),
    };

    Mesh* triangle = new Mesh(name);
    std::vector<unsigned int> indices = { 0, 1, 2 };

    if (!fill) {
        triangle->SetDrawMode(GL_LINE_LOOP);
    }
    else {
        triangle->SetDrawMode(GL_TRIANGLE_FAN);
    }

    triangle->InitFromData(vertices, indices);
    return triangle;
}
