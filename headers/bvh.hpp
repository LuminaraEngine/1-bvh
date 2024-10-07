#pragma once

#include <triangle.hpp>
#include <object.hpp>
#include <bvh_node.hpp>

namespace bvh {

  BvhNode *precompute_bvh(Triangle *tris, int num_tris, int startIndex);
  BvhNode *build_bvh(Object *objs, int num_objs);

}
