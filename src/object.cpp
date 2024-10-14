#include <vec2.hpp>
#include <object.hpp>
#include <bvh_node.hpp>
#include <bvh.hpp>

#include <cstdio>
#include <iostream>
#include <queue>

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

BvhNode *parse_bvh_file(char *bvh_filename)
{

  // Open the file
  FILE *file = fopen(bvh_filename, "r");
  if (!file)
  {
    std::cerr << "Error: Could not open file " << bvh_filename << std::endl;
    return nullptr;
  }

  // 1st pass: count the number of nodes
  int num_nodes = 0;
  int num_leaves = 0;

  char line[256];
  while (fgets(line, 256, file))
  {
    if (line[0] == 'n')
    {
      num_nodes++;
    }
    else if (line[0] == 'l')
    {
      num_leaves++;
    }
  }

  fseek(file, 0, SEEK_SET); // rewind

  // 2nd pass: read the nodes and leaves
  BvhNode *nodes = new BvhNode[num_nodes];
  BvhLeaf *leaves = new BvhLeaf[num_leaves];

  int nodes_index = 0;
  int leaves_index = 0;

  vec3<float> min, max;
  while (fgets(line, 256, file))
  {
    if (line[0] == 'n')
    {
      sscanf(line, "n %f %f %f %f %f %f", &min.x, &min.y, &min.z, &max.x, &max.y, &max.z);
      nodes[nodes_index] = BvhNode(min, max);
    }
    else if (line[0] == 'l')
    {
      int num_triangles = 0;
      int indices[BVH_LEAF_SIZE];

      num_triangles = sscanf(
                          line, "l %f %f %f %f %f %f %d %d %d %d %d %d %d %d",
                          &min.x, &min.y, &min.z, &max.x, &max.y, &max.z,
                          &indices[0], &indices[1], &indices[2], &indices[3], &indices[4], &indices[5], &indices[6], &indices[7]) -
                      6;

      leaves[leaves_index++] = BvhLeaf(min, max, num_triangles, indices);
    }
  }

  fclose(file);

  // Build the BVH
  BvhNode *root;

  if (num_nodes > 0)
  { // track the root node
    root = nodes;
  }
  else if (num_leaves > 0)
  {
    root = leaves;
  }
  else
  {
    return nullptr;
  }

  // Link the nodes
  for (int i = 1; i <= num_nodes; i++)
  {
    int i_left = 2 * i;
    int i_right = 2 * i + 1;

    if (i_right <= num_nodes)
    {
      nodes[i - 1].left = &nodes[i_left - 1];
      nodes[i - 1].right = &nodes[i_right - 1];
    }
    else
    {
      nodes[i - 1].left = &leaves[i_left - num_nodes - 1];
      nodes[i - 1].right = &leaves[i_right - num_nodes - 1];
    }
  }

  // Cleanup
  // Do not delete the nodes and leaves, as they live in the heap and are now owned by the root node

  return root;
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
  // BvhNode* bvh = precompute_bvh(triangles, num_triangles);

  // Save the BVH to a file
  // save_bvh(bvh_filename, bvh);

  return;
}

// TODO: write function to load the BVH from a file

/**
 * @brief Function to save the BVH structure to a file
 *
 * @param bvh_filename The name of the file to save the BVH structure to
 * @param bvh The root node of the BVH structure
 */
void Object::save_bvh(char *bvh_filename, BvhNode *bvh)
{
  // Open the BVH file for writing
  FILE *file = fopen(bvh_filename, "w"); // Open for writing
  if (!file)
  {
    std::cerr << "Error: Could not open file " << bvh_filename << " for writing." << std::endl;
    return;
  }

  // Breadth-first traversal to save BVH structure
  std::queue<BvhNode *> node_tree_queue;
  node_tree_queue.push(bvh); // Store the root node
  // std::cout << "Processing root node " << bvh << " with children " << bvh->left << " and " << bvh->right << std::endl;
  while (!node_tree_queue.empty())
  {
    BvhNode *current_node = node_tree_queue.front(); // Get the current node
    node_tree_queue.pop();                           // Remove the current node from the queue

    // Put leaf information inside the BVH file
    if (dynamic_cast<BvhLeaf *>(current_node)) // Check if the current node is a leaf
    {
      BvhLeaf *current_leaf = dynamic_cast<BvhLeaf *>(current_node); // Get the node as a leaf
      // std::cout << "Processing leaf node " << current_leaf << " with " << current_leaf->num_triangles << " triangles." << std::endl;

      // Format in BVH file: l <min.x> <min.y> <min.z> <max.x> <max.y> <max.z> <triangle_1> [<triangle_2> <triangle_3> ...]
      fprintf(file, "l %f %f %f %f %f %f %d ",
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
          fprintf(file, "%d\n", current_leaf->indices[i]);
        }
      }
    }

    // Put node information inside the BVH file
    else // It's an internal node
    {
      // std::cout << "Processing internal node " << current_node << " with children " << current_node->left << " and " << current_node->right << std::endl;
      // Format in BVH file: n <min.x> <min.y> <min.z> <max.x> <max.y> <max.z>
      fprintf(file, "n %f %f %f %f %f %f\n",
              current_node->bounding_box.min.x, current_node->bounding_box.min.y, current_node->bounding_box.min.z,
              current_node->bounding_box.max.x, current_node->bounding_box.max.y, current_node->bounding_box.max.z);

      // Save the node's children
      if (current_node->left)
      {
        node_tree_queue.push(current_node->left);
      }
      if (current_node->right)
      {
        node_tree_queue.push(current_node->right);
      }
    }

    // std::cout << "Queue size: " << node_tree_queue.size() << std::endl;
  }

  fclose(file);
}

Object *Object::load(char *obj_filename, char *bvh_filename)
{
  // TODO: Testing team

  // Parse the obj file
  Triangle *triangles;
  int num_triangles = parse_obj_file(obj_filename, &triangles);

  if (num_triangles == -1)
  {
    return nullptr;
  }

  // Parse the bvh file
  BvhNode *bvh = parse_bvh_file(bvh_filename);

  if (bvh == nullptr)
  {
    return nullptr;
  }

  // Build and return object
  return new Object(vec3<float>(0, 0, 0), vec3<float>(0, 0, 0), vec3<float>(1, 1, 1), triangles, num_triangles, bvh);
}
