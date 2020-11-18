#ifndef NODE_HPP
#define NODE_HPP

#include <glm/gtc/type_precision.hpp>

#include <vector>
#include <string>

class Node {
public:
    Node& getParent() const;
    void setParent(Node const& node);

    Node& getChildren(std::string const& name) const;
    std::vector<Node&> getChildrenList() const; //How do you save the elements inside the container? Reference blabla

    std::string getName() const;
    std::string getPath() const;
    int getDepth() const;
    
    glm::fmat4 getLocalTransform() const;
    void setLocalTransform(glm::fmat4 const& mat);

    glm::fmat4 getWorldTransform() const;
    void setWorldTransform(glm::fmat4 const& mat);

    void addChildren(Node const& child);
    Node& removeChildren(std::string child); //Same question, how do you return the Node being a complex data type.

protected:
    Node* parent;
    std::vector<Node> children;
    std::string name;
    std::string path;
    int depth;
    glm::fmat4 localtransform;
    glm::fmat4 globaltransform;
};

#endif //NODE_HPP