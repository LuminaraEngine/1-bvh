#include <vec2.hpp>
#include <object.hpp>
#include <bvh_node.hpp>
#include <bvh.hpp>

#include <cstdio>
#include <iostream>

using namespace bvh;

int parse_obj_file(char* obj_filename, Triangle** triangles) {

  // Open the file
  FILE* file = fopen(obj_filename, "r");
  if (!file) {
    std::cerr << "Error: Could not open file " << obj_filename << std::endl;
    return -1;
  }

  // 1st pass: count the number
  int num_vertices = 0;
  int num_texture_coords = 0;
  int num_normals = 0;
  int num_faces = 0;

  char line[256];
  while (fgets(line, 256, file)) {
    if (line[0] == 'v') {
      if (line[1] == ' ') {
        num_vertices++;
      } else if (line[1] == 't') {
        num_texture_coords++;
      } else if (line[1] == 'n') {
        num_normals++;
      }
    } else if (line[0] == 'f') {
      num_faces++;
    }
  }

  fseek(file, 0, SEEK_SET);   // rewind

  // 2nd pass: read the vertices and faces
  vec3<float>* vertices = new vec3<float>[num_vertices];
  vec2<float>* texture_coords = new vec2<float>[num_texture_coords];
  vec3<float>* normals = new vec3<float>[num_normals];
  *triangles = new Triangle[num_faces];

  int vertex_index = 0;
  int texture_coord_index = 0;
  int normal_index = 0;
  int face_index = 0;

  int v0, vt0, vn0, v1, vt1, vn1, v2, vt2, vn2;
  while (fgets(line, 256, file)) {
    if (line[0] == 'v') {
      if (line[1] == ' ') {
        sscanf(line, "v %f %f %f", &vertices[vertex_index].x, &vertices[vertex_index].y, &vertices[vertex_index].z);
        vertex_index++;
      } else if (line[1] == 't') {
        sscanf(line, "vt %f %f", &texture_coords[texture_coord_index].x, &texture_coords[texture_coord_index].y);
        texture_coord_index++;
      } else if (line[1] == 'n') {
        sscanf(line, "vn %f %f %f", &normals[normal_index].x, &normals[normal_index].y, &normals[normal_index].z);
        normal_index++;
      }
    } else if (line[0] == 'f') {
      sscanf(line, "f %d/%d/%d %d/%d/%d %d/%d/%d", &v0, &vt0, &vn0, &v1, &vt1, &vn1, &v2, &vt2, &vn2);

      (*triangles)[face_index].vertices[0] = vertices[v0 - 1];
      (*triangles)[face_index].vertices[1] = vertices[v1 - 1];
      (*triangles)[face_index].vertices[2] = vertices[v2 - 1];

      (*triangles)[face_index].uv[0] = texture_coords[vt0 - 1];
      (*triangles)[face_index].uv[1] = texture_coords[vt1 - 1];
      (*triangles)[face_index].uv[2] = texture_coords[vt2 - 1];

      (*triangles)[face_index].normals[0] = normals[vn0 - 1];
      (*triangles)[face_index].normals[1] = normals[vn1 - 1];
      (*triangles)[face_index].normals[2] = normals[vn2 - 1];

      face_index++;
    }
    // TODO: Handle groups, smoothing, materials, etc.
  }

  fclose(file);

  // Cleanup
  delete[] vertices;
  delete[] texture_coords;
  delete[] normals;

  return num_faces;

}

Object::Object(vec3<float> position, vec3<float> rotation, vec3<float> scale, Triangle *triangles, int num_triangles, BvhNode* bvh)
  : position(position), rotation(rotation), scale(scale), triangles(triangles), num_triangles(num_triangles), bvh(bvh) {}

Object::~Object() {
  delete[] triangles;
  delete bvh;
}

void Object::build_bvh(char* obj_filename, char* bvh_filename) {

  // Parse the obj file
  Triangle* triangles;
  int num_triangles = parse_obj_file(obj_filename, &triangles);

  if (num_triangles == -1) {
    return;
  }

  // Build the BVH
  BvhNode* bvh = precompute_bvh(triangles, num_triangles);

  // Save the BVH to a file
  // TODO:
  // 1- design .bvh file format
  // 2- write function to save the BVH to a file
  // 3- write function to load the BVH from a file


  return;

}

Object* Object::load(char* obj_filename, char* bvh_filename) {
  // TODO: Testing team

  // Parse the obj file

  return nullptr;
}
