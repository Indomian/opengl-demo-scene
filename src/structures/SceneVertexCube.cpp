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


Vertex::Vertex(float x1, float y1, float z1) {
    values[0] = x1;
    values[1] = y1;
    values[2] = z1;
}


float Vertex::component(const int index) {
    return values[index];
}

Triangle::Triangle() {
    vertexes = new Vertex*[3];
    vertexes[0] = new Vertex(-1.0f, 1.0f, 1.0f);
    vertexes[1] = new Vertex(1.0f, 1.0f, 1.0f);
    vertexes[2] = new Vertex(1.0f, -1.0f, 1.0f);
}


Triangle::Triangle(Vertex *a,Vertex *b,Vertex *c) {
    vertexes = new Vertex*[3];
    vertexes[0] = a;
    vertexes[1] = b;
    vertexes[2] = c;
}


Triangle::Triangle(float x1, float y1, float z1, float x2, float y2, float z2, float x3, float y3, float z3) {
    vertexes = new Vertex*[3];
    vertexes[0] = new Vertex(x1, y1, z1);
    vertexes[1] = new Vertex(x2, y2, z2);
    vertexes[2] = new Vertex(x3, y3, z3);
}


Triangle::~Triangle() {
    delete[] vertexes;
}


int Triangle::list_vertexes(GLfloat *result, const int from) {
    for (int i = 0; i < 3; i++) {
        for (int j = 0; j < 3; j++) {
            result[from + i * 3 + j] = vertexes[i]->component(j);
        }
    }

    return from + 9;
}


SceneVertexCube::SceneVertexCube() {
    vertexes = new GLfloat[VERTEXES_COUNT * VERTEX_SIZE];

    int pos = 0;

    pos = Triangle(1.0f, 1.0f, -1.0f, 1.0f, 1.0f, 1.0f, 1.0f, -1.0f, 1.0f).list_vertexes(vertexes, pos);
    pos = Triangle(1.0f, -1.0f, 1.0f, 1.0f, -1.0f, -1.0f, 1.0f, 1.0f, -1.0f).list_vertexes(vertexes, pos);

    pos = Triangle(-1.0f, 1.0f, -1.0f, -1.0f, -1.0f, 1.0f, -1.0f, 1.0f, 1.0f).list_vertexes(vertexes, pos);
    pos = Triangle(-1.0f, -1.0f, 1.0f, -1.0f, 1.0f, -1.0f, -1.0f, -1.0f, -1.0f).list_vertexes(vertexes, pos);

    pos = Triangle(-1.0f, 1.0f, -1.0f, -1.0f, 1.0f, 1.0f, 1.0f, 1.0f, 1.0f).list_vertexes(vertexes, pos);
    pos = Triangle(1.0f, 1.0f, 1.0f, 1.0f, 1.0f, -1.0f, -1.0f, 1.0f, -1.0f).list_vertexes(vertexes, pos);

    pos = Triangle(-1.0f, -1.0f, -1.0f, 1.0f, -1.0f, 1.0f, -1.0f, -1.0f, 1.0f).list_vertexes(vertexes, pos);
    pos = Triangle(1.0f, -1.0f, 1.0f, -1.0f, -1.0f, -1.0f, 1.0f, -1.0f, -1.0f).list_vertexes(vertexes, pos);

    pos = Triangle(-1.0f, 1.0f, -1.0f, 1.0f, 1.0f, -1.0f, 1.0f, -1.0f, -1.0f).list_vertexes(vertexes, pos);
    pos = Triangle(1.0f, -1.0f, -1.0f, -1.0f, -1.0f, -1.0f, -1.0f, 1.0f, -1.0f).list_vertexes(vertexes, pos);

    pos = Triangle(-1.0f, 1.0f, 1.0f, 1.0f, -1.0f, 1.0f, 1.0f, 1.0f, 1.0f).list_vertexes(vertexes, pos);
    pos = Triangle(1.0f, -1.0f, 1.0f, -1.0f, 1.0f, 1.0f, -1.0f, -1.0f, 1.0f).list_vertexes(vertexes, pos);

    glGenBuffers(1, &vbo);
    glBindBuffer(GL_ARRAY_BUFFER, vbo);
    glBufferData(
            GL_ARRAY_BUFFER, sizeof(GLfloat) * VERTEXES_COUNT * VERTEX_SIZE,
            vertexes, GL_STATIC_DRAW
    );

    glGenVertexArrays(1, &vao);
    glBindVertexArray(vao);
    glBindBuffer(GL_ARRAY_BUFFER, vbo);

    glVertexAttribPointer(0, 3, GL_FLOAT, GL_FALSE, 0, nullptr);

    glBindBuffer(GL_ARRAY_BUFFER, 0); // unbind VBO
    glBindVertexArray(0); // unbind VAO

    shader = new Shader("shaders/default.vert", "shaders/default.frag");

    projection_matrix = glGetUniformLocation(shader->Program, "MVP");

    glyph_shader = new Shader("shaders/glyphs.vert", "shaders/glyphs.frag");

    ch = new Character('A');
}

SceneVertexCube::~SceneVertexCube() {
    glDeleteVertexArrays(1, &vao);
    glDeleteBuffers(1, &vbo);
    delete[] vertexes;
    delete shader;
}

void SceneVertexCube::render() {
    // поле зрения (Field of View, FOV) в 80 градусов
// отношение w:h = 4:3
// видим между 1 и 100 единицами перед собой
    glm::mat4 P = glm::perspective(80.0f, 4.0f / 3.0f, 1.0f, 100.0f);

// камера находится в точке (0,0,5)
// она смотрит на точку (0,0,0)
// вектор, идущий из центра камеры к ее верху, равен (0, 1, 0)
// то есть, камера расположена горизонтально
    glm::mat4 V = glm::lookAt(glm::vec3(0, 0, 7), glm::vec3(0, 0, 0),
                              glm::vec3(0, 1, 0));

// модель повернута относительно оси OY на 30 градусов
// по часовой стрелке, если смотреть вдоль этой оси
    glm::mat4 M = glm::rotate(xrf, glm::vec3(1.0f, 0.0f, 0.0f)) *
                  glm::rotate(yrf, glm::vec3(0.0f, 1.0f, 0.0f)) *
                  glm::rotate(zrf, glm::vec3(0.0f, 0.0f, 1.0f));

    glm::mat4 MVP = P * V * M;

    glUniformMatrix4fv(projection_matrix, 1, GL_FALSE, &MVP[0][0]);

    shader->Use();
    glBindVertexArray(vao);
    glEnableVertexAttribArray(0);
    glDrawArrays(GL_TRIANGLES, 0, VERTEXES_COUNT);

    // glyph_shader->Use();
    //ch->Render(glyph_shader, 0.0f, 0.0f, 0.1f, glm::vec3(0.5f, 0.8f, 0.2f));
}

void SceneVertexCube::tick(double dt) {
    float speed = 90;
    if (xrf < 360) {
        xrf += speed * dt;
    } else {
        if (yrf < 360) {
            yrf += speed * dt;
        } else {
            if (zrf < 360) {
                zrf += speed * dt;
            } else {
                xrf = 0.0f;
                yrf = 0.0f;
                zrf = 0.0f;
            }
        }
    }
}

