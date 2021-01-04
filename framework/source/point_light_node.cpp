#include "point_light_node.hpp"

PointLightNode::PointLightNode(float intensity, glm::vec3 color) : 
    lightIntensity_{intensity},
    lightColor_{color}
{}