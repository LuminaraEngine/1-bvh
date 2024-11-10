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
   * @return The root node of the BVH
   */
  BvhNode *precompute_bvh(Triangle* tris, int start, int end);

  /**
   * @brief Builds the BVH for a list of objects given the start index
   * @param objs The list of objects
   * @param num_objs The number of objects
   * @param start The start index of the list of objects
   * @param new_triangles_list (OUTPUT) The list of triangles that will contain all the triangles of the objs. If the list passed here is not nullptr, the function will delete it and allocate a new one
   * @param total_num_triangles (OUTPUT) The total number of triangles in the list
   * @return The root node of the BVH
   */
  BvhNode *build_bvh_from_objects(Object *objs, int num_objs, int start, Triangle** new_triangles_list, int* total_num_triangles);

}
