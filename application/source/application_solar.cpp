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
 ,star_object{}
 ,m_view_transform{glm::translate(glm::fmat4{}, glm::fvec3{0.0f, 0.0f, 4.0f})}
 ,m_view_projection{utils::calculate_projection_matrix(initial_aspect_ratio)}
{
  initializeSceneGraph();
  initializeStars();
  initializeGeometry();
  initializeShaderPrograms();
}

ApplicationSolar::~ApplicationSolar() {
  glDeleteBuffers(1, &planet_object.vertex_BO);
  glDeleteBuffers(1, &planet_object.element_BO);
  glDeleteVertexArrays(1, &planet_object.vertex_AO);
  glDeleteBuffers(1, &star_object.vertex_BO);
  glDeleteBuffers(1, &star_object.element_BO);
  glDeleteVertexArrays(1, &star_object.vertex_AO);
}

void ApplicationSolar::initializeSceneGraph() {

  //Create root node and make it a shared ptr
  auto raum = std::make_shared<Node>(nullptr, "root");
  raum_ = raum;
  //Initialize Scenegraph with root node
  Scenegraph solar_system("Solarium", raum);

  model planet_model = model_loader::obj(m_resource_path + "models/sphere.obj", model::NORMAL);
  
  //Create vector to store children of planet 
  //std::vector<std::shared_ptr<Node>> laFerrariChildren;
  //Create Planet holder
  auto laFerrariHolder = std::make_shared<Node>(raum, "La Ferrari", "root->laFerrari", 1);
  //Connect Planet with root node
  raum->addChild(laFerrariHolder);
  //set root as parent of holder
  laFerrariHolder->setParent(raum);
  //Create GeometryNode of Planet
  auto laFerrari = std::make_shared<GeometryNode>(laFerrariHolder, "La Ferrari Geometry");
  //Connect Geometry Node with Planet holder
  laFerrariHolder->addChild(laFerrari);
  //set holder as parent of geometry
  laFerrari->setParent(laFerrariHolder);
  //Assign Planet Model to Geometry Node member variable
  laFerrari->setGeometry(planet_model);
  laFerrari->setWorldTransform(glm::rotate(glm::fmat4{}, float(glfwGetTime())*0.1f, glm::fvec3{0.0f, 1.0f, 0.0f}));
  laFerrari->setWorldTransform(glm::translate(laFerrari->getWorldTransform(), glm::fvec3{0.0f, 0.0f, 23.0f}));

  //std::vector<std::shared_ptr<Node>> astonMartinOneChildren;
  auto astonMartinOneHolder = std::make_shared<Node>(raum, "Aston Martin One-77", "root->astonMartinOne", 1);
  raum->addChild(astonMartinOneHolder);
  astonMartinOneHolder->setParent(raum);
  auto astonMartinOne = std::make_shared<GeometryNode>(astonMartinOneHolder, "Aston Martin One-77 Geometry");
  astonMartinOneHolder->addChild(astonMartinOne);
  astonMartinOne->setParent(astonMartinOneHolder);
  astonMartinOne->setGeometry(planet_model);
  astonMartinOne->setWorldTransform(glm::rotate(glm::fmat4{}, float(glfwGetTime())*0.8f, glm::fvec3{0.0f, 1.0f, 0.0f}));
  astonMartinOne->setWorldTransform(glm::translate(astonMartinOne->getWorldTransform(), glm::fvec3{0.0f, 0.0f, 3.0f}));
  astonMartinOne->setLocalTransform(glm::inverseTranspose(glm::inverse(m_view_transform) * astonMartinOne->getWorldTransform()));

  //std::vector<std::shared_ptr<Node>> koenigsEggOneChildren;
  auto koenigsEggOneHolder = std::make_shared<Node>(raum, "Koenigsegg One", "root->koenigsEggOne", 1);
  raum->addChild(koenigsEggOneHolder);
  koenigsEggOneHolder->setParent(raum);
  auto koenigsEggOne = std::make_shared<GeometryNode>(koenigsEggOneHolder, "Koenigsegg One Geometry");
  koenigsEggOneHolder->addChild(koenigsEggOne);
  koenigsEggOne->setParent(koenigsEggOneHolder);
  koenigsEggOne->setGeometry(planet_model);
  koenigsEggOne->setWorldTransform(glm::rotate(glm::fmat4{}, float(glfwGetTime())*0.7f, glm::fvec3{0.0f, 1.0f, 0.0f}));
  koenigsEggOne->setWorldTransform(glm::translate(koenigsEggOne->getWorldTransform(), glm::fvec3{0.0f, 0.0f, 5.0f}));
  koenigsEggOne->setLocalTransform(glm::inverseTranspose(glm::inverse(m_view_transform) * koenigsEggOne->getWorldTransform()));

  //std::vector<std::shared_ptr<Node>> ferrariFAmericaChildren;
  auto ferrariFAmericaHolder = std::make_shared<Node>(raum, "Ferrari F60 America", "root->ferrariFAmerica", 1);
  raum->addChild(ferrariFAmericaHolder);
  ferrariFAmericaHolder->setParent(raum);
  auto ferrariFAmerica = std::make_shared<GeometryNode>(ferrariFAmericaHolder, "Ferrari F60 America Geometry");
  ferrariFAmericaHolder->addChild(ferrariFAmerica);
  ferrariFAmerica->setParent(ferrariFAmericaHolder);
  ferrariFAmerica->setGeometry(planet_model);
  ferrariFAmerica->setWorldTransform(glm::rotate(glm::fmat4{}, float(glfwGetTime())*0.6f, glm::fvec3{0.0f, 1.0f, 0.0f}));
  ferrariFAmerica->setWorldTransform(glm::translate(ferrariFAmerica->getWorldTransform(), glm::fvec3{0.0f, 0.0f, 8.0f}));
  ferrariFAmerica->setLocalTransform(glm::inverseTranspose(glm::inverse(m_view_transform) * ferrariFAmerica->getWorldTransform()));

  //std::vector<std::shared_ptr<Node>> bugattiVeyronChildren;
  auto bugattiVeyronHolder = std::make_shared<Node>(raum, "Mansory Vivere Bugatti Veyron", "root->bugattiVeyron", 1);
  raum->addChild(bugattiVeyronHolder);
  bugattiVeyronHolder->setParent(raum);
  auto bugattiVeyron = std::make_shared<GeometryNode>(bugattiVeyronHolder, "Mansory Vivere Bugatti Veyron Geometry");
  bugattiVeyronHolder->addChild(bugattiVeyron);
  bugattiVeyron->setParent(bugattiVeyronHolder);
  bugattiVeyron->setGeometry(planet_model);
  bugattiVeyron->setWorldTransform(glm::rotate(glm::fmat4{}, float(glfwGetTime())*0.5f, glm::fvec3{0.0f, 1.0f, 0.0f}));
  bugattiVeyron->setWorldTransform(glm::translate(bugattiVeyron->getWorldTransform(), glm::fvec3{0.0f, 0.0f, 11.0f}));
  bugattiVeyron->setLocalTransform(glm::inverseTranspose(glm::inverse(m_view_transform) * bugattiVeyron->getWorldTransform()));

  //Das Zentrum des Universums, das Licht alles Lebens, die Sonne, ich
  //std::vector<std::shared_ptr<Node>> ichChildren;
  auto ichHolder = std::make_shared<Node>(raum, "Ich", "root->ich", 1);
  raum->addChild(ichHolder);
  ichHolder->setParent(raum);
  auto ich = std::make_shared<GeometryNode>(ichHolder, "Ich Geometry");
  ichHolder->addChild(ich);
  ich->setParent(ichHolder);
  ich->setGeometry(planet_model);
  ich->setWorldTransform(glm::rotate(glm::fmat4{}, float(glfwGetTime()), glm::fvec3{0.0f, 1.0f, 0.0f}));
  ich->setWorldTransform(glm::scale(ich->getWorldTransform(), glm::fvec3{2.0f}));

  //std::vector<std::shared_ptr<Node>> lamborghiniVenenoChildren;
  auto lamborghiniVenenoHolder = std::make_shared<Node>(raum, "Lamborghini Veneno", "root->lamborghiniVeneno", 1);
  raum->addChild(lamborghiniVenenoHolder);
  lamborghiniVenenoHolder->setParent(raum);
  auto lamborghiniVeneno = std::make_shared<GeometryNode>(lamborghiniVenenoHolder, "Lamborghini Veneno Geometry");
  lamborghiniVenenoHolder->addChild(lamborghiniVeneno);
  lamborghiniVeneno->setParent(lamborghiniVenenoHolder);
  lamborghiniVeneno->setGeometry(planet_model);
  lamborghiniVeneno->setWorldTransform(glm::rotate(glm::fmat4{}, float(glfwGetTime())*0.4f, glm::fvec3{0.0f, 1.0f, 0.0f}));
  lamborghiniVeneno->setWorldTransform(glm::translate(lamborghiniVeneno->getWorldTransform(), glm::fvec3{0.0f, 0.0f, 14.0f}));
  lamborghiniVeneno->setLocalTransform(glm::inverseTranspose(glm::inverse(m_view_transform) * lamborghiniVeneno->getWorldTransform()));

  //std::vector<std::shared_ptr<Node>> rollsRoycePhantomChildren;
  auto rollsRoycePhantomHolder = std::make_shared<Node>(raum, "Rolls Royce Phantom", "root->rollsRoycePhantom", 1);
  raum->addChild(rollsRoycePhantomHolder);
  rollsRoycePhantomHolder->setParent(raum);
  auto rollsRoycePhantom = std::make_shared<GeometryNode>(rollsRoycePhantomHolder, "Rolls Royce Phantom Geometry");
  rollsRoycePhantomHolder->addChild(rollsRoycePhantom);
  rollsRoycePhantom->setParent(rollsRoycePhantomHolder);
  rollsRoycePhantom->setGeometry(planet_model);
  rollsRoycePhantom->setWorldTransform(glm::rotate(glm::fmat4{}, float(glfwGetTime())*0.3f, glm::fvec3{0.0f, 1.0f, 0.0f}));
  rollsRoycePhantom->setWorldTransform(glm::translate(rollsRoycePhantom->getWorldTransform(), glm::fvec3{0.0f, 0.0f, 17.0f}));
  rollsRoycePhantom->setLocalTransform(glm::inverseTranspose(glm::inverse(m_view_transform) * rollsRoycePhantom->getWorldTransform()));

  //std::vector<std::shared_ptr<Node>> mercedesMaybachChildren;
  auto mercedesMaybachHolder = std::make_shared<Node>(raum, "Mercedes Maybach S650", "root->mercedesMaybach", 1);
  raum->addChild(mercedesMaybachHolder);
  mercedesMaybachHolder->setParent(raum);
  auto mercedesMaybach = std::make_shared<GeometryNode>(mercedesMaybachHolder, "Mercedes Maybach S650 Geometry");
  mercedesMaybachHolder->addChild(mercedesMaybach);
  mercedesMaybach->setParent(mercedesMaybachHolder);
  mercedesMaybach->setGeometry(planet_model);
  mercedesMaybach->setWorldTransform(glm::rotate(glm::fmat4{}, float(glfwGetTime())*0.2f, glm::fvec3{0.0f, 1.0f, 0.0f}));
  mercedesMaybach->setWorldTransform(glm::translate(mercedesMaybach->getWorldTransform(), glm::fvec3{0.0f, 0.0f, 20.0f}));
  mercedesMaybach->setLocalTransform(glm::inverseTranspose(glm::inverse(m_view_transform) * mercedesMaybach->getWorldTransform()));

  //std::vector<std::shared_ptr<Node>> bentleyFlyingSpurChildren;
  auto bentleyFlyingSpurHolder = std::make_shared<Node>(raum, "Bentley Flying Spur", "root->bentleyFlyingSpur", 1);
  auto bentleyFlyingSpur = std::make_shared<GeometryNode>(bentleyFlyingSpurHolder, "Bentley Flying Spur Geometry");
  bentleyFlyingSpurHolder->addChild(bentleyFlyingSpur);
  bentleyFlyingSpur->setParent(bentleyFlyingSpurHolder);
  bentleyFlyingSpur->setGeometry(planet_model); 
  //bentley is moon of mercedes
  mercedesMaybachHolder->addChild(bentleyFlyingSpurHolder);
  bentleyFlyingSpurHolder->setParent(mercedesMaybachHolder);
  bentleyFlyingSpur->setWorldTransform(glm::rotate(bentleyFlyingSpur->getParent()->getParent()->getChildren("Mercedes Maybach S650 Geometry")->getWorldTransform(), float(glfwGetTime())*10, glm::fvec3{0.0f, 1.0f, 0.0f}));
  bentleyFlyingSpur->setWorldTransform(glm::translate(bentleyFlyingSpur->getWorldTransform(), glm::fvec3{0.0f, 0.0f, 1.4f}));
  bentleyFlyingSpur->setWorldTransform(glm::scale(bentleyFlyingSpur->getWorldTransform(), glm::fvec3{0.4f}));
  bentleyFlyingSpur->setLocalTransform(glm::inverseTranspose(glm::inverse(m_view_transform) * bentleyFlyingSpur->getWorldTransform()));


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

void ApplicationSolar::initializeStars() {
  
}

void ApplicationSolar::render() const {
  renderPlanets();
}

void ApplicationSolar::renderPlanets() const {

  for(auto& planet : geomList) {
      // bind shader to upload uniforms

        glUseProgram(m_shaders.at("planet").handle);

        glUniformMatrix4fv(m_shaders.at("planet").u_locs.at("ModelMatrix"),
                          1, GL_FALSE, glm::value_ptr(planet->getWorldTransform()));

        // extra matrix for normal transformation to keep them orthogonal to surface
        glUniformMatrix4fv(m_shaders.at("planet").u_locs.at("NormalMatrix"),
                          1, GL_FALSE, glm::value_ptr(planet->getLocalTransform()));

        // bind the VAO to draw
        glBindVertexArray(planet_object.vertex_AO);

        // draw bound vertex array using bound shader
        glDrawElements(planet_object.draw_mode, planet_object.num_elements, model::INDEX.type, NULL);
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

  // store star shader   
  m_shaders.emplace("stars", shader_program{{{GL_VERTEX_SHADER,m_resource_path + "shaders/vao.vert"}, {GL_FRAGMENT_SHADER, m_resource_path + "shaders/vao.frag"}}});      
  // request uniform locations for shader program   
  m_shaders.at("stars").u_locs["ModelViewMatrix"] = -1;   
  m_shaders.at("stars").u_locs["ProjectionMatrix"] = -1;
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