#include <object.hpp>

#include <iostream>

int main() {
  char obj_filename[] = "./test.obj";
  char bvh_filename[] = "./test.bvh";

  bvh::Object::build_bvh(obj_filename, bvh_filename);
  return 0;
}
