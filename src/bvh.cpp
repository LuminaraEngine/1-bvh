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

// Splitting mechanics choose the longest axis. If the axis is the same, choose the one with the smallest minimum value.
    int chooseSplitAxis(const BoundingBox& box) {
        vec3<float> boxSize = box.max - box.min;
        if (boxSize.x > boxSize.y && boxSize.x > boxSize.z) {
            return 0; // Split along the x-axis
        } else if (boxSize.y > boxSize.z) {
            return 1; // Split along the y-axis
        } else {
            return 2; // Split along the z-axis
        }
    }
    
static BvhNode* precompute_helper(Triangle* tris, std::vector<int> index_list, int start, int end){
    int num_tris = end - start;

    // Handle empty or invalid input
    if (start < 0 || end > start+num_tris || start >= end || index_list.data() == nullptr) {
      return nullptr; // Return null for invalid ranges
    }

    // Initialize bounding box values
    float min_x = std::numeric_limits<float>::max();
    float min_y = std::numeric_limits<float>::max();
    float min_z = std::numeric_limits<float>::max();
    float max_x = std::numeric_limits<float>::lowest();
    float max_y = std::numeric_limits<float>::lowest();
    float max_z = std::numeric_limits<float>::lowest();
    std::vector<int> indices(index_list.begin() + start, index_list.begin() + end);

    // Calculate the bounding box
    for (int i = 0; i < num_tris; i++) {
        for (int j = 0; j < 3; j++) {
            const vec3<float>& vertex = tris[indices[i]].vertices[j];
            min_x = std::min(min_x, vertex.x);
            min_y = std::min(min_y, vertex.y);
            min_z = std::min(min_z, vertex.z);
            max_x = std::max(max_x, vertex.x);
            max_y = std::max(max_y, vertex.y);
            max_z = std::max(max_z, vertex.z);
        }
    }

    // Construct the bounding box for the current node
    vec3<float> min(min_x, min_y, min_z);
    vec3<float> max(max_x, max_y, max_z);

    if (num_tris <= BVH_LEAF_SIZE) {
        return new BvhLeaf(min, max, num_tris, indices.data());
    }

    // Split the triangles in half
    int mid = start + num_tris / 2;

    // Recursively build the left and right child nodes
    BvhNode* leftChild = precompute_helper(tris,index_list, start, mid);
    BvhNode* rightChild = precompute_helper(tris,index_list, mid, end); // Use end here

    // Create and return an internal node with the bounding box and child nodes
    BvhNode* node = new BvhNode(min, max);
    node->left = leftChild;
    node->right = rightChild;

    return node;
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

    // Construct the bounding box for the current node
    vec3<float> min(min_x, min_y, min_z);
    vec3<float> max(max_x, max_y, max_z);

    // Compute the centroids of the triangles to decide on partitioning
    std::vector<vec3<float>> centroids(end - start);
    for (int i = start; i < end; i++) {
        centroids[i - start] = (tris[i].vertices[0] + tris[i].vertices[1] + tris[i].vertices[2]) / 3.0f;
        std::cout << "Centroid: " << centroids[i - start] << "for triangle" << i << std::endl;
    }

    // Initialize indices
    std::vector<int> indices(centroids.size());
    for (int i = 0; i < indices.size(); i++) {
        indices[i] = i;  // Fill with 0, 1, 2, ..., N-1
    }

    int splitAxis = chooseSplitAxis(BoundingBox(min, max));

    // Sort indices based on the longest axis
    std::sort(indices.begin(), indices.end(), [&centroids, splitAxis](int a, int b) {
    if (splitAxis == 0) {
        return centroids[a].x < centroids[b].x;
    } else if (splitAxis == 1) {
        return centroids[a].y < centroids[b].y;
    } else {
        return centroids[a].z < centroids[b].z;
    }
});

    // After sorting, update the triangles based on the sorted indices
    std::vector<bvh::Triangle> sortedTris(end - start);
    for (int i = 0; i < indices.size(); i++) {
        std::cout << "Index: " << indices[i] << std::endl;
        sortedTris[i] = tris[start + indices[i]];
    }

    // Handle the leaf case MAYBE MOVE THIS AFTER SORTING
    if (num_tris <= BVH_LEAF_SIZE) {
        std::vector<int> indices(num_tris);
        for (int i = 0; i < num_tris; i++) {
            indices[i] = start + i;  // Store triangle indices
        }
        return new BvhLeaf(min, max, num_tris, indices.data());
    }

    // Split the triangles in half
    int mid = start + num_tris / 2;

    // Recursively build the left and right child nodes
    BvhNode* leftChild = precompute_helper(tris, indices, start, mid);
    BvhNode* rightChild = precompute_helper(tris, indices, mid, end); // Use end here

    // Create and return an internal node with the bounding box and child nodes
    BvhNode* node = new BvhNode(min, max);
    node->left = leftChild;
    node->right = rightChild;

    return node;
}

