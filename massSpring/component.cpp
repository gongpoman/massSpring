#include<iostream>

#include<glad/glad.h>
#include<GLFW/glfw3.h>

#include<glm/glm.hpp>

#include"component.h"
#include"msSystem.h"
#include"shader.h"
#include"camera.h"
#include"joint.h"

#include<pmp/SurfaceMesh.h>
#include<pmp/algorithms/Subdivision.h>


extern Camera cam;

extern const int SCR_WIDTH;
extern const int SCR_HEIGHT;

extern float deltaTime;

void __project_to_unit_sphere(pmp::SurfaceMesh& mesh);
pmp::SurfaceMesh __icosahedron();
pmp::SurfaceMesh __icosphere(size_t n_subdivisions);

Component::Component()
{
    std::cout << "Comp!!!" << std::endl;

}

Component::~Component()
{
    std::cout << "COMPO~" << std::endl;
}



FixedPoint::FixedPoint()
{
    __drawSetup();
}
FixedPoint::FixedPoint(glm::vec3 position)
{
    type = FP;
    pos = position;
    __drawSetup();
}

FixedPoint::~FixedPoint()
{
    delete shader;
    std::cout << "FP~" << std::endl;
}


SpringL::SpringL()
{
    __drawSetup();
}

SpringL::SpringL(glm::vec3 position1, glm::vec3 position2, float ela )
{
    type = SPRINGL;
    pos1 = position1;
    pos2 = position2;
    elasticity = ela;
    defaultLen = glm::length(pos1 - pos2);
    currentLen = defaultLen;
    __drawSetup();
}

SpringL::~SpringL()
{
    delete shader;
    std::cout << "SPR~" << std::endl;
}

Mass::Mass() {
    std::cout << "MASS!!" << std::endl;
    type = MASS;
}
Mass::~Mass() {
    std::cout << "Mass~" << std::endl;
}

glm::vec3 Mass::getPosition() {
    return pos;
}


Ball::Ball()
{
    __drawSetup();
}
Ball::Ball(glm::vec3 position, float r, float d)
{
    std::cout << "Ball Init...!!!" << std::endl;
    pos = position;
    vel = glm::vec3(0);
    acc = glm::vec3(0);

    netF = glm::vec3(0);
    netT = glm::vec3(0);

    radius = r;
    density = d;
    mass = 4 / 3 * M_PI * r * r * r;
    __drawSetup();
}

Ball::~Ball()
{
    delete shader;
    std::cout << "Ball~" << std::endl;
}

CompType Component::getType() {
    return type;
}


void FixedPoint::__drawSetup() {
    shader = new Shader("resources/shader/box_vs.txt", "resources/shader/box_fs.txt");

    float vertices[] = {
   -0.05f, -0.05f, -0.05f,
    0.05f, -0.05f, -0.05f,
    0.05f,  0.05f, -0.05f,
    0.05f,  0.05f, -0.05f,
   -0.05f,  0.05f, -0.05f,
   -0.05f, -0.05f, -0.05f,

   -0.05f, -0.05f,  0.05f,
    0.05f, -0.05f,  0.05f,
    0.05f,  0.05f,  0.05f,
    0.05f,  0.05f,  0.05f,
   -0.05f,  0.05f,  0.05f,
   -0.05f, -0.05f,  0.05f,

   -0.05f,  0.05f,  0.05f,
   -0.05f,  0.05f, -0.05f,
   -0.05f, -0.05f, -0.05f,
   -0.05f, -0.05f, -0.05f,
   -0.05f, -0.05f,  0.05f,
   -0.05f,  0.05f,  0.05f,

    0.05f,  0.05f,  0.05f,
    0.05f,  0.05f, -0.05f,
    0.05f, -0.05f, -0.05f,
    0.05f, -0.05f, -0.05f,
    0.05f, -0.05f,  0.05f,
    0.05f,  0.05f,  0.05f,

   -0.05f, -0.05f, -0.05f,
    0.05f, -0.05f, -0.05f,
    0.05f, -0.05f,  0.05f,
    0.05f, -0.05f,  0.05f,
   -0.05f, -0.05f,  0.05f,
   -0.05f, -0.05f, -0.05f,

   -0.05f,  0.05f, -0.05f,
    0.05f,  0.05f, -0.05f,
    0.05f,  0.05f,  0.05f,
    0.05f,  0.05f,  0.05f,
   -0.05f,  0.05f,  0.05f,
   -0.05f,  0.05f, -0.05f,
    };
    unsigned int VBO;

    glGenBuffers(1, &VBO);
    glGenVertexArrays(1, &VAO);

    glBindVertexArray(VAO);
    glBindBuffer(GL_ARRAY_BUFFER, VBO);

    glBufferData(GL_ARRAY_BUFFER, sizeof(vertices), vertices, GL_STATIC_DRAW);

    glEnableVertexAttribArray(0);
    glVertexAttribPointer(0, 3, GL_FLOAT, GL_FALSE, 3 * sizeof(float), (void*)0);

    glBindVertexArray(0);
    glBindBuffer(GL_ARRAY_BUFFER, 0);
}

