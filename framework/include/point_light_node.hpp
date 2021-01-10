#ifndef POINT_LIGHT_NODE_HPP
#define POINT_LIGHT_NODE_HPP

#include "node.hpp"
#include <string>
#include <iostream>

class PointLightNode : public Node {
public:
    PointLightNode();
    ~PointLightNode();

    PointLightNode(float const& intensity, glm::vec3 const& color);
    PointLightNode(std::shared_ptr<Node> const& parent, std::string const& name, float const& intensity, glm::vec3 const& color, glm::vec3 pos);

    void setIntensity(float const& intensity);
    float getIntensity();
    void setColor(glm::vec3 const& color);
    glm::vec3 getColor();
    void setPosition(glm::vec3 const& pos);
    glm::vec3 getPosition();

private:
    float lightIntensity_;
    glm::vec3 lightColor_;
    glm::vec3 position_;

};

#endif //POINT_LIGHT_NODE_HPP