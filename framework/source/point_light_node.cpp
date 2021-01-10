#include "point_light_node.hpp"

PointLightNode::PointLightNode(float const& intensity, glm::vec3 const& color) : 
    lightIntensity_{intensity},
    lightColor_{color}
{}

PointLightNode::PointLightNode(std::shared_ptr<Node> const& parent, std::string const& name, float const& intensity, glm::vec3 const& color, glm::vec3 pos) : 
    Node(parent, name),
    lightIntensity_{intensity},
    lightColor_{color},
    position_{pos}
{}

PointLightNode::~PointLightNode() {
    std::cout << "Rip Light" << std::endl;
}

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

void PointLightNode::setPosition(glm::vec3 const& pos){
    position_ = pos;
}

glm::vec3 PointLightNode::getPosition(){
    return position_;
}




