#pragma once

#include <triangle.hpp>
#include <object.hpp>
#include <bvh_node.hpp>

namespace bvh {
  /**
   * @brief Precomputes the BVH for a list of triangles delimited by the indices [start, end[
   * @param tris The list of triangles
   * @param start The start index of the list
   * @param end The end index of the list (excluded)
   */
  BvhNode *precompute_bvh(Triangle* tris, int start, int end);

  /**
   * @brief Builds the BVH for a list of objects given the start index
   * @param objs The list of objects
   * @param num_objs The number of objects
   * @param start The start index of the list of objects
   */
  BvhNode *build_bvh_from_objects(Object *objs, int num_objs, int start);
  BoundingBox computeCombinedBoundingBox(const std::vector<BvhNode*>& bvhNodes);

}