BvhNode* build_bvh_recursion(std::vector<BvhNode*>& bvhNodes, int start, int end) {
    int bvhSize = end - start;

    // Base cases
    if (bvhSize == 1) {
        return bvhNodes[start]; // Single node, return it as-is
    }
    if (bvhSize == 2) {
        BvhNode* parent = new BvhNode();
        parent->left = bvhNodes[start];
        parent->right = bvhNodes[start + 1];
        parent->bounding_box = computeCombinedBoundingBox({parent->left, parent->right});
        return parent;
    }

    // Initialize variables for best split
    float bestCost = std::numeric_limits<float>::infinity();
    int bestSplit = start + 1; // Default to the first possible split
    int bestAxis = -1;

    // Iterate over each axis (0: x, 1: y, 2: z)
    for (int axis = 0; axis < 3; ++axis) {
        // Sort nodes based on the current axis
        std::sort(bvhNodes.begin() + start, bvhNodes.begin() + end,
                  [axis](BvhNode* a, BvhNode* b) {
                      return (axis == 0) ? (a->bounding_box.min.x < b->bounding_box.min.x) :
                             (axis == 1) ? (a->bounding_box.min.y < b->bounding_box.min.y) :
                                           (a->bounding_box.min.z < b->bounding_box.min.z);
                  });

        // Create bounding boxes for left and right splits
        BoundingBox leftBox, rightBox;
        for (int i = start; i < end; ++i) {
            if (i > start) {
                // Update left bounding box for the current node
                leftBox = computeCombinedBoundingBox({bvhNodes[start], bvhNodes[i - 1]});
            }

            if (i < end) {
                // Update right bounding box for the current node
                rightBox = computeCombinedBoundingBox({bvhNodes[i], bvhNodes[end - 1]});
            }

            // Calculate SAH cost
            float leftArea = leftBox.surfaceArea();
            float rightArea = rightBox.surfaceArea();
            float cost = leftArea * (i - start) + rightArea * (end - i);

            // Check if this is the best cost found so far
            if (cost < bestCost) {
                bestCost = cost;
                bestSplit = i; // Update best split point
                bestAxis = axis; // Store the best axis (if needed)
            }
        }
    }

    // Create parent node with the best split found
    BvhNode* parent = new BvhNode();
    parent->left = build_bvh_recursion(bvhNodes, start, bestSplit);
    parent->right = build_bvh_recursion(bvhNodes, bestSplit, end);
    parent->bounding_box = computeCombinedBoundingBox({parent->left, parent->right});

    return parent;
}


BvhNode* build_bvh_from_objects(Object* objs, int num_objs, int start) {
    std::vector<BvhNode*> bvhNodes;

    // Collect BVH nodes from objects
    for (int i = start; i < start + num_objs; ++i) {
        if (BvhNode* objBvh = objs[i].getBvh()) {
            bvhNodes.push_back(objBvh);
        }
    }

    // Sort bounding boxes by the min x-coordinate
    std::sort(bvhNodes.begin(), bvhNodes.end(), [](BvhNode* a, BvhNode* b) {
        return a->bounding_box.min.x < b->bounding_box.min.x;
    });

    // Start recursive BVH building
    if (!bvhNodes.empty()) {
        return build_bvh_recursion(bvhNodes, 0, bvhNodes.size());
    } else {
        return nullptr;
    }
}


}