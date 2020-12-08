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


  //create node for all stars in the scene
  auto star_container = std::make_shared<Node>(raum, "stars");
  raum->addChild(star_container);
  //Create Planet holder
  auto laFerrariHolder = std::make_shared<Node>(raum, "La Ferrari", "root->laFerrari", 1);
  //Connect Planet with root node
  raum->addChild(laFerrariHolder);
  laFerrariHolder->setWorldTransform(glm::rotate(glm::fmat4{}, 2.0f, glm::fvec3{0.0f, 1.0f, 0.0f}));
  laFerrariHolder->setWorldTransform(glm::translate(laFerrariHolder->getWorldTransform(), glm::fvec3{0.0f, 0.0f, 23.0f}));
  //Create GeometryNode of Planet
  auto laFerrari = std::make_shared<GeometryNode>(laFerrariHolder, "La Ferrari Geometry");
  //Connect Geometry Node with Planet holder
  laFerrariHolder->addChild(laFerrari);
  laFerrari->setWorldTransform(laFerrariHolder->getWorldTransform());

  
  auto astonMartinOneHolder = std::make_shared<Node>(raum, "Aston Martin One-77", "root->astonMartinOne", 1);
  raum->addChild(astonMartinOneHolder);
  astonMartinOneHolder->setWorldTransform(glm::rotate(glm::fmat4{}, 1.0f, glm::fvec3{0.0f, 1.0f, 0.0f}));
  astonMartinOneHolder->setWorldTransform(glm::translate(astonMartinOneHolder->getWorldTransform(), glm::fvec3{0.0f, 0.0f, 3.0f}));
  auto astonMartinOne = std::make_shared<GeometryNode>(astonMartinOneHolder, "Aston Martin One-77 Geometry");
  astonMartinOneHolder->addChild(astonMartinOne);
  astonMartinOne->setWorldTransform(astonMartinOneHolder->getWorldTransform());

  auto koenigsEggOneHolder = std::make_shared<Node>(raum, "Koenigsegg One", "root->koenigsEggOne", 1);
  raum->addChild(koenigsEggOneHolder);
  koenigsEggOneHolder->setWorldTransform(glm::rotate(glm::fmat4{}, 3.8f, glm::fvec3{0.0f, 1.0f, 0.0f}));
  koenigsEggOneHolder->setWorldTransform(glm::translate(koenigsEggOneHolder->getWorldTransform(), glm::fvec3{0.0f, 0.0f, 5.0f}));
  auto koenigsEggOne = std::make_shared<GeometryNode>(koenigsEggOneHolder, "Koenigsegg One Geometry");
  koenigsEggOneHolder->addChild(koenigsEggOne);
  koenigsEggOne->setWorldTransform(koenigsEggOneHolder->getWorldTransform());

  auto ferrariFAmericaHolder = std::make_shared<Node>(raum, "Ferrari F60 America", "root->ferrariFAmerica", 1);
  raum->addChild(ferrariFAmericaHolder);
  ferrariFAmericaHolder->setWorldTransform(glm::rotate(glm::fmat4{}, float(glfwGetTime())*0.6f, glm::fvec3{0.0f, 1.0f, 0.0f}));
  ferrariFAmericaHolder->setWorldTransform(glm::translate(ferrariFAmericaHolder->getWorldTransform(), glm::fvec3{0.0f, 0.0f, 8.0f}));
  auto ferrariFAmerica = std::make_shared<GeometryNode>(ferrariFAmericaHolder, "Ferrari F60 America Geometry");
  ferrariFAmericaHolder->addChild(ferrariFAmerica);
  ferrariFAmerica->setWorldTransform(ferrariFAmericaHolder->getWorldTransform());

  auto bugattiVeyronHolder = std::make_shared<Node>(raum, "Mansory Vivere Bugatti Veyron", "root->bugattiVeyron", 1);
  raum->addChild(bugattiVeyronHolder);
  bugattiVeyronHolder->setWorldTransform(glm::rotate(glm::fmat4{}, float(glfwGetTime())*0.5f, glm::fvec3{0.0f, 1.0f, 0.0f}));
  bugattiVeyronHolder->setWorldTransform(glm::translate(bugattiVeyronHolder->getWorldTransform(), glm::fvec3{0.0f, 0.0f, 11.0f}));
  auto bugattiVeyron = std::make_shared<GeometryNode>(bugattiVeyronHolder, "Mansory Vivere Bugatti Veyron Geometry");
  bugattiVeyronHolder->addChild(bugattiVeyron);
  bugattiVeyron->setWorldTransform(bugattiVeyronHolder->getWorldTransform());
  
  //Das Zentrum des Universums, das Licht alles Lebens, die Sonne, ich
  auto ichHolder = std::make_shared<Node>(raum, "Ich", "root->ich", 1);
  raum->addChild(ichHolder);
  ichHolder->setWorldTransform(glm::rotate(glm::fmat4{}, 0.5f, glm::fvec3{0.0f, 1.0f, 0.0f}));
  ichHolder->setWorldTransform(glm::scale(ichHolder->getWorldTransform(), glm::fvec3{2.0f, 2.0f, 2.0f}));
  auto ich = std::make_shared<GeometryNode>(ichHolder, "Ich Geometry");
  ichHolder->addChild(ich);
  ich->setWorldTransform(ichHolder->getWorldTransform());

  auto lamborghiniVenenoHolder = std::make_shared<Node>(raum, "Lamborghini Veneno", "root->lamborghiniVeneno", 1);
  raum->addChild(lamborghiniVenenoHolder);
  lamborghiniVenenoHolder->setWorldTransform(glm::rotate(glm::fmat4{}, 20.0f, glm::fvec3{0.0f, 1.0f, 0.0f}));
  lamborghiniVenenoHolder->setWorldTransform(glm::translate(lamborghiniVenenoHolder->getWorldTransform(), glm::fvec3{0.0f, 0.0f, 5.0f}));
  auto lamborghiniVeneno = std::make_shared<GeometryNode>(lamborghiniVenenoHolder, "Lamborghini Veneno Geometry");
  lamborghiniVenenoHolder->addChild(lamborghiniVeneno);
  lamborghiniVeneno->setWorldTransform(lamborghiniVenenoHolder->getWorldTransform());

  auto rollsRoycePhantomHolder = std::make_shared<Node>(raum, "Rolls Royce Phantom", "root->rollsRoycePhantom", 1);
  raum->addChild(rollsRoycePhantomHolder);
  rollsRoycePhantomHolder->setWorldTransform(glm::rotate(glm::fmat4{}, 12.0f, glm::fvec3{0.0f, 1.0f, 0.0f}));
  rollsRoycePhantomHolder->setWorldTransform(glm::translate(rollsRoycePhantomHolder->getWorldTransform(), glm::fvec3{0.0f, 0.0f, 8.0f}));
  auto rollsRoycePhantom = std::make_shared<GeometryNode>(rollsRoycePhantomHolder, "Rolls Royce Phantom Geometry");
  rollsRoycePhantomHolder->addChild(rollsRoycePhantom);
  rollsRoycePhantom->setWorldTransform(rollsRoycePhantomHolder->getWorldTransform());

  auto mercedesMaybachHolder = std::make_shared<Node>(raum, "Mercedes Maybach S650", "root->mercedesMaybach", 1);
  raum->addChild(mercedesMaybachHolder);
  mercedesMaybachHolder->setWorldTransform(glm::rotate(glm::fmat4{}, 5.5f, glm::fvec3{0.0f, 1.0f, 0.0f}));
  mercedesMaybachHolder->setWorldTransform(glm::translate(mercedesMaybachHolder->getWorldTransform(), glm::fvec3{0.0f, 0.0f, 12.0f}));
  auto mercedesMaybach = std::make_shared<GeometryNode>(mercedesMaybachHolder, "Mercedes Maybach S650 Geometry");
  mercedesMaybachHolder->addChild(mercedesMaybach);
  mercedesMaybach->setWorldTransform(mercedesMaybachHolder->getWorldTransform());
  
  auto bentleyFlyingSpurHolder = std::make_shared<Node>(mercedesMaybachHolder, "Bentley Flying Spur", "root->bentleyFlyingSpur", 1);
  mercedesMaybachHolder->addChild(bentleyFlyingSpurHolder);
  bentleyFlyingSpurHolder->setWorldTransform(glm::rotate(glm::fmat4{}, 8.0f, glm::fvec3{0.0f, 1.0f, 0.0f}));
  bentleyFlyingSpurHolder->setWorldTransform(glm::translate(bentleyFlyingSpurHolder->getWorldTransform(), glm::fvec3{0.0f, 0.0f, 1.4f}));
  auto bentleyFlyingSpur = std::make_shared<GeometryNode>(bentleyFlyingSpurHolder, "Bentley Flying Spur Geometry");
  bentleyFlyingSpurHolder->addChild(bentleyFlyingSpur);
  bentleyFlyingSpur->setWorldTransform(bentleyFlyingSpurHolder->getWorldTransform());
  

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

