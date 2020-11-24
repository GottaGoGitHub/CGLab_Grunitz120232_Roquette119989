#ifndef GEOMETRY_HPP
#define GEOMETRY_HPP

#include "node.hpp"
#include "model.hpp"
#include <iostream>
#include <string>

class GeometryNode : public Node {
public:
    GeometryNode();
    GeometryNode(model const& model);
    ~GeometryNode();

    model getGeometry();
    void setGeometry(model const& model);
    
private:
    model geometry_;
};

#endif //GEOMETRY_HPP