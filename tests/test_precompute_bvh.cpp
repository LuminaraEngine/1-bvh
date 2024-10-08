#include <test_precompute_bvh.hpp>
#include <custom_assert.hpp>
#include <bvh.hpp>
#include <vector>
#include <triangle.hpp>
#include <iostream>
#include <random>
#include <generateRandomTriangles.hpp>


void bvh::tests::precompute_bvh() {
    std::cout << "Starting precompute_bvh tests..." << std::endl;

    // Test Case 1: Basic Validity Test
    std::vector<Triangle> randomTriangles = generateRandomTriangles(16, 0.0f, 10.0f);

    BvhNode* rootNode = precompute_bvh(randomTriangles.data(), 0, randomTriangles.size());
    assert(rootNode != nullptr, "BVH root node should not be null");
    std::cout << "Test Case 1 passed: Root node created." << std::endl;

    // Test Case 2: Bounding Box Test
    // Calculate the expected bounding box from the random triangles
    vec3<float> expectedMin(std::numeric_limits<float>::max(), std::numeric_limits<float>::max(), std::numeric_limits<float>::max());
    vec3<float> expectedMax(std::numeric_limits<float>::lowest(), std::numeric_limits<float>::lowest(), std::numeric_limits<float>::lowest());

    for (const Triangle& tri : randomTriangles) {
        for (const vec3<float>& vertex : tri.vertices) {
            expectedMin.x = std::min(expectedMin.x, vertex.x);
            expectedMin.y = std::min(expectedMin.y, vertex.y);
            expectedMin.z = std::min(expectedMin.z, vertex.z);
            expectedMax.x = std::max(expectedMax.x, vertex.x);
            expectedMax.y = std::max(expectedMax.y, vertex.y);
            expectedMax.z = std::max(expectedMax.z, vertex.z);
        }
    }

    // Assert the calculated bounding box matches the BVH root node's bounding box
    assert(rootNode->bounding_box.min == expectedMin, "Expected min bounding box does not match");
    assert(rootNode->bounding_box.max == expectedMax, "Expected max bounding box does not match");
    std::cout << "Test Case 2 passed: Bounding box matches expected values." << std::endl;

    // Test Case 3: Leaf Node Test
    Triangle singleTriangle[] = {
        { { vec3<float>(0.0f, 0.0f, 0.0f), vec3<float>(1.0f, 0.0f, 0.0f), vec3<float>(0.0f, 1.0f, 0.0f) } }
    };
    BvhNode* leafNode = precompute_bvh(singleTriangle, 0, 1);
    assert(dynamic_cast<BvhLeaf*>(leafNode) != nullptr, "Should create a leaf node for a single triangle");
    std::cout << "Test Case 3 passed: Leaf node created successfully." << std::endl;

    // Test Case 4: Hierarchy Test
    Triangle additionalTris[] = {
        { { vec3<float>(0.0f, 0.0f, 0.0f), vec3<float>(1.0f, 0.0f, 0.0f), vec3<float>(0.0f, 1.0f, 0.0f) } },
        { { vec3<float>(0.5f, 0.5f, 0.0f), vec3<float>(1.5f, 0.5f, 0.0f), vec3<float>(0.5f, 1.5f, 0.0f) } },
        { { vec3<float>(1.0f, 1.0f, 0.0f), vec3<float>(2.0f, 1.0f, 0.0f), vec3<float>(1.0f, 2.0f, 0.0f) } }
    };

    BvhNode* hierarchicalRoot = precompute_bvh(additionalTris, 0, 3);
    assert(hierarchicalRoot != nullptr, "BVH should not be null for multiple triangles");
    std::cout << "Test Case 4 passed: Hierarchical root node created." << std::endl;

    // Clean up
    delete rootNode; // Make sure to clean up allocated nodes
    delete leafNode;
    delete hierarchicalRoot;

    std::cout << "All precompute_bvh tests completed successfully." << std::endl;
}