void ApplicationSolar::initializeStars(){

  int const amount = 10000;
  std::vector<float> stars;
  for(int i = 0; i < amount; i++){
    //init position
    float x = (float)(rand() % 100) - 20;
    stars.push_back(x);
    float y = (float)(rand() % 100) - 20;
    stars.push_back(y);
    float z = (float)(rand() % 100) - 20;
    stars.push_back(z);
    //init color
    float r = (float)(rand() % 255) / 255.0f;
    stars.push_back(r);
    float g = (float)(rand() % 255) / 255.0f;
    stars.push_back(g);
    float b = (float)(rand() % 255) / 255.0f;
    stars.push_back(b);
  }

    // generate vertex array object
  glGenVertexArrays(1, &star_object.vertex_AO);
  // bind the array for attaching buffers
  glBindVertexArray(star_object.vertex_AO);

  // generate generic buffer
  glGenBuffers(1, &star_object.vertex_BO);
  // bind this as an vertex array buffer containing all attributes
  glBindBuffer(GL_ARRAY_BUFFER, star_object.vertex_BO);
  // configure currently bound array buffer
  glBufferData(GL_ARRAY_BUFFER, sizeof(float) * stars.size(), stars.data(), GL_STATIC_DRAW);

  // activate first attribute on gpu
  glEnableVertexAttribArray(0);
  // first attribute is 3 floats with no offset & stride

  //!!!
  glVertexAttribPointer(0, 3, GL_FLOAT, GL_FALSE, GLsizei(6*sizeof(float)), 0);
  //!!!


  // activate second attribute on gpu
  glEnableVertexAttribArray(1);
  // second attribute is 3 floats with no offset & stride


  //!!!
  glVertexAttribPointer(1, 3, GL_FLOAT, GL_FALSE, GLsizei(6*sizeof(float)), (void*)(sizeof(float) * 3));
  //!!!

  // store type of primitive to draw
  star_object.draw_mode = GL_POINTS;
  // transfer number of indices to model object 
  star_object.num_elements = GLsizei(amount);

}

