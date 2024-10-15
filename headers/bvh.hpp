#pragma once

#include <triangle.hpp>
#include <object.hpp>
#include <bvh_node.hpp>

namespace bvh {

  BvhNode *precompute_bvh(Triangle* tris, int start, int end);
  BvhNode *build_bvh_from_objects(Object *objs, int num_objs, int start);
  BoundingBox computeCombinedBoundingBox(const std::vector<BvhNode*>& bvhNodes);

}
