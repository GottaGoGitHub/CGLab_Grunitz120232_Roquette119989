#ifndef CAMERA_NODE_HPP
#define CAMERA_NODE_HPP

#include "node.hpp"
#include <iostream>

class CameraNode : public Node {
public:
    CameraNode();
    CameraNode(std::shared_ptr<Node> const& parent, std::string const& name, bool pers, bool enabled, glm::fmat4 const& projMat);
    ~CameraNode();

    bool getPerspective() const;

    bool getEnabled() const;
    void setEnabled(bool setter);

    glm::fmat4 getProjectionMatrix() const;
    void setProjectionMatrix(glm::fmat4 const& mat);

private:
    bool isPerspective_;
    bool isEnabled_;
    glm::fmat4 projectionMatrix_;
};

#endif //CAMERA_NODE_HPP