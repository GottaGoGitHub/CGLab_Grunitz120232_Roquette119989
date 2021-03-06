#include "scenegraph.hpp"
#include <iostream>

Scenegraph::Scenegraph() : 
    name_{"Solar System"},
    root_{nullptr} 
{}

Scenegraph::Scenegraph(std::string const& name, std::shared_ptr<Node> const& root) : 
    name_{name},
    root_{root}
{}

Scenegraph::~Scenegraph() { 
    std::cout << "Scenegraph " << name_ << " deleted" << std::endl;
}

std::string Scenegraph::getName() const {
    std::cout << "The Scenegraph is called: " << name_ << std::endl;
}

void Scenegraph::setName(std::string const& newName) {
    name_ = newName;
}

std::shared_ptr<Node> Scenegraph::getRoot() const {
    return root_;
}

void Scenegraph::setRoot(Node const& node) {
    auto newRoot = std::make_shared<Node>(node);
    root_ = newRoot;
}

std::string Scenegraph::printGraph() const {
    
}

void Scenegraph::addLightNode(std::shared_ptr<PointLightNode> const& light){
    lightNodes_.push_back(light);
}

std::list<std::shared_ptr<PointLightNode>> Scenegraph::getAllLights(){
    return lightNodes_;
}