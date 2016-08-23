//
// Created by ebolgov on 23.08.16.
//

#include <GL/glew.h>
#include <application/Shader.h>
#include <glm/vec3.hpp>
#include <glm/glm.hpp>
#include <glm/gtc/type_ptr.hpp>
#include <glm/gtx/transform.hpp>
#include "SceneVertexCube.h"


SceneVertexCube::SceneVertexCube() {
    vertexes = new GLfloat[VERTEXES_COUNT];
    vertexes[0] = -1.0f; vertexes[1] = -1.0f; vertexes[2] = 0.0f;
    vertexes[3] = 1.0f;  vertexes[4] = -1.0f; vertexes[5] = 0.0f;
    vertexes[6] = 0.0f;  vertexes[7] =  1.0f; vertexes[8] = 0.0f;


    glGenBuffers(1, &vbo);
    glBindBuffer(GL_ARRAY_BUFFER, vbo);
    glBufferData(
            GL_ARRAY_BUFFER, sizeof(GLfloat) * VERTEXES_COUNT,
            vertexes, GL_STATIC_DRAW
    );

    glGenVertexArrays(1, &vao);
    glBindVertexArray(vao);
    glBindBuffer(GL_ARRAY_BUFFER, vbo);

    glVertexAttribPointer(0, 3, GL_FLOAT, GL_FALSE, 0, nullptr);

    glBindBuffer(GL_ARRAY_BUFFER, 0); // unbind VBO
    glBindVertexArray(0); // unbind VAO

    shader = new Shader("shaders/default.vert", "shaders/default.frag");
}

SceneVertexCube::~SceneVertexCube() {
    glDeleteVertexArrays(1, &vao);
    glDeleteBuffers(1, &vbo);
    delete[] vertexes;
    delete shader;
}

void SceneVertexCube::render() {
    shader->Use();
    glBindVertexArray(vao);
    glEnableVertexAttribArray(0);
    glDrawArrays(GL_TRIANGLES, 0, 3);
    glDisableVertexAttribArray(0);
}

void SceneVertexCube::tick(double dt) {
    // поле зрения (Field of View, FOV) в 80 градусов
// отношение w:h = 4:3
// видим между 1 и 100 единицами перед собой
    glm::mat4 P = glm::perspective(80.0f, 4.0f / 3.0f, 1.0f, 100.0f);

// камера находится в точке (0,0,5)
// она смотрит на точку (0,0,0)
// вектор, идущий из центра камеры к ее верху, равен (0, 1, 0)
// то есть, камера расположена горизонтально
    glm::mat4 V = glm::lookAt(glm::vec3(0, 0, 5), glm::vec3(0, 0, 0),
                              glm::vec3(0, 1, 0));

// модель повернута относительно оси OY на 30 градусов
// по часовой стрелке, если смотреть вдоль этой оси
    glm::mat4 M = glm::rotate(30.0f, glm::vec3(0.0f, 1.0f, 0.0f));

    glm::mat4 MVP = P * V * M;

    GLint matrixId = glGetUniformLocation(shader->Program, "MVP");
    glUniformMatrix4fv(matrixId, 1, GL_FALSE, &MVP[0][0]);
}

