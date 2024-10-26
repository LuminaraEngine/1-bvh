#include <test_build_bvh.hpp>
#include <custom_assert.hpp>
#include <bvh.hpp>
#include <vector>
#include <triangle.hpp>
#include <object.hpp>
#include <iostream>
#include <random>
#include <bvh_node.hpp>
#include <generateRandomTriangles.hpp>


namespace bvh::tests {


// Helper function, only used in this file
static bool check_all_bounding_box(BvhNode* node) {
    if (node == nullptr) { 
        std::cout << "node is null" << std::endl;
        return true;
    } // true -> no box to check

    if (!node->left && !node->right) {
        //std::cout << "found leaf node" << std::endl;
        return true; // should be good, passed in precompute_bvh tests
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

    bool is_left_node_valid = check_all_bounding_box(node->left);
    bool is_right_node_valid = check_all_bounding_box(node->right);

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

    if (!node->left || !node->right) {
        std::cout << "one child is null, something went wrong" << std::endl;
        return 0;
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



void build_bvh() {
    std::cout << "Starting build_bvh tests..." << std::endl;

    std::vector<Triangle> randTriangles1 = generateRandomTriangles(23, 0.0f, 10.0f);
    std::vector<Triangle> randTriangles2 = generateRandomTriangles(15, 0.0f, 10.0f);
    std::vector<Triangle> randTriangles3 = generateRandomTriangles(46, 0.0f, 10.0f);
    std::vector<Triangle> randTriangles4 = generateRandomTriangles(17, 0.0f, 10.0f);
    std::vector<Triangle> randTriangles5 = generateRandomTriangles(2, 0.0f, 10.0f);

    // create deep copy of the triangles to avoid double deletion caused by std::vector
    Triangle* tris1 = new Triangle[randTriangles1.size()];
    Triangle* tris2 = new Triangle[randTriangles2.size()];
    Triangle* tris3 = new Triangle[randTriangles3.size()];
    Triangle* tris4 = new Triangle[randTriangles4.size()];
    Triangle* tris5 = new Triangle[randTriangles5.size()];
    std::copy(randTriangles1.begin(), randTriangles1.end(), tris1);
    std::copy(randTriangles2.begin(), randTriangles2.end(), tris2);
    std::copy(randTriangles3.begin(), randTriangles3.end(), tris3);
    std::copy(randTriangles4.begin(), randTriangles4.end(), tris4);
    std::copy(randTriangles5.begin(), randTriangles5.end(), tris5);

    BvhNode* bvhroot1 = precompute_bvh(tris1, 0, randTriangles1.size());
    BvhNode* bvhroot2 = precompute_bvh(tris2, 0, randTriangles2.size());
    BvhNode* bvhroot3 = precompute_bvh(tris3, 0, randTriangles3.size());
    BvhNode* bvhroot4 = precompute_bvh(tris4, 0, randTriangles4.size());
    BvhNode* bvhroot5 = precompute_bvh(tris5, 0, randTriangles5.size());

    Object objects[] = {
        Object(vec3<float>(0.0f, 0.0f, 0.0f), vec3<float>(0.0f, 0.0f, 0.0f), vec3<float>(1.0f, 1.0f, 1.0f), tris1, randTriangles1.size(), bvhroot1),
        Object(vec3<float>(2.0f, 2.0f, 2.0f), vec3<float>(0.0f, 0.0f, 0.0f), vec3<float>(1.0f, 1.0f, 1.0f), tris2, randTriangles2.size(), bvhroot2),
        Object(vec3<float>(4.0f, 4.0f, 4.0f), vec3<float>(0.0f, 0.0f, 0.0f), vec3<float>(1.0f, 1.0f, 1.0f), tris3, randTriangles3.size(), bvhroot3),
        Object(vec3<float>(6.0f, 6.0f, 6.0f), vec3<float>(0.0f, 0.0f, 0.0f), vec3<float>(1.0f, 1.0f, 1.0f), tris4, randTriangles4.size(), bvhroot4),
        Object(vec3<float>(8.0f, 8.0f, 8.0f), vec3<float>(0.0f, 0.0f, 0.0f), vec3<float>(1.0f, 1.0f, 1.0f), tris5, randTriangles5.size(), bvhroot5)
    };

    std::cout << "Number of objects: " << sizeof(objects) / sizeof(Object) << std::endl;
    std::cout << "Total number of triangles: " << randTriangles1.size() + randTriangles2.size() + randTriangles3.size() + randTriangles4.size() + randTriangles5.size() << std::endl;

    std::cout << "Calling build_bvh_from_objects..." << std::endl;

    BvhNode* bigroot = build_bvh_from_objects(objects, 5, 0);

    // Test 1: bvh structure
    assert(bigroot!= nullptr, "Root BVH should not be null.");
    assert(check_bvh_integrity(bigroot), "BVH structure is invalid");

    int expected_total_leaf_nodes = count_expected_leaf_nodes(randTriangles1.size()) + count_expected_leaf_nodes(randTriangles2.size()) + count_expected_leaf_nodes(randTriangles3.size()) + count_expected_leaf_nodes(randTriangles4.size()) + count_expected_leaf_nodes(randTriangles5.size());
    int actual_total_leaf_nodes = count_leaf_nodes(bigroot);
    std::cout << "Total leaf nodes computed: " << actual_total_leaf_nodes << std::endl;
    std::cout << "Expected: " << expected_total_leaf_nodes << std::endl;
    assert(expected_total_leaf_nodes == actual_total_leaf_nodes, "Number of leaf nodes mismatch");

    std::cout << "Test Case 1 passed: BVH has proper structure" << std::endl;
    bigroot->print(0);

    // Test 2: bounding box
    assert(check_all_bounding_box(bigroot), "Bounding box mismatch");
    std::cout << "Test Case 2 passed: Bounding box matches expected values for all nodes." << std::endl;

    // Test 3: create bvh from part of the objects
    BvhNode* bigroot2 = build_bvh_from_objects(objects, 2, 2);
    assert(bigroot2 != nullptr, "Root BVH should not be null.");
    assert(check_bvh_integrity(bigroot2), "BVH structure is invalid");
    assert(check_all_bounding_box(bigroot2), "Bounding box mismatch");
    bigroot2->print(0);
    int expected_total_leaf_nodes2 = count_expected_leaf_nodes(randTriangles3.size()) + count_expected_leaf_nodes(randTriangles4.size());
    int actual_total_leaf_nodes2 = count_leaf_nodes(bigroot2);
    std::cout << "Total leaf nodes computed: " << actual_total_leaf_nodes2 << std::endl;
    std::cout << "Expected: " << expected_total_leaf_nodes2 << std::endl;
    assert(expected_total_leaf_nodes2 == actual_total_leaf_nodes2, "Number of leaf nodes mismatch");
    std::cout << "Test Case 3 passed: BVH for some objects in the provided list" << std::endl;

    // Test 4: create bvh from part of the objects
    BvhNode* bigroot3 = build_bvh_from_objects(objects, 1, 3);
    assert(bigroot3 != nullptr, "Root BVH should not be null.");
    assert(check_bvh_integrity(bigroot3), "BVH structure is invalid");
    assert(check_all_bounding_box(bigroot3), "Bounding box mismatch");
    int expected_total_leaf_nodes3 = count_expected_leaf_nodes(randTriangles4.size());
    int actual_total_leaf_nodes3 = count_leaf_nodes(bigroot3);
    std::cout << "Total leaf nodes computed: " << actual_total_leaf_nodes3 << std::endl;
    std::cout << "Expected: " << expected_total_leaf_nodes3 << std::endl;
    assert(expected_total_leaf_nodes3 == actual_total_leaf_nodes3, "Number of leaf nodes mismatch");
    std::cout << "Test Case 4 passed: BVH for one object in the provided list" << std::endl;
    bigroot3->print(0);

    std::cout << "BVH structure has been successfully built and validated." << std::endl;

}

}
