#include "application_solar.hpp"
#include "window_handler.hpp"

#include "utils.hpp"
#include "shader_loader.hpp"
#include "model_loader.hpp"

#include <glbinding/gl/gl.h>
// use gl definitions from glbinding 
using namespace gl;

//dont load gl bindings from glfw
#define GLFW_INCLUDE_NONE
#include <GLFW/glfw3.h>

#include <glm/gtc/matrix_transform.hpp>
#include <glm/gtc/matrix_inverse.hpp>
#include <glm/gtc/type_ptr.hpp>

#include <iostream>

ApplicationSolar::ApplicationSolar(std::string const& resource_path)
 :Application{resource_path}
 ,planet_object{}
 ,m_view_transform{glm::translate(glm::fmat4{}, glm::fvec3{0.0f, 0.0f, 4.0f})}
 ,m_view_projection{utils::calculate_projection_matrix(initial_aspect_ratio)}
{
  initializeSceneGraph();
  initializeGeometry();
  initializeShaderPrograms();
}

ApplicationSolar::~ApplicationSolar() {
  glDeleteBuffers(1, &planet_object.vertex_BO);
  glDeleteBuffers(1, &planet_object.element_BO);
  glDeleteVertexArrays(1, &planet_object.vertex_AO);
}

void ApplicationSolar::initializeSceneGraph() {

  //Create root node and make it a shared ptr
  auto raum = std::make_shared<Node>(nullptr, "root");
  raum_ = raum;
  //Initialize Scenegraph with root node
  Scenegraph solar_system("Solarium", raum);

  model planet_model = model_loader::obj(m_resource_path + "models/sphere.obj", model::NORMAL);
  
  //Create vector to store children of planet 
  std::vector<std::shared_ptr<Node>> laFerrariChildren;
  //Create Planet holder
  auto laFerrariHolder = std::make_shared<Node>(raum, laFerrariChildren, "La Ferrari", "root->laFerrari", 1);
  //Connect Planet with root node
  raum->addChild(laFerrariHolder);
  //Create GeometryNode of Planet
  auto laFerrari = std::make_shared<GeometryNode>(laFerrariHolder, "La Ferrari Geometry");
  //Connect Geometry Node with Planet holder
  laFerrariHolder->addChild(laFerrari);
  //Assign Planet Model to Geometry Node member variable
  laFerrari->setGeometry(planet_model);

  std::vector<std::shared_ptr<Node>> astonMartinOneChildren;
  auto astonMartinOneHolder = std::make_shared<Node>(raum, astonMartinOneChildren, "Aston Martin One-77", "root->astonMartinOne", 1);
  raum->addChild(astonMartinOneHolder);
  auto astonMartinOne = std::make_shared<GeometryNode>(astonMartinOneHolder, "Aston Martin One-77 Geometry");
  astonMartinOneHolder->addChild(astonMartinOne);
  astonMartinOne->setGeometry(planet_model);

  std::vector<std::shared_ptr<Node>> koenigsEggOneChildren;
  auto koenigsEggOneHolder = std::make_shared<Node>(raum, koenigsEggOneChildren, "Koenigsegg One", "root->koenigsEggOne", 1);
  raum->addChild(koenigsEggOneHolder);
  auto koenigsEggOne = std::make_shared<GeometryNode>(koenigsEggOneHolder, "Koenigsegg One Geometry");
  koenigsEggOneHolder->addChild(koenigsEggOne);
  koenigsEggOne->setGeometry(planet_model);

  std::vector<std::shared_ptr<Node>> ferrariFAmericaChildren;
  auto ferrariFAmericaHolder = std::make_shared<Node>(raum, ferrariFAmericaChildren, "Ferrari F60 America", "root->ferrariFAmerica", 1);
  raum->addChild(ferrariFAmericaHolder);
  auto ferrariFAmerica = std::make_shared<GeometryNode>(ferrariFAmericaHolder, "Ferrari F60 America Geometry");
  ferrariFAmericaHolder->addChild(ferrariFAmerica);
  ferrariFAmerica->setGeometry(planet_model);

  std::vector<std::shared_ptr<Node>> bugattiVeyronChildren;
  auto bugattiVeyronHolder = std::make_shared<Node>(raum, bugattiVeyronChildren, "Mansory Vivere Bugatti Veyron", "root->bugattiVeyron", 1);
  raum->addChild(bugattiVeyronHolder);
  auto bugattiVeyron = std::make_shared<GeometryNode>(bugattiVeyronHolder, "Mansory Vivere Bugatti Veyron Geometry");
  bugattiVeyronHolder->addChild(bugattiVeyron);
  bugattiVeyron->setGeometry(planet_model);

  //Das Zentrum des Universums, das Licht alles Lebens, die Sonne, ich
  std::vector<std::shared_ptr<Node>> ichChildren;
  auto ichHolder = std::make_shared<Node>(raum, ichChildren, "Ich", "root->ich", 1);
  raum->addChild(ichHolder);
  auto ich = std::make_shared<GeometryNode>(ichHolder, "Ich Geometry");
  ichHolder->addChild(ich);
  ich->setGeometry(planet_model);

  std::vector<std::shared_ptr<Node>> lamborghiniVenenoChildren;
  auto lamborghiniVenenoHolder = std::make_shared<Node>(raum, lamborghiniVenenoChildren, "Lamborghini Veneno", "root->lamborghiniVeneno", 1);
  raum->addChild(lamborghiniVenenoHolder);
  auto lamborghiniVeneno = std::make_shared<GeometryNode>(lamborghiniVenenoHolder, "Lamborghini Veneno Geometry");
  lamborghiniVenenoHolder->addChild(lamborghiniVeneno);
  lamborghiniVeneno->setGeometry(planet_model);

  std::vector<std::shared_ptr<Node>> rollsRoycePhantomChildren;
  auto rollsRoycePhantomHolder = std::make_shared<Node>(raum, rollsRoycePhantomChildren, "Rolls Royce Phantom", "root->rollsRoycePhantom", 1);
  raum->addChild(rollsRoycePhantomHolder);
  auto rollsRoycePhantom = std::make_shared<GeometryNode>(rollsRoycePhantomHolder, "Rolls Royce Phantom Geometry");
  rollsRoycePhantomHolder->addChild(rollsRoycePhantom);
  rollsRoycePhantom->setGeometry(planet_model);

  std::vector<std::shared_ptr<Node>> mercedesMaybachChildren;
  auto mercedesMaybachHolder = std::make_shared<Node>(raum, mercedesMaybachChildren, "Mercedes Maybach S650", "root->mercedesMaybach", 1);
  raum->addChild(mercedesMaybachHolder);
  auto mercedesMaybach = std::make_shared<GeometryNode>(mercedesMaybachHolder, "Mercedes Maybach S650 Geometry");
  mercedesMaybachHolder->addChild(mercedesMaybach);
  mercedesMaybach->setGeometry(planet_model);

  std::vector<std::shared_ptr<Node>> bentleyFlyingSpurChildren;
  auto bentleyFlyingSpurHolder = std::make_shared<Node>(raum, bentleyFlyingSpurChildren, "Bentley Flying Spur", "root->bentleyFlyingSpur", 1);
  mercedesMaybachHolder->addChild(bentleyFlyingSpurHolder);
  auto bentleyFlyingSpur = std::make_shared<GeometryNode>(bentleyFlyingSpurHolder, "Bentley Flying Spur Geometry");
  bentleyFlyingSpurHolder->addChild(bentleyFlyingSpur);
  bentleyFlyingSpur->setGeometry(planet_model); 


  //Add all Geometry Node to the List
  geomList.push_back(laFerrari);
  geomList.push_back(astonMartinOne);
  geomList.push_back(koenigsEggOne);
  geomList.push_back(ferrariFAmerica);
  geomList.push_back(bugattiVeyron);
  geomList.push_back(ich);
  geomList.push_back(lamborghiniVeneno);
  geomList.push_back(rollsRoycePhantom);
  geomList.push_back(mercedesMaybach);
  geomList.push_back(bentleyFlyingSpur);

}

