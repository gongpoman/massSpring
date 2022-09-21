#include<iostream>

#include<glad/glad.h>
#include<GLFW/glfw3.h>

#include<glm/glm.hpp>

#include"component.h"
#include"msSystem.h"
#include"shader.h"

#include<pmp/SurfaceMesh.h>
#include<pmp/algorithms/Subdivision.h>


void __project_to_unit_sphere(pmp::SurfaceMesh& mesh);
pmp::SurfaceMesh __icosahedron();
pmp::SurfaceMesh __icosphere(size_t n_subdivisions);

Component::Component()
{
}

Component::~Component()
{
}


FixedPoint::FixedPoint()
{
}

FixedPoint::~FixedPoint()
{
}


Spring::Spring()
{
}

Spring::~Spring()
{
}


Ball::Ball()
{
}

Ball::~Ball()
{
}


void FixedPoint::render() {

}

void FixedPoint::update() {

}
void FixedPoint::drawSetup() {
	shaderID = new Shader("resources/shader/box_vs.txt", "resources/shader/box_fs.txt");


}


void Spring::render() {

}

void Spring::update() {

}
void Spring::drawSetup() {

}

void Ball::drawSetup() {

    pmp::SurfaceMesh sphere = __icosphere(1);

    std::vector<float> vertices = std::vector<float>();
    std::vector<int> indices = std::vector<int>();

    for (auto f : sphere.faces()) {
        std::cout << f.idx() << "th face idx list : ";
        for (auto v : sphere.vertices(f)) {
            auto p = sphere.position(v);
            std::cout << v.idx() << " , ";
        }
        std::cout << std::endl;
    }

    for (auto f : sphere.faces()) {
        std::cout << f.idx() << "th face idx list : ";
        for (auto v : sphere.vertices(f)) {
            auto p = sphere.position(v);
            vertices.push_back(p[0]);
            vertices.push_back(p[1]);
            vertices.push_back(p[2]);
            indices.push_back(v.idx());
            std::cout << v.idx() << " , ";
            std::cout << " , position :    ";
            std::cout << p[0] << " , ";
            std::cout << p[1] << " , ";
            std::cout << p[2] << " , ";
        }
        std::cout << std::endl;
    }
}

void Ball::render() {

}

void Ball::update() {

}







void __project_to_unit_sphere(pmp::SurfaceMesh& mesh)
{
    using namespace pmp;
    for (auto v : mesh.vertices()) {
        auto p = mesh.position(v);
        auto n = norm(p);
        mesh.position(v) = (1.0 / n) * p;
    }
}
pmp::SurfaceMesh __icosahedron()
{
    pmp::SurfaceMesh mesh;

    float phi = (1.0f + sqrt(5.0f)) * 0.5f; // golden ratio
    float a = 1.0f;
    float b = 1.0f / phi;

    // add vertices
    auto v1 = mesh.add_vertex(pmp::Point(0, b, -a));
    auto v2 = mesh.add_vertex(pmp::Point(b, a, 0));
    auto v3 = mesh.add_vertex(pmp::Point(-b, a, 0));
    auto v4 = mesh.add_vertex(pmp::Point(0, b, a));
    auto v5 = mesh.add_vertex(pmp::Point(0, -b, a));
    auto v6 = mesh.add_vertex(pmp::Point(-a, 0, b));
    auto v7 = mesh.add_vertex(pmp::Point(0, -b, -a));
    auto v8 = mesh.add_vertex(pmp::Point(a, 0, -b));
    auto v9 = mesh.add_vertex(pmp::Point(a, 0, b));
    auto v10 = mesh.add_vertex(pmp::Point(-a, 0, -b));
    auto v11 = mesh.add_vertex(pmp::Point(b, -a, 0));
    auto v12 = mesh.add_vertex(pmp::Point(-b, -a, 0));

    __project_to_unit_sphere(mesh);

    // add triangles
    mesh.add_triangle(v3, v2, v1);
    mesh.add_triangle(v2, v3, v4);
    mesh.add_triangle(v6, v5, v4);
    mesh.add_triangle(v5, v9, v4);
    mesh.add_triangle(v8, v7, v1);
    mesh.add_triangle(v7, v10, v1);
    mesh.add_triangle(v12, v11, v5);
    mesh.add_triangle(v11, v12, v7);
    mesh.add_triangle(v10, v6, v3);
    mesh.add_triangle(v6, v10, v12);
    mesh.add_triangle(v9, v8, v2);
    mesh.add_triangle(v8, v9, v11);
    mesh.add_triangle(v3, v6, v4);
    mesh.add_triangle(v9, v2, v4);
    mesh.add_triangle(v10, v3, v1);
    mesh.add_triangle(v2, v8, v1);
    mesh.add_triangle(v12, v10, v7);
    mesh.add_triangle(v8, v11, v7);
    mesh.add_triangle(v6, v12, v5);
    mesh.add_triangle(v11, v9, v5);

    return mesh;
}
pmp::SurfaceMesh __icosphere(size_t n_subdivisions)
{
    auto mesh = __icosahedron();
    pmp::Subdivision subdivision(mesh);
    for (size_t i = 0; i < n_subdivisions; i++)
    {
        subdivision.loop();
        __project_to_unit_sphere(mesh);
    }
    return mesh;
}

void __temp_PMPTestMain() {

    pmp::SurfaceMesh sphere = __icosphere(1);

    std::vector<float> vertices = std::vector<float>();
    std::vector<int> indices = std::vector<int>();

    for (auto f : sphere.faces()) {
        std::cout << f.idx() << "th face idx list : ";
        for (auto v : sphere.vertices(f)) {
            auto p = sphere.position(v);
            std::cout << v.idx() << " , ";
        }
        std::cout << std::endl;
    }

    for (auto f : sphere.faces()) {
        std::cout << f.idx() << "th face idx list : ";
        for (auto v : sphere.vertices(f)) {
            auto p = sphere.position(v);
            vertices.push_back(p[0]);
            vertices.push_back(p[1]);
            vertices.push_back(p[2]);
            indices.push_back(v.idx());
            std::cout << v.idx() << " , ";
            std::cout << " , position :    ";
            std::cout << p[0] << " , ";
            std::cout << p[1] << " , ";
            std::cout << p[2] << " , ";
        }
        std::cout << std::endl;
    }

}