void FixedPoint::render() {
    shader->use();
    glBindVertexArray(VAO);

    glm::mat4 worldMat = glm::mat4(1.0f);
    worldMat = glm::translate(worldMat, pos);

    glm::mat4 viewMat = cam.getViewMatrix();
    glm::mat4 projMat = glm::perspective(glm::radians(cam.zoom), (float)SCR_WIDTH / (float)SCR_HEIGHT, 0.1f, 100.0f);

    glUniformMatrix4fv(glGetUniformLocation(shader->ID, "worldMat"), 1, GL_FALSE, &worldMat[0][0]);
    glUniformMatrix4fv(glGetUniformLocation(shader->ID, "viewMat"), 1, GL_FALSE, &viewMat[0][0]);
    glUniformMatrix4fv(glGetUniformLocation(shader->ID, "projMat"), 1, GL_FALSE, &projMat[0][0]);

    glDrawArrays(GL_TRIANGLES, 0, 36);
    glBindVertexArray(0);
}

void FixedPoint::update() {

}

glm::vec3 FixedPoint::getPosition() {
    return pos;
}

void SpringL::__drawSetup() {

    shader = new Shader("resources/shader/springL_vs.txt", "resources/shader/springL_fs.txt");

    float vertices[] = {
        pos1[0],pos1[1],pos1[2],
        pos2[0],pos2[1],pos2[2],
    };

    unsigned int VBO;

    glGenBuffers(1, &VBO);
    glGenVertexArrays(1,&VAO);

    glBindVertexArray(VAO);
    glBindBuffer(GL_ARRAY_BUFFER, VBO); 

    glBufferData(GL_ARRAY_BUFFER, sizeof(vertices), vertices, GL_DYNAMIC_DRAW);

    glEnableVertexAttribArray(0);
    glVertexAttribPointer(0, 3, GL_FLOAT, GL_FALSE, 3 * sizeof(float), (void*)0);

    glBindBuffer(GL_ARRAY_BUFFER, 0);
    glBindVertexArray(0);

}

void SpringL::render() {
    shader->use();
    glBindVertexArray(VAO);

    glm::mat4 worldMat = glm::mat4(1.0f);

    glm::mat4 viewMat = cam.getViewMatrix();
    glm::mat4 projMat = glm::perspective(glm::radians(cam.zoom), (float)SCR_WIDTH / (float)SCR_HEIGHT, 0.1f, 100.0f);

    glUniformMatrix4fv(glGetUniformLocation(shader->ID, "worldMat"), 1, GL_FALSE, &worldMat[0][0]);
    glUniformMatrix4fv(glGetUniformLocation(shader->ID, "viewMat"), 1, GL_FALSE, &viewMat[0][0]);
    glUniformMatrix4fv(glGetUniformLocation(shader->ID, "projMat"), 1, GL_FALSE, &projMat[0][0]);

    glDrawArrays(GL_LINES, 0, 2);
    glBindVertexArray(0);
}

