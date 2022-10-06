#include<iostream>
#include<vector>

#include<glad/glad.h>
#include<GLFW/glfw3.h>

#include<glm/glm.hpp>
#include<glm/gtc/matrix_transform.hpp>

#include"shader.h"
#include"camera.h"
#include"msSystem.h"
#include"component.h"
#include"joint.h"


extern const int SCR_WIDTH = 1280, SCR_HEIGHT = 720;    //REF :  const global variable은 extern을 붙여줘야 된다. https://community.arm.com/support-forums/f/compilers-and-libraries-forum/47925/c-extern-const-link-error

float deltaTime= 0.0f;

Camera cam(glm::vec3(0.0f,0.0f,10.0f));
msSystem MSSystem;

float lastX, lastY;
bool isFirstMove = true;

void processInput(GLFWwindow* window);
void framebuffer_size_callback(GLFWwindow* window, int width, int height);
void mouse_callback(GLFWwindow* window, double xpos, double ypos);
void scroll_callback(GLFWwindow* window, double xoffset, double yoffset);

void setScene1();
void setScene2();
void setScene3();

int main() {

	glfwInit();
	glfwWindowHint(GLFW_CONTEXT_VERSION_MAJOR, 3);
	glfwWindowHint(GLFW_CONTEXT_VERSION_MINOR, 3);
	glfwWindowHint(GLFW_OPENGL_PROFILE, GLFW_OPENGL_CORE_PROFILE);

	GLFWwindow* window = glfwCreateWindow(SCR_WIDTH, SCR_HEIGHT, "massSpring", NULL, NULL);
	if (window == NULL) {
		std::cout << "Failed to create GLFW window" << std::endl;
		glfwTerminate();
		return -1;
	}
	glfwMakeContextCurrent(window);

	if (!gladLoadGLLoader((GLADloadproc)glfwGetProcAddress)) {
		std::cout << "Failed to initilaize GLAD" << std::endl;
		return -1;
	}

	glViewport(0, 0, SCR_WIDTH, SCR_HEIGHT);
	glEnable(GL_DEPTH_TEST);

    glPolygonMode(GL_FRONT_AND_BACK, GL_LINE);

	glfwSetFramebufferSizeCallback(window, framebuffer_size_callback);
    glfwSetCursorPosCallback(window, mouse_callback);
    glfwSetScrollCallback(window, scroll_callback);
    glfwSetInputMode(window, GLFW_CURSOR, GLFW_CURSOR_DISABLED);

    setScene1();
    //setScene2();
    //setScene3();

	while (!glfwWindowShouldClose(window)) {
        std::cout << "loop start!!!!////////////////////////////////////////////////////////////////////////////////////////////////" << std::endl;
		processInput(window);

        glClearColor(0.9f, 0.9f, 0.9f, 1.0f);
        glClear(GL_COLOR_BUFFER_BIT | GL_DEPTH_BUFFER_BIT);

        MSSystem.updateAll();

        MSSystem.renderAll();

		glfwSwapBuffers(window);
		glfwPollEvents();
        std::cout << "loop end!!!!/////////////////////////////////////////////////////////////////////////////////" << std::endl << std::endl;

	}
    

	glfwTerminate();
	return 0;
}

void processInput(GLFWwindow* window) {

	if (glfwGetKey(window, GLFW_KEY_ESCAPE) == GLFW_PRESS)
		glfwSetWindowShouldClose(window, true);

    if (glfwGetKey(window, GLFW_KEY_W) == GLFW_PRESS)
        cam.processKeyboard(FORWARD);
    if (glfwGetKey(window, GLFW_KEY_A) == GLFW_PRESS)
        cam.processKeyboard(LEFT);
    if (glfwGetKey(window, GLFW_KEY_S) == GLFW_PRESS)
        cam.processKeyboard(BACKWARD);
    if (glfwGetKey(window, GLFW_KEY_D) == GLFW_PRESS)
        cam.processKeyboard(RIGHT);

    static int SPACE_OLDSTATE = GLFW_RELEASE;
    int SPACE_NEWSTATE = glfwGetKey(window, GLFW_KEY_SPACE);
    if (SPACE_NEWSTATE == GLFW_RELEASE && SPACE_OLDSTATE == GLFW_PRESS)
        deltaTime = (deltaTime != 0) ? 0 : 1 / 60.0f;
    SPACE_OLDSTATE = SPACE_NEWSTATE;
}

void framebuffer_size_callback(GLFWwindow* window, int width, int height) {
	glViewport(0, 0, width, height);
}

void mouse_callback(GLFWwindow* window, double xpos, double ypos) {

    if (isFirstMove) {
        isFirstMove = false;
        lastX = xpos;
        lastY = ypos;
    }
    
    float xoffset = static_cast<float>(xpos) - lastX;
    float yoffset = static_cast<float>(ypos) - lastY;
    lastX = xpos;
    lastY = ypos;

//    cam.processMovement(xoffset, yoffset);
}
void scroll_callback(GLFWwindow* window, double xoffset, double yoffset) {
    cam.processMouseScroll(yoffset);
}

