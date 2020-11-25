#ifndef NODE_HPP
#define NODE_HPP

#include <glbinding/gl/types.h>
// use gl definitions from glbinding 
using namespace gl;

#include <glm/gtc/type_precision.hpp>

#include <memory>
#include <vector>
#include <string>
#include <algorithm>

class Node {
public:
    Node();
    Node(std::shared_ptr<Node> const& parent, std::string const& name);
    Node(std::shared_ptr<Node> const& parent, std::vector<std::shared_ptr<Node>> const& children, std::string const& name, std::string const& path, GLuint const& depth);
    ~Node();

    std::shared_ptr<Node> getParent() const; 
    void setParent(Node const& node);

    std::shared_ptr<Node> getChildren(std::string const& name) const; 
    std::vector<std::shared_ptr<Node>> getChildrenList() const; 
    
    std::string getName() const;
    std::string getPath() const;
    GLuint getDepth() const;
    
    glm::fmat4 getLocalTransform() const;
    void setLocalTransform(glm::fmat4 const& mat);

    glm::fmat4 getWorldTransform() const;
    void setWorldTransform(glm::fmat4 const& mat);

    void addChild(std::shared_ptr<Node> const& newChild);
    std::shared_ptr<Node> removeChildren(std::string const& name); 

    bool isEqual(std::string name, std::string nodename);

protected:
    std::shared_ptr<Node> parent_;
    std::vector<std::shared_ptr<Node>> children_;
    std::string name_;
    std::string path_;
    GLuint depth_;
    glm::fmat4 localtransform_;
    glm::fmat4 worldtransform_;
};

#endif //NODE_HPP