void SpringL::update() {

}



void Ball::__drawSetup() {

    shader = new Shader("resources/shader/sphere_vs.txt", "resources/shader/sphere_fs.txt");
    pmp::SurfaceMesh sphere = __icosphere(2);
    std::vector<float> vertices = std::vector<float>();

    for (auto f : sphere.faces()) {
        for (auto v : sphere.vertices(f)) {
            auto p = sphere.position(v);
            vertices.push_back(p[0]*radius);
            vertices.push_back(p[1] * radius);
            vertices.push_back(p[2] * radius);
        }
    }

    //REF : binding order of EBO,VAO https://stackoverflow.com/questions/69382389/opengl-binding-order-between-ebo-and-vao
    // 근데 애초에 EBO를 쓰려고 했기 때문에 버그가 발생했던거임.. 아무튼 고쳤다.
    unsigned int VBO;
    glGenBuffers(1, &VBO);
    glGenVertexArrays(1, &VAO);

    glBindBuffer(GL_ARRAY_BUFFER, VBO);
    glBindVertexArray(VAO);

    glBufferData(GL_ARRAY_BUFFER, sizeof(float) * vertices.size(), &vertices[0], GL_STATIC_DRAW);
 
    glEnableVertexAttribArray(0);
    glVertexAttribPointer(0, 3, GL_FLOAT, GL_FALSE, 3 * sizeof(float), (void*)0);

    glBindVertexArray(0);
    glBindBuffer(GL_ARRAY_BUFFER, 0);
 
}
void Ball::render() {
    
    shader->use();
    glBindVertexArray(VAO);

    glm::mat4 worldMat = glm::mat4(1.0f);
    worldMat = glm::translate(worldMat, pos);

    glm::mat4 viewMat = cam.getViewMatrix();
    glm::mat4 projMat = glm::perspective(glm::radians(cam.zoom), (float)SCR_WIDTH / (float)SCR_HEIGHT, 0.1f, 100.0f);

    glUniformMatrix4fv(glGetUniformLocation(shader->ID, "worldMat"), 1, GL_FALSE,&worldMat[0][0]);
    glUniformMatrix4fv(glGetUniformLocation(shader->ID, "viewMat"), 1, GL_FALSE, &viewMat[0][0]);
    glUniformMatrix4fv(glGetUniformLocation(shader->ID, "projMat"), 1, GL_FALSE, &projMat[0][0]);

    glDrawArrays(GL_TRIANGLES, 0, 960); //REF : drawcall은 굉장히 비싼 함수다. 그래서 최대한 적게 사용해야 됨. 특히 프로젝트 규모가 커질수록 그렇다고 함.

    glBindVertexArray(0);
}

// 공의 위치에 따라서. spring의 끝 점의 위치가 dependent한 방식으로 짜게 되면.
// update 할 때 힘을 분리 시키지 않으면 어떤 ball 이 먼저 update 되었는 지에 따라서.

void Mass::ftProcess() {

    //get all forces generated in joints

    //TODO Force process.

    const glm::vec3 gravity = mass * glm::vec3(0, -9.8f, 0);
    const glm::vec3 grav_Pos = pos;

    glm::vec3 netForce = glm::vec3(0);

    netForce += gravity;

    netF = netForce;

    //TODO  Torque process.


}

void Ball::update() {
    
    acc = netF / mass;

    pos += deltaTime * vel + 0.5f * deltaTime * deltaTime * acc;
    vel += deltaTime * acc;

    std::cout << pos.x << " : " << pos.y << " : " << pos.z << std::endl;
}


// REF : https://www.danielsieger.com/blog/2021/03/27/generating-spheres.html
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
