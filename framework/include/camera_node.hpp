#ifndef CAMERA_NODE_HPP
#define CAMERA_NODE_HPP

#include "node.hpp"
#include <iostream>

class CameraNode : public Node {
public:
    CameraNode();
    ~CameraNode();

    bool getPerspective() const;

    bool getEnabled() const;
    void setEnabled(bool setter);

    glm::fmat4 getProjectionMatrix() const;
    void setProjectionMatrix(glm::fmat4 const& mat);

private:
    bool isPerspective;
    bool isEnabled;
    glm::fmat4 projectionMatrix;
};

#endif //CAMERA_NODE_HPP