#ifndef PROGRAM_HPP
#define PROGRAM_HPP
#pragma once


// System headers
#include <GLFW/glfw3.h>
#include <glad/glad.h>
#include <string>
#include <sceneGraph.hpp>
#include <sphere.hpp>
void runOv4(GLFWwindow* window);
SceneNode* generateSystem(int slices, int layers);
// Main OpenGL program
void runProgram(GLFWwindow* window);
void runProgram2(GLFWwindow* window);
void setup(GLFWwindow* window);
unsigned int createVAO(float* vertices, int vertLen, unsigned int* indices, int indLen, float* colors, int colorsLen);
void renderPlanet(SceneNode* planet, glm::mat4 vp, std::stack<glm::mat4>* stack, int indLen);
void updatePlanet(SceneNode* planet,float timeDelta,glm::mat4 oldMat);
// GLFW callback mechanisms
void keyboardCallback(GLFWwindow* window, int key, int scancode,
                      int action, int mods);



// Checks for whether an OpenGL error occurred. If one did,
// it prints out the error type and ID
inline void printGLError() {
    int errorID = glGetError();

    if(errorID != GL_NO_ERROR) {
        std::string errorString;

        switch(errorID) {
            case GL_INVALID_ENUM:
                errorString = "GL_INVALID_ENUM";
                break;
            case GL_INVALID_OPERATION:
                errorString = "GL_INVALID_OPERATION";
                break;
            case GL_INVALID_FRAMEBUFFER_OPERATION:
                errorString = "GL_INVALID_FRAMEBUFFER_OPERATION";
                break;
            case GL_OUT_OF_MEMORY:
                errorString = "GL_OUT_OF_MEMORY";
                break;
            case GL_STACK_UNDERFLOW:
                errorString = "GL_STACK_UNDERFLOW";
                break;
            case GL_STACK_OVERFLOW:
                errorString = "GL_STACK_OVERFLOW";
                break;
            default:
                errorString = "[Unknown error ID]";
                break;
        }

        fprintf(stderr, "An OpenGL error occurred (%i): %s.\n",
                errorID, errorString.c_str());
    }
}


#endif
