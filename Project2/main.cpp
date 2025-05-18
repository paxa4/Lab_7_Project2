#define GLEW_DLL
#define GLFW_DLL
#include <cstdio>
#include <string>
#include <iostream>
#include <vector>
#include <cmath>
#include "GL/glew.h"
#include "GLFW/glfw3.h"
#include "shader_loader.h"
#include "glm.hpp"
#include "gtc/matrix_transform.hpp"
#include "gtc/type_ptr.hpp"
#include "assimp/Importer.hpp"
#include "assimp/scene.h"
#include "assimp/postprocess.h"
#include "Mesh.h"
#include "Model.h"

GLfloat yaw = -90.0f;
GLfloat pitch = 0.0f;
bool firstMouse = true;
const unsigned int SCR_WIDTH = 1024;
const unsigned int SCR_HEIGHT = 768;
float lastX = SCR_WIDTH / 2.0f;
float lastY = SCR_HEIGHT / 2.0f;


glm::mat4 translatePart1 = glm::mat4(1.0); 
glm::mat4 translatePart2 = glm::mat4(1.0); 
glm::mat4 translatePart3 = glm::mat4(1.0); 


float posZ1 = 0.0f;  
float posX2 = 0.0f;  
float posY3 = 0.0f, posZ3 = 0.0f;              

float deltaTime = 0.0f;
float lastFrame = 0.0f;

void mouse_callback(GLFWwindow* window, double xposIn, double yposIn) {
    float xpos = static_cast<float>(xposIn);
    float ypos = static_cast<float>(yposIn);
    if (firstMouse) {
        lastX = xpos;
        lastY = ypos;
        firstMouse = false;
    }
    float xoffset = xpos - lastX;
    float yoffset = lastY - ypos;
    lastX = xpos;
    lastY = ypos;
    float sensitivity = 0.1f;
    xoffset *= sensitivity;
    yoffset *= sensitivity;
    yaw += xoffset;
    pitch += yoffset;
    if (pitch > 89.0f) pitch = 89.0f;
    if (pitch < -89.0f) pitch = -89.0f;
}

