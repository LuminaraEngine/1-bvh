#include <test_precompute_bvh.hpp>
#include <custom_assert.hpp>
#include <bvh.hpp>
#include <vector>
#include <triangle.hpp>
#include <iostream>
#include <random>
#include <generateRandomTriangles.hpp>

namespace bvh::tests {

// Helper function, only used in this file
static bool verify_leaf_bounding_box(BvhNode* node, Triangle* tris) {
    //std::cout << "verify_leaf_bounding_box start" << std::endl;
    if (node == nullptr) { return true; } // true -> no box to check

    BvhLeaf* leaf = dynamic_cast<BvhLeaf*>(node);
    if (leaf == nullptr) { return true; }

    vec3<float> computed_min(std::numeric_limits<float>::max(), std::numeric_limits<float>::max(), std::numeric_limits<float>::max());
    vec3<float> computed_max(std::numeric_limits<float>::lowest(), std::numeric_limits<float>::lowest(), std::numeric_limits<float>::lowest());

    for (int i = 0; i < leaf->num_triangles; i++) {
        for (int j = 0; j < 3; j++) {
            const vec3<float>& vertex = tris[leaf->indices[i]].vertices[j];
            computed_min.x = std::min(computed_min.x, vertex.x);
            computed_min.y = std::min(computed_min.y, vertex.y);
            computed_min.z = std::min(computed_min.z, vertex.z);
            computed_max.x = std::max(computed_max.x, vertex.x);
            computed_max.y = std::max(computed_max.y, vertex.y);
            computed_max.z = std::max(computed_max.z, vertex.z);
        }
    }

    if (computed_min != node->bounding_box.min || computed_max != node->bounding_box.max) {
        std::cout << "!! Bounding Box Mismatch (leaf) !!" << std::endl;
        std::cout << "Computed MIN from triangles: " << computed_min << std::endl;
        std::cout << "Currently stored MIN: " << node->bounding_box.min << std::endl;
        std::cout << "Computed MAX from triangles: " << computed_max << std::endl;
        std::cout << "Currently stored MAX: " << node->bounding_box.max << std::endl;
        std::cout << "(triangle indices might be incorrect)" << std::endl;
        return false;
    }

    return true;
}

// Helper function, only used in this file
static bool check_all_bounding_box(BvhNode* node, Triangle* tris) {
    if (node == nullptr) { 
        std::cout << "node is null" << std::endl;
        return true;
    } // true -> no box to check

    if (!node->left && !node->right) {
        //std::cout << "found leaf node" << std::endl;
        return verify_leaf_bounding_box(node, tris);
    }

    if (!node->left || !node->right) {
        std::cout << "one null child, something went wrong" << std::endl;
        return false;
    }

    // verify bounding box of current node using its child nodes
    vec3<float> computed_min(
        std::min(node->left->bounding_box.min.x, node->right->bounding_box.min.x),
        std::min(node->left->bounding_box.min.y, node->right->bounding_box.min.y),
        std::min(node->left->bounding_box.min.z, node->right->bounding_box.min.z)
    );
    vec3<float> computed_max(
        std::max(node->left->bounding_box.max.x, node->right->bounding_box.max.x),
        std::max(node->left->bounding_box.max.y, node->right->bounding_box.max.y),
        std::max(node->left->bounding_box.max.z, node->right->bounding_box.max.z)
    );

    if (computed_min != node->bounding_box.min || computed_max != node->bounding_box.max) {
        std::cout << "!! Bounding Box Mismatch !!" << std::endl;
        std::cout << "Computed MIN from child nodes: " << computed_min << std::endl;
        std::cout << "Currently stored MIN: " << node->bounding_box.min << std::endl;
        std::cout << "Computed MAX from child nodes: " << computed_max << std::endl;
        std::cout << "Currently stored MAX: " << node->bounding_box.max << std::endl;
        return false;
    }

    bool is_left_node_valid = check_all_bounding_box(node->left, tris);
    bool is_right_node_valid = check_all_bounding_box(node->right, tris);

    return is_left_node_valid && is_right_node_valid;
}

// Helper function, only used in this file
static bool check_bvh_integrity(BvhNode* node) {
    if (node == nullptr) { 
        std::cout << "Node is null" << std::endl;
        return false;
    }

    if (!node->left && !node->right) {
        BvhLeaf* leaf = dynamic_cast<BvhLeaf*>(node);
        if (leaf == nullptr) {
            std::cout << "Node is not leaf even though it has no child nodes" << std::endl;
            return false;
        }
        return leaf->num_triangles > 0 && leaf->num_triangles <= BVH_LEAF_SIZE;
    }

    if (!node->left || !node->right) { 
        std::cout << "Only one child for BvhNode" << std::endl;
        return false; 
    }

    return check_bvh_integrity(node->left) && check_bvh_integrity(node->right);
}

// Helper function, only used in this file
static int count_leaf_nodes(BvhNode* node) {
    if (node == nullptr) {
        return 0;
    }

    if (!node->left && !node->right) {
        return 1;
    }

    return count_leaf_nodes(node->left) + count_leaf_nodes(node->right);
}

static int count_expected_leaf_nodes(int numTriangles) {
    int d = std::ceil(std::log2((numTriangles + BVH_LEAF_SIZE - 1) / BVH_LEAF_SIZE)); // depth of the binary tree
    int leaves = std::pow(2, d);
    // Unique case when precompute_bvh splits the triangles:
    // if the number of triangles is divisible by 17 and results in 2^k for some k=0,1,2,...
    // need to substract the expected leaf nodes by that 2^k
    if (numTriangles % 17 == 0) {
        int n = numTriangles / 17;
        if (std::ceil(std::log2(n)) == std::floor(std::log2(n))) { // n is a power of 2
            leaves -= n;
        }
    }
    return leaves;
}

static void check_leaf_node_triangles(BvhNode* node, Triangle* tris) {
    std::cout << "check_leaf_node_triangles start" << std::endl;
    BvhLeaf* leaf = dynamic_cast<BvhLeaf*>(node);
    if (leaf == nullptr) {
        check_leaf_node_triangles(node->left, tris);
        check_leaf_node_triangles(node->right, tris);
        return;
    }

    for (int i = 0; i < leaf->num_triangles; i++) {
        printf("Triangle %d: (%f, %f, %f), (%f, %f, %f), (%f, %f, %f)\n", leaf->indices[i],
            tris[leaf->indices[i]].vertices[0].x, tris[leaf->indices[i]].vertices[0].y, tris[leaf->indices[i]].vertices[0].z,
            tris[leaf->indices[i]].vertices[1].x, tris[leaf->indices[i]].vertices[1].y, tris[leaf->indices[i]].vertices[1].z,
            tris[leaf->indices[i]].vertices[2].x, tris[leaf->indices[i]].vertices[2].y, tris[leaf->indices[i]].vertices[2].z);
        printf("Centroid: (%f, %f, %f)\n",
            (tris[leaf->indices[i]].vertices[0].x + tris[leaf->indices[i]].vertices[1].x + tris[leaf->indices[i]].vertices[2].x) / 3.0f,
            (tris[leaf->indices[i]].vertices[0].y + tris[leaf->indices[i]].vertices[1].y + tris[leaf->indices[i]].vertices[2].y) / 3.0f,
            (tris[leaf->indices[i]].vertices[0].z + tris[leaf->indices[i]].vertices[1].z + tris[leaf->indices[i]].vertices[2].z) / 3.0f);
    }
}

void precompute_bvh() {
    std::cout << "Starting precompute_bvh tests..." << std::endl;

    // Generate random triangles for all tests
    std::vector<Triangle> randomTriangles = generateRandomTriangles(10, 0.0f, 10.0f);
    std::cout << "Number of triangles: " << randomTriangles.size() << std::endl;

    // Test Case 1: BVH Structure Integrity Test
    BvhNode* rootNode = precompute_bvh(randomTriangles.data(), 0, randomTriangles.size());

    // // check triangles stored in the leaf nodes
    // std::cout << "============================" << std::endl;
    // rootNode->print();
    // std::cout << "============================" << std::endl;
    // std::cout << "Checking triangles stored in leaf nodes:" << std::endl;
    // check_leaf_node_triangles(rootNode, randomTriangles.data());
    // std::cout << "============================" << std::endl;

    // delete rootNode;
    // return;


    assert(rootNode != nullptr, "BVH root node should not be null");
    assert(check_bvh_integrity(rootNode), "BVH structure is invalid");

    int numLeafNodes = count_leaf_nodes(rootNode);
    assert(numLeafNodes > 0, "BVH should have at least one leaf node");
    std::cout << "Number of leaf nodes in computed bvh: " << numLeafNodes << std::endl;
    int expectednumLeafNodes = count_expected_leaf_nodes(randomTriangles.size());
    std::cout << "Expected: " << expectednumLeafNodes << " leaf nodes" << std::endl;

    // this test would fail if the binary tree splitting heuristic of precompute_bvh is changed
    assert(numLeafNodes == expectednumLeafNodes, "Number of leaf nodes should match the expected value");
    
    std::cout << "Test Case 1 passed: BVH has proper structure" << std::endl;

    // Test Case 2: Bounding Box Test for All Nodes
    bool is_bounding_box_valid = check_all_bounding_box(rootNode, randomTriangles.data());
    assert(is_bounding_box_valid, "Bounding box mismatch in the hierarchy");
    std::cout << "Test Case 2 passed: Bounding box matches expected values for all nodes." << std::endl;

    // Test Case 3: Leaf Node Test (random subset of 1 triangle)
    BvhNode* leafNode = precompute_bvh(randomTriangles.data(), 0, 1);
    assert(dynamic_cast<BvhLeaf*>(leafNode) != nullptr, "Should create a leaf node for a single triangle");
    assert(check_bvh_integrity(leafNode), "Leaf node structure is invalid");
    assert(check_all_bounding_box(leafNode, randomTriangles.data()), "Bounding box mismatch in the leaf node");
    assert(count_leaf_nodes(leafNode) == 1, "Leaf node should be the only node in the hierarchy");
    std::cout << "Test Case 3 passed: Leaf node created successfully." << std::endl;

    // Test Case 4: Hierarchy Test (use a random subset of triangles <= BVH_LEAF_SIZE)
    BvhNode* hierarchicalRoot = precompute_bvh(randomTriangles.data(), 0, 3);
    assert(hierarchicalRoot != nullptr, "BVH should not be null for multiple triangles");
    assert(check_bvh_integrity(hierarchicalRoot), "BVH structure is invalid");
    assert(check_all_bounding_box(hierarchicalRoot, randomTriangles.data()), "Bounding box mismatch in the hierarchy");
    assert(count_leaf_nodes(hierarchicalRoot) == 1, "Leaf node should be the only node in the hierarchy");
    std::cout << "Test Case 4 passed: Hierarchical root node created." << std::endl;

    // Clean up
    delete rootNode;
    delete leafNode;
    delete hierarchicalRoot;

    std::cout << "All precompute_bvh tests completed successfully." << std::endl;
}


}