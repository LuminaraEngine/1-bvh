#include <test_load_bvh.hpp>
#include <custom_assert.hpp>
#include <object.hpp>

void bvh::tests::load_bvh_leaf() {
  Object *obj = Object::load("../tests/data/final/triangle.obj", "../tests/data/final/leaf.bvh");

  if (obj == nullptr) {
    assert(false, "Object::load() returned nullptr");
  }

  assert(obj->bvh != nullptr, "obj->bvh is nullptr");
  assert(obj->bvh->left == nullptr, "obj->bvh is not a leaf node - has a left child");
  assert(obj->bvh->right == nullptr, "obj->bvh is not a leaf node - has a right child");
  bvh::BvhLeaf *leaf = dynamic_cast<bvh::BvhLeaf*>(obj->bvh);
  assert(leaf != nullptr, "obj->bvh is not a leaf node - does not cast to BvhLeaf");
  assert(leaf->bounding_box.min.x == 0.0f, "obj->bvh does not contain the correct bounding box min x");
  assert(leaf->bounding_box.min.y == 0.0f, "obj->bvh does not contain the correct bounding box min y");
  assert(leaf->bounding_box.min.z == 0.0f, "obj->bvh does not contain the correct bounding box min z");
  assert(leaf->bounding_box.max.x == 1.0f, "obj->bvh does not contain the correct bounding box max x");
  assert(leaf->bounding_box.max.y == 1.0f, "obj->bvh does not contain the correct bounding box max y");
  assert(leaf->bounding_box.max.z == 1.0f, "obj->bvh does not contain the correct bounding box max z");
  assert(leaf->num_triangles == 4, "obj->bvh does not contain the correct (4) number of triangles");
  int expected_triangle_indices[4] = {0, 3, 2, 1};
  for (int i = 0; i < 4; i++) {
    assert(leaf->indices[i] == expected_triangle_indices[i], "obj->bvh does not contain the correct triangle indices");
  }
}

