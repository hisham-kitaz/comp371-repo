#include <vector>

#define GLEW_STATIC 1   // This allows linking with Static Library on Windows, without DLL
#include <GL/glew.h>    // Include GLEW - OpenGL Extension Wrangler

// initializing OpenGL and binding inputs
#include <glm/glm.hpp>  // GLM is an optimized math library with syntax similar to OpenGL Shading Language
#include <glm/gtc/matrix_transform.hpp> // include this to create transformation matrices

#include "vaos.h"

using namespace glm;
using namespace std;

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
        (void*)sizeof(vec3)      // color has offset a vec3 (comes after position)
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
        (void*)sizeof(vec3)      // color has offset a vec3 (comes after position)
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
        (void*)sizeof(vec3)      // color has offset a vec3 (comes after position)
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
        (void*)sizeof(vec3)      // color has offset a vec3 (comes after position)
    );
    glEnableVertexAttribArray(1);

    glBindBuffer(GL_ARRAY_BUFFER, 0); // VAO already stored the state we just defined, safe to unbind buffer
    glBindVertexArray(0); // Unbind to not modify the VAO

    return vertexArrayObject;
}

vec3 computeFaceNormal(const vec3& v1, const vec3& v2, const vec3& v3) {
    vec3 edge1 = v2 - v1;
    vec3 edge2 = v3 - v1;
    return normalize(cross(edge1, edge2));
}

