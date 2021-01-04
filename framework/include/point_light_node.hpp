#ifndef POINT_LIGHT_NODE_HPP
#define POINT_LIGHT_NODE_HPP

#include "node.hpp"
#include "model.hpp"
#include "structs.hpp"
#include <iostream>
#include <string>

class PointLightNode : public Node {
public:
    PointLightNode();
    ~PointLightNode();

    PointLightNode(float intensity, glm::vec3 color);

private:
    float lightIntensity;
    glm::vec3 lightColor;

};

#endif //POINT_LIGHT_NODE_HPP