void bvh::tests::load_bvh_node() {
  Object *obj = Object::load("../tests/data/final/triangle.obj", "../tests/data/node.bvh");

  if (obj == nullptr) {
    assert(false, "Object::load() returned nullptr");
  }

  // The bvh was loaded
  assert(obj->bvh != nullptr, "obj->bvh is nullptr");

  // The root of the bvh is an inner node
  assert(obj->bvh->left != nullptr, "obj->bvh is not an inner node - does not have a left child");
  assert(obj->bvh->right != nullptr, "obj->bvh is not an inner node - does not have a right child");
  bvh::BvhLeaf* node = dynamic_cast<bvh::BvhLeaf*>(obj->bvh);
  assert(node == nullptr, "obj->bvh is not an inner node - casts to BvhLeaf");
  assert(obj->bvh->bounding_box.min.x == 0.0f, "obj->bvh does not contain the correct bounding box min x");
  assert(obj->bvh->bounding_box.min.y == 0.0f, "obj->bvh does not contain the correct bounding box min y");
  assert(obj->bvh->bounding_box.min.z == 0.0f, "obj->bvh does not contain the correct bounding box min z");
  assert(obj->bvh->bounding_box.max.x == 10.0f, "obj->bvh does not contain the correct bounding box max x");
  assert(obj->bvh->bounding_box.max.y == 10.0f, "obj->bvh does not contain the correct bounding box max y");
  assert(obj->bvh->bounding_box.max.z == 10.0f, "obj->bvh does not contain the correct bounding box max z");

  // The left child of the root is a leaf node
  bvh::BvhLeaf *left = dynamic_cast<bvh::BvhLeaf*>(obj->bvh->left);
  assert(left != nullptr, "obj->bvh left child is not a leaf node - does not cast to BvhLeaf");
  assert(left->bounding_box.min.x == 0.0f, "obj->bvh left child does not contain the correct bounding box min x");
  assert(left->bounding_box.min.y == 0.0f, "obj->bvh left child does not contain the correct bounding box min y");
  assert(left->bounding_box.min.z == 0.0f, "obj->bvh left child does not contain the correct bounding box min z");
  assert(left->bounding_box.max.x == 1.0f, "obj->bvh left child does not contain the correct bounding box max x");
  assert(left->bounding_box.max.y == 1.0f, "obj->bvh left child does not contain the correct bounding box max y");
  assert(left->bounding_box.max.z == 1.0f, "obj->bvh left child does not contain the correct bounding box max z");
  assert(left->num_triangles == 3, "obj->bvh left child does not contain the correct (3) number of triangles");
  int expected_left_triangle_indices[3] = {1, 2, 3};
  for (int i = 0; i < 3; i++) {
    assert(left->indices[i] == expected_left_triangle_indices[i], "obj->bvh left child does not contain the correct triangle indices");
  }

  // The right child of the root is a leaf node
  bvh::BvhLeaf *right = dynamic_cast<bvh::BvhLeaf*>(obj->bvh->right);
  assert(right != nullptr, "obj->bvh right child is not a leaf node - does not cast to BvhLeaf");
  assert(right->bounding_box.min.x == 9.0f, "obj->bvh right child does not contain the correct bounding box min x");
  assert(right->bounding_box.min.y == 9.0f, "obj->bvh right child does not contain the correct bounding box min y");
  assert(right->bounding_box.min.z == 9.0f, "obj->bvh right child does not contain the correct bounding box min z");
  assert(right->bounding_box.max.x == 10.0f, "obj->bvh right child does not contain the correct bounding box max x");
  assert(right->bounding_box.max.y == 10.0f, "obj->bvh right child does not contain the correct bounding box max y");
  assert(right->bounding_box.max.z == 10.0f, "obj->bvh right child does not contain the correct bounding box max z");
  assert(right->num_triangles == 8, "obj->bvh right child does not contain the correct (8) number of triangles");
  int expected_right_triangle_indices[8] = {4, 5, 6, 7, 8, 9, 10, 11};
  for (int i = 0; i < 8; i++) {
    assert(right->indices[i] == expected_right_triangle_indices[i], "obj->bvh right child does not contain the correct triangle indices");
  }
}

void bvh::tests::load_bvh_with_comment() {
  Object *obj = Object::load("../tests/data/final/triangle.obj", "../tests/data/final/leaf_with_comment.bvh");

  if (obj == nullptr) {
    assert(false, "Object::load() returned nullptr");
  }

  assert(obj->bvh != nullptr, "obj->bvh is nullptr");
  assert(obj->bvh->left == nullptr, "obj->bvh is not a leaf node - has a left child");
  assert(obj->bvh->right == nullptr, "obj->bvh is not a leaf node - has a right child");
  bvh::BvhLeaf *leaf = dynamic_cast<bvh::BvhLeaf*>(obj->bvh);
  assert(leaf != nullptr, "obj->bvh is not a leaf node - does not cast to BvhLeaf");
  assert(leaf->num_triangles == 4, "obj->bvh does not contain the correct (4) number of triangles");
  assert(leaf->bounding_box.min.x == 0.0f, "obj->bvh does not contain the correct bounding box min x");
  assert(leaf->bounding_box.min.y == 0.0f, "obj->bvh does not contain the correct bounding box min y");
  assert(leaf->bounding_box.min.z == 0.0f, "obj->bvh does not contain the correct bounding box min z");
  assert(leaf->bounding_box.max.x == 1.0f, "obj->bvh does not contain the correct bounding box max x");
  assert(leaf->bounding_box.max.y == 1.0f, "obj->bvh does not contain the correct bounding box max y");
  assert(leaf->bounding_box.max.z == 1.0f, "obj->bvh does not contain the correct bounding box max z");
  int expected_triangle_indices[4] = {0, 3, 2, 1};
  for (int i = 0; i < 4; i++) {
    assert(leaf->indices[i] == expected_triangle_indices[i], "obj->bvh does not contain the correct triangle indices");
  }
}
