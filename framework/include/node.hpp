#ifndef NODE_HPP
#define NODE_HPP

#include <glm/gtc/type_precision.hpp>

#include <memory>
#include <vector>
#include <string>

class Node {
public:
    Node getParent() const; 
    void setParent(Node const& node);

    Node getChildren(std::string const& name) const;
    std::vector<std::shared_ptr<Node>> getChildrenList() const; 

    std::string getName() const;
    std::string getPath() const;
    int getDepth() const;
    
    glm::fmat4 getLocalTransform() const;
    void setLocalTransform(glm::fmat4 const& mat);

    glm::fmat4 getWorldTransform() const;
    void setWorldTransform(glm::fmat4 const& mat);

    Node removeChildren(std::string child); 
    void addChildren(Node const& child);

protected:
    std::shared_ptr<Node> parent;
    std::vector<std::shared_ptr<Node>> children;
    std::string name;
    std::string path;
    int depth;
    glm::fmat4 localtransform;
    glm::fmat4 globaltransform;
};

#endif //NODE_HPP