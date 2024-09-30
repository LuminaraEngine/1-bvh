#include <object.hpp>

using namespace bvh;

Object::Object(vec3<float> position, vec3<float> rotation, vec3<float> scale, Triangle *triangles, int num_triangles, BvhNode* bvh)
  : position(position), rotation(rotation), scale(scale), triangles(triangles), num_triangles(num_triangles), bvh(bvh) {}

Object::~Object() {
  delete[] triangles;
  delete bvh;
}

Object* Object::build(char* obj_filename) {
  // TODO: Testing team

  return nullptr;
}

Object* Object::build(char* obj_filename, char* bvh_filename) {
  // TODO: Testing team

  return nullptr;
}
