#include <test_build_bvh.hpp>
#include <custom_assert.hpp>
#include <bvh.hpp>
#include <vector>
#include <triangle.hpp>
#include <object.hpp>
#include <iostream>
#include <random>
#include <bvh_node.hpp>


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
    Triangle tris4[] = {
        { vec3<float>(6.0f, 6.0f, 6.0f), vec3<float>(7.0f, 6.0f, 6.0f), vec3<float>(6.0f, 7.0f, 6.0f) }
    };

    // Step 3: Create the mock BVH nodes for these objects with bounding boxes
    BvhNode* bvh1 = new BvhNode();
    bvh1->bounding_box = BoundingBox(vec3<float>(0.0f, 0.0f, 0.0f), vec3<float>(1.0f, 1.0f, 0.0f));

    BvhNode* bvh2 = new BvhNode();
    bvh2->bounding_box = BoundingBox(vec3<float>(2.0f, 2.0f, 2.0f), vec3<float>(3.0f, 3.0f, 2.0f));

    BvhNode* bvh3 = new BvhNode();
    bvh3->bounding_box = BoundingBox(vec3<float>(4.0f, 4.0f, 4.0f), vec3<float>(5.0f, 5.0f, 4.0f));

    BvhNode* bvh4 = new BvhNode();
    bvh4->bounding_box = BoundingBox(vec3<float>(6.0f, 6.0f, 6.0f), vec3<float>(7.0f, 7.0f, 6.0f));


    // Step 4: Create mock objects with position, rotation, scale, triangles, and BVH
    Object obj1(vec3<float>(0.0f, 0.0f, 0.0f), vec3<float>(0.0f, 0.0f, 0.0f), vec3<float>(1.0f, 1.0f, 1.0f), tris1, 1, bvh1);
    Object obj2(vec3<float>(2.0f, 2.0f, 2.0f), vec3<float>(0.0f, 0.0f, 0.0f), vec3<float>(1.0f, 1.0f, 1.0f), tris2, 1, bvh2);
    Object obj3(vec3<float>(4.0f, 4.0f, 4.0f), vec3<float>(0.0f, 0.0f, 0.0f), vec3<float>(1.0f, 1.0f, 1.0f), tris3, 1, bvh3);
    Object obj4(vec3<float>(6.0f, 6.0f, 6.0f), vec3<float>(0.0f, 0.0f, 0.0f), vec3<float>(1.0f, 1.0f, 1.0f), tris4, 1, bvh4);

    // Step 5: Create an array of these objects
    Object objects[] = { obj1, obj2, obj3, obj4 };

    // Step 6: Call build_bvh
    BvhNode* rootBvh = build_bvh_from_objects(objects, 4, 0);
    if (!rootBvh) {
        std::cerr << "Error: Root BVH is null." << std::endl;
        return;
    }
    
    rootBvh->print();

    // Step 7: Assert and validate the hierarchy
    assert(rootBvh->left != nullptr, "Left child of root BVH should not be null.");
    assert(rootBvh->right != nullptr, "Right child of root BVH should not be null.");

    // Step 8: Validate Bounding Boxes
    BoundingBox combinedBox = computeCombinedBoundingBox({ bvh1, bvh2, bvh3, bvh4 });
    assert(rootBvh->bounding_box == combinedBox, "Combined bounding box does not match the root BVH bounding box.");

    std::cout << "BVH structure has been successfully built and validated." << std::endl;

    // Step 9: Clean up
    bvh1->print();
    delete bvh1;
}
