#include <limits>
#include <vector>
#include <bvh.hpp>
#include <algorithm>
#include <iostream>
#include "object.hpp"

namespace bvh{

// Utility function to compute the bounding box for combined BVHs
    BoundingBox computeCombinedBoundingBox(const std::vector<BvhNode*>& bvhNodes) {
        if (bvhNodes.empty()) return BoundingBox(); // Return empty box if no BVH nodes

        vec3<float> minCorner = bvhNodes[0]->bounding_box.min;
        vec3<float> maxCorner = bvhNodes[0]->bounding_box.max;

        for (const auto& node : bvhNodes) {
            minCorner.x = std::min(minCorner.x, node->bounding_box.min.x);
            minCorner.y = std::min(minCorner.y, node->bounding_box.min.y);
            minCorner.z = std::min(minCorner.z, node->bounding_box.min.z);

            maxCorner.x = std::max(maxCorner.x, node->bounding_box.max.x);
            maxCorner.y = std::max(maxCorner.y, node->bounding_box.max.y);
            maxCorner.z = std::max(maxCorner.z, node->bounding_box.max.z);
        }

        return BoundingBox(minCorner, maxCorner);
    }

BvhNode* precompute_bvh(Triangle* tris, int start, int end) {
    int num_tris = end - start;

    // Handle empty or invalid input
    if (start < 0 || end > start+num_tris || start >= end || tris == nullptr) {
      return nullptr; // Return null for invalid ranges
    }

    // Initialize bounding box values
    float min_x = std::numeric_limits<float>::max();
    float min_y = std::numeric_limits<float>::max();
    float min_z = std::numeric_limits<float>::max();
    float max_x = std::numeric_limits<float>::lowest();
    float max_y = std::numeric_limits<float>::lowest();
    float max_z = std::numeric_limits<float>::lowest();

    // Calculate the bounding box
    for (int i = start; i < end; i++) {
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
    //std::cout << "min_x: " << min_x << " min_y: " << min_y << " min_z: " << min_z << std::endl;
    //std::cout << "max_x: " << max_x << " max_y: " << max_y << " max_z: " << max_z << std::endl;

    // Construct the bounding box for the current node
    vec3<float> min(min_x, min_y, min_z);
    vec3<float> max(max_x, max_y, max_z);

    // Handle the leaf case MAYBE MOVE THIS AFTER SORTING
    if (num_tris <= BVH_LEAF_SIZE) {
        std::vector<int> indices(num_tris);
        for (int i = 0; i < num_tris; i++) {
            indices[i] = start + i;  // Store triangle indices
        }
        return new BvhLeaf(min, max, num_tris, indices.data());
    }
    // Compute the centroids of the triangles to decide on partitioning
    std::vector<vec3<float>> centroids(end - start);
    for (int i = start; i < end; i++) {
        centroids[i - start] = (tris[i].vertices[0] + tris[i].vertices[1] + tris[i].vertices[2]) / 3.0f;
    }

    // Initialize indices
    std::vector<int> indices(centroids.size());
    for (int i = 0; i < indices.size(); ++i) {
        indices[i] = i;  // Fill with 0, 1, 2, ..., N-1
    }

    // Print initial centroids
    //for (int i = start; i < end; i++) {
    //    std::cout << "Triangle Index: " << (i - start) << " Centroid: " << centroids[i - start].x << std::endl;
    //}

    // Sort indices based on the centroid values along the x-axis
    std::sort(indices.begin(), indices.end(), [&centroids](int a, int b) {
        return centroids[a].x < centroids[b].x;
    });

    // After sorting, update the triangles based on the sorted indices
    std::vector<bvh::Triangle> sortedTris(end - start);
    for (int i = 0; i < indices.size(); ++i) {
        sortedTris[i] = tris[start + indices[i]];
    }

    // Copy the sorted triangles back to the original tris array
    for (int i = 0; i < sortedTris.size(); ++i) {
        tris[start + i] = sortedTris[i];
    }

    // Print the centroids after sorting to confirm
    //for (int i = start; i < end; i++) {
    //    std::cout << "Triangle Index: " << indices[i - start] << "Triangle Data" << tris[i-start].vertices->x << " Centroid: " << centroids[indices[i - start]].x << std::endl;
    //}


    // Split the triangles in half
    int mid = start + num_tris / 2;

    // Recursively build the left and right child nodes
    BvhNode* leftChild = precompute_bvh(tris, start, mid);
    BvhNode* rightChild = precompute_bvh(tris, mid, end); // Use end here

    // Create and return an internal node with the bounding box and child nodes
    BvhNode* node = new BvhNode(min, max);
    node->left = leftChild;
    node->right = rightChild;

    return node;
}



BvhNode* build_bvh_from_objects(Object* objs, int num_objs, int start) {
    std::vector<BvhNode*> bvhNodes;

    // Collect BVHs from each object starting from 'start'
    for (int i = start; i < start + num_objs; ++i) {
        Object& obj = objs[i];
        BvhNode* objBvh = obj.getBvh();
        if (objBvh) {
            bvhNodes.push_back(objBvh);
        }
    }

    // Check if we have any BVH nodes to combine
    if (bvhNodes.empty()) {
        return nullptr; // No BVH nodes to build from
    }

    // Sort the bounding boxes based on the chosen axis (e.g., min x-coordinate)
    std::sort(bvhNodes.begin(), bvhNodes.end(), [](BvhNode* a, BvhNode* b) {
        return a->bounding_box.min.x < b->bounding_box.min.x; // Sort by min x-coordinate
    });

    // Create the new parent BVH node
    BvhNode* parentBvh = new BvhNode();
    BoundingBox combinedBoundingBox = computeCombinedBoundingBox(bvhNodes);
    parentBvh->bounding_box = combinedBoundingBox;

    // Base case handling
    size_t bvhSize = bvhNodes.size();

    // Case when there's only one BVH node (no need for splitting)
    if (bvhSize == 1) {
        std::cout << "Assigning node: " << bvhNodes[0] << std::endl;
        return bvhNodes[0];  // Return the single BVH node directly
    }
    // Case when there are two BVH nodes (direct assignment to left and right)
    else if (bvhSize == 2) {
        parentBvh->left = bvhNodes[0];
        parentBvh->right = bvhNodes[1];
        std::cout << "Assigning left: " << bvhNodes[0] << std::endl;
        std::cout << "Assigning right: " << bvhNodes[1] << std::endl;
    }
    // Case when there are more than two BVH nodes (recursive splitting)
    else {
        size_t midIndex = bvhSize / 2;  // Find the midpoint for splitting

        // Recursively build the left and right BVHs
        parentBvh->left = build_bvh_from_objects(objs, num_objs - midIndex, start + midIndex);
        parentBvh->right = build_bvh_from_objects(objs, midIndex, start);  
    }
    std::cout << "Returning parent: " << parentBvh << std::endl;
    return parentBvh; // Return the new parent BVH node
}
}