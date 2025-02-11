#include "CAD/Transformation.hh"
#include <glad/glad.h>
// DO NOT REMOVE THIS COMMENT. IDIOTIC SORTING FORMAT WILL MOVE glad.h after glfw3 and screw up the build.
// What genius made the headers order-dependent?

#include <GLFW/glfw3.h>

#include <glm/glm.hpp>
#include <glm/gtc/matrix_transform.hpp>
#include <glm/gtc/type_ptr.hpp>
#include <glm/gtx/string_cast.hpp>

#include "opengl/Canvas.hh"
#include "opengl/Shader.hh"
#include "opengl/Style.hh"
void Transformation::init() {
  // vector of origin points
  shape->init();
}

void Transformation::render() {
  // apply the transform
  Shader* shader = Shader::useShader(style->getShaderIndex());
  Camera* camera = c->getCamera();
  const glm::mat4 mvp = camera->getViewProjection() * transform;
  shader->setMat4("projection", mvp);
  // shader->setMat4("projection",*(parentCanvas->getProjection()));
  std::cout << "Transform Matrix: " << glm::to_string(mvp) << std::endl;
  // unsigned int transformLocation = glGetUniformLocation(shader->getID(),
  // "transform"); glUniformMatrix4fv(transformLocation, 1, GL_FALSE,
  // glm::value_ptr(transform));
  shader->setVec4("solidColor", style->getFgColor());
  shape->render();
}

void Transformation::update() {}