int main() {
    if (!glfwInit()) {
        std::cerr << "ERROR: couldn't start GLFW3\n";
        return 1;
    }
    glfwWindowHint(GLFW_CONTEXT_VERSION_MAJOR, 4);
    glfwWindowHint(GLFW_CONTEXT_VERSION_MINOR, 6);
    glfwWindowHint(GLFW_OPENGL_PROFILE, GLFW_OPENGL_CORE_PROFILE);
    GLFWwindow* window = glfwCreateWindow(SCR_WIDTH, SCR_HEIGHT, "Model Parts Movement", nullptr, nullptr);
    if (!window) {
        std::cerr << "Failed to create GLFW window\n";
        glfwTerminate();
        return -1;
    }
    glfwMakeContextCurrent(window);
    glewExperimental = GL_TRUE;
    if (glewInit() != GLEW_OK) {
        std::cerr << "ERROR: failed to initialize GLEW\n";
        return 1;
    }
    glfwSetInputMode(window, GLFW_CURSOR, GLFW_CURSOR_DISABLED);
    glfwSetCursorPosCallback(window, mouse_callback);

    Shader_loader shadering;
    GLuint shader_program = shadering.oneLinkProgram();
    Model model("Model.obj");

    glm::vec3 cameraPosition = glm::vec3(0.0f, 0.0f, 3.0f);
    glm::vec3 cameraTarget = glm::vec3(0.0f, 0.0f, -1.0f);
    glm::vec3 cameraUp = glm::vec3(0.0f, 1.0f, 0.0f);

    glClearColor(0.4f, 0.7f, 0.9f, 1.0f);
    glEnable(GL_DEPTH_TEST);

    while (!glfwWindowShouldClose(window)) {
        float currentFrame = glfwGetTime();
        deltaTime = currentFrame - lastFrame;
        lastFrame = currentFrame;

       
        glm::vec3 front;
        front.x = cos(glm::radians(yaw)) * cos(glm::radians(pitch));
        front.y = sin(glm::radians(pitch));
        front.z = sin(glm::radians(yaw)) * cos(glm::radians(pitch));
        cameraTarget = glm::normalize(front);

        const float cameraSpeed = 10.0f * deltaTime;
        if (glfwGetKey(window, GLFW_KEY_W) == GLFW_PRESS)
            cameraPosition += cameraSpeed * cameraTarget;
        if (glfwGetKey(window, GLFW_KEY_S) == GLFW_PRESS)
            cameraPosition -= cameraSpeed * cameraTarget;
        if (glfwGetKey(window, GLFW_KEY_A) == GLFW_PRESS)
            cameraPosition -= glm::normalize(glm::cross(cameraTarget, cameraUp)) * cameraSpeed;
        if (glfwGetKey(window, GLFW_KEY_D) == GLFW_PRESS)
            cameraPosition += glm::normalize(glm::cross(cameraTarget, cameraUp)) * cameraSpeed;

   
        const float moveSpeed = 1.5f * deltaTime;

        // J/K
        if (glfwGetKey(window, GLFW_KEY_K) == GLFW_PRESS) {
            posZ1 -= moveSpeed;
            posZ3 -= moveSpeed;
        }
        if (glfwGetKey(window, GLFW_KEY_J) == GLFW_PRESS) {
            posZ1 += moveSpeed;
            posZ3 += moveSpeed;
        }
        translatePart1 = glm::translate(glm::mat4(1.0), glm::vec3(0.0f, 0.0f, posZ1));

        // U/I
        if (glfwGetKey(window, GLFW_KEY_U) == GLFW_PRESS) posX2 -= moveSpeed;
        if (glfwGetKey(window, GLFW_KEY_I) == GLFW_PRESS) posX2 += moveSpeed;
        translatePart2 = glm::translate(glm::mat4(1.0), glm::vec3(posX2, 0.0f, 0.0f));
        // N/M
        if (glfwGetKey(window, GLFW_KEY_N) == GLFW_PRESS) posY3 -= moveSpeed;
        if (glfwGetKey(window, GLFW_KEY_M) == GLFW_PRESS) posY3 += moveSpeed;
        translatePart3 = glm::translate(glm::mat4(1.0), glm::vec3(0.0f, posY3, posZ3));


        posZ1 = glm::clamp(posZ1, -2.0f, 1.0f);
        posX2 = glm::clamp(posX2, -1.5f, 0.5f);
        posY3 = glm::clamp(posY3, -0.5f, 0.0f);
        posZ3 = glm::clamp(posZ3, -2.0f, 1.0f);

        
        glm::mat4 projection = glm::perspective(glm::radians(45.0f),
            (float)SCR_WIDTH / (float)SCR_HEIGHT, 0.1f, 100.0f);
        glm::mat4 view = glm::lookAt(cameraPosition, cameraPosition + cameraTarget, cameraUp);

   
        glClear(GL_COLOR_BUFFER_BIT | GL_DEPTH_BUFFER_BIT);
        glUseProgram(shader_program);

 
        shadering.uniform_set_vec(shader_program, "material.ambient", 1.0f, 0.5f, 0.31f);
        shadering.uniform_set_vec(shader_program, "material.diffuse", 1.0f, 0.5f, 0.31f);
        shadering.uniform_set_vec(shader_program, "material.specular", 0.5f, 0.5f, 0.5f);
        shadering.uniform_set_vec(shader_program, "material.shininess", 32.0f);
        shadering.uniform_set_vec(shader_program, "light.position", 1.2f, 1.0f, 2.0f);
        shadering.uniform_set_vec(shader_program, "light.ambient", 0.2f, 0.2f, 0.2f);
        shadering.uniform_set_vec(shader_program, "light.diffuse", 0.5f, 0.5f, 0.5f);
        shadering.uniform_set_vec(shader_program, "light.specular", 1.0f, 1.0f, 1.0f);
        shadering.uniform_set_vec(shader_program, "viewPos", cameraPosition.x, cameraPosition.y, cameraPosition.z);

        shadering.uniform_set_vec(shader_program, "view", 1, glm::value_ptr(view), GL_FALSE, Shader_loader::M4);
        shadering.uniform_set_vec(shader_program, "projection", 1, glm::value_ptr(projection), GL_FALSE, Shader_loader::M4);


        model.Draw1(shader_program, translatePart1, translatePart2, translatePart3);

        glfwSwapBuffers(window);
        glfwPollEvents();
    }

    glfwTerminate();
    return 0;
}