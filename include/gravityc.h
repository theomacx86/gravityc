#ifndef GRAVITYC_H
#define GRAVITYC_H

#ifdef __APPLE__
#define GL_SILENCE_DEPRECATION 1
#endif

#define GLFW_INCLUDE_NONE
#include <GLFW/glfw3.h>
#include <OpenGL/gl3.h>

/**
 * @brief Process users input
 * 
 * @param window 
 */
void processInput(GLFWwindow *window);

/**
 * @brief Callback function for window resizing actions.
 * 
 * @param window 
 * @param width 
 * @param height 
 */
void framebuffer_size_callback(GLFWwindow* window, int width, int height);

void renderLoop();

void debugLogShader(unsigned int vertexShader);

#endif
