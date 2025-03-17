#include "gravityc.h"
#include "shader.h"
#include <stdlib.h>
#include <stdio.h>
#include <stdbool.h>

float vertices[] = {
    0.5f,  0.5f, 0.0f,  // top right
    0.5f, -0.5f, 0.0f,  // bottom right
   -0.5f, -0.5f, 0.0f,  // bottom left
   -0.5f,  0.5f, 0.0f   // top left 
};
unsigned int indices[] = {  // note that we start from 0!
   0, 1, 3,  // first Triangle
   1, 2, 3   // second Triangle
};


void framebuffer_size_callback(GLFWwindow* window, int width, int height)
{
    glViewport(0, 0, width, height);    //Adjust viewport to fill the window again.
}  

void processInput(GLFWwindow *window)
{
    if(glfwGetKey(window, GLFW_KEY_ESCAPE) == GLFW_PRESS)
        glfwSetWindowShouldClose(window, true);
}

void renderLoop()
{
    glClearColor(0.2f, 0.3f, 0.3f, 1.0f);
    glClear(GL_COLOR_BUFFER_BIT);
}

void debugLogShader(unsigned int vertexShader)
{
    #ifdef DEBUG
    int  success;
    char infoLog[512];
    glGetShaderiv(vertexShader, GL_COMPILE_STATUS, &success);

    if(!success)
    {
        glGetShaderInfoLog(vertexShader, 512, NULL, infoLog);
        printf("FAILED TO COMPILE SHADER\n%s\n", infoLog);
    }
    #endif
    return;
}

int main()
{
    unsigned int VBO;
    unsigned int VAO;
    unsigned int EBO;   //Element buffer object

    Shader_s * shader = NULL;
    GLFWwindow* window = NULL;

    glfwInit();
    glfwWindowHint(GLFW_CONTEXT_VERSION_MAJOR, 3);
    glfwWindowHint(GLFW_CONTEXT_VERSION_MINOR, 3);                  //Set OpenGL 3.3 as the target
    glfwWindowHint(GLFW_OPENGL_PROFILE, GLFW_OPENGL_CORE_PROFILE);  //Smaller subset, disable backward compatibilty
    #ifdef __APPLE__
    glfwWindowHint(GLFW_OPENGL_FORWARD_COMPAT, GL_TRUE);            //macOS specific
    #endif

    window = glfwCreateWindow(800, 600, "GravityC", NULL, NULL); //Window creation

    if (window == NULL)
    {
        puts("Failed to create Window");
        glfwTerminate();
        return -1;
    }

    glfwMakeContextCurrent(window);
    glViewport(0, 0, 800, 600);                                                 //Tell renderer the current size
    glfwSetFramebufferSizeCallback(window, framebuffer_size_callback);          //Register window resizing callback function

    shader = shader_Init("shaders/vertex.glsl", "shaders/fragment.glsl");
    if(shader == NULL)
    {
        return EXIT_FAILURE;
    }

    /*vertexShader = glCreateShader(GL_VERTEX_SHADER);
    glShaderSource(vertexShader, 1, &vertexShaderSource, NULL);
    glCompileShader(vertexShader);
    debugLogShader(vertexShader);

    fragmentShader = glCreateShader(GL_FRAGMENT_SHADER);
    glShaderSource(fragmentShader, 1, &fragmentShaderSource, NULL);
    glCompileShader(fragmentShader);
    debugLogShader(fragmentShader);

    shaderProgram = glCreateProgram();
    glAttachShader(shaderProgram, vertexShader);
    glAttachShader(shaderProgram, fragmentShader);
    glLinkProgram(shaderProgram);*/

    glGenVertexArrays(1, &VAO);                                                     //Creates Vertex buffer array
    glGenBuffers(1, &VBO);                                                          //Creates buffer
    glGenBuffers(1, &EBO);

    glBindVertexArray(VAO);                                                         //Bind to current context

    glBindBuffer(GL_ARRAY_BUFFER, VBO);                                             //Bind buffer to GL_ARRAY_BUFFER (where vertices are stored.)
    glBufferData(GL_ARRAY_BUFFER, sizeof(vertices), vertices, GL_STATIC_DRAW);     //Copy to GPU

    glBindBuffer(GL_ELEMENT_ARRAY_BUFFER,  EBO);
    glBufferData(GL_ELEMENT_ARRAY_BUFFER, sizeof(vertices), indices, GL_STATIC_DRAW);

    glVertexAttribPointer(0, 3, GL_FLOAT, GL_FALSE, 3 * sizeof(float), (void*) 0);  //Set attributes, layout=0, vec3, float, not normalized, 
                                                                                    // 3*size of floats, starts at address 0 in VBO.
                                                                                                                                                                 
    glEnableVertexAttribArray(0);

    glBindVertexArray(0); 
    
    while(!glfwWindowShouldClose(window))
    {
        processInput(window);   //Process user inputs.

        glClearColor(0.2f, 0.3f, 0.3f, 1.0f);
        glClear(GL_COLOR_BUFFER_BIT);
        shader_Use(shader);
        //glUseProgram(shaderProgram);
        glBindVertexArray(VAO);
        //glBindBuffer(GL_ELEMENT_ARRAY_BUFFER, EBO);
        glDrawElements(GL_TRIANGLES, 6, GL_UNSIGNED_INT, 0);

        glfwSwapBuffers(window);
        glfwPollEvents();    
    }

    //cleanup

    glfwTerminate();
    return 0;
}
