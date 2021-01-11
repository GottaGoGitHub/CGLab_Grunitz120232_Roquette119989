#include "node.hpp"
#include <iostream>

Node::Node() : 
    parent_{nullptr},
    children_{},
    name_{"root"},
    path_{"none"},
    depth_{0},
    localtransform_{},
    worldtransform_{}
{
    std::cout << "Node " + name_ + " created" << std::endl;
}

Node::Node(std::shared_ptr<Node> const& parent, std::string const& name) : 
    parent_{parent},
    name_{name}, 
    localtransform_{glm::fmat4()}
{}

Node::Node(std::shared_ptr<Node> const& parent, std::string const& name, glm::fmat4 const& local) : 
    parent_{parent},
    name_{name},
    localtransform_{local}
{}

Node::Node(std::shared_ptr<Node> const& parent, std::string const& name, std::string const& path, GLuint const& depth) : 
    parent_{parent}, 
    name_{name},
    path_{path},
    depth_{depth}
{}


Node::Node(std::shared_ptr<Node> const& parent, std::vector<std::shared_ptr<Node>> const& children, std::string const& name, std::string const& path, GLuint const& depth) : 
    parent_{parent}, 
    children_{children},
    name_{name},
    path_{path},
    depth_{depth}
{}


Node::Node(std::shared_ptr<Node> const& parent, std::vector<std::shared_ptr<Node>> const& children, std::string const& name, std::string const& path, GLuint const& depth, glm::fmat4 const& local, glm::fmat4 const & world) : 
    parent_{parent}, 
    children_{children},
    name_{name},
    path_{path},
    depth_{depth},
    localtransform_{local},
    worldtransform_{world}
{}

Node::~Node() { 
    std::cout << "Node has been destroyed." << std::endl;
}

std::shared_ptr<Node> Node::getParent() const {
    return parent_;
}

void Node::setParent(std::shared_ptr<Node> const& node) {
    parent_ = node;
}

std::shared_ptr<Node> Node::getChildren(std::string const& name) const { 
    for(auto& i : children_) {
        if(i->name_ == name) {
            return i;
        } else { //Try children of children until you reach the end
            auto childname = i->getChildren(name);
            if(childname != nullptr) {
                return i->getChildren(name);
            }
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
    if(parent_ == nullptr) {
        return localtransform_;
    } else {
        return parent_->getWorldTransform() * localtransform_;
    } 
}

void Node::setWorldTransform(glm::fmat4 const& mat) {
    worldtransform_ = mat;
}

void Node::addChild(std::shared_ptr<Node> const& newChild) {
    children_.push_back(newChild);
}

std::shared_ptr<Node> Node::removeChildren(std::string const& name) {
    for(auto& i : children_){
        auto it = children_.begin();
        if(name_ == name){
            i->setParent(nullptr);
            children_.erase(it);
        }
        it++;
    }
}

bool Node::isEqual(std::string name, std::string nodename){
    return name == nodename;
}
