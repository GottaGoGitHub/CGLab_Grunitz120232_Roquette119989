#ifndef GEOMETRY_HPP
#define GEOMETRY_HPP

#include "node.hpp"
#include "model.hpp"
#include "structs.hpp"
#include <iostream>
#include <string>

class GeometryNode : public Node {
public:
    GeometryNode();
    GeometryNode(std::shared_ptr<Node> parent, std::string const& name);
    GeometryNode(model const& model);
    ~GeometryNode();

    model getGeometry();
    void setGeometry(model const& model);

    model_object getObject();
    
private:
    model geometry_;
    model_object geometryObject_;
};

#endif //GEOMETRY_HPP