void setScene1() {

    //add components
    Ball* ball1 = new Ball(glm::vec3(1.0f, -1.0f, 0.0f), 1.0f);
    MSSystem.addComponent(ball1);

    SpringL* spr1 = new SpringL(glm::vec3(-3.0f, 0.0f, 0.0f), glm::vec3(-4.0f, 0.0f, 0.0f));
    MSSystem.addComponent(spr1);

    FixedPoint* fp1 = new FixedPoint(glm::vec3(0.0f, 0.0f, 0.0f));
    MSSystem.addComponent(fp1);

    // def linkage 

    Joint* j1_On_Ball1 = new Joint(ball1, glm::vec3(0.0f, 0.0f, 0.0f));      
    j1_On_Ball1->linkSpring(spr1, 0); // spring의 1번 end를 joint에 연결.     

    Joint* j1_On_Fp1 = new Joint(fp1);                                     
    j1_On_Fp1->linkSpring(spr1, 1);

}

void setScene2() {
    //add components
    Ball* ball1 = new Ball(glm::vec3(1.0f, -1.0f, 0.0f), 1.0f);
    MSSystem.addComponent(ball1);

    SpringL* spr1 = new SpringL(glm::vec3(-3.0f, 0.0f, 0.0f), glm::vec3(-4.0f, 0.0f, 0.0f));
    MSSystem.addComponent(spr1);

    SpringL* spr2 = new SpringL(glm::vec3(-3.0f, 0.0f, 0.0f), glm::vec3(-4.0f, 0.0f, 0.0f));
    MSSystem.addComponent(spr2);

    FixedPoint* fp1 = new FixedPoint(glm::vec3(0.0f, 0.0f, 0.0f));
    MSSystem.addComponent(fp1);

    FixedPoint* fp2 = new FixedPoint(glm::vec3(3.0f, 0.0f, 0.0f));
    MSSystem.addComponent(fp2);

    // def linkage 

    Joint* j1_On_Ball1 = new Joint(ball1, glm::vec3(0.0f, 0.0f, 0.0f));
    j1_On_Ball1->linkSpring(spr1, 0); // spring의 1번 end를 joint에 연결.     

    Joint* j1_On_Fp1 = new Joint(fp1);
    j1_On_Fp1->linkSpring(spr1, 1);

    Joint* j2_On_Ball1 = new Joint(ball1, glm::vec3(0.0f, 0.0f, 0.0f));
    j2_On_Ball1->linkSpring(spr2, 0); // spring의 1번 end를 joint에 연결.     

    Joint* j1_On_Fp2 = new Joint(fp2);
    j1_On_Fp2->linkSpring(spr2, 1);

}

void setScene3() {
    //add components
    Ball* ball1 = new Ball(glm::vec3(1.0f, -1.0f, 0.0f), 1.0f);
    MSSystem.addComponent(ball1);

    SpringL* spr1 = new SpringL(glm::vec3(-3.0f, 0.0f, 0.0f), glm::vec3(-4.0f, 0.0f, 0.0f));
    MSSystem.addComponent(spr1);

    SpringL* spr2 = new SpringL(glm::vec3(-3.0f, 0.0f, 0.0f), glm::vec3(-4.0f, 0.0f, 0.0f));
    MSSystem.addComponent(spr2);

    SpringL* spr3 = new SpringL(glm::vec3(-3.0f, 0.0f, 0.0f), glm::vec3(-4.0f, 0.0f, 0.0f));
    MSSystem.addComponent(spr3);

    FixedPoint* fp1 = new FixedPoint(glm::vec3(0.0f, 0.0f, 0.0f));
    MSSystem.addComponent(fp1);

    FixedPoint* fp2 = new FixedPoint(glm::vec3(3.0f, 0.0f, 0.0f));
    MSSystem.addComponent(fp2);

    FixedPoint* fp3 = new FixedPoint(glm::vec3(1.5f, -1.0f, 0.0f));
    MSSystem.addComponent(fp3);

    // def linkage 

    Joint* j1_On_Ball1 = new Joint(ball1, glm::vec3(0.0f, 0.0f, 0.0f));
    j1_On_Ball1->linkSpring(spr1, 0); // spring의 1번 end를 joint에 연결.     

    Joint* j1_On_Fp1 = new Joint(fp1);
    j1_On_Fp1->linkSpring(spr1, 1);

    Joint* j2_On_Ball1 = new Joint(ball1, glm::vec3(0.0f, 0.0f, 0.0f));
    j2_On_Ball1->linkSpring(spr2, 0); // spring의 1번 end를 joint에 연결.     

    Joint* j1_On_Fp2 = new Joint(fp2);
    j1_On_Fp2->linkSpring(spr2, 1);


    Joint* j3_On_Ball1 = new Joint(ball1, glm::vec3(0.0f, 0.0f, 0.0f));
    j3_On_Ball1->linkSpring(spr3, 0); // spring의 1번 end를 joint에 연결.     

    Joint* j1_On_Fp3 = new Joint(fp3);
    j1_On_Fp3->linkSpring(spr3, 1);

}