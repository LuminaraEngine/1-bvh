#include <bvh_node.hpp>

#include <cstdio>

using namespace bvh;

BvhNode::BvhNode() 
    : bounding_box(vec3<float>(0, 0, 0), vec3<float>(0, 0, 0)),  // Initialize bounding box
      left(nullptr), right(nullptr) {}  // Initialize left and right pointers to nullptr

BvhNode::BvhNode(vec3<float> min, vec3<float> max) : bounding_box(min, max) {
  this->left = nullptr;
  this->right = nullptr;
}

BvhNode::~BvhNode() {
  //std::cout << "Deleting BvhNode" << this <<  std::endl;
  //std::cout << "Deleting left" << this->left <<  std::endl;
  //std::cout << "Deleting right" << this->right <<  std::endl;
  delete left;
  delete right;
}


void BvhNode::print(int depth) {
    // Print the current node's bounding box
    printf("%*sBvhNode\n", depth * 2, ""); // Indent based on depth
    printf("%*smin: %f %f %f\n", depth * 2, "", this->bounding_box.min.x, this->bounding_box.min.y, this->bounding_box.min.z);
    printf("%*smax: %f %f %f\n", depth * 2, "", this->bounding_box.max.x, this->bounding_box.max.y, this->bounding_box.max.z);

    // Print left child if it exists
    if (this->left) {
        this->left->print(depth + 1); // Increase depth for indentation
    } else {
        printf("%*sleft: nullptr\n", depth * 2, "");
    }

    // Print right child if it exists
    if (this->right) {
        this->right->print(depth + 1); // Increase depth for indentation
    } else {
        printf("%*sright: nullptr\n", depth * 2, "");
    }
}

// void BvhNode::deleteBvh() {
//     if (this == nullptr) return;

//     // Recursively delete children if they exist
//     if (this->left) {
//         this->left->deleteBvh();
//         this->left = nullptr;  // Set pointer to nullptr to avoid double deletion
//     }
//     if (this->right) {
//         this->right->deleteBvh();
//         this->right = nullptr;  // Set pointer to nullptr to avoid double deletion
//     }

//     // Delete the node itself
//     delete this;
// }

BvhLeaf::BvhLeaf() : BvhNode() {
  this->num_triangles = 0;
}

BvhLeaf::BvhLeaf(vec3<float> min, vec3<float> max, int num_triangles, int* indices) : BvhNode(min, max) {
  this->num_triangles = num_triangles;
  for (int i = 0; i < num_triangles; i++) {
    this->indices[i] = indices[i];
  }
}

BvhLeaf::~BvhLeaf() {}

void BvhLeaf::print(int depth=0) {
  printf("%*sBvhLeaf\n", depth * 2, "");
  printf("%*smin: %f %f %f\n", depth * 2, "", this->bounding_box.min.x, this->bounding_box.min.y, this->bounding_box.min.z);
  printf("%*smax: %f %f %f\n", depth * 2, "", this->bounding_box.max.x, this->bounding_box.max.y, this->bounding_box.max.z);
  printf("%*snum_triangles: %d\n", depth * 2, "", this->num_triangles);
  for (int i = 0; i < this->num_triangles; i++) {
    if (i == 0) {
      printf("%*s", depth * 2, "");
    }
    printf("%i ", this->indices[i]);
  }
  printf("\n");
}
