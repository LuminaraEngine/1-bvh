#include <test_load_bvh.hpp>
#include <object.hpp>

bool bvh::test_load_bvh() {
  Object *obj = Object::load("../tests/data/final/triangle.obj", "../tests/data/final/leaf.bvh");

  if (obj == nullptr) {
    return false;
  }

  // TODO: check that the object was loaded correctly

  return true;
}
