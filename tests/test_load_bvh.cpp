#include <test_load_bvh.hpp>
#include <custom_assert.hpp>
#include <object.hpp>

void bvh::tests::load_bvh() {
  Object *obj = Object::load("../tests/data/final/triangle.obj", "../tests/data/final/leaf.bvh");

  if (obj == nullptr) {
    assert(false, "Failed to load object from .obj and .bvh files");
  }

  assert(obj->num_triangles == 12, "num_triangles should be 12");

  // TODO: check that the object was loaded correctly
}
