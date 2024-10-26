#include <bounding_box.hpp>

using namespace bvh;

bool BoundingBox::operator==(const BoundingBox& other) const {
    return min == other.min && max == other.max;
}

BoundingBox::BoundingBox(vec3<float> min, vec3<float> max) : min(min), max(max) {}
