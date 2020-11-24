#include "node.hpp"
#include <iostream>

Node::Node() : 
    parent_{nullptr}, //Is this supposed to be root? Vermutlich
    children_{},
    name_{"root"},
    path_{"none"}, //How are we supposed to "name" this? Ist weird den path als string anzulegen
    depth_{},
    localtransform_{},
    worldtransform_{}
{}

Node::Node(std::shared_ptr<Node> const& parent, std::string const& name) : 
    parent_{parent},
    name_{name}
{}

Node::Node(std::shared_ptr<Node> const& parent, std::vector<std::shared_ptr<Node>> const& children, std::string const& name, std::string const& path, GLuint const& depth, glm::fmat4 const& local, glm::fmat4 const& world) : 
    parent_{parent}, //Alternatively use Node as input with makeshared for "simpler" coding later?
    children_{children},
    name_{name},
    path_{path},
    depth_{depth},
    localtransform_{local},
    worldtransform_{world}
{//depth_ = parent_->getDepth() + 1;
}

Node::~Node() { 
    std::cout << "Node has been destroyed." << std::endl;
}

std::shared_ptr<Node> Node::getParent() const {
    return parent_;
}

void Node::setParent(Node const& node) {
    auto newParent = std::make_shared<Node>(node);
    parent_ = newParent;
}

std::shared_ptr<Node> Node::getChildren(std::string const& name) const { 
    for(auto& i : children_) {
        if(i->name_ == name) {
            return i;
        }
    };
}

std::vector<std::shared_ptr<Node>> Node::getChildrenList() const {
    return children_;
}

std::string Node::getName() const {
    return name_;
}

std::string Node::getPath() const {
    return path_;
}

GLuint Node::getDepth() const {
    return depth_;
}

glm::fmat4 Node::getLocalTransform() const {
    return localtransform_;
}

void Node::setLocalTransform(glm::fmat4 const& mat) {
    localtransform_ = mat;
}

glm::fmat4 Node::getWorldTransform() const {
    return worldtransform_;
}

void Node::setWorldTransform(glm::fmat4 const& mat) {
    worldtransform_ = mat;
}

void Node::addChild(std::shared_ptr<Node> const& newChild) {
    children_.push_back(newChild);
}

std::shared_ptr<Node> Node::removeChildren(std::string const& name) {
    //children_.erase(std::remove_if(children_.begin(), children_.end(), [](std::string const& name){isEqual}))
}

bool Node::isEqual(std::string name, std::string nodename){
    return name == nodename;
}