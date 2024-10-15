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
    // Create a few mock triangles for each object
    // Triangle tris1[] = {
    //     { vec3<float>(0.0f, 0.0f, 0.0f), vec3<float>(1.0f, 0.0f, 0.0f), vec3<float>(0.0f, 1.0f, 0.0f) }
    // };
    // Triangle tris2[] = {
    //     { vec3<float>(2.0f, 2.0f, 2.0f), vec3<float>(3.0f, 2.0f, 2.0f), vec3<float>(2.0f, 3.0f, 2.0f) }
    // };
    // Triangle tris3[] = {
    //     { vec3<float>(4.0f, 4.0f, 4.0f), vec3<float>(5.0f, 4.0f, 4.0f), vec3<float>(4.0f, 5.0f, 4.0f) }
    // };
    // Triangle tris4[] = {
    //     { vec3<float>(6.0f, 6.0f, 6.0f), vec3<float>(7.0f, 6.0f, 6.0f), vec3<float>(6.0f, 7.0f, 6.0f) }
    // };

    Triangle* tri1 = new Triangle();
    Triangle* tri2 = new Triangle();
    Triangle* tri3 = new Triangle();
    Triangle* tri4 = new Triangle();
    tri1->vertices[0] = vec3<float>(0.0f, 0.0f, 0.0f);
    tri1->vertices[1] = vec3<float>(1.0f, 0.0f, 0.0f);
    tri1->vertices[2] = vec3<float>(0.0f, 1.0f, 0.0f);
    tri2->vertices[0] = vec3<float>(2.0f, 2.0f, 2.0f);
    tri2->vertices[1] = vec3<float>(3.0f, 2.0f, 2.0f);
    tri2->vertices[2] = vec3<float>(2.0f, 3.0f, 2.0f);
    tri3->vertices[0] = vec3<float>(4.0f, 4.0f, 4.0f);
    tri3->vertices[1] = vec3<float>(5.0f, 4.0f, 4.0f);
    tri3->vertices[2] = vec3<float>(4.0f, 5.0f, 4.0f);
    tri4->vertices[0] = vec3<float>(6.0f, 6.0f, 6.0f);
    tri4->vertices[1] = vec3<float>(7.0f, 6.0f, 6.0f);
    tri4->vertices[2] = vec3<float>(6.0f, 7.0f, 6.0f);

    // Create the mock BVH nodes for these objects with bounding boxes
    BvhNode* bvh1 = new BvhNode();
    bvh1->bounding_box = BoundingBox(vec3<float>(0.0f, 0.0f, 0.0f), vec3<float>(1.0f, 1.0f, 0.0f));

    BvhNode* bvh2 = new BvhNode();
    bvh2->bounding_box = BoundingBox(vec3<float>(2.0f, 2.0f, 2.0f), vec3<float>(3.0f, 3.0f, 2.0f));

    BvhNode* bvh3 = new BvhNode();
    bvh3->bounding_box = BoundingBox(vec3<float>(4.0f, 4.0f, 4.0f), vec3<float>(5.0f, 5.0f, 4.0f));

    BvhNode* bvh4 = new BvhNode();
    bvh4->bounding_box = BoundingBox(vec3<float>(6.0f, 6.0f, 6.0f), vec3<float>(7.0f, 7.0f, 6.0f));


    // Create mock objects with position, rotation, scale, triangles, and BVH

    // Create an array of these objects
    Object objects[] = {
        Object(vec3<float>(0.0f, 0.0f, 0.0f), vec3<float>(0.0f, 0.0f, 0.0f), vec3<float>(1.0f, 1.0f, 1.0f), tri1, 1, bvh1),
        Object(vec3<float>(2.0f, 2.0f, 2.0f), vec3<float>(0.0f, 0.0f, 0.0f), vec3<float>(1.0f, 1.0f, 1.0f), tri2, 1, bvh2),
        Object(vec3<float>(4.0f, 4.0f, 4.0f), vec3<float>(0.0f, 0.0f, 0.0f), vec3<float>(1.0f, 1.0f, 1.0f), tri3, 1, bvh3),
        Object(vec3<float>(6.0f, 6.0f, 6.0f), vec3<float>(0.0f, 0.0f, 0.0f), vec3<float>(1.0f, 1.0f, 1.0f), tri4, 1, bvh4)
    };

    // Call build_bvh
    BvhNode* rootBvh = build_bvh_from_objects(objects, 4, 0);
    if (!rootBvh) {
        std::cerr << "Error: Root BVH is null." << std::endl;
        return;
    }
    
    rootBvh->print();

    // Assert and validate the hierarchy
    assert(rootBvh->left != nullptr, "Left child of root BVH should not be null.");
    assert(rootBvh->right != nullptr, "Right child of root BVH should not be null.");

    // Validate Bounding Boxes
    BoundingBox combinedBox = computeCombinedBoundingBox({ bvh1, bvh2, bvh3, bvh4 });
    assert(rootBvh->bounding_box == combinedBox, "Combined bounding box does not match the root BVH bounding box.");

    std::cout << "BVH structure has been successfully built and validated." << std::endl;

}
