#ifndef APPLICATION_SOLAR_HPP
#define APPLICATION_SOLAR_HPP

#include "node.hpp"
#include "geometry_node.hpp"
#include "scenegraph.hpp"
#include "application.hpp"
#include "model.hpp"
#include "structs.hpp"
#include "point_light_node.hpp"
#include "camera_node.hpp"

// gpu representation of model
class ApplicationSolar : public Application {
 public:
  // allocate and initialize objects
  ApplicationSolar(std::string const& resource_path);
  // free allocated objects
  ~ApplicationSolar();

  //Initializes the complete "Solar System" with the help of a Graph
  void initializeSceneGraph();


  // react to key input
  void keyCallback(int key, int action, int mods);
  //handle delta mouse movement input
  void mouseCallback(double pos_x, double pos_y);
  //handle resizing
  void resizeCallback(unsigned width, unsigned height);

  // draw all objects
  void render() const;
  // render all planets
  void renderPlanets() const;
  void renderStars() const;

 protected:
  void initializeShaderPrograms();
  void initializeGeometry();
  void initializeStars();
  //void initializeLights();

  // update uniform values
  void uploadUniforms();
  // upload projection matrix
  void uploadProjection();
  // upload view matrix
  void uploadView();

  // cpu representation of model
  model_object planet_object;
  model_object star_object;

  std::shared_ptr<Node> raum_;
  Scenegraph scenegraph_;

  std::vector<std::shared_ptr<GeometryNode>> geomList;
  std::map<std::string, glm::vec3> colors;
  std::list<std::shared_ptr<PointLightNode>> lightList;
  
  // camera transform matrix
  glm::fmat4 m_view_transform;
  // camera projection matrix
  glm::fmat4 m_view_projection;
};

#endif