void ApplicationSolar::render() const {
  renderPlanets();
}

void ApplicationSolar::renderPlanets() const {

  for(auto& planet : geomList) {
      // bind shader to upload uniforms
      if(planet->getName() == "Ich Geometry"){

        glUseProgram(m_shaders.at("planet").handle);

        planet->setWorldTransform(glm::rotate(glm::fmat4{}, float(glfwGetTime()), glm::fvec3{0.0f, 1.0f, 0.0f}));
        planet->setWorldTransform(glm::scale(planet->getWorldTransform(), glm::fvec3{2.0f}));
        glUniformMatrix4fv(m_shaders.at("planet").u_locs.at("ModelMatrix"),
                          1, GL_FALSE, glm::value_ptr(planet->getWorldTransform()));

        // extra matrix for normal transformation to keep them orthogonal to surface
        planet->setLocalTransform(glm::inverseTranspose(glm::inverse(m_view_transform) * planet->getWorldTransform()));
        glUniformMatrix4fv(m_shaders.at("planet").u_locs.at("NormalMatrix"),
                          1, GL_FALSE, glm::value_ptr(planet->getLocalTransform()));

        // bind the VAO to draw
        glBindVertexArray(planet_object.vertex_AO);

        // draw bound vertex array using bound shader
        glDrawElements(planet_object.draw_mode, planet_object.num_elements, model::INDEX.type, NULL);

      }

      else if(planet->getName() == "Aston Martin One-77 Geometry"){

        glUseProgram(m_shaders.at("planet").handle);

        planet->setWorldTransform(glm::rotate(glm::fmat4{}, float(glfwGetTime()), glm::fvec3{0.0f, 1.0f, 0.0f}));
        planet->setWorldTransform(glm::translate(planet->getWorldTransform(), glm::fvec3{0.0f, 0.0f, 3.0f}));
        glUniformMatrix4fv(m_shaders.at("planet").u_locs.at("ModelMatrix"),
                          1, GL_FALSE, glm::value_ptr(planet->getWorldTransform()));

        // extra matrix for normal transformation to keep them orthogonal to surface
        planet->setLocalTransform(glm::inverseTranspose(glm::inverse(m_view_transform) * planet->getWorldTransform()));
        glUniformMatrix4fv(m_shaders.at("planet").u_locs.at("NormalMatrix"),
                          1, GL_FALSE, glm::value_ptr(planet->getLocalTransform()));

        // bind the VAO to draw
        glBindVertexArray(planet_object.vertex_AO);

        // draw bound vertex array using bound shader
        glDrawElements(planet_object.draw_mode, planet_object.num_elements, model::INDEX.type, NULL);

      }

      else if(planet->getName() == "Koenigsegg One Geometry"){

        glUseProgram(m_shaders.at("planet").handle);

        planet->setWorldTransform(glm::rotate(glm::fmat4{}, float(glfwGetTime())*1.5f, glm::fvec3{0.0f, 1.0f, 0.0f}));
        planet->setWorldTransform(glm::translate(planet->getWorldTransform(), glm::fvec3{0.0f, 0.0f, 5.0f}));
        glUniformMatrix4fv(m_shaders.at("planet").u_locs.at("ModelMatrix"),
                          1, GL_FALSE, glm::value_ptr(planet->getWorldTransform()));

        // extra matrix for normal transformation to keep them orthogonal to surface
        planet->setLocalTransform(glm::inverseTranspose(glm::inverse(m_view_transform) * planet->getWorldTransform()));
        glUniformMatrix4fv(m_shaders.at("planet").u_locs.at("NormalMatrix"),
                          1, GL_FALSE, glm::value_ptr(planet->getLocalTransform()));

        // bind the VAO to draw
        glBindVertexArray(planet_object.vertex_AO);

        // draw bound vertex array using bound shader
        glDrawElements(planet_object.draw_mode, planet_object.num_elements, model::INDEX.type, NULL);

      }

      else if(planet->getName() == "Ferrari F60 America Geometry"){

        glUseProgram(m_shaders.at("planet").handle);

        planet->setWorldTransform(glm::rotate(glm::fmat4{}, float(glfwGetTime())*2.0f, glm::fvec3{0.0f, 1.0f, 0.0f}));
        planet->setWorldTransform(glm::translate(planet->getWorldTransform(), glm::fvec3{0.0f, 0.0f, 8.0f}));
        glUniformMatrix4fv(m_shaders.at("planet").u_locs.at("ModelMatrix"),
                          1, GL_FALSE, glm::value_ptr(planet->getWorldTransform()));

        // extra matrix for normal transformation to keep them orthogonal to surface
        planet->setLocalTransform(glm::inverseTranspose(glm::inverse(m_view_transform) * planet->getWorldTransform()));
        glUniformMatrix4fv(m_shaders.at("planet").u_locs.at("NormalMatrix"),
                          1, GL_FALSE, glm::value_ptr(planet->getLocalTransform()));

        // bind the VAO to draw
        glBindVertexArray(planet_object.vertex_AO);

        // draw bound vertex array using bound shader
        glDrawElements(planet_object.draw_mode, planet_object.num_elements, model::INDEX.type, NULL);

      }

      else if(planet->getName() == "Mansory Vivere Bugatti Veyron Geometry"){

        glUseProgram(m_shaders.at("planet").handle);

        planet->setWorldTransform(glm::rotate(glm::fmat4{}, float(glfwGetTime())*2.5f, glm::fvec3{0.0f, 1.0f, 0.0f}));
        planet->setWorldTransform(glm::translate(planet->getWorldTransform(), glm::fvec3{0.0f, 0.0f, 11.0f}));
        glUniformMatrix4fv(m_shaders.at("planet").u_locs.at("ModelMatrix"),
                          1, GL_FALSE, glm::value_ptr(planet->getWorldTransform()));

        // extra matrix for normal transformation to keep them orthogonal to surface
        planet->setLocalTransform(glm::inverseTranspose(glm::inverse(m_view_transform) * planet->getWorldTransform()));
        glUniformMatrix4fv(m_shaders.at("planet").u_locs.at("NormalMatrix"),
                          1, GL_FALSE, glm::value_ptr(planet->getLocalTransform()));

        // bind the VAO to draw
        glBindVertexArray(planet_object.vertex_AO);

        // draw bound vertex array using bound shader
        glDrawElements(planet_object.draw_mode, planet_object.num_elements, model::INDEX.type, NULL);

      }

      else if(planet->getName() == "Lamborghini Veneno Geometry"){

        glUseProgram(m_shaders.at("planet").handle);

        planet->setWorldTransform(glm::rotate(glm::fmat4{}, float(glfwGetTime())*3.0f, glm::fvec3{0.0f, 1.0f, 0.0f}));
        planet->setWorldTransform(glm::translate(planet->getWorldTransform(), glm::fvec3{0.0f, 0.0f, 14.0f}));
        glUniformMatrix4fv(m_shaders.at("planet").u_locs.at("ModelMatrix"),
                          1, GL_FALSE, glm::value_ptr(planet->getWorldTransform()));

        // extra matrix for normal transformation to keep them orthogonal to surface
        planet->setLocalTransform(glm::inverseTranspose(glm::inverse(m_view_transform) * planet->getWorldTransform()));
        glUniformMatrix4fv(m_shaders.at("planet").u_locs.at("NormalMatrix"),
                          1, GL_FALSE, glm::value_ptr(planet->getLocalTransform()));

        // bind the VAO to draw
        glBindVertexArray(planet_object.vertex_AO);

        // draw bound vertex array using bound shader
        glDrawElements(planet_object.draw_mode, planet_object.num_elements, model::INDEX.type, NULL);

      }

      else if(planet->getName() == "Rolls Royce Phantom Geometry"){

        glUseProgram(m_shaders.at("planet").handle);

        planet->setWorldTransform(glm::rotate(glm::fmat4{}, float(glfwGetTime())*3.5f, glm::fvec3{0.0f, 1.0f, 0.0f}));
        planet->setWorldTransform(glm::translate(planet->getWorldTransform(), glm::fvec3{0.0f, 0.0f, 17.0f}));
        glUniformMatrix4fv(m_shaders.at("planet").u_locs.at("ModelMatrix"),
                          1, GL_FALSE, glm::value_ptr(planet->getWorldTransform()));

        // extra matrix for normal transformation to keep them orthogonal to surface
        planet->setLocalTransform(glm::inverseTranspose(glm::inverse(m_view_transform) * planet->getWorldTransform()));
        glUniformMatrix4fv(m_shaders.at("planet").u_locs.at("NormalMatrix"),
                          1, GL_FALSE, glm::value_ptr(planet->getLocalTransform()));

        // bind the VAO to draw
        glBindVertexArray(planet_object.vertex_AO);

        // draw bound vertex array using bound shader
        glDrawElements(planet_object.draw_mode, planet_object.num_elements, model::INDEX.type, NULL);

      }

      else if(planet->getName() == "Mercedes Maybach S650 Geometry"){

        glUseProgram(m_shaders.at("planet").handle);

        planet->setWorldTransform(glm::rotate(glm::fmat4{}, float(glfwGetTime())*4, glm::fvec3{0.0f, 1.0f, 0.0f}));
        planet->setWorldTransform(glm::translate(planet->getWorldTransform(), glm::fvec3{0.0f, 0.0f, 20.0f}));
        glUniformMatrix4fv(m_shaders.at("planet").u_locs.at("ModelMatrix"),
                          1, GL_FALSE, glm::value_ptr(planet->getWorldTransform()));

        // extra matrix for normal transformation to keep them orthogonal to surface
        planet->setLocalTransform(glm::inverseTranspose(glm::inverse(m_view_transform) * planet->getWorldTransform()));
        glUniformMatrix4fv(m_shaders.at("planet").u_locs.at("NormalMatrix"),
                          1, GL_FALSE, glm::value_ptr(planet->getLocalTransform()));

        // bind the VAO to draw
        glBindVertexArray(planet_object.vertex_AO);

        // draw bound vertex array using bound shader
        glDrawElements(planet_object.draw_mode, planet_object.num_elements, model::INDEX.type, NULL);

      }

      else if(planet->getName() == "La Ferrari Geometry"){

        glUseProgram(m_shaders.at("planet").handle);

        planet->setWorldTransform(glm::rotate(glm::fmat4{}, float(glfwGetTime())*4.5f, glm::fvec3{0.0f, 1.0f, 0.0f}));
        planet->setWorldTransform(glm::translate(planet->getWorldTransform(), glm::fvec3{0.0f, 0.0f, 23.0f}));
        glUniformMatrix4fv(m_shaders.at("planet").u_locs.at("ModelMatrix"),
                          1, GL_FALSE, glm::value_ptr(planet->getWorldTransform()));

        // extra matrix for normal transformation to keep them orthogonal to surface
        planet->setLocalTransform(glm::inverseTranspose(glm::inverse(m_view_transform) * planet->getWorldTransform()));
        glUniformMatrix4fv(m_shaders.at("planet").u_locs.at("NormalMatrix"),
                          1, GL_FALSE, glm::value_ptr(planet->getLocalTransform()));

        // bind the VAO to draw
        glBindVertexArray(planet_object.vertex_AO);

        // draw bound vertex array using bound shader
        glDrawElements(planet_object.draw_mode, planet_object.num_elements, model::INDEX.type, NULL);

      }

      else if(planet->getName() == "Bentley Flying Spur Geometry"){

        glUseProgram(m_shaders.at("planet").handle);

        planet->setWorldTransform(glm::rotate(glm::fmat4{}, float(glfwGetTime())*4.5f, glm::fvec3{0.0f, 1.0f, 0.0f}));
        planet->setWorldTransform(glm::translate(planet->getWorldTransform(), glm::fvec3{0.0f, 0.0f, 3.0f}));
        glUniformMatrix4fv(m_shaders.at("planet").u_locs.at("ModelMatrix"),
                          1, GL_FALSE, glm::value_ptr(planet->getWorldTransform()));

        // extra matrix for normal transformation to keep them orthogonal to surface
        planet->setLocalTransform(glm::inverseTranspose(glm::inverse(m_view_transform) * planet->getWorldTransform()));
        glUniformMatrix4fv(m_shaders.at("planet").u_locs.at("NormalMatrix"),
                          1, GL_FALSE, glm::value_ptr(planet->getLocalTransform()));

        // bind the VAO to draw
        glBindVertexArray(planet_object.vertex_AO);

        // draw bound vertex array using bound shader
        glDrawElements(planet_object.draw_mode, planet_object.num_elements, model::INDEX.type, NULL);

      }
      
    
  }
}

