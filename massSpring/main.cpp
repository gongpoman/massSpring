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

msSystem MSSystem1(glm::vec3(5,0,-5),60.0f);
msSystem MSSystem2(glm::vec3(-5, 0, -5), 60.0f);
msSystem MSSystem3(glm::vec3(5,0,0),60.0f);
msSystem MSSystem4(glm::vec3(-5, 0, 0), 60.0f);

void setScene1();
void setScene2();
void setScene3();
void setScene4();


// setScene function 안에서는 msSystem안에서의 local좌표계가 있다고 생각하고 거기서 좌표를 설정해준다.
// 그리고 msSystem에 system의 중심을 나타내는 좌표가 있어서 rendering할 떄만 그만큼 옮겨서 그려준다.

float lastX, lastY;
bool isFirstMove = true;

void processInput(GLFWwindow* window);
void framebuffer_size_callback(GLFWwindow* window, int width, int height);
void mouse_callback(GLFWwindow* window, double xpos, double ypos);
void scroll_callback(GLFWwindow* window, double xoffset, double yoffset);


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
    setScene2();
    setScene3();
    setScene4();

	while (!glfwWindowShouldClose(window)) {
        std::cout << "loop start!!!!////////////////////////////////////////////////////////////////////////////////////////////////" << std::endl;
		processInput(window);

        glClearColor(0.9f, 0.9f, 0.9f, 1.0f);
        glClear(GL_COLOR_BUFFER_BIT | GL_DEPTH_BUFFER_BIT);

        MSSystem1.updateAll();
        MSSystem2.updateAll();
        MSSystem3.updateAll();
        MSSystem4.updateAll();

        MSSystem1.renderAll();
        MSSystem2.renderAll();
        MSSystem3.renderAll();
        MSSystem4.renderAll();

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

    cam.processMovement(xoffset, yoffset);
}
void scroll_callback(GLFWwindow* window, double xoffset, double yoffset) {
    cam.processMouseScroll(yoffset);
}

void setScene1() {
    // 공 만듦
    Ball* ball1 = new Ball(glm::vec3(1.0f, -1.0f, 0.0f), 1.0f);
    ball1->setIsDamped(true);
    MSSystem1.addComponent(ball1);
    //spring 만듦. 이때 사실 위치 받는데. 크게 의미는 없고 이걸로 default 길이 정해짐.
    SpringL* spr1 = new SpringL(glm::vec3(-3.0f, 0.0f, 0.0f), glm::vec3(-4.0f, 0.0f, 0.0f));
    MSSystem1.addComponent(spr1);
    // fixed point 만듦.
    FixedPoint* fp1 = new FixedPoint(glm::vec3(0.0f, 0.0f, 0.0f));
    MSSystem1.addComponent(fp1);

    // 공1 위에 joint1 만듦. 0,0,0은 공의 local position.
    Joint* j1_On_Ball1 = new Joint(ball1, glm::vec3(0.0f, 0.0f, 0.0f));      
    j1_On_Ball1->linkSpring(spr1, 0); // spring의 1번 end를 joint에 연결.     

    // fixed point에 joint 만듦.
    Joint* j1_On_Fp1 = new Joint(fp1);                                     
    j1_On_Fp1->linkSpring(spr1, 1);

}

void setScene2() {
    Ball* ball1 = new Ball(glm::vec3(1.0f, -1.0f, 0.0f),1.0f,0.2f);
    MSSystem2.addComponent(ball1);

    SpringL* spr1 = new SpringL(glm::vec3(1.0f, -1.0f, 0.0f), glm::vec3(0.0f, 0.0f, 0.0f),30.0f);
    MSSystem2.addComponent(spr1);

    FixedPoint* fp1 = new FixedPoint(glm::vec3(0.0f, 0.0f, 0.0f));
    MSSystem2.addComponent(fp1);


    Joint* j1_On_Ball1 = new Joint(ball1, glm::vec3(0.0f, 0.0f, 0.0f));
    j1_On_Ball1->linkSpring(spr1, 0);    

    Joint* j1_On_Fp1 = new Joint(fp1);
    j1_On_Fp1->linkSpring(spr1, 1);

    Ball* ball2 = new Ball(glm::vec3(0.0f, 0.0f, 1.0f),1.0f, 0.2f);
    MSSystem2.addComponent(ball2);

    SpringL* spr2 = new SpringL(glm::vec3(0.0f, 0.0f, 1.0f), glm::vec3(1.0f, -1.0f, 0.0f), 30.0f);
    MSSystem2.addComponent(spr2);

    Joint* j1_On_Ball2 = new Joint(ball2, glm::vec3(0.0f, 0.0f, 0.0f));
    j1_On_Ball2->linkSpring(spr2, 0);      

    j1_On_Ball1->linkSpring(spr2, 1);

}

