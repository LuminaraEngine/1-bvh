#include <bounding_box.hpp>

using namespace bvh;

BoundingBox::BoundingBox(vec3<float> min, vec3<float> max) : min(min), max(max) {}
