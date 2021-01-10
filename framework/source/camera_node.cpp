#include "camera_node.hpp"

CameraNode::CameraNode()    :
    Node{},
    isPerspective_{false},
    isEnabled_{false},
    projectionMatrix_{}
{}

CameraNode::CameraNode(std::shared_ptr<Node> const& parent, std::string const& name, bool pers, bool enabled, glm::fmat4 const& projMat)    :
    Node(parent, name),
    isPerspective_{pers},
    isEnabled_{enabled},
    projectionMatrix_{projMat}
{}

CameraNode::~CameraNode(){
    std::cout << "Camera Node destroyed" << std::endl;
}

bool CameraNode::getPerspective() const{
    return isPerspective_;
}

bool CameraNode::getEnabled() const{
    return isEnabled_;
}

void CameraNode::setEnabled(bool setter) {
    isEnabled_ = setter;
}

glm::fmat4 CameraNode::getProjectionMatrix() const{
    return projectionMatrix_;
}

void CameraNode::setProjectionMatrix(glm::fmat4 const& mat){
    projectionMatrix_ = mat;
}