void ApplicationSolar::render() const {
  renderPlanets();
  renderStars();
}

void ApplicationSolar::renderPlanets() const {

  model planet_model = model_loader::obj(m_resource_path + "models/sphere.obj", model::NORMAL);

  glm::fmat4 model_matrix = glm::rotate(glm::fmat4{}, float(glfwGetTime()), glm::fvec3{1.0f});
  glm::fmat4 normal_matrix = glm::inverseTranspose(glm::inverse(m_view_transform) * model_matrix);

  for(auto& planet : geomList) {
    planet->setGeometry(planet_model);

    glUseProgram(m_shaders.at("planet").handle);

    glUniformMatrix4fv(m_shaders.at("planet").u_locs.at("ModelMatrix"),
                          1, GL_FALSE, glm::value_ptr(planet->getWorldTransform()));

    planet->setLocalTransform(glm::inverseTranspose(glm::inverse(m_view_transform) * planet->getWorldTransform()));
    // extra matrix for normal transformation to keep them orthogonal to surface
    glUniformMatrix4fv(m_shaders.at("planet").u_locs.at("NormalMatrix"),
                          1, GL_FALSE, glm::value_ptr(planet->getLocalTransform()));

    // bind the VAO to draw
    glBindVertexArray(planet_object.vertex_AO);

    // draw bound vertex array using bound shader
    glDrawElements(planet_object.draw_mode, planet_object.num_elements, model::INDEX.type, NULL);
  }   
}

void ApplicationSolar::renderStars() const{
  
  glUseProgram(m_shaders.at("stars").handle);

  glBindVertexArray(star_object.vertex_AO);
  
  glDrawArrays(star_object.draw_mode, GLint(0), star_object.num_elements);

}

void ApplicationSolar::uploadView() {
  // vertices are transformed in camera space, so camera transform must be inverted
  glm::fmat4 view_matrix = glm::inverse(m_view_transform);
  // upload matrix to gpu

  glUseProgram(m_shaders.at("planet").handle);
  glUniformMatrix4fv(m_shaders.at("planet").u_locs.at("ViewMatrix"),
                     1, GL_FALSE, glm::value_ptr(view_matrix));

  glUseProgram(m_shaders.at("stars").handle);
  glUniformMatrix4fv(m_shaders.at("stars").u_locs.at("ModelViewMatrix"),
                     1, GL_FALSE, glm::value_ptr(view_matrix));
}

void ApplicationSolar::uploadProjection() {
  // upload matrix to gpu

  glUseProgram(m_shaders.at("planet").handle);
  glUniformMatrix4fv(m_shaders.at("planet").u_locs.at("ProjectionMatrix"),
                     1, GL_FALSE, glm::value_ptr(m_view_projection));


  glUseProgram(m_shaders.at("stars").handle);
  glUniformMatrix4fv(m_shaders.at("stars").u_locs.at("ProjectionMatrix"),
                     1, GL_FALSE, glm::value_ptr(m_view_projection));
}

// update uniform locations
void ApplicationSolar::uploadUniforms() { 
  // bind shader to which to upload unforms
  //glUseProgram(m_shaders.at("planet").handle);
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
  m_shaders.at("stars").u_locs["NormalMatrix"] = -1;
  m_shaders.at("stars").u_locs["ModelViewMatrix"] = -1;   
  m_shaders.at("stars").u_locs["ProjectionMatrix"] = -1;
  m_shaders.at("stars").u_locs["ViewMatrix"] = -1;
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
  m_view_transform = glm::rotate(m_view_transform, glm::radians(-(float)pos_x), glm::vec3{0.0f, 1.0f, 0.0f});
  m_view_transform = glm::rotate(m_view_transform, glm::radians(-(float)pos_y), glm::vec3{1.0f, 0.0f, 0.0f});
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