#ifndef POINT_LIGHT_NODE_HPP
#define POINT_LIGHT_NODE_HPP

#include "node.hpp"
#include <string>

class PointLightNode : public Node {
public:
    PointLightNode();
    ~PointLightNode();

    PointLightNode(float const& intensity, glm::vec3 const& color);

    void setIntensity(float const& intensity);
    float getIntensity();
    void setColor(glm::vec3 const& color);
    glm::vec3 getColor();

private:
    float lightIntensity_;
    glm::vec3 lightColor_;

};

#endif //POINT_LIGHT_NODE_HPP