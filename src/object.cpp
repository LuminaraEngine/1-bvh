#include <vec2.hpp>
#include <object.hpp>
#include <bvh_node.hpp>
#include <bvh.hpp>

#include <cstdio>
#include <iostream>
#include <stack>

using namespace bvh;

int parse_obj_file(char *obj_filename, Triangle **triangles)
{

  // Open the file
  FILE *file = fopen(obj_filename, "r");
  if (!file)
  {
    std::cerr << "Error: Could not open file " << obj_filename << std::endl;
    return -1;
  }

  // 1st pass: count the number
  int num_vertices = 0;
  int num_texture_coords = 0;
  int num_normals = 0;
  int num_faces = 0;

  char line[256];
  while (fgets(line, 256, file))
  {
    if (line[0] == 'v')
    {
      if (line[1] == ' ')
      {
        num_vertices++;
      }
      else if (line[1] == 't')
      {
        num_texture_coords++;
      }
      else if (line[1] == 'n')
      {
        num_normals++;
      }
    }
    else if (line[0] == 'f')
    {
      num_faces++;
    }
  }

  fseek(file, 0, SEEK_SET); // rewind

  // 2nd pass: read the vertices and faces
  vec3<float> *vertices = new vec3<float>[num_vertices];
  vec2<float> *texture_coords = new vec2<float>[num_texture_coords];
  vec3<float> *normals = new vec3<float>[num_normals];
  *triangles = new Triangle[num_faces];

  int vertex_index = 0;
  int texture_coord_index = 0;
  int normal_index = 0;
  int face_index = 0;

  int v0, vt0, vn0, v1, vt1, vn1, v2, vt2, vn2;
  while (fgets(line, 256, file))
  {
    if (line[0] == 'v')
    {
      if (line[1] == ' ')
      {
        sscanf(line, "v %f %f %f", &vertices[vertex_index].x, &vertices[vertex_index].y, &vertices[vertex_index].z);
        vertex_index++;
      }
      else if (line[1] == 't')
      {
        sscanf(line, "vt %f %f", &texture_coords[texture_coord_index].x, &texture_coords[texture_coord_index].y);
        texture_coord_index++;
      }
      else if (line[1] == 'n')
      {
        sscanf(line, "vn %f %f %f", &normals[normal_index].x, &normals[normal_index].y, &normals[normal_index].z);
        normal_index++;
      }
    }
    else if (line[0] == 'f')
    {
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

Object::Object(vec3<float> position, vec3<float> rotation, vec3<float> scale, Triangle *triangles, int num_triangles, BvhNode *bvh)
    : position(position), rotation(rotation), scale(scale), triangles(triangles), num_triangles(num_triangles), bvh(bvh) {}

Object::~Object()
{
  delete[] triangles;
  delete bvh;
}

void Object::build_bvh(char *obj_filename, char *bvh_filename)
{
  // Parse the obj file
  Triangle *triangles;
  int num_triangles = parse_obj_file(obj_filename, &triangles);

  if (num_triangles == -1)
  {
    return;
  }

  // Build the BVH
  BvhNode *bvh = precompute_bvh(triangles, num_triangles);

  // Save the BVH to a file
  save_bvh(bvh_filename, bvh);

  return;
}

// TODO:
// 1- design .bvh file format
// 2- write function to save the BVH to a file
// 3- write function to load the BVH from a file

// Function to save the BVH structure to a file
void Object::save_bvh(char *bvh_filename, BvhNode *bvh)
{
  // Open the BVH file for writing
  FILE *file = fopen(bvh_filename, "w"); // Open for writing
  if (!file)
  {
    std::cerr << "Error: Could not open file " << bvh_filename << " for writing." << std::endl;
    return;
  }

  int num_nodes = 1; // Count the number of nodes that were stored in the BVH file

  // Depth-first traversal to save BVH structure
  std::vector<BvhNode *> node_tree_stack; // Stack to hold nodes for depth-first traversal
  node_tree_stack.push_back(bvh);         // Store the root node
  while (!node_tree_stack.empty())
  {
    BvhNode *current_node = node_tree_stack.back(); // Get the current node
    node_tree_stack.pop_back();                     // Remove the current node from the stack

    // Put leaf information inside the BVH file
    if (dynamic_cast<BvhLeaf *>(current_node)) // Check if the current node is a leaf
    {
      BvhLeaf *current_leaf = dynamic_cast<BvhLeaf *>(current_node); // Get the node as a leaf

      // Format in BVH file: l <min.x> <min.y> <min.z> <max.x> <max.y> <max.z> <triangle_1> [<triangle_2> <triangle_3> ...]
      fprintf(file, "l %f %f %f %f %f %f %d [",
              current_leaf->bounding_box.min.x, current_leaf->bounding_box.min.y, current_leaf->bounding_box.min.z,
              current_leaf->bounding_box.max.x, current_leaf->bounding_box.max.y, current_leaf->bounding_box.max.z,
              current_leaf->indices[0]);
      for (int i = 1; i < current_leaf->num_triangles; i++)
      {
        if (i < current_leaf->num_triangles - 1)
        {
          fprintf(file, "%d ", current_leaf->indices[i]);
        }
        else if (i == current_leaf->num_triangles - 1)
        {
          fprintf(file, "%d]\n", current_leaf->indices[i]);
        }
      }
      num_nodes++;
    }

    // Put node information inside the BVH file
    else // It's an internal node
    {
      // Format in BVH file: n <min.x> <min.y> <min.z> <max.x> <max.y> <max.z>
      fprintf(file, "n %f %f %f %f %f %f\n",
              current_node->bounding_box.min.x, current_node->bounding_box.min.y, current_node->bounding_box.min.z,
              current_node->bounding_box.max.x, current_node->bounding_box.max.y, current_node->bounding_box.max.z);

      // Save the node's children
      if (current_node->left)
      {
        node_tree_stack.push_back(current_node->left); // Push left child onto stack
      }
      if (current_node->right)
      {
        node_tree_stack.push_back(current_node->right); // Push right child onto stack
      }
      num_nodes++;
    }
  }

  // Put tree information inside the BVH file
  for (int line_num = 1; line_num <= num_nodes; ++line_num)
  {
    int left_child_index = (2 * line_num);
    int right_child_index = (2 * line_num) + 1;

    // If the node has two children
    if (left_child_index < line_num) // Make sure the child indices do not exceed the total number of nodes
    {
      fprintf(file, "t %d %d %d\n", line_num, left_child_index, right_child_index);
    }

    // If the node only has a left child
    else if (right_child_index == line_num)
    {
      fprintf(file, "t %d %d -1\n", line_num, left_child_index);
    }

    // If the node is a leaf
    else
    {
      fprintf(file, "t %d -1 -1\n", line_num);
    }
  }

  fclose(file);
}

Object *Object::load(char *obj_filename, char *bvh_filename)
{
  // TODO: Testing team

  // Parse the obj file

  return nullptr;
}
