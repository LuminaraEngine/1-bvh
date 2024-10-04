#include <object.hpp>
#include <triangle.hpp>
#include <bvh.hpp>
#include <cstdlib> // For rand() and RAND_MAX
#include <ctime>   // For time() to seed random number generator
#include <iostream>
#include <iomanip> // For std::setprecision

using namespace bvh;

// Function to print BVH nodes
void print_bvh(BvhNode* node, int depth = 0) {
    // Check if the node is null
    if (node == nullptr) {
        return;
    }

    // Print indentation based on depth
    std::cout << std::string(depth * 2, ' '); // 2 spaces per depth level

    // Print bounding box information with set precision for float values
    std::cout << "Node Bounding Box: ["
              << std::fixed << std::setprecision(2) // Set precision for floating-point numbers
              << "Min: (" << node->bounding_box.min.x << ", " 
              << node->bounding_box.min.y << ", " 
              << node->bounding_box.min.z << "), "
              << "Max: (" << node->bounding_box.max.x << ", " 
              << node->bounding_box.max.y << ", " 
              << node->bounding_box.max.z << ")]";

    // Check if the node is a leaf
    BvhLeaf* leaf = dynamic_cast<BvhLeaf*>(node);
    if (leaf && leaf->indices != nullptr) {
        std::cout << " | Leaf Node with " << leaf->num_triangles << " triangles: ";
        for (int i = 0; i < leaf->num_triangles; ++i) {
            std::cout << leaf->indices[i] << (i < leaf->num_triangles - 1 ? ", " : "");
        }
    }

    std::cout << std::endl; // New line for better readability

    // Recursively print left and right children
    print_bvh(node->left, depth + 1);
    print_bvh(node->right, depth + 1);
}

// Create the objects which contain the triangles
void create_objects(bvh::Object* objs, int num_objs) {
    std::srand(static_cast<unsigned int>(std::time(nullptr))); // Seed with current time

    for (int i = 0; i < num_objs; ++i) {
        objs[i].num_triangles = 20; // Set the number of triangles for each object
        objs[i].triangles = new Triangle[objs[i].num_triangles]; // Dynamically allocate triangles


        // Check if allocation was successful
        if (objs[i].triangles == nullptr) {
            std::cerr << "Memory allocation failed!" << std::endl;
            exit(1);
        }

        // Initialize the triangles with random vertices
        for (int j = 0; j < objs[i].num_triangles; ++j) {
            // Generate random coordinates for each vertex
            for (int k = 0; k < 3; ++k) {
                objs[i].triangles[j].vertices[k] = vec3<float>(
                    static_cast<float>(rand()) / RAND_MAX * 100.0f,
                    static_cast<float>(rand()) / RAND_MAX * 100.0f,
                    static_cast<float>(rand()) / RAND_MAX * 100.0f
                );
            }
        }
    }
}


// Main function
int main() {
    // Create an array of objects
    int num_objs = 3;
    bvh::Object* objs = new bvh::Object[num_objs];

    // Create objects and initialize triangles
    create_objects(objs, num_objs);

    // Print the triangle vertices for each object
    for (int i = 0; i < num_objs; ++i) {
        std::cout << "Object " << i << " contains the following triangles:\n";
        for (int j = 0; j < objs[i].num_triangles; ++j) {
            std::cout << "Triangle " << j << ": "
                      << "V0: (" << objs[i].triangles[j].vertices[0].x << ", "
                      << objs[i].triangles[j].vertices[0].y << ", "
                      << objs[i].triangles[j].vertices[0].z << "), "
                      << "V1: (" << objs[i].triangles[j].vertices[1].x << ", "
                      << objs[i].triangles[j].vertices[1].y << ", "
                      << objs[i].triangles[j].vertices[1].z << "), "
                      << "V2: (" << objs[i].triangles[j].vertices[2].x << ", "
                      << objs[i].triangles[j].vertices[2].y << ", "
                      << objs[i].triangles[j].vertices[2].z << ")\n";
        }
    }

    // Build BVH for each object
    for (int i = 0; i < num_objs; ++i) {
        objs[i].bvh = build_bvh(objs, num_objs); // Make sure to allocate the BVH node
    }

    // Print the BVH nodes
    for (int i = 0; i < num_objs; ++i) {
        std::cout << "Object " << i << " BVH:\n";
        print_bvh(objs[i].bvh);
    }

    // Clean up allocated memory
    delete[] objs; // Delete the objects array
    return 0;
}
//   char obj_filename[] = "./test.obj";
//   char bvh_filename[] = "./test.bvh";

//   bvh::Object::build_bvh(obj_filename, bvh_filename);
//   return 0;
// }