void ApplicationSolar::uploadView() {
  // vertices are transformed in camera space, so camera transform must be inverted
  glm::fmat4 view_matrix = glm::inverse(m_view_transform);
  // upload matrix to gpu
  glUniformMatrix4fv(m_shaders.at("planet").u_locs.at("ViewMatrix"),
                     1, GL_FALSE, glm::value_ptr(view_matrix));glBindVertexArray(planet_object.vertex_AO);
}

void ApplicationSolar::uploadProjection() {
  // upload matrix to gpu
  glUniformMatrix4fv(m_shaders.at("planet").u_locs.at("ProjectionMatrix"),
                     1, GL_FALSE, glm::value_ptr(m_view_projection));
}

// update uniform locations
void ApplicationSolar::uploadUniforms() { 
  // bind shader to which to upload unforms
  glUseProgram(m_shaders.at("planet").handle);
  // upload uniform values to new locations
  uploadView();
  uploadProjection();
}

///////////////////////////// intialisation functions /////////////////////////
// load shader sources
void ApplicationSolar::initializeShaderPrograms() {
  // store shader program objects in container
  m_shaders.emplace("planet", shader_program{{{GL_VERTEX_SHADER,m_resource_path + "shaders/simple.vert"},
                                           {GL_FRAGMENT_SHADER, m_resource_path + "shaders/simple.frag"}}});
  // request uniform locations for shader program
  m_shaders.at("planet").u_locs["NormalMatrix"] = -1;
  m_shaders.at("planet").u_locs["ModelMatrix"] = -1;
  m_shaders.at("planet").u_locs["ViewMatrix"] = -1;
  m_shaders.at("planet").u_locs["ProjectionMatrix"] = -1;
}

