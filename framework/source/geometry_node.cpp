#include "geometry_node.hpp"

GeometryNode::GeometryNode():
    Node(),
    geometry_{}
{}

GeometryNode::GeometryNode(std::shared_ptr<Node> parent, std::string const& name) : 
    Node(parent, name),
    geometry_{}
{}

GeometryNode::GeometryNode(std::shared_ptr<Node> parent, std::string const& name, glm::fmat4 const& local) : 
    Node(parent, name, local),
    geometry_{}
{}

GeometryNode::GeometryNode(model const& model) : 
    geometry_{model}
{}

GeometryNode::~GeometryNode() {
    std::cout << "Geometry Node has been destroyed" << std::endl;
}

model GeometryNode::getGeometry(){
    return geometry_;
}

void GeometryNode::setGeometry(model const& model){
    geometry_ = model;
}

model_object GeometryNode::getObject() {
    return geometryObject_;
}
