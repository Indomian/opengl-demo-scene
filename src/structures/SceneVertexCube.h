//
// Created by ebolgov on 23.08.16.
//

#ifndef DEMO_SCENE_SCENEVERTEXCUBE_H
#define DEMO_SCENE_SCENEVERTEXCUBE_H


#include <GLES2/gl2.h>
#include <application/Character.h>
#include "BaseScene.h"

class Vertex {
private:
    float values[3];
public:
    Vertex(float, float, float);
    float component(const int);
};

class Triangle {
private:
    Vertex** vertexes;
public:
    Triangle();
    Triangle(Vertex* a,Vertex* b,Vertex* c);
    Triangle(float, float, float, float, float, float, float, float, float);
    ~Triangle();
    int list_vertexes(GLfloat*, const int);
};

class SceneVertexCube: public BaseScene {
private:
    const static int VERTEX_SIZE = 3;
    const static int VERTEXES_COUNT = 36;
    GLuint vbo;
    GLuint vao;

    GLfloat* vertexes;

    float xrf, yrf, zrf;

    GLint projection_matrix;

    Shader* shader;
    Shader *glyph_shader;
    Character *ch;
public:
    SceneVertexCube();
    ~SceneVertexCube();
    void render();
    void tick(double dt);
};


#endif //DEMO_SCENE_SCENEVERTEXCUBE_H
