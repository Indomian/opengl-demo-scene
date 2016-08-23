//
// Created by ebolgov on 23.08.16.
//

#ifndef DEMO_SCENE_SCENEVERTEXCUBE_H
#define DEMO_SCENE_SCENEVERTEXCUBE_H


#include <GLES2/gl2.h>
#include "BaseScene.h"

class SceneVertexCube: public BaseScene {
private:
    const static int VERTEXES_COUNT = 9;
    GLuint vbo;
    GLuint vao;

    GLfloat* vertexes;

    Shader* shader;
public:
    SceneVertexCube();
    ~SceneVertexCube();
    void render();
    void tick(double dt);
};


#endif //DEMO_SCENE_SCENEVERTEXCUBE_H
