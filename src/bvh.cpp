#include <limits>
#include <vector>
#include <bvh.hpp>
#include <algorithm>
#include <iostream>

namespace bvh {

std::vector<Triangle> sort_triangles_by_centroid(std::vector<Triangle>& triangles) {
    std::sort(triangles.begin(), triangles.end(), [](const Triangle& a, const Triangle& b) {
        vec3<float> centroid_a = (a.vertices[0] + a.vertices[1] + a.vertices[2]) / 3.0f;
        vec3<float> centroid_b = (b.vertices[0] + b.vertices[1] + b.vertices[2]) / 3.0f;
        return centroid_a.x < centroid_b.x; // Sort along the x-axis
    });
    return triangles;
}

// Need start index 
BvhNode* precompute_bvh(Triangle* tris, int num_tris, int start) {
    // Handle empty or invalid input
    if (num_tris <= 0 || tris == nullptr) { return nullptr; }

    // Initialize bounding box values
    float min_x = std::numeric_limits<float>::max();
    float min_y = std::numeric_limits<float>::max();
    float min_z = std::numeric_limits<float>::max();
    float max_x = std::numeric_limits<float>::lowest();
    float max_y = std::numeric_limits<float>::lowest();
    float max_z = std::numeric_limits<float>::lowest();

    // Calculate the bounding box
    for (int i = 0; i < num_tris; i++) {
        for (int j = 0; j < 3; j++) {
            const vec3<float>& vertex = tris[i].vertices[j];
            min_x = std::min(min_x, vertex.x);
            min_y = std::min(min_y, vertex.y);
            min_z = std::min(min_z, vertex.z);
            max_x = std::max(max_x, vertex.x);
            max_y = std::max(max_y, vertex.y);
            max_z = std::max(max_z, vertex.z);
        }
    }

    // Handle the leaf case
    if (num_tris <= BVH_LEAF_SIZE) {
        std::vector<int> indices(num_tris);
        for (int i = 0; i < num_tris; i++) {
            indices[i] = i;  // Store triangle indices
        }

        return new BvhLeaf(vec3<float>(min_x, min_y, min_z), vec3<float>(max_x, max_y, max_z), num_tris, indices.data());
    }

    // Create a new BVH node
    BvhNode* node = new BvhNode(vec3<float>(min_x, min_y, min_z), vec3<float>(max_x, max_y, max_z));

    // Split triangles into left and right children
    int mid = num_tris / 2; // This can be improved with a better heuristic
    std::vector<Triangle> left_tris(tris, tris + mid);
    std::vector<Triangle> right_tris(tris + mid, tris + num_tris);

    // Recursively compute BVH for left and right children
    node->left = precompute_bvh(left_tris.data(), mid, 0);
    node->right = precompute_bvh(right_tris.data(), num_tris - mid, mid);

    return node;
}

// This should create a list of all the objects in the scene and a new BVH of all the objects' BVH where the indices point to the huge list of triangles.
BvhNode* build_bvh(Object* objs, int num_objs) {
    // Collect all triangles from the objects
    std::vector<Triangle> all_tris;

    for (int i = 0; i < num_objs; ++i) {
        Object& obj = objs[i];

        // Check for valid triangle data
        if (obj.triangles == nullptr || obj.num_triangles <= 0) {
            std::cerr << "Warning: Object " << i << " has invalid triangle data." << std::endl;
            continue; // Skip this object
        }

        // Collect triangles
        for (int j = 0; j < obj.num_triangles; ++j) {
            all_tris.push_back(obj.triangles[j]);
        }
    }

    // If there are no triangles, return null
    if (all_tris.empty()) {
        std::cerr << "Error: No triangles available to build BVH." << std::endl;
        return nullptr;
    }

    // Sort the triangles by their centroids before building the BVH
    sort_triangles_by_centroid(all_tris);

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


    // Build the BVH using the collected triangles
    //return precompute_bvh(all_tris.data(), all_tris.size());
    // Compile all the BVHs into one large BVH
}

}