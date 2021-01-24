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
  initializeTextures();
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
  scenegraph_ = Scenegraph{"Solarium", raum};

  //light
  auto light = std::make_shared<PointLightNode>(raum, "lightSource", 3.0f, glm::vec3{1.0f, 1.0f, 1.0f}, glm::vec3{0.0f, 0.0f, 0.0f});
  //light->setLocalTransform(glm::translate({}, glm::vec3{0.0f, 0.0f, 0.0f}));
  raum->addChild(light);

  //cam
  auto camera = std::make_shared<CameraNode>(raum, "cam", true, true, glm::fmat4{});
  raum->addChild(camera);

  //Das Zentrum des Universums, das Licht alles Lebens, die Sonne, ich
  auto ich = std::make_shared<GeometryNode>(light, "ich_geom");
  light->addChild(ich);
  ich->setLocalTransform(glm::scale({}, glm::fvec3{1.0f, 1.0f, 1.0f}));

  //create node for all stars in the scene
  auto star_container = std::make_shared<Node>(raum, "stars");
  raum->addChild(star_container);

  //Create Planet holder
  auto laFerrariHolder = std::make_shared<Node>(raum, "laFerrari", "root->laFerrari", 1);
  //Connect Planet with root node
  raum->addChild(laFerrariHolder);
  //Set Planet Position
  laFerrariHolder->setLocalTransform(glm::translate({}, glm::fvec3{0.0f, 0.0f, 18.0f}));
  //Set Planet Rotation Speed
  laFerrariHolder->setLocalTransform(glm::rotate(glm::fmat4{}, 2.0f, glm::fvec3{0.0f, 1.0f, 0.0f}) * laFerrariHolder->getLocalTransform());
  //Create GeometryNode of Planet
  auto laFerrari = std::make_shared<GeometryNode>(laFerrariHolder, "laFerrari_geom");
  //Connect Geometry Node with Planet holder
  laFerrariHolder->addChild(laFerrari);
  //Set size of planet
  laFerrari->setLocalTransform(glm::scale({}, glm::fvec3{0.41f, 0.41f, 0.41f}));

  auto astonMartinOneHolder = std::make_shared<Node>(raum, "astonMartinOne", "root->astonMartinOne", 1);
  raum->addChild(astonMartinOneHolder);
  astonMartinOneHolder->setLocalTransform(glm::translate({}, glm::fvec3{0.0f, 0.0f, 6.5f}));
  astonMartinOneHolder->setLocalTransform(glm::rotate(glm::fmat4{}, 1.0f, glm::fvec3{0.0f, 1.0f, 0.0f}) * astonMartinOneHolder->getLocalTransform());
  auto astonMartinOne = std::make_shared<GeometryNode>(astonMartinOneHolder, "astonMartinOne_geom");
  astonMartinOneHolder->addChild(astonMartinOne);
  astonMartinOne->setLocalTransform(glm::scale({}, glm::fvec3{0.57f, 0.57f, 0.57f}));

  auto koenigsEggOneHolder = std::make_shared<Node>(raum, "koenigsEggOne", "root->koenigsEggOne", 1);
  raum->addChild(koenigsEggOneHolder);
  koenigsEggOneHolder->setLocalTransform(glm::translate({}, glm::fvec3{0.0f, 0.0f, 3.0f}));
  koenigsEggOneHolder->setLocalTransform(glm::rotate(glm::fmat4{}, 3.8f, glm::fvec3{0.0f, 1.0f, 0.0f}) * koenigsEggOneHolder->getLocalTransform());
  auto koenigsEggOne = std::make_shared<GeometryNode>(koenigsEggOneHolder, "koenigsEggOne_geom");
  koenigsEggOneHolder->addChild(koenigsEggOne);
  koenigsEggOne->setLocalTransform(glm::scale({}, glm::fvec3{0.66f, 0.66f, 0.66f}));

  auto ferrariFAmericaHolder = std::make_shared<Node>(raum, "ferrariFAmerica", "root->ferrariFAmerica", 1);
  raum->addChild(ferrariFAmericaHolder);
  ferrariFAmericaHolder->setLocalTransform(glm::translate({}, glm::fvec3{0.0f, 0.0f, 9.0f}));
  ferrariFAmericaHolder->setLocalTransform(glm::rotate(glm::fmat4{}, 13.8f, glm::fvec3{0.0f, 1.0f, 0.0f}) * ferrariFAmericaHolder->getLocalTransform());
  auto ferrariFAmerica = std::make_shared<GeometryNode>(ferrariFAmericaHolder, "ferrariFAmerica_geom");
  ferrariFAmericaHolder->addChild(ferrariFAmerica);
  ferrariFAmerica->setLocalTransform(glm::scale({}, glm::fvec3{0.32f, 0.32f, 0.32f}));

  auto bugattiVeyronHolder = std::make_shared<Node>(raum, "bugattiVeyron", "root->bugattiVeyron", 1);
  raum->addChild(bugattiVeyronHolder);
  bugattiVeyronHolder->setLocalTransform(glm::translate({}, glm::fvec3{0.0f, 0.0f, 11.0f}));
  bugattiVeyronHolder->setLocalTransform(glm::rotate(glm::fmat4{}, 17.0f, glm::fvec3{0.0f, 1.0f, 0.0f}) * bugattiVeyronHolder->getLocalTransform());
  auto bugattiVeyron = std::make_shared<GeometryNode>(bugattiVeyronHolder, "bugattiVeyron_geom");
  bugattiVeyronHolder->addChild(bugattiVeyron);
  bugattiVeyron->setLocalTransform(glm::scale({}, glm::fvec3{0.78f, 0.78f, 0.78f}));
  
  auto lamborghiniVenenoHolder = std::make_shared<Node>(raum, "lamborghiniVeneno", "root->lamborghiniVeneno", 1);
  raum->addChild(lamborghiniVenenoHolder);
  lamborghiniVenenoHolder->setLocalTransform(glm::translate({}, glm::fvec3{0.0f, 0.0f, 5.0f}));
  lamborghiniVenenoHolder->setLocalTransform(glm::rotate(glm::fmat4{}, 20.0f, glm::fvec3{0.0f, 1.0f, 0.0f}) * lamborghiniVenenoHolder->getLocalTransform());
  auto lamborghiniVeneno = std::make_shared<GeometryNode>(lamborghiniVenenoHolder, "lamborghiniVeneno_geom");
  lamborghiniVenenoHolder->addChild(lamborghiniVeneno);
  lamborghiniVeneno->setLocalTransform(glm::scale({}, glm::fvec3{0.5f, 0.5f, 0.5f}));

  auto rollsRoycePhantomHolder = std::make_shared<Node>(raum, "rollsRoycePhantom", "root->rollsRoycePhantom", 1);
  raum->addChild(rollsRoycePhantomHolder);
  rollsRoycePhantomHolder->setLocalTransform(glm::translate({}, glm::fvec3{0.0f, 0.0f, 8.0f}));
  rollsRoycePhantomHolder->setLocalTransform(glm::rotate(glm::fmat4{}, 12.0f, glm::fvec3{0.0f, 1.0f, 0.0f}) * rollsRoycePhantomHolder->getLocalTransform());
  auto rollsRoycePhantom = std::make_shared<GeometryNode>(rollsRoycePhantomHolder, "rollsRoycePhantom_geom");
  rollsRoycePhantomHolder->addChild(rollsRoycePhantom);
  rollsRoycePhantom->setLocalTransform(glm::scale({}, glm::fvec3{0.43f, 0.43f, 0.43f}));

  auto mercedesMaybachHolder = std::make_shared<Node>(raum, "mercedesMaybach", "root->mercedesMaybach", 1);
  raum->addChild(mercedesMaybachHolder);
  mercedesMaybachHolder->setLocalTransform(glm::translate({}, glm::fvec3{0.0f, 0.0f, 14.0f}));
  mercedesMaybachHolder->setLocalTransform(glm::rotate(glm::fmat4{}, 5.5f, glm::fvec3{0.0f, 1.0f, 0.0f}) * mercedesMaybachHolder->getLocalTransform());
  auto mercedesMaybach = std::make_shared<GeometryNode>(mercedesMaybachHolder, "mercedesMaybach_geom");
  mercedesMaybachHolder->addChild(mercedesMaybach);
  mercedesMaybach->setLocalTransform(glm::scale({}, glm::fvec3{0.67f, 0.67f, 0.67f}));

  //Moon of mercedes maybach
  auto bentleyFlyingSpurHolder = std::make_shared<Node>(mercedesMaybachHolder, "bentleyFlyingSpur", "mercedesMaybach->bentleyFlyingSpur", 1);
  mercedesMaybachHolder->addChild(bentleyFlyingSpurHolder);
  bentleyFlyingSpurHolder->setLocalTransform(glm::translate({}, glm::fvec3{0.0f, 0.0f, 1.4f}));
  bentleyFlyingSpurHolder->setLocalTransform(glm::rotate(glm::fmat4{}, 8.0f, glm::fvec3{0.0f, 1.0f, 0.0f}) * bentleyFlyingSpurHolder->getLocalTransform());
  auto bentleyFlyingSpur = std::make_shared<GeometryNode>(bentleyFlyingSpurHolder, "bentleyFlyingSpur_geom");
  bentleyFlyingSpurHolder->addChild(bentleyFlyingSpur);
  bentleyFlyingSpur->setLocalTransform(glm::scale({}, glm::fvec3{0.1f, 0.1f, 0.1f}));

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

  //Color List
  glm::vec3 lightBlue{0.3f, 0.7f, 1.0f};
  glm::vec3 mintGreen{0.3f, 0.8f, 0.6f};
  glm::vec3 darkViolet{0.4f, 0.3f, 0.7f};
  glm::vec3 orange{1.0f, 0.7f, 0.0f};
  glm::vec3 pink{1.0f, 0.7f, 1.0f};
  glm::vec3 yellow{1.0f, 1.0f, 0.2f};
  glm::vec3 black{0.0f, 0.0f, 0.0f};
  glm::vec3 darkGreen{0.0f, 0.5f, 0.4f};
  glm::vec3 bordeaux{0.7f, 0.0f, 0.3f};
  glm::vec3 grey{0.7f, 0.7f, 0.7f};

  //Match Colors to cars and insert into map
  colors.insert({"laFerrari", bordeaux});
  colors.insert({"astonMartinOne", lightBlue});
  colors.insert({"koenigsEggOne", mintGreen});
  colors.insert({"ferrariFAmerica", pink});
  colors.insert({"bugattiVeyron", darkViolet});
  colors.insert({"ich", yellow});
  colors.insert({"lamborghiniVeneno", darkGreen});
  colors.insert({"rollsRoycePhantom", black});
  colors.insert({"mercedesMaybach", grey});
  colors.insert({"bentleyFlyingSpur", orange});

  lightList.push_back(light);
}

