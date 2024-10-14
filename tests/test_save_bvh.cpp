#include <iostream>
#include <cstdio>

#include <test_save_bvh.hpp>
#include <object.hpp>
#include <vec3.hpp>
#include <triangle.hpp>
#include <bvh_node.hpp>
#include <bvh.hpp>
#include <custom_assert.hpp>

void save_bvh_test_base_case()
{
    // Create test triangles for the BVH leaf nodes
    int num_triangles = 3;
    bvh::Triangle *test_triangles = new bvh::Triangle[num_triangles];

    test_triangles[0].vertices[0] = {0, 0, 0};
    test_triangles[0].vertices[1] = {1, 0, 0};
    test_triangles[0].vertices[2] = {0, 1, 0};

    test_triangles[1].vertices[0] = {0, 0, 0};
    test_triangles[1].vertices[1] = {1, 1, 0};
    test_triangles[1].vertices[2] = {0, 1, 0};

    test_triangles[2].vertices[0] = {0, 0, 1};
    test_triangles[2].vertices[1] = {1, 0, 1};
    test_triangles[2].vertices[2] = {0, 1, 1};

    // Create bounding boxes' minimums and maximums for the leaf nodes
    bvh::vec3<float> min_left_leaf_1(0, 0, 0);
    bvh::vec3<float> max_left_leaf_1(1, 1, 1);
    bvh::vec3<float> min_right_leaf_1(0, 0, 0);
    bvh::vec3<float> max_right_leaf_1(2, 2, 2);

    // Create leaf nodes
    bvh::BvhLeaf *left_leaf_1 = new bvh::BvhLeaf(min_left_leaf_1, max_left_leaf_1, 3, new int[3]{0, 1, 2});
    bvh::BvhLeaf *right_leaf_1 = new bvh::BvhLeaf(min_right_leaf_1, max_right_leaf_1, 3, new int[3]{0, 1, 3});

    // Create the root node
    bvh::vec3<float> root_min(0, 0, 0);
    bvh::vec3<float> root_max(2, 2, 2);
    bvh::BvhNode *root = new bvh::BvhNode(root_min, root_max);
    root->left = left_leaf_1;
    root->right = right_leaf_1;

    // Create an object to save the BVH
    bvh::vec3<float> position(0, 0, 0);
    bvh::vec3<float> rotation(0, 0, 0);
    bvh::vec3<float> scale(1, 1, 1);

    bvh::Object obj(position, rotation, scale, test_triangles, 3, root);

    // Save the BVH to a file
    const char *bvh_filename = "./tests/test_save_bvh_base_case.bvh";
    obj.save_bvh(const_cast<char *>(bvh_filename), root);
}