// load models
void ApplicationSolar::initializeGeometry() {
  model planet_model = model_loader::obj(m_resource_path + "models/sphere.obj", model::NORMAL);

  // generate vertex array object
  glGenVertexArrays(1, &planet_object.vertex_AO);
  // bind the array for attaching buffers
  glBindVertexArray(planet_object.vertex_AO);

  // generate generic buffer
  glGenBuffers(1, &planet_object.vertex_BO);
  // bind this as an vertex array buffer containing all attributes
  glBindBuffer(GL_ARRAY_BUFFER, planet_object.vertex_BO);
  // configure currently bound array buffer
  glBufferData(GL_ARRAY_BUFFER, sizeof(float) * planet_model.data.size(), planet_model.data.data(), GL_STATIC_DRAW);

  // activate first attribute on gpu
  glEnableVertexAttribArray(0);
  // first attribute is 3 floats with no offset & stride
  glVertexAttribPointer(0, model::POSITION.components, model::POSITION.type, GL_FALSE, planet_model.vertex_bytes, planet_model.offsets[model::POSITION]);
  // activate second attribute on gpu
  glEnableVertexAttribArray(1);
  // second attribute is 3 floats with no offset & stride
  glVertexAttribPointer(1, model::NORMAL.components, model::NORMAL.type, GL_FALSE, planet_model.vertex_bytes, planet_model.offsets[model::NORMAL]);

   // generate generic buffer
  glGenBuffers(1, &planet_object.element_BO);
  // bind this as an vertex array buffer containing all attributes
  glBindBuffer(GL_ELEMENT_ARRAY_BUFFER, planet_object.element_BO);
  // configure currently bound array buffer
  glBufferData(GL_ELEMENT_ARRAY_BUFFER, model::INDEX.size * planet_model.indices.size(), planet_model.indices.data(), GL_STATIC_DRAW);

  // store type of primitive to draw
  planet_object.draw_mode = GL_TRIANGLES;
  // transfer number of indices to model object 
  planet_object.num_elements = GLsizei(planet_model.indices.size());
}