void ApplicationSolar::initializeStars(){

  //Create interleaved vec with 10000 stars x,y,z,r,g,b
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

  // index, size, type, normalized (fixed-point data - YES/NO), stride (offset), pointer (offset to the first component of the first generic vertex attribute in the array)
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

void ApplicationSolar::initializeTextures() {
  int i = 0;
  for(auto& planet : geomList) {
    auto holder = planet->getParent();
    std::string planet_name = holder->getName();

    pixel_data pixel;

    try
    {
      pixel = texture_loader::file(m_resource_path + "textures/" + planet_name + "_map.png");
    }
    catch(std::exception e)
    {
      std::cout << e.what() << "Error: "<< planet_name << '\n';
    }
    
    GLsizei width = (GLsizei)pixel.width;
    GLsizei height = (GLsizei)pixel.height;
    GLenum channel = pixel.channels;
    GLenum channelType = pixel.channel_type;

    glActiveTexture(GL_TEXTURE1 + 2 * i);
    texture_object texture;
    glGenTextures(1, &texture.handle);
    texture.target = GL_TEXTURE_2D;
    std::string textureName = planet_name + "_tex";
    textList.insert({textureName, texture});

    //glTexImage2D(texture.target, 0, GL_RGB, width, height, GL_RGBA, GL_UNSIGNED_BYTE, data);

    glTexParameteri(texture.target, GL_TEXTURE_MAG_FILTER, GL_NEAREST);
    glTexParameteri(texture.target, GL_TEXTURE_MIN_FILTER, GL_NEAREST);

    glBindTexture(texture.target, texture.handle);
    i++;
  }
}

void ApplicationSolar::render() const {
  renderPlanets();
  renderStars();
}

void ApplicationSolar::renderPlanets() const {
  auto root = scenegraph_.getRoot();
  //init counter to have each planet different from another
  int count = 0;
  //Load planet_model into function
  model planet_model = model_loader::obj(m_resource_path + "models/sphere.obj", model::NORMAL);

  //Iterate over all Planets added to geomList
  for(auto& planet : geomList) {
    auto holder = planet->getParent();
    std::string planet_name = holder->getName();
    
    //init rotation matrix
    glm::fmat4 rotation_matrix;
    
    //Set different rotation matrix for Moon (Bentley)
    if(planet->getName() != "bentleyFlyingSpur_geom") {
      rotation_matrix = glm::rotate(glm::fmat4{}, float(((20 - (count * 2)) * 0.0005f)), glm::fvec3{0, 1, 0});
    } else {
      rotation_matrix = glm::rotate(glm::fmat4{}, 0.05f, glm::fvec3{0, 1, 0});
    }

    //Set Geometry of Planet
    planet->setGeometry(planet_model);

    //Set Localtransform of holder w "custom" matrix
    holder->setLocalTransform(rotation_matrix * holder->getLocalTransform());

    glUseProgram(m_shaders.at("planet").handle);

    glUniformMatrix4fv(m_shaders.at("planet").u_locs.at("ModelMatrix"),
                          1, GL_FALSE, glm::value_ptr(planet->getWorldTransform()));

    planet->setLocalTransform(rotation_matrix * planet->getLocalTransform());

    // extra matrix for normal transformation to keep them orthogonal to surface
    glUniformMatrix4fv(m_shaders.at("planet").u_locs.at("NormalMatrix"),
                          1, GL_FALSE, glm::value_ptr(glm::inverseTranspose(glm::inverse(m_view_transform) * planet->getWorldTransform())));

    // bind the VAO to draw
    glBindVertexArray(planet_object.vertex_AO);
        
    texture_object texture = textList.at(planet_name + "_tex");
    glActiveTexture(GL_TEXTURE1 + 2 * count);
    glBindTexture(texture.target, texture.handle);
    int simp = glGetUniformLocation(m_shaders.at("planet").handle, "texture_simp");
    glUniform1i(simp, texture.handle);

    auto colorLocation = glGetUniformLocation(m_shaders.at("planet").handle, "planetColor");
    auto color = colors.find(planet_name);
    glUniform3f(colorLocation, color->second.x, color->second.y, color->second.z);

    //update point light
    auto light_node = root->getChildren("lightSource");
    auto light = std::static_pointer_cast<PointLightNode>(light_node);
    glm::vec3 light_color = light->getColor();
    float light_intensity = light->getIntensity();
    glm::fvec4 light_position = light->getWorldTransform()*glm::fvec4(0.0f, 0.0f, 0.0f, 1.0f);

    auto lightPositionLocation = glGetUniformLocation(m_shaders.at("planet").handle, "lightPos");
    glUniform3f(lightPositionLocation, light_position.x, light_position.y, light_position.z);
    //glUniform3f(lightPositionLocation, 0.0f, 0.0f, 0.0f);

    auto lightColorLocation = glGetUniformLocation(m_shaders.at("planet").handle, "lightColor");
    glUniform3f(lightColorLocation, light_color.x, light_color.y, light_color.z);

    auto lightIntensityLocation = glGetUniformLocation(m_shaders.at("planet").handle, "lightIntensity");
    glUniform1f(lightIntensityLocation, light_intensity);


    // draw bound vertex array using bound shader
    glDrawElements(planet_object.draw_mode, planet_object.num_elements, model::INDEX.type, NULL);

    count++;
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
  //m_shaders.at("stars").u_locs["NormalMatrix"] = -1;
  m_shaders.at("stars").u_locs["ModelViewMatrix"] = -1;   
  m_shaders.at("stars").u_locs["ProjectionMatrix"] = -1;
  //m_shaders.at("stars").u_locs["ViewMatrix"] = -1;
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

  /*

  planet_model.data.data(): A pointer to the data in system memory that will be copied to the data store during initialization 

  GL_STATIC_DRAW: Specifies that the data store contents will be modified once by the application and drawn many times
  
  */

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
    m_view_transform = glm::translate(m_view_transform, glm::fvec3{0.0f, 0.0f, -0.2f});
    uploadView();
  }
  else if (key == GLFW_KEY_S  && (action == GLFW_PRESS || action == GLFW_REPEAT)) {
    m_view_transform = glm::translate(m_view_transform, glm::fvec3{0.0f, 0.0f, 0.2f});
    uploadView();
  }
  else if (key == GLFW_KEY_A  && (action == GLFW_PRESS || action == GLFW_REPEAT)) {
    m_view_transform = glm::translate(m_view_transform, glm::fvec3{-0.2f, 0.0f, 0.0f});
    uploadView();
  }
  else if (key == GLFW_KEY_D  && (action == GLFW_PRESS || action == GLFW_REPEAT)) {
    m_view_transform = glm::translate(m_view_transform, glm::fvec3{0.2f, 0.0f, 0.0f});
    uploadView();
  }
  else if(key == GLFW_KEY_O  && (action == GLFW_PRESS || action == GLFW_REPEAT)) {
    m_view_transform = glm::lookAt(glm::vec3(m_view_transform[3]), glm::vec3(0.0f, 0.0f, 0.0f), glm::vec3(0.0f, 1.0f, 0.0f));
    uploadView();
  }
}

//handle delta mouse movement input
void ApplicationSolar::mouseCallback(double pos_x, double pos_y) {
  // mouse handling

  float x = pos_x;
  float y = pos_y;

  float sens = 0.1f;
  x *= sens;
  y *= sens;

  m_view_transform = glm::rotate(m_view_transform, glm::radians(-(float)x), glm::vec3{0.0f, 1.0f, 0.0f});
  m_view_transform = glm::rotate(m_view_transform, glm::radians(-(float)y), glm::vec3{1.0f, 0.0f, 0.0f});

  //glm::look_at
  //glm::rotate -> around 1 axis at the time
  //reset for looking at origin

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