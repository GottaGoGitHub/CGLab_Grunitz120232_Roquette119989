#include "geometry_node.hpp"

GeometryNode::GeometryNode():
    geometry{}
{}

GeometryNode::~GeometryNode() {
    std::cout << "Geometry Node has been destroyed" << std::endl;
}

model GeometryNode::getGeometry(){
    return geometry;
}

void GeometryNode::setGeometry(model const& model){
    geometry = model;
}
