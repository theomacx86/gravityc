#ifndef SHADER_H
#define SHADER_H

/**
 * @file shader.h
 * @author Théo Jehl
 * @brief Shader class
 * @version 0.1
 * @date 2025-03-17
 * 
 * @copyright Copyright (c) 2025
 * 
 */

#include <stdbool.h>

typedef struct Shader 
{
    unsigned int shaderID;
    const char * vertexPath;
    const char * fragmentPath;
} Shader_s;

Shader_s *  shader_Init(char * vertexPath, char * fragmentPath);

void shader_Use(const Shader_s * this);

void shader_SetBool(const Shader_s * this, const char * name, bool value);

void shader_SetInt(const Shader_s * this, const char * name, int value);

void Shader_SetFloat(const Shader_s * this, const char * name, float value);


#endif
