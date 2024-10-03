#include <bvh.hpp>

namespace bvh {

  BvhNode* precompute_bvh(Triangle* tris, int num_tris) {
    // TODO: Implementation team
    // Should handle arrays of < 8 triangles by returning a leaf node

    if (num_tris <= 0 || tris == nullptr) { return nullptr; }

    float min_x = tris[0].vertices[0].x; // for bounding box
    float min_y = tris[0].vertices[0].y;
    float min_z = tris[0].vertices[0].z;
    float max_x = tris[0].vertices[0].x;
    float max_y = tris[0].vertices[0].y;
    float max_z = tris[0].vertices[0].z;

    for (int i = 0; i < num_tris; i++) {
      for (int j = 0; j < 3; j++) {
        if (tris[i].vertices[j].x < min_x) { min_x = tris[i].vertices[j].x; }
        if (tris[i].vertices[j].y < min_y) { min_y = tris[i].vertices[j].y; }
        if (tris[i].vertices[j].z < min_z) { min_z = tris[i].vertices[j].z; }
        if (tris[i].vertices[j].x > max_x) { max_x = tris[i].vertices[j].x; }
        if (tris[i].vertices[j].y > max_y) { max_y = tris[i].vertices[j].y; }
        if (tris[i].vertices[j].z > max_z) { max_z = tris[i].vertices[j].z; }
      }
    }

    if (num_tris <= BVH_LEAF_SIZE) {
      int* indices = new int[num_tris]; // idk
      for (int i = 0; i < num_tris; i++) {
        indices[i] = i;
      }

      return new BvhLeaf(vec3<float>(min_x, min_y, min_z), vec3<float>(max_x, max_y, max_z), num_tris, indices);
    }

    // > 8 triangles

    BvhNode* node = new BvhNode(vec3<float>(min_x, min_y, min_z), vec3<float>(max_x, max_y, max_z));

    int mid = num_tris / 2;
    Triangle* left_tris = new Triangle[mid];
    Triangle* right_tris = new Triangle[num_tris - mid];

    for (int i = 0; i < mid; i++) {
      left_tris[i] = tris[i];
    }
    for (int i = mid; i < num_tris; i++) {
      right_tris[i - mid] = tris[i];
    }

    node->left = precompute_bvh(left_tris, mid);
    node->right = precompute_bvh(right_tris, num_tris - mid);

    return node;
  }

  BvhNode* build_bvh(Object* objs, int num_objs) {
    // TODO: Implementation team
    return nullptr;
  }

}