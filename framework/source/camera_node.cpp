#include "camera_node.hpp"

CameraNode::CameraNode()    :
    isPerspective{false},
    isEnabled{false},
    projectionMatrix{}
{}

CameraNode::~CameraNode(){
    std::cout << "Camera Node destroyed" << std::endl;
}

bool CameraNode::getPerspective() const{
    return isPerspective;
}

bool CameraNode::getEnabled() const{
    return isEnabled;
}

void CameraNode::setEnabled(bool setter) {
    isEnabled = setter;
}

glm::fmat4 CameraNode::getProjectionMatrix() const{
    return projectionMatrix;
}

void CameraNode::setProjectionMatrix(glm::fmat4 const& mat){
    projectionMatrix = mat;
}