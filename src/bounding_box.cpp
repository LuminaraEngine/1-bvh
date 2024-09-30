#include <bounding_box.hpp>

using namespace bvh;

BoundingBox::BoundingBox(float3 min, float3 max) {
  this->min = min;
  this->max = max;
}
