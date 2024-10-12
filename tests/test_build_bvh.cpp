#include <test_build_bvh.hpp>
#include <custom_assert.hpp>
#include <bvh.hpp>
#include <vector>
#include <triangle.hpp>
#include <object.hpp>
#include <iostream>
#include <random>


void bvh::tests::build_bvh() {
    // Step 2: Create a few mock triangles for each object
    Triangle tris1[] = {
        { vec3<float>(0.0f, 0.0f, 0.0f), vec3<float>(1.0f, 0.0f, 0.0f), vec3<float>(0.0f, 1.0f, 0.0f) }
    };
    Triangle tris2[] = {
        { vec3<float>(2.0f, 2.0f, 2.0f), vec3<float>(3.0f, 2.0f, 2.0f), vec3<float>(2.0f, 3.0f, 2.0f) }
    };
    Triangle tris3[] = {
        { vec3<float>(4.0f, 4.0f, 4.0f), vec3<float>(5.0f, 4.0f, 4.0f), vec3<float>(4.0f, 5.0f, 4.0f) }
    };

    // Step 3: Create the mock BVH nodes for these objects
    BvhNode* bvh1 = new BvhNode();  // Set up bounding boxes as needed
    BvhNode* bvh2 = new BvhNode();
    BvhNode* bvh3 = new BvhNode();

    // Step 4: Create mock objects with position, rotation, scale, triangles, and BVH
    Object obj1(vec3<float>(0.0f,0.0f, 0.0f), vec3<float>(0.0f,0.0f,0.0f), vec3<float>(1.0f,1.0f,1.0f), tris1, 1, bvh1);
    Object obj2(vec3<float>(2.0f, 2.0f, 2.0f), vec3<float>(0.0f,0.0f,0.0f), vec3<float>(1.0f, 1.0f, 1.0f), tris2, 1, bvh2);
    Object obj3(vec3<float>(4.0f, 4.0f, 4.0f), vec3<float>(0.0f,0.0f,0.0f), vec3<float>(1.0f, 1.0f, 1.0f), tris3, 1, bvh3);

    // Step 5: Create an array of these objects
    Object objects[] = { obj1, obj2, obj3 };

    // Step 6: Call build_bvh
    BvhNode* rootBvh = build_bvh_from_objects(objects, 3, 0);

    // Step 7: Assert and validate the hierarchy
    assert(rootBvh != nullptr, "Root BVH should not be null");
    assert(rootBvh->left != nullptr, "Left child of root should not be null");
    assert(rootBvh->right != nullptr, "Right child of root should not be null");

    std::cout << "BVH structure has been successfully built." << std::endl;

    // Cleanup
    delete bvh1;
    delete bvh2;
    delete bvh3;
}