void setScene3() {
    Ball* ball1 = new Ball(glm::vec3(1.0f, -1.0f, -1.0f), 1.0f);
    MSSystem3.addComponent(ball1);

    SpringL* spr1 = new SpringL(glm::vec3(-3.0f, 0.0f, 0.0f), glm::vec3(-4.0f, 0.0f, 0.0f));
    MSSystem3.addComponent(spr1);

    SpringL* spr2 = new SpringL(glm::vec3(-3.0f, 0.0f, 0.0f), glm::vec3(-4.0f, 0.0f, 0.0f));
    MSSystem3.addComponent(spr2);

    SpringL* spr3 = new SpringL(glm::vec3(-3.0f, 0.0f, 0.0f), glm::vec3(-4.0f, 0.0f, 0.0f));
    MSSystem3.addComponent(spr3);

    FixedPoint* fp1 = new FixedPoint(glm::vec3(0.0f, 0.0f, 0.0f));
    MSSystem3.addComponent(fp1);

    FixedPoint* fp2 = new FixedPoint(glm::vec3(3.0f, 0.0f, 0.0f));
    MSSystem3.addComponent(fp2);

    FixedPoint* fp3 = new FixedPoint(glm::vec3(1.5f, -1.0f, 0.0f));
    MSSystem3.addComponent(fp3);


    Joint* j1_On_Ball1 = new Joint(ball1, glm::vec3(0.0f, 0.0f, 0.0f));
    j1_On_Ball1->linkSpring(spr1, 0);   

    Joint* j1_On_Fp1 = new Joint(fp1);
    j1_On_Fp1->linkSpring(spr1, 1);

    // 사실 같은 joint에 spring 여러개 걸 수 있어서 이 밑으로는 좀 불필요한 코드긴 함... 귀찮...
    Joint* j2_On_Ball1 = new Joint(ball1, glm::vec3(0.0f, 0.0f, 0.0f));
    j2_On_Ball1->linkSpring(spr2, 0);      

    Joint* j1_On_Fp2 = new Joint(fp2);
    j1_On_Fp2->linkSpring(spr2, 1);

    Joint* j3_On_Ball1 = new Joint(ball1, glm::vec3(0.0f, 0.0f, 0.0f));
    j3_On_Ball1->linkSpring(spr3, 0);     

    Joint* j1_On_Fp3 = new Joint(fp3);
    j1_On_Fp3->linkSpring(spr3, 1);

}

void setScene4() {

    Ball* ball1 = new Ball(glm::vec3(1.0f, -1.0f, 0.0f), 1.0f, 0.2f);
    MSSystem4.addComponent(ball1);

    SpringL* spr1 = new SpringL(glm::vec3(1.0f, -1.0f, 0.0f), glm::vec3(0.0f, 0.0f, 0.0f), 30.0f);
    MSSystem4.addComponent(spr1);

    FixedPoint* fp1 = new FixedPoint(glm::vec3(0.0f, 0.0f, 0.0f));
    MSSystem4.addComponent(fp1);

    Joint* j1_On_Ball1 = new Joint(ball1, glm::vec3(0.0f, 0.0f, 0.0f));
    j1_On_Ball1->linkSpring(spr1, 0);     

    Joint* j1_On_Fp1 = new Joint(fp1);
    j1_On_Fp1->linkSpring(spr1, 1);



    Ball* ball2 = new Ball(glm::vec3(0.0f, -1.0f, 0.0f), 1.0f, 0.2f);
    MSSystem4.addComponent(ball2);

    SpringL* spr2 = new SpringL(glm::vec3(0.0f, -1.0f, 0.0f), glm::vec3(1.0f, -1.0f, 0.0f), 30.0f);
    MSSystem4.addComponent(spr2);

    Joint* j1_On_Ball2 = new Joint(ball2, glm::vec3(0.0f, 0.0f, 0.0f));
    j1_On_Ball2->linkSpring(spr2, 0);      

    j1_On_Ball1->linkSpring(spr2, 1);



    Ball* ball3 = new Ball(glm::vec3(0.0f, 0.0f, 1.0f), 1.0f, 0.2f);
    MSSystem4.addComponent(ball3);

    SpringL* spr3 = new SpringL(glm::vec3(0.0f, 0.0f, 1.0f), glm::vec3(1.0f, -1.0f, 0.0f), 30.0f);
    MSSystem4.addComponent(spr3);

    Joint* j1_On_Ball3 = new Joint(ball3, glm::vec3(0.0f, 0.0f, 0.0f));
    j1_On_Ball3->linkSpring(spr3, 0);

    j1_On_Ball1->linkSpring(spr3, 1);


    Ball* ball4 = new Ball(glm::vec3(-1.0f, 0.0f, 1.0f), 1.0f, 0.2f);
    MSSystem4.addComponent(ball4);

    SpringL* spr4 = new SpringL(glm::vec3(-1.0f, 0.0f, 1.0f), glm::vec3(0.0f,0.0f, 0.0f), 30.0f);
    MSSystem4.addComponent(spr4);

    Joint* j1_On_Ball4 = new Joint(ball4, glm::vec3(0.0f, 0.0f, 0.0f));
    j1_On_Ball4->linkSpring(spr4, 0);
    j1_On_Fp1->linkSpring(spr4, 1);


    FixedPoint* fp2 = new FixedPoint(glm::vec3(-2.0f, 0.0f, 0.0f));
    MSSystem4.addComponent(fp2);

    SpringL* spr5 = new SpringL(glm::vec3(-2.0f, 0.0f, 0.0f), glm::vec3(-1.0f, 0.0f, 1.0f), 30.0f);
    MSSystem4.addComponent(spr5);

    j1_On_Ball4->linkSpring(spr5, 0);
    Joint* j1_On_Fp2 = new Joint(fp2);
    j1_On_Fp2->linkSpring(spr5, 1);


    SpringL* spr6 = new SpringL(glm::vec3(0.0f, 0.0f, 1.0f), glm::vec3(-1.0f, 0.0f, 1.0f), 30.0f);
    MSSystem4.addComponent(spr6);

    j1_On_Ball4->linkSpring(spr6, 0);
    j1_On_Ball3->linkSpring(spr6, 1);
}