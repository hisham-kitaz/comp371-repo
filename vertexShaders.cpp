//This assignment's resources are the labs provided in the course. Learnopengl.com the cherno youtube channel and sometimes stackovoverflow
#include <iostream>
#include <vector>
#define GLEW_STATIC 1   // This allows linking with Static Library on Windows, without DLL
#include <GL/glew.h>    // Include GLEW - OpenGL Extension Wrangler
#include <GLFW/glfw3.h> // cross-platform interface for creating a graphical context,
// initializing OpenGL and binding inputs
#include <glm/glm.hpp>  // GLM is an optimized math library with syntax to similar to OpenGL Shading Language
#include <glm/gtc/matrix_transform.hpp> // include this to create transformation matrices
#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include "vertexShaders.h"
using namespace glm;// we do this to call glm functions without using full fucntion name
using namespace std;// we dp this to call std functions without using full fucntion name

int createGrid()
{
    // A vertex is a point on a polygon, it contains positions and other data (eg: colors)
    vec3 vertexArray[] = {
            vec3(-10.0f,  0.0f, -10.0f), //top center vertex for a line
            vec3(1.0f,  1.0f, 0.0f), //first vertex color
            vec3(-10.0f,  0.0f, 10.0f),
            vec3(1.0f,  1.0f, 0.0f),
    };
    // Create a vertex array
    GLuint vertexArrayObject;
    glGenVertexArrays(1, &vertexArrayObject);
    glBindVertexArray(vertexArrayObject);

    // Upload Vertex Buffer to the GPU, keep a reference to it (vertexBufferObject)
    GLuint vertexBufferObject;
    glGenBuffers(1, &vertexBufferObject);
    glBindBuffer(GL_ARRAY_BUFFER, vertexBufferObject);
    glBufferData(GL_ARRAY_BUFFER, sizeof(vertexArray), vertexArray, GL_STATIC_DRAW);

    glVertexAttribPointer(0,                   // attribute 0 matches aPos in Vertex Shader
        3,                   // size
        GL_FLOAT,            // type
        GL_FALSE,            // normalized?
        2 * sizeof(vec3), // stride - each vertex contain 2 vec3 (position, color)
        (void*)0             // array buffer offset
    );
    glEnableVertexAttribArray(0);


    glVertexAttribPointer(1,                            // attribute 1 matches aColor in Vertex Shader
        3,
        GL_FLOAT,
        GL_FALSE,
        2 * sizeof(vec3),
        (void*)sizeof(vec3)      // color is offseted a vec3 (comes after position)
    );
    glEnableVertexAttribArray(1);

    glBindBuffer(GL_ARRAY_BUFFER, 0); // VAO already stored the state we just defined, safe to unbind buffer
    glBindVertexArray(0); // Unbind to not modify the VAO

    return vertexArrayObject;
}
int createX()
{
    // A vertex is a point on a polygon, it contains positions and other data (eg: colors)
    vec3 vertexArray[] = {
            vec3(0.0f,  0.0f, 0.0f), //top center vertex for a line
            vec3(1, 0, 0), //first vertex color
            vec3(2.50f, 0, 0),
            vec3(1, 0, 0)
    };

    // Create a vertex array
    GLuint vertexArrayObject;
    glGenVertexArrays(1, &vertexArrayObject);
    glBindVertexArray(vertexArrayObject);

    // Upload Vertex Buffer to the GPU, keep a reference to it (vertexBufferObject)
    GLuint vertexBufferObject;
    glGenBuffers(1, &vertexBufferObject);
    glBindBuffer(GL_ARRAY_BUFFER, vertexBufferObject);
    glBufferData(GL_ARRAY_BUFFER, sizeof(vertexArray), vertexArray, GL_STATIC_DRAW);

    glVertexAttribPointer(0,                   // attribute 0 matches aPos in Vertex Shader
        3,                   // size
        GL_FLOAT,            // type
        GL_FALSE,            // normalized?
        2 * sizeof(vec3), // stride - each vertex contain 2 vec3 (position, color)
        (void*)0             // array buffer offset
    );
    glEnableVertexAttribArray(0);


    glVertexAttribPointer(1,                            // attribute 1 matches aColor in Vertex Shader
        3,
        GL_FLOAT,
        GL_FALSE,
        2 * sizeof(vec3),
        (void*)sizeof(vec3)      // color is offseted a vec3 (comes after position)
    );
    glEnableVertexAttribArray(1);

    glBindBuffer(GL_ARRAY_BUFFER, 0); // VAO already stored the state we just defined, safe to unbind buffer
    glBindVertexArray(0); // Unbind to not modify the VAO

    return vertexArrayObject;
}
int createY()
{
    // A vertex is a point on a polygon, it contains positions and other data (eg: colors)
    vec3 vertexArray[] = {
            vec3(0.0f,  0.0f, 0.0f), //top center vertex for a line
            vec3(0, 1, 0), //first vertex color
            vec3(0, 2.50f, 0),
            vec3(0, 1, 0)
    };
    // Create a vertex array
    GLuint vertexArrayObject;
    glGenVertexArrays(1, &vertexArrayObject);
    glBindVertexArray(vertexArrayObject);

    // Upload Vertex Buffer to the GPU, keep a reference to it (vertexBufferObject)
    GLuint vertexBufferObject;
    glGenBuffers(1, &vertexBufferObject);
    glBindBuffer(GL_ARRAY_BUFFER, vertexBufferObject);
    glBufferData(GL_ARRAY_BUFFER, sizeof(vertexArray), vertexArray, GL_STATIC_DRAW);
    glVertexAttribPointer(0,                   // attribute 0 matches aPos in Vertex Shader
        3,                   // size
        GL_FLOAT,            // type
        GL_FALSE,            // normalized?
        2 * sizeof(vec3), // stride - each vertex contain 2 vec3 (position, color)
        (void*)0             // array buffer offset
    );
    glEnableVertexAttribArray(0);
    glVertexAttribPointer(1,                            // attribute 1 matches aColor in Vertex Shader
        3,
        GL_FLOAT,
        GL_FALSE,
        2 * sizeof(vec3),
        (void*)sizeof(vec3)      // color is offseted a vec3 (comes after position)
    );
    glEnableVertexAttribArray(1);
    glBindBuffer(GL_ARRAY_BUFFER, 0); // VAO already stored the state we just defined, safe to unbind buffer
    glBindVertexArray(0); // Unbind to not modify the VAO
    return vertexArrayObject;
}
int createZ()
{
    // A vertex is a point on a polygon, it contains positions and other data (eg: colors)
    vec3 vertexArray[] = {
            vec3(0.0f,  0.0f, 0.0f), //top center vertex for a line
            vec3(0, 0, 1), //first vertex color
            vec3(0, 0, 2.5f),
            vec3(0, 0, 1)
    };

    // Create a vertex array
    GLuint vertexArrayObject;
    glGenVertexArrays(1, &vertexArrayObject);
    glBindVertexArray(vertexArrayObject);

    // Upload Vertex Buffer to the GPU, keep a reference to it (vertexBufferObject)
    GLuint vertexBufferObject;
    glGenBuffers(1, &vertexBufferObject);
    glBindBuffer(GL_ARRAY_BUFFER, vertexBufferObject);
    glBufferData(GL_ARRAY_BUFFER, sizeof(vertexArray), vertexArray, GL_STATIC_DRAW);

    glVertexAttribPointer(0,                   // attribute 0 matches aPos in Vertex Shader
        3,                   // size
        GL_FLOAT,            // type
        GL_FALSE,            // normalized?
        2 * sizeof(vec3), // stride - each vertex contain 2 vec3 (position, color)
        (void*)0             // array buffer offset
    );
    glEnableVertexAttribArray(0);


    glVertexAttribPointer(1,                            // attribute 1 matches aColor in Vertex Shader
        3,
        GL_FLOAT,
        GL_FALSE,
        2 * sizeof(vec3),
        (void*)sizeof(vec3)      // color is offseted a vec3 (comes after position)
    );
    glEnableVertexAttribArray(1);

    glBindBuffer(GL_ARRAY_BUFFER, 0); // VAO already stored the state we just defined, safe to unbind buffer
    glBindVertexArray(0); // Unbind to not modify the VAO

    return vertexArrayObject;
}
// cube vbo
int createCubeCoordinate()
{
    // A vertex is a point on a polygon, it contains positions and other data (eg: colors)
    vec3 vertexArray[] = {
        // point position          color
        vec3(-0.25f, -1.0f, -0.25f), vec3(0.6f, 0.5176f, 0.4039f), // triangle 1 : begin
        vec3(-0.25f, -1.0f,  0.25f), vec3(0.6f, 0.5176f, 0.4039f),
        vec3(-0.25f,  1.0f,  0.25f), vec3(0.6f, 0.5176f, 0.4039f),

        vec3(0.25f,  1.0f, -0.25f), vec3(0.6f, 0.5176f, 0.4039f), // triangle 2 : begin
        vec3(-0.25f, -1.0f, -0.25f), vec3(0.6f, 0.5176f, 0.4039f),
        vec3(-0.25f,  1.0f, -0.25f), vec3(0.6f, 0.5176f, 0.4039f),

        vec3(0.25f, -1.0f,  0.25f), vec3(0.6f, 0.5176f, 0.4039f), // triangle 3 : begin
        vec3(-0.25f, -1.0f, -0.25f), vec3(0.6f, 0.5176f, 0.4039f),
        vec3(0.25f, -1.0f, -0.25f), vec3(0.6f, 0.5176f, 0.4039f),

        vec3(0.25f,  1.0f, -0.25f), vec3(0.6f, 0.5176f, 0.4039f), // triangle 4 : begin
        vec3(0.25f, -1.0f, -0.25f), vec3(0.6f, 0.5176f, 0.4039f),
        vec3(-0.25f, -1.0f, -0.25f), vec3(0.6f, 0.5176f, 0.4039f),

        vec3(-0.25f, -1.0f, -0.25f), vec3(0.6f, 0.5176f, 0.4039f), // triangle 5 : begin
        vec3(-0.25f,  1.0f,  0.25f), vec3(0.6f, 0.5176f, 0.4039f),
        vec3(-0.25f,  1.0f, -0.25f), vec3(0.6f, 0.5176f, 0.4039f),

        vec3(0.25f, -1.0f,  0.25f), vec3(0.6f, 0.5176f, 0.4039f), // triangle 6 : begin
        vec3(-0.25f, -1.0f,  0.25f), vec3(0.6f, 0.5176f, 0.4039f),
        vec3(-0.25f, -1.0f, -0.25f), vec3(0.6f, 0.5176f, 0.4039f),

        vec3(-0.25f,  1.0f,  0.25f), vec3(0.6f, 0.5176f, 0.4039f), // triangle 7 : begin
        vec3(-0.25f, -1.0f,  0.25f), vec3(0.6f, 0.5176f, 0.4039f),
        vec3(0.25f, -1.0f,  0.25f), vec3(0.6f, 0.5176f, 0.4039f),

        vec3(0.25f,  1.0f,  0.25f), vec3(0.6f, 0.5176f, 0.4039f), // triangle 8 : begin
        vec3(0.25f, -1.0f, -0.25f), vec3(0.6f, 0.5176f, 0.4039f),
        vec3(0.25f,  1.0f, -0.25f), vec3(0.6f, 0.5176f, 0.4039f),

        vec3(0.25f, -1.0f, -0.25f), vec3(0.6f, 0.5176f, 0.4039f), // triangle 9 : begin
        vec3(0.25f,  1.0f,  0.25f), vec3(0.6f, 0.5176f, 0.4039f),
        vec3(0.25f, -1.0f,  0.25f), vec3(0.6f, 0.5176f, 0.4039f),

        vec3(0.25f,  1.0f,  0.25f), vec3(0.6f, 0.5176f, 0.4039f), // triangle 10 : begin
        vec3(0.25f,  1.0f, -0.25f), vec3(0.6f, 0.5176f, 0.4039f),
        vec3(-0.25f,  1.0f, -0.25f), vec3(0.6f, 0.5176f, 0.4039f),

        vec3(0.25f,  1.0f,  0.25f), vec3(0.6f, 0.5176f, 0.4039f), // triangle 11 : begin
        vec3(-0.25f,  1.0f, -0.25f), vec3(0.6f, 0.5176f, 0.4039f),
        vec3(-0.25f,  1.0f,  0.25f), vec3(0.6f, 0.5176f, 0.4039f),

        vec3(0.25f,  1.0f,  0.25f), vec3(0.6f, 0.5176f, 0.4039f), // triangle 12 : begin
        vec3(-0.25f,  1.0f,  0.25f), vec3(0.6f, 0.5176f, 0.4039f),
        vec3(0.25f, -1.0f,  0.25f), vec3(0.6f, 0.5176f, 0.4039f)
    };

    // Create a vertex array
    GLuint vertexArrayObject;
    glGenVertexArrays(1, &vertexArrayObject);
    glBindVertexArray(vertexArrayObject);

    // Upload Vertex Buffer to the GPU, keep a reference to it (vertexBufferObject)
    GLuint vertexBufferObject;
    glGenBuffers(1, &vertexBufferObject);
    glBindBuffer(GL_ARRAY_BUFFER, vertexBufferObject);
    glBufferData(GL_ARRAY_BUFFER, sizeof(vertexArray), vertexArray, GL_STATIC_DRAW);

    // For position attribute
    glVertexAttribPointer(0,  // attribute 0 matches aPos in Vertex Shader
        3,  // size (number of components per vertex attribute)
        GL_FLOAT,  // type
        GL_FALSE,  // normalized?
        6 * sizeof(float),  // stride - each vertex contains 6 floats (3 for position, 3 for color)
        (void*)0  // array buffer offset for position (starts from the beginning)
    );
    glEnableVertexAttribArray(0);  // Enable the position attribute

    // For color attribute
    glVertexAttribPointer(1,  // attribute 1 matches aColor in Vertex Shader
        3,  // size (number of components per vertex attribute)
        GL_FLOAT,  // type
        GL_FALSE,  // normalized?
        6 * sizeof(float),  // stride - each vertex contains 6 floats (3 for position, 3 for color)
        (void*)(3 * sizeof(float))  // array buffer offset for color (starts after 3 floats for position)
    );
    glEnableVertexAttribArray(1);  // Enable the color attribute

    glBindBuffer(GL_ARRAY_BUFFER, 0); // VAO already stored the state we just defined, safe to unbind buffer
    glBindVertexArray(0); // Unbind to not modify the VAO

    return vertexArrayObject;
}