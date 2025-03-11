#include "gravityc.h"
#include <stdio.h>
#include <stdbool.h>

float vertices[] = {
    -0.5f, -0.5f, 0.0f,
     0.5f, -0.5f, 0.0f,
     0.0f,  0.5f, 0.0f
};  

/*
Vertex shared,
this just tells OpenGL how/where to draw a single vertex
OpenGL wants a vec4 output for this perVertex shader, tho we don't really need 4d
Instead we send our vec3 vector and set 1 as the 4th dimension
*/
const char *vertexShaderSource = "#version 330 core\n"
    "layout (location = 0) in vec3 aPos;\n"
    "void main()\n"
    "{\n"
    "   gl_Position = vec4(aPos.x, aPos.y, aPos.z, 1.0);\n"
    "}\0";

/*
Fragment shader
expects a vec4 for rgba
*/
const char *fragmentShaderSource = "#version 330 core\n"
    "out vec4 FragColor;\n"
    "void main()\n"
    "{\n"
    "   FragColor = vec4(1.0f, 0.5f, 0.2f, 1.0f);\n"
    "}\n\0";

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

    unsigned int vertexShader;
    unsigned int fragmentShader;
    unsigned int shaderProgram;

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

    vertexShader = glCreateShader(GL_VERTEX_SHADER);
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
    glLinkProgram(shaderProgram);


    glGenVertexArrays(1, &VAO);                                                     //Creates Vertex buffer array
    glGenBuffers(1, &VBO);                                                          //Creates buffer

    glBindVertexArray(VAO);                                                         //Bind to current context
    glBindBuffer(GL_ARRAY_BUFFER, VBO);                                             //Bind buffer to GL_ARRAY_BUFFER (where vertices are stored.)
    glBufferData(GL_ARRAY_BUFFER, sizeof(vertices), vertices, GL_STATIC_DRAW);     //Copy to GPU

    glVertexAttribPointer(0, 3, GL_FLOAT, GL_FALSE, 3 * sizeof(float), (void*) 0);  //Set attributes, layout=0, vec3, float, not normalized, 
                                                                                    // 3*size of floats, starts at address 0 in VBO.
                                                                                                                                                                    
    glEnableVertexAttribArray(0);
    glBindBuffer(GL_ARRAY_BUFFER, 0); 
    glBindVertexArray(0); 
    while(!glfwWindowShouldClose(window))
    {
        processInput(window);   //Process user inputs.

        glUseProgram(shaderProgram);
        glBindVertexArray(VAO);
        glDrawArrays(GL_TRIANGLES, 0, 3);


        glfwSwapBuffers(window);
        glfwPollEvents();    
    }

    //cleanup
    glDeleteShader(vertexShader);
    glDeleteShader(fragmentShader); 

    glfwTerminate();
    return 0;
}