///////////////////////////// callback functions for window events ////////////
// handle key input
void ApplicationSolar::keyCallback(int key, int action, int mods) {
  if (key == GLFW_KEY_W  && (action == GLFW_PRESS || action == GLFW_REPEAT)) {
    m_view_transform = glm::translate(m_view_transform, glm::fvec3{0.0f, 0.0f, -0.1f});
    uploadView();
  }
  else if (key == GLFW_KEY_S  && (action == GLFW_PRESS || action == GLFW_REPEAT)) {
    m_view_transform = glm::translate(m_view_transform, glm::fvec3{0.0f, 0.0f, 0.1f});
    uploadView();
  }
  else if (key == GLFW_KEY_A  && (action == GLFW_PRESS || action == GLFW_REPEAT)) {
    m_view_transform = glm::translate(m_view_transform, glm::fvec3{-0.1f, 0.0f, 0.0f});
    uploadView();
  }
  else if (key == GLFW_KEY_D  && (action == GLFW_PRESS || action == GLFW_REPEAT)) {
    m_view_transform = glm::translate(m_view_transform, glm::fvec3{0.1f, 0.0f, 0.f});
    uploadView();
  }
}

//handle delta mouse movement input
void ApplicationSolar::mouseCallback(double pos_x, double pos_y) {
  // mouse handling
  m_view_transform = glm::translate(m_view_transform, glm::fvec3{pos_x * 0.01f, pos_y * -0.01f, 0.0f});
  uploadView();
}

//handle resizing
void ApplicationSolar::resizeCallback(unsigned width, unsigned height) {
  // recalculate projection matrix for new aspect ration
  m_view_projection = utils::calculate_projection_matrix(float(width) / float(height));
  // upload new projection matrix
  uploadProjection();
}


// exe entry point
int main(int argc, char* argv[]) {
  Application::run<ApplicationSolar>(argc, argv, 3, 2);
}