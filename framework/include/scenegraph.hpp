#ifndef SCENEGRAPH_HPP
#define SCENEGRAPH_HPP

#include "node.hpp"
#include "point_light_node.hpp"
#include <string>
#include <list>

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

    void addLightNode(std::shared_ptr<PointLightNode> const& light);
    std::list<std::shared_ptr<PointLightNode>> getAllLights();

private:
    std::string name_;
    std::shared_ptr<Node> root_; 

protected:
    std::list<std::shared_ptr<PointLightNode>> lightNodes_;
};

#endif //SCENEGRAPH_HPP
