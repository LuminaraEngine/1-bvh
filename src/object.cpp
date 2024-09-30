#include <object.hpp>

using namespace bvh;

Object::Object(float3 position, float3 rotation, float3 scale, Triangle *triangles, int num_triangles, BvhNode* bvh) {
  this->position = position;
  this->rotation = rotation;
  this->scale = scale;
  this->triangles = triangles;
  this->num_triangles = num_triangles;
  this->bvh = bvh;
}

Object::~Object() {
  delete[] triangles;
  delete bvh;
}

Object* Object::build(char* obj_filename) {
  // TODO: Testing team
}

Object* Object::build(char* obj_filename, char* bvh_filename) {
  // TODO: Testing team
}