// cube vbo
int createCubeCoordinate()
{
    // A vertex is a point on a polygon, it contains positions and other data (eg: colors)
    vec3 vertexArray[] = {
        // point position          color
        vec3(-0.25f, -1.0f, -0.25f), vec3(1.0f, 0.0f, 0.0f), // triangle 1 : begin
        vec3(-0.25f, -1.0f,  0.25f), vec3(1.0f, 0.0f, 0.0f),
        vec3(-0.25f,  1.0f,  0.25f), vec3(1.0f, 0.0f, 0.0f),

        vec3(0.25f,  1.0f, -0.25f), vec3(1.0f, 0.0f, 0.0f), // triangle 2 : begin
        vec3(-0.25f, -1.0f, -0.25f), vec3(1.0f, 0.0f, 0.0f),
        vec3(-0.25f,  1.0f, -0.25f), vec3(1.0f, 0.0f, 0.0f),

        vec3(0.25f, -1.0f,  0.25f), vec3(1.0f, 0.0f, 0.0f), // triangle 3 : begin
        vec3(-0.25f, -1.0f, -0.25f), vec3(1.0f, 0.0f, 0.0f),
        vec3(0.25f, -1.0f, -0.25f), vec3(1.0f, 0.0f, 0.0f),

        vec3(0.25f,  1.0f, -0.25f), vec3(1.0f, 0.0f, 0.0f), // triangle 4 : begin
        vec3(0.25f, -1.0f, -0.25f), vec3(1.0f, 0.0f, 0.0f),
        vec3(-0.25f, -1.0f, -0.25f), vec3(1.0f, 0.0f, 0.0f),

        vec3(-0.25f, -1.0f, -0.25f), vec3(1.0f, 0.0f, 0.0f), // triangle 5 : begin
        vec3(-0.25f,  1.0f,  0.25f), vec3(1.0f, 0.0f, 0.0f),
        vec3(-0.25f,  1.0f, -0.25f), vec3(1.0f, 0.0f, 0.0f),

        vec3(0.25f, -1.0f,  0.25f), vec3(1.0f, 0.0f, 0.0f), // triangle 6 : begin
        vec3(-0.25f, -1.0f,  0.25f), vec3(1.0f, 0.0f, 0.0f),
        vec3(-0.25f, -1.0f, -0.25f), vec3(1.0f, 0.0f, 0.0f),

        vec3(-0.25f,  1.0f,  0.25f), vec3(1.0f, 0.0f, 0.0f), // triangle 7 : begin
        vec3(-0.25f, -1.0f,  0.25f), vec3(1.0f, 0.0f, 0.0f),
        vec3(0.25f, -1.0f,  0.25f), vec3(1.0f, 0.0f, 0.0f),

        vec3(0.25f,  1.0f,  0.25f), vec3(1.0f, 0.0f, 0.0f), // triangle 8 : begin
        vec3(0.25f, -1.0f, -0.25f), vec3(1.0f, 0.0f, 0.0f),
        vec3(0.25f,  1.0f, -0.25f), vec3(1.0f, 0.0f, 0.0f),

        vec3(0.25f, -1.0f, -0.25f), vec3(1.0f, 0.0f, 0.0f), // triangle 9 : begin
        vec3(0.25f,  1.0f,  0.25f), vec3(1.0f, 0.0f, 0.0f),
        vec3(0.25f, -1.0f,  0.25f), vec3(1.0f, 0.0f, 0.0f),

        vec3(0.25f,  1.0f,  0.25f), vec3(1.0f, 0.0f, 0.0f), // triangle 10 : begin
        vec3(0.25f,  1.0f, -0.25f), vec3(1.0f, 0.0f, 0.0f),
        vec3(-0.25f,  1.0f, -0.25f), vec3(1.0f, 0.0f, 0.0f),

        vec3(0.25f,  1.0f,  0.25f), vec3(1.0f, 0.0f, 0.0f), // triangle 11 : begin
        vec3(-0.25f,  1.0f, -0.25f), vec3(1.0f, 0.0f, 0.0f),
        vec3(-0.25f,  1.0f,  0.25f), vec3(1.0f, 0.0f, 0.0f),

        vec3(0.25f,  1.0f,  0.25f), vec3(1.0f, 0.0f, 0.0f), // triangle 12 : begin
        vec3(-0.25f,  1.0f,  0.25f), vec3(1.0f, 0.0f, 0.0f),
        vec3(0.25f, -1.0f,  0.25f), vec3(1.0f, 0.0f, 0.0f)
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

    vector<vec3> normals;
    for (size_t i = 0; i < sizeof(vertexArray) / sizeof(vertexArray[0]); i += 3) {
        vec3 v1 = vertexArray[i];
        vec3 v2 = vertexArray[i + 1];
        vec3 v3 = vertexArray[i + 2];

        vec3 faceNormal = computeFaceNormal(v1, v2, v3);

        for (int j = 0; j < 3; j++) {
            normals.push_back(faceNormal);
        }
    }

    // Upload Normals Buffer to the GPU, keep a reference to it (normalBufferObject)
    GLuint normalBufferObject;
    glGenBuffers(1, &normalBufferObject);
    glBindBuffer(GL_ARRAY_BUFFER, normalBufferObject);
    glBufferData(GL_ARRAY_BUFFER, normals.size() * sizeof(glm::vec3), &normals[0], GL_STATIC_DRAW);

    // For normal attribute
    glVertexAttribPointer(1,  // attribute 1 matches normal in Vertex Shader
                          3,  // size (number of components per vertex attribute)
                          GL_FLOAT,  // type
                          GL_FALSE,  // normalized?
                          3 * sizeof(float),  // stride - each normal contains 3 floats
                          (void*)0  // array buffer offset for normal (starts from the beginning)
    );
    glEnableVertexAttribArray(2);  // Enable the normal attribute


    glBindBuffer(GL_ARRAY_BUFFER, 0); // VAO already stored the state we just defined, safe to unbind buffer
    glBindVertexArray(0); // Unbind to not modify the VAO

    return vertexArrayObject;
}

GLuint createSphere(int resolution, float radius)
{
    struct TexturedVertex
    {
        TexturedVertex(glm::vec3 _position, glm::vec2 _uv)
                : position(_position), uv(_uv) {}

        glm::vec3 position;
        glm::vec2 uv; // UV coordinates
    };

    std::vector<TexturedVertex> vertices;
    std::vector<GLuint> indices;

    for (int i = 0; i < resolution / 2; i++)
    {
        for (int j = 0; j < resolution; j++)
        {
            float iRads = static_cast<float>(i) / resolution * glm::two_pi<float>();
            float jRads = static_cast<float>(j) / resolution * glm::two_pi<float>();

            float x = std::sin(jRads) * std::sin(iRads);
            float y = std::cos(iRads);
            float z = std::cos(jRads) * std::sin(iRads);

            float u = static_cast<float>(j) / (resolution - 1);
            float v = static_cast<float>(i) / ((resolution / 2) - 1);

            vertices.push_back(TexturedVertex(glm::vec3(x, y, z) * radius, glm::vec2(u, v)));
        }
    }

    // Bottom vertex
    vertices.push_back(TexturedVertex(glm::vec3(0, -radius, 0), glm::vec2(0.0f, 0.0f)));

    int vertexCount = vertices.size();

    for (int i = 0; i < resolution * ((resolution - 2) / 2); i++)
    {
        indices.push_back(i);
        indices.push_back(i + resolution - 1);
        indices.push_back(i + resolution);

        indices.push_back(i + resolution);
        indices.push_back(i + 1);
        indices.push_back(i);
    }

    // Bottom faces
    for (int i = 0; i < resolution - 1; i++)
    {
        indices.push_back(vertexCount - 1);
        indices.push_back(vertexCount - 1 - resolution + i + 1);
        indices.push_back(vertexCount - 1 - resolution + i);
    }

    // Last triangle
    indices.push_back(vertexCount - resolution - 1);
    indices.push_back(vertexCount - 2);
    indices.push_back(vertexCount - 1);

    std::vector<GLfloat> vertexDataVector;

    for (int i = 0; i < vertexCount; i++)
    {
        TexturedVertex vertex = vertices[i];
        vertexDataVector.push_back(vertex.position.x);
        vertexDataVector.push_back(vertex.position.y);
        vertexDataVector.push_back(vertex.position.z);
        vertexDataVector.push_back(vertex.uv.x);
        vertexDataVector.push_back(vertex.uv.y);
    }

    // Create a vertex array object
    GLuint vertexArrayObject;
    glGenVertexArrays(1, &vertexArrayObject);
    glBindVertexArray(vertexArrayObject);

    // Upload Vertex Buffer to the GPU, keep a reference to it (vertexBufferObject)
    GLuint vertexBufferObject;
    glGenBuffers(1, &vertexBufferObject);
    glBindBuffer(GL_ARRAY_BUFFER, vertexBufferObject);
    glBufferData(GL_ARRAY_BUFFER, vertexDataVector.size() * sizeof(GLfloat), &vertexDataVector[0], GL_STATIC_DRAW);

    // Set the vertex attribute pointers
    glVertexAttribPointer(0, 3, GL_FLOAT, GL_FALSE, 5 * sizeof(GLfloat), (void*)0); // Vertex position
    glVertexAttribPointer(1, 2, GL_FLOAT, GL_FALSE, 5 * sizeof(GLfloat), (void*)(3 * sizeof(GLfloat))); // UV coordinates

    glEnableVertexAttribArray(0);
    glEnableVertexAttribArray(1);

    // Upload Index Buffer to the GPU, keep a reference to it (indexBufferObject)
    GLuint indexBufferObject;
    glGenBuffers(1, &indexBufferObject);
    glBindBuffer(GL_ELEMENT_ARRAY_BUFFER, indexBufferObject);
    glBufferData(GL_ELEMENT_ARRAY_BUFFER, indices.size() * sizeof(GLuint), &indices[0], GL_STATIC_DRAW);

    // Unbind buffers
    glBindBuffer(GL_ARRAY_BUFFER, 0);
    glBindVertexArray(0);

    return vertexArrayObject;
}

int createGround() {
    struct TexturedColoredVertex
    {
        TexturedColoredVertex(vec3 _position, vec3 _color, vec2 _uv)
                : position(_position), color(_color), uv(_uv) {}

        vec3 position;
        vec3 color;
        vec2 uv;
    };

    // Textured Cube model
    const TexturedColoredVertex texturedCubeVertexArray[] = {  // position,                            color
            TexturedColoredVertex(vec3(-0.5f,-0.5f,-0.5f), vec3(1.0f, 0.0f, 0.0f), vec2(0.0f, 0.0f)), //left - red
            TexturedColoredVertex(vec3(-0.5f,-0.5f, 0.5f), vec3(1.0f, 0.0f, 0.0f), vec2(0.0f, 1.0f)),
            TexturedColoredVertex(vec3(-0.5f, 0.5f, 0.5f), vec3(1.0f, 0.0f, 0.0f), vec2(1.0f, 1.0f)),

            TexturedColoredVertex(vec3(-0.5f,-0.5f,-0.5f), vec3(1.0f, 0.0f, 0.0f), vec2(0.0f, 0.0f)),
            TexturedColoredVertex(vec3(-0.5f, 0.5f, 0.5f), vec3(1.0f, 0.0f, 0.0f), vec2(1.0f, 1.0f)),
            TexturedColoredVertex(vec3(-0.5f, 0.5f,-0.5f), vec3(1.0f, 0.0f, 0.0f), vec2(1.0f, 0.0f)),

            TexturedColoredVertex(vec3(0.5f, 0.5f,-0.5f), vec3(0.0f, 0.0f, 1.0f), vec2(1.0f, 1.0f)), // far - blue
            TexturedColoredVertex(vec3(-0.5f,-0.5f,-0.5f), vec3(0.0f, 0.0f, 1.0f), vec2(0.0f, 0.0f)),
            TexturedColoredVertex(vec3(-0.5f, 0.5f,-0.5f), vec3(0.0f, 0.0f, 1.0f), vec2(0.0f, 1.0f)),

            TexturedColoredVertex(vec3(0.5f, 0.5f,-0.5f), vec3(0.0f, 0.0f, 1.0f), vec2(1.0f, 1.0f)),
            TexturedColoredVertex(vec3(0.5f,-0.5f,-0.5f), vec3(0.0f, 0.0f, 1.0f), vec2(1.0f, 0.0f)),
            TexturedColoredVertex(vec3(-0.5f,-0.5f,-0.5f), vec3(0.0f, 0.0f, 1.0f), vec2(0.0f, 0.0f)),

            TexturedColoredVertex(vec3(0.5f,-0.5f, 0.5f), vec3(0.0f, 1.0f, 1.0f), vec2(1.0f, 1.0f)), // bottom - turquoise
            TexturedColoredVertex(vec3(-0.5f,-0.5f,-0.5f), vec3(0.0f, 1.0f, 1.0f), vec2(0.0f, 0.0f)),
            TexturedColoredVertex(vec3(0.5f,-0.5f,-0.5f), vec3(0.0f, 1.0f, 1.0f), vec2(1.0f, 0.0f)),

            TexturedColoredVertex(vec3(0.5f,-0.5f, 0.5f), vec3(0.0f, 1.0f, 1.0f), vec2(1.0f, 1.0f)),
            TexturedColoredVertex(vec3(-0.5f,-0.5f, 0.5f), vec3(0.0f, 1.0f, 1.0f), vec2(0.0f, 1.0f)),
            TexturedColoredVertex(vec3(-0.5f,-0.5f,-0.5f), vec3(0.0f, 1.0f, 1.0f), vec2(0.0f, 0.0f)),

            TexturedColoredVertex(vec3(-0.5f, 0.5f, 0.5f), vec3(0.0f, 1.0f, 0.0f), vec2(0.0f, 1.0f)), // near - green
            TexturedColoredVertex(vec3(-0.5f,-0.5f, 0.5f), vec3(0.0f, 1.0f, 0.0f), vec2(0.0f, 0.0f)),
            TexturedColoredVertex(vec3(0.5f,-0.5f, 0.5f), vec3(0.0f, 1.0f, 0.0f), vec2(1.0f, 0.0f)),

            TexturedColoredVertex(vec3(0.5f, 0.5f, 0.5f), vec3(0.0f, 1.0f, 0.0f), vec2(1.0f, 1.0f)),
            TexturedColoredVertex(vec3(-0.5f, 0.5f, 0.5f), vec3(0.0f, 1.0f, 0.0f), vec2(0.0f, 1.0f)),
            TexturedColoredVertex(vec3(0.5f,-0.5f, 0.5f), vec3(0.0f, 1.0f, 0.0f), vec2(1.0f, 0.0f)),

            TexturedColoredVertex(vec3(0.5f, 0.5f, 0.5f), vec3(1.0f, 0.0f, 1.0f), vec2(1.0f, 1.0f)), // right - purple
            TexturedColoredVertex(vec3(0.5f,-0.5f,-0.5f), vec3(1.0f, 0.0f, 1.0f), vec2(0.0f, 0.0f)),
            TexturedColoredVertex(vec3(0.5f, 0.5f,-0.5f), vec3(1.0f, 0.0f, 1.0f), vec2(1.0f, 0.0f)),

            TexturedColoredVertex(vec3(0.5f,-0.5f,-0.5f), vec3(1.0f, 0.0f, 1.0f), vec2(0.0f, 0.0f)),
            TexturedColoredVertex(vec3(0.5f, 0.5f, 0.5f), vec3(1.0f, 0.0f, 1.0f), vec2(1.0f, 1.0f)),
            TexturedColoredVertex(vec3(0.5f,-0.5f, 0.5f), vec3(1.0f, 0.0f, 1.0f), vec2(0.0f, 1.0f)),

            TexturedColoredVertex(vec3(0.5f, 0.5f, 0.5f), vec3(1.0f, 1.0f, 0.0f), vec2(1.0f, 1.0f)), // top - yellow
            TexturedColoredVertex(vec3(0.5f, 0.5f,-0.5f), vec3(1.0f, 1.0f, 0.0f), vec2(1.0f, 0.0f)),
            TexturedColoredVertex(vec3(-0.5f, 0.5f,-0.5f), vec3(1.0f, 1.0f, 0.0f), vec2(0.0f, 0.0f)),

            TexturedColoredVertex(vec3(0.5f, 0.5f, 0.5f), vec3(1.0f, 1.0f, 0.0f), vec2(1.0f, 1.0f)),
            TexturedColoredVertex(vec3(-0.5f, 0.5f,-0.5f), vec3(1.0f, 1.0f, 0.0f), vec2(0.0f, 0.0f)),
            TexturedColoredVertex(vec3(-0.5f, 0.5f, 0.5f), vec3(1.0f, 1.0f, 0.0f), vec2(0.0f, 1.0f))
    };
    GLuint vertexArrayObject;
    glGenVertexArrays(1, &vertexArrayObject);
    glBindVertexArray(vertexArrayObject);

    // Upload Vertex Buffer to the GPU, keep a reference to it (vertexBufferObject)
    GLuint vertexBufferObject;
    glGenBuffers(1, &vertexBufferObject);
    glBindBuffer(GL_ARRAY_BUFFER, vertexBufferObject);
    glBufferData(GL_ARRAY_BUFFER, sizeof(texturedCubeVertexArray), texturedCubeVertexArray, GL_STATIC_DRAW);

    glVertexAttribPointer(0,                   // attribute 0 matches aPos in Vertex Shader
                          3,                   // size
                          GL_FLOAT,            // type
                          GL_FALSE,            // normalized?
                          sizeof(TexturedColoredVertex), // stride - each vertex contain 2 vec3 (position, color)
                          (void*)0             // array buffer offset
    );
    glEnableVertexAttribArray(0);


    glVertexAttribPointer(1,                            // attribute 1 matches aColor in Vertex Shader
                          3,
                          GL_FLOAT,
                          GL_FALSE,
                          sizeof(TexturedColoredVertex),
                          (void*)sizeof(vec3)      // color is offseted a vec3 (comes after position)
    );
    glEnableVertexAttribArray(1);

    glVertexAttribPointer(2,                            // attribute 2 matches aUV in Vertex Shader
                          2,
                          GL_FLOAT,
                          GL_FALSE,
                          sizeof(TexturedColoredVertex),
                          (void*)(2 * sizeof(vec3))      // uv is offseted by 2 vec3 (comes after position and color)
    );
    glEnableVertexAttribArray(2);

    return vertexArrayObject;
}

