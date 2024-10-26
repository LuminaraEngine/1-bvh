#include <vector>
#include <random>
#include <iostream>
#include <triangle.hpp>

namespace bvh{
    std::vector<Triangle> generateRandomTriangles(int numTriangles, float rangeMin, float rangeMax) {
    std::vector<Triangle> triangles;
    std::random_device rd;   // Obtain a random number generator
    std::mt19937 gen(rd());  // Seed the generator
    std::uniform_real_distribution<float> dis(rangeMin, rangeMax); // Define the range

    for (int i = 0; i < numTriangles; ++i) {
        Triangle tri;
        for (int j = 0; j < 3; ++j) { // Generate 3 vertices for the triangle
            tri.vertices[j] = vec3<float>(dis(gen), dis(gen), dis(gen)); // Random vertex within the range
        }
        triangles.push_back(tri); // Add triangle to the list
        printf("Triangle %d: (%f, %f, %f), (%f, %f, %f), (%f, %f, %f)\n",
            i, tri.vertices[0].x, tri.vertices[0].y, tri.vertices[0].z,
            tri.vertices[1].x, tri.vertices[1].y, tri.vertices[1].z,
            tri.vertices[2].x, tri.vertices[2].y, tri.vertices[2].z);
        printf("Centroid: (%f, %f, %f)\n",
            (tri.vertices[0].x + tri.vertices[1].x + tri.vertices[2].x) / 3.0f,
            (tri.vertices[0].y + tri.vertices[1].y + tri.vertices[2].y) / 3.0f,
            (tri.vertices[0].z + tri.vertices[1].z + tri.vertices[2].z) / 3.0f);
    }
        
    return triangles; // Return the list of triangles
    }
}  