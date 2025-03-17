#include "shader.h"
#include <OpenGL/gltypes.h>
#include <stdbool.h>
#include <stdio.h>
#include <stdlib.h>
#include <gravityc.h>

/**
 * @brief Read a shader file in memory
 * 
 * @param filePath File path.
 * @return char* A buffer containing the shader source code.
 */
char * _shader_ReadFile(char * filePath)
{
    FILE * shaderSourceFile = NULL;
    int shaderSourceFileSize = 0, sizeRead = 0;
    char * shaderSource = NULL;

    shaderSourceFile = fopen(filePath, "r");
    if(shaderSourceFile == NULL)
    {
        printf("An error occured when opening file %s", filePath);
        return NULL;
    }

    fseek(shaderSourceFile, 0, SEEK_END);
    shaderSourceFileSize = ftell(shaderSourceFile);
    shaderSource = malloc(sizeof(char) * shaderSourceFileSize + 1);
    if(shaderSource == NULL)
    {
        printf("An error occured when allocating memory for file %s", filePath);
        fclose(shaderSourceFile);
        return NULL;
    }

    fseek(shaderSourceFile, 0L, SEEK_SET);
    sizeRead = fread((void*) shaderSource, sizeof(char), shaderSourceFileSize, shaderSourceFile);
    
    if(sizeRead != shaderSourceFileSize)
    {
        printf("An error occured when reading file %s (sizes %d %d)", filePath, sizeRead, shaderSourceFileSize);
        free(shaderSource);
        fclose(shaderSourceFile);
        return NULL;
    }

    fclose((shaderSourceFile));
    return shaderSource;
}

/**
 * @brief Compiles a shader
 * 
 * @param this 
 * @param shaderProgram Buffer containing the shader source code
 * @param type Shader type
 * @return int Shader ID.
 */
int _shader_CompileShader(Shader_s * this, const char * shaderProgram, GLenum type)
{
    unsigned int shader;
    int succcess;
    shader = glCreateShader(type);  

    glShaderSource(shader, 1, &shaderProgram, NULL);
    glCompileShader(shader);    
    glGetShaderiv(shader, GL_COMPILE_STATUS, &succcess);

    if(!succcess)
    {
        return -1;
    }

    glAttachShader(this->shaderID, shader);

    return shader;
}

/**
 * @brief Initialize a shader and compiles its corresponding vertex and fragment shader
 * 
 * @param vertexPath Path to Vertex shader source code file
 * @param fragmentPath Path to Fragment shader source code file
 * @return Shader_s* Shader_S struct is success, NULL is failure.
 */
Shader_s * shader_Init(char * vertexPath, char * fragmentPath)
{
    Shader_s * this = NULL;                                             // this object
    char * vertexShaderSource = NULL, * fragmentShaderSource = NULL;    // Shader source code buffer
    int vertex, fragment;                                      // Compilation result

    //Create shader object.
    this = malloc(sizeof(Shader_s));
    if(this == NULL)
    {
        return NULL;
    }

    this->shaderID = glCreateProgram();

    //Load vertex and shader source in memory.
    vertexShaderSource = _shader_ReadFile(vertexPath);
    if(vertexShaderSource == NULL)
    {
        free(this);
        return NULL;
    }

    fragmentShaderSource = _shader_ReadFile(fragmentPath);
    if(fragmentShaderSource == NULL)
    {
        free(this);
        free(vertexShaderSource);
        return NULL;
    }

    vertex = _shader_CompileShader(this, vertexShaderSource, GL_VERTEX_SHADER);
    fragment = _shader_CompileShader(this, fragmentShaderSource, GL_FRAGMENT_SHADER);

    glLinkProgram(this->shaderID);
    glDeleteShader(vertex);
    glDeleteShader(fragment);

    return this;
}

void shader_Use(const Shader_s * this)
{
    glUseProgram(this->shaderID);
}


void shader_SetInt(const Shader_s * this, const char * name, int value)
{
    glUniform1i(glGetUniformLocation(this->shaderID, name), value); 
}


void shader_SetBool(const Shader_s * this, const char * name, bool value)
{
    //A bool is just an int
    shader_SetInt(this, name, (int) value);
}
