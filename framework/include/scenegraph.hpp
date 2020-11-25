#ifndef SCENEGRAPH_HPP
#define SCENEGRAPH_HPP

#include "node.hpp"
#include <string>

struct Scenegraph {
public:
    Scenegraph();
    Scenegraph(std::string const& name, std::shared_ptr<Node> const& root);
    ~Scenegraph();

    std::string getName() const;
    void setName(std::string const& newName);

    std::shared_ptr<Node> getRoot() const;
    void setRoot(Node const& node);

    std::string printGraph() const;

private:
    std::string name_;
    std::shared_ptr<Node> root_; 

};

#endif //SCENEGRAPH_HPP
