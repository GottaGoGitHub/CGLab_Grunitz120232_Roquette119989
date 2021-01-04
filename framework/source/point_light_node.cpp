#include "point_light_node.hpp"

PointLightNode::PointLightNode(float const& intensity, glm::vec3 const& color) : 
    lightIntensity_{intensity},
    lightColor_{color}
{}

void PointLightNode::setIntensity(float const& intensity) {
    lightIntensity_ = intensity;
}

float PointLightNode::getIntensity() {
    return lightIntensity_;
}

void PointLightNode::setColor(glm::vec3 const& color) { 
    lightColor_ = color;
}

glm::vec3 PointLightNode::getColor() {
    return lightColor_;
}


