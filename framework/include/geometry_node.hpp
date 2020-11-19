#ifndef GEOMETRY_HPP
#define GEOMETRY_HPP

#include "node.hpp"
#include "model.hpp"

#include <string>

class GeometryNode : public Node {
public:
    GeometryNode();
    ~GeometryNode();

    model getGeometry();
    void setGeometry(model const& model);
    
private:
    model geometry;
};

#endif //GEOMETRY_HPP