/*
         R
      /     \
    li1     ri1
   /  \     /  \
ll1   rl1 ll2  rl2
*/
void save_bvh_test_n_case()
{
    // Create test triangles for the BVH leaf nodes
    int num_triangles = 6;
    bvh::Triangle *test_triangles = new bvh::Triangle[num_triangles];

    test_triangles[0].vertices[0] = {0, 0, 0};
    test_triangles[0].vertices[1] = {1, 0, 0};
    test_triangles[0].vertices[2] = {0, 1, 0};

    test_triangles[1].vertices[0] = {0, 0, 0};
    test_triangles[1].vertices[1] = {1, 1, 0};
    test_triangles[1].vertices[2] = {0, 1, 0};

    test_triangles[2].vertices[0] = {0, 0, 1};
    test_triangles[2].vertices[1] = {1, 0, 1};
    test_triangles[2].vertices[2] = {0, 1, 1};

    // Create bounding boxes' minimums and maximums for the leaf nodes
    bvh::vec3<float> min_left_leaf_1(0, 0, 0);
    bvh::vec3<float> max_left_leaf_1(1, 1, 1);
    bvh::vec3<float> min_right_leaf_1(0, 0, 0);
    bvh::vec3<float> max_right_leaf_1(2, 2, 2);
    bvh::vec3<float> min_left_leaf_2(1, 2, 3);
    bvh::vec3<float> max_left_leaf_2(4, 3, 4);
    bvh::vec3<float> min_right_leaf_2(4, 3, 5);
    bvh::vec3<float> max_right_leaf_2(7, 5, 7);

    // Create leaf nodes
    bvh::BvhLeaf *left_leaf_1 = new bvh::BvhLeaf(min_left_leaf_1, max_left_leaf_1, 3, new int[3]{0, 1, 2});
    bvh::BvhLeaf *right_leaf_1 = new bvh::BvhLeaf(min_right_leaf_1, max_right_leaf_1, 3, new int[3]{0, 1, 3});
    bvh::BvhLeaf *left_leaf_2 = new bvh::BvhLeaf(min_left_leaf_2, max_left_leaf_2, 3, new int[3]{3, 2, 1});
    bvh::BvhLeaf *right_leaf_2 = new bvh::BvhLeaf(min_right_leaf_2, max_right_leaf_2, 3, new int[3]{2, 1, 0});

    // Create bounding boxes' minimums and maximums for the internal nodes
    bvh::vec3<float> min_left_internal_1(10, 10, 10);
    bvh::vec3<float> max_left_internal_1(11, 11, 11);
    bvh::vec3<float> min_right_internal_1(11, 12, 13);
    bvh::vec3<float> max_right_internal_1(14, 13, 14);

    // Create internal nodes
    bvh::BvhNode *left_internal_1 = new bvh::BvhNode(min_left_internal_1, max_left_internal_1);
    bvh::BvhNode *right_internal_1 = new bvh::BvhNode(min_right_internal_1, max_right_internal_1);
    left_internal_1->left = left_leaf_1;
    left_internal_1->right = right_leaf_1;
    right_internal_1->left = left_leaf_2;
    right_internal_1->right = right_leaf_2;

    // Create the root node
    bvh::vec3<float> root_min(0, 0, 0);
    bvh::vec3<float> root_max(2, 2, 2);
    bvh::BvhNode *root = new bvh::BvhNode(root_min, root_max);
    root->left = left_internal_1;
    root->right = right_internal_1;

    // Create an object to save the BVH
    bvh::vec3<float> position(0, 0, 0);
    bvh::vec3<float> rotation(0, 0, 0);
    bvh::vec3<float> scale(1, 1, 1);

    bvh::Object obj(position, rotation, scale, test_triangles, 3, root);

    // Save the BVH to a file
    const char *bvh_filename = "./tests/test_save_bvh_n_case.bvh";
    obj.save_bvh(const_cast<char *>(bvh_filename), root);
}

void compare_bvh_files(const char *filename1, const char *filename2)
{
    FILE *file1 = fopen(filename1, "r");
    if (file1 == NULL) // Manage if file could not be opened

    {
        assert(false, "Could not open file " + std::string(filename1));
    }

    FILE *file2 = fopen(filename2, "r");
    if (file2 == NULL) // Manage if file could not be opened
    {
        assert(false, "Could not open file " + std::string(filename2));
    }

    char line1[1000];
    char line2[1000];
    int line_number = 1;

    while (fgets(line1, 1000, file1) && fgets(line2, 1000, file2))
    {
        // Remove newline characters
        line1[strcspn(line1, "\n")] = 0;
        line2[strcspn(line2, "\n")] = 0;

        // Compare both lines
        if (strcmp(line1, line2) != 0)
        {
            // Show the difference
            assert(false, "Files are not equal at line " + std::to_string(line_number) + ":\n");
            std::cout << "File 1: " << line1;
            std::cout << "File 2: " << line2;
        }
        line_number++;
    }

    fclose(file1);
    fclose(file2);
}

void bvh::tests::save_bvh_test()
{
    // save_bvh_test_base_case();
    // compare_bvh_files("test_save_bvh_base_case.bvh", "test_save_bvh_base_case_results.bvh");

    save_bvh_test_n_case();
    compare_bvh_files("./tests/test_save_bvh_n_case.bvh", "./tests/test_save_bvh_n_case_results.bvh");
}
