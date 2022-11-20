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

void __vecPrint(glm::vec3);

void __mapBuffer(unsigned int& vbo, void* data, unsigned int size);
void __project_to_unit_sphere(pmp::SurfaceMesh& mesh);
pmp::SurfaceMesh __icosahedron();
pmp::SurfaceMesh __icosphere(size_t n_subdivisions);


Component::Component()
{
    std::cout << "Comp!!!" << std::endl;

}
Component::~Component()
{
    delete shader;
    std::cout<<shader << "    In COMPO~" << std::endl;
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
    for (auto iter = joints.begin(); iter != joints.end(); iter++) {
        auto delJ = (*iter);
        delete delJ;
        std::cout << "deleteJoint in FP" << std::endl;
    }
    std::cout << "FP~" << std::endl;
}

Mass::Mass() {
    std::cout << "MASS!!" << std::endl;
    type = MASS;
    dampingRatio = -1.0f;
}
Mass::~Mass() {

    for (auto iter = joints.begin(); iter != joints.end(); iter++) {
        Joint* delJ = (*iter);
        delete delJ;
        std::cout << "deleteJoint In mass" << std::endl;
    }

    std::cout << "Mass~" << std::endl;
}


Ball::Ball()
{
    __drawSetup();
}
Ball::Ball(glm::vec3 position, float r, float d,bool renderSmall)
{
    std::cout << "Ball Init...!!!" << std::endl;
    pos = position;
    vel = glm::vec3(0);
    acc = glm::vec3(0);
        
    netF = glm::vec3(0);
    netT = glm::vec3(0);

    simpleRender = renderSmall;

    radius = r;
    density = d;
    mass = 4 / 3 * M_PI * r * r * r;
    __drawSetup();
}
Ball::~Ball()
{
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

void FixedPoint::render(glm::vec3 globalPos ) {
    shader->use();
    glBindVertexArray(VAO);

    glm::mat4 worldMat = glm::mat4(1.0f);
    worldMat = glm::translate(worldMat, pos + globalPos);

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

void FixedPoint::addJoint(Joint* joint) {
    joints.push_back(joint);
}
/*
void Mass::ftProcess() {// TODONOW. orientation...
    // interpolation을 결국에는 해야되는데. 여러번 자르고, update를 몇번에 걸쳐서 하고 등등 이런 어떻게 할 지는 나중에 생각하고
    //일단 방향, 회전부터.
    //근데 일단 이렇게 평균을 낸 것을 이용해서 update를 여러번 하는 것이 가장 나은 것 같은데?

    // with interpolation. 하나의 timestep 대해서 interpolation만 하고.
    // 여러번 update를 할 지 말지는 msSystem에서 정하는 걸로 하자. 

    const glm::vec3 gravity = mass * glm::vec3(0, -9.8f, 0);

    std::vector<glm::vec3> jointForce;
    std::vector<glm::vec3> jointPos;
    glm::vec3 netTorque1(0);

    for (auto iter = joints.begin(); iter != joints.end(); iter++) {
        const glm::vec3 jF = (*iter)->getJointForce();
        const glm::vec3 jP = (*iter)->getJointPos();
        jointForce.push_back(jF);
        jointPos.push_back(jP);
        netTorque1 += glm::cross(jP, jF);
    }

    glm::vec3 netForce1 = glm::vec3(0);

    netForce1 += gravity;
    for (std::vector<glm::vec3>::iterator iter = jointForce.begin(); iter != jointForce.end(); iter++) {
        netForce1 += (*iter);
    }

    // get next step F,torque
    glm::vec3 netForce2(0);
    glm::vec3 netTorque2(0);

    //tempporary update.
    glm::vec3 tempDPos;
    void* tempOri;
    tempDPos = deltaTime * vel + 0.5f * deltaTime * deltaTime * netF / mass;
    pos += tempDPos;
    //get next step f,t
    for (auto iter = joints.begin(); iter != joints.end(); iter++) {
        const glm::vec3 jF = (*iter)->getJointForce();
        const glm::vec3 jP = (*iter)->getJointPos();
        jointForce.push_back(jF);
        jointPos.push_back(jP);
        netTorque2 += glm::cross(jP, jF);
    }

    // return to current state
    pos -= tempDPos;

    netF = 0.5f*(netForce1 + netForce2);

}
*/
void Mass::ftProcess() { // no Interpolation


    std::vector<glm::vec3> jointForce;
    std::vector<glm::vec3> jointPos;

    for (auto iter = joints.begin(); iter != joints.end(); iter++) {
        jointForce.push_back((*iter)->getJointForce());
        jointPos.push_back((*iter)->getJointPos());
    }

    const glm::vec3 gravity = mass * glm::vec3(0, -9.8f, 0);
    const glm::vec3 grav_Pos = pos;

    glm::vec3 netForce = glm::vec3(0);

    netForce += gravity;

    for (std::vector<glm::vec3>::iterator iter = jointForce.begin(); iter != jointForce.end(); iter++) {
        netForce += (*iter);
    }

    netF = netForce;

    if (dampingRatio>=FLT_EPSILON) {
        netF -= dampingRatio * mass * vel;
    }
}


glm::vec3 Mass::getVelocity() {
    return vel;
}
glm::vec3 Mass::getPosition() {
    return pos;
}
void Mass::addJoint(Joint* joint) {
    joints.push_back(joint);
}

float Mass::getMass() {
    return mass;
}

void Mass::setDampingRatio(float damp) {
    dampingRatio = damp;
}

void Ball::__drawSetup() {

    int fold = (radius < 0.51 || simpleRender) ? 0 : 2;
    shader = new Shader("resources/shader/sphere_vs.txt", "resources/shader/sphere_fs.txt");
    pmp::SurfaceMesh sphere = __icosphere(fold);
    std::vector<float> vertices = std::vector<float>();

    float renderRadi = (simpleRender) ? 0.1 : radius;

    for (auto f : sphere.faces()) {
        for (auto v : sphere.vertices(f)) {
            auto p = sphere.position(v);
            vertices.push_back(p[0]* renderRadi);
            vertices.push_back(p[1] * renderRadi);
            vertices.push_back(p[2] * renderRadi);
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
void Ball::render(glm::vec3 globalPos) {
    
    shader->use();
    glBindVertexArray(VAO);

    glm::mat4 worldMat = glm::mat4(1.0f);
    worldMat = glm::translate(worldMat, pos + globalPos);

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

void Ball::update() {
    
    acc = netF / mass;
    vel += deltaTime * acc;
    pos += deltaTime * vel;
}



SpringL::SpringL()
{
    __drawSetup();
}

SpringL::SpringL(glm::vec3 position1, glm::vec3 position2, float ela)
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
    std::cout << "SPR~" << std::endl;
}



void SpringL::__drawSetup() {

    shader = new Shader("resources/shader/springL_vs.txt", "resources/shader/springL_fs.txt");

    float vertices[] = {
        pos1[0],pos1[1],pos1[2],
        pos2[0],pos2[1],pos2[2],
    };

    glGenBuffers(1, &VBO);
    glGenVertexArrays(1, &VAO);

    glBindVertexArray(VAO);
    glBindBuffer(GL_ARRAY_BUFFER, VBO);

    glBufferData(GL_ARRAY_BUFFER, sizeof(vertices), vertices, GL_DYNAMIC_DRAW);

    glEnableVertexAttribArray(0);
    glVertexAttribPointer(0, 3, GL_FLOAT, GL_FALSE, 3 * sizeof(float), (void*)0);

    glBindBuffer(GL_ARRAY_BUFFER, 0);
    glBindVertexArray(0);

}

void SpringL::render(glm::vec3 globalPos) {
    shader->use();
    glBindVertexArray(VAO);

    glm::mat4 worldMat = glm::mat4(1.0f);
    worldMat = glm::translate(worldMat, globalPos);

    glm::mat4 viewMat = cam.getViewMatrix();
    glm::mat4 projMat = glm::perspective(glm::radians(cam.zoom), (float)SCR_WIDTH / (float)SCR_HEIGHT, 0.1f, 100.0f);

    glUniformMatrix4fv(glGetUniformLocation(shader->ID, "worldMat"), 1, GL_FALSE, &worldMat[0][0]);
    glUniformMatrix4fv(glGetUniformLocation(shader->ID, "viewMat"), 1, GL_FALSE, &viewMat[0][0]);
    glUniformMatrix4fv(glGetUniformLocation(shader->ID, "projMat"), 1, GL_FALSE, &projMat[0][0]);

    glDrawArrays(GL_LINES, 0, 2);
    glBindVertexArray(0);
}

void SpringL::update() {
    pos1 = endP[0]->getJointPos();
    pos2 = endP[1]->getJointPos();

    currentLen = glm::length(pos1 - pos2);

    //VAO, VBO update for rendering.
    float vertices[] = {
      pos1[0],pos1[1],pos1[2],
      pos2[0],pos2[1],pos2[2],
    };
    __mapBuffer(VBO,vertices,sizeof(vertices));
}
void SpringL::linkJoint(Joint* joint, bool id) {
    endP[id] = joint;

    if (!id) {
        pos1 = joint->getJointPos();
    }
    else {
        pos2 = joint->getJointPos();
    }
    currentLen = glm::length(pos1 - pos2);
}

glm::vec3 SpringL::getSpringForce(bool id) {
    glm::vec3 f = (id)? glm::normalize(pos1 - pos2) :glm::normalize(pos2 - pos1);
    f *= elasticity;
    f *= glm::abs(defaultLen - currentLen);

    std::cout<<" force : "  << f.x << " : " << f.y << " : " << f.z << "force : " << glm::length(f) << std::endl;
    return f;
}

void __mapBuffer(unsigned int &vbo,void* data, unsigned int size) {
    glBindBuffer(GL_ARRAY_BUFFER, vbo);

    void* ptr = glMapBuffer(GL_ARRAY_BUFFER, GL_WRITE_ONLY);

    memcpy(ptr, data, size);

    glUnmapBuffer(GL_ARRAY_BUFFER);

    glBindBuffer(GL_ARRAY_BUFFER,0);
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

void __vecPrint(glm::vec3 v) {
    std::cout << " x : " << v.x << " y : " << v.y << " z : " << v.z << std::endl;
}