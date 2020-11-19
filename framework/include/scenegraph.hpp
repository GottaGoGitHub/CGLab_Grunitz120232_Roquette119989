#ifndef SCENEGRAPH_HPP
#define SCENEGRAPH_HPP

#include "node.hpp"
#include <string>

struct Scenegraph {
public:
    std::string getName() const;
    void setName(std::string const& name);

    Node getRoot() const;
    void setRoot(Node const& name);

    std::string printGraph() const;

private:
    std::string name;
    std::shared_ptr<Node> root; //Is this supposed to be a pointer, or smart pointer?
};

#endif //SCENEGRAPH_HPP
