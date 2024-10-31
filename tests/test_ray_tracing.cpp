#include <test_ray_tracing.hpp>

#include <custom_assert.hpp>
#include <vec3.hpp>
#include <triangle.hpp>
#include <object.hpp>

#include <limits>
#include <math.h>
#include <chrono>

/**
 * Helper function to clamp the values of an image between 0 and 1
 * 
 * @param image The image to clamp as a 1D array of vec3<float>
 * @param width The width of the image
 * @param height The height of the image
 */
void clamp_zero_one(bvh::vec3<float>* image, int width, int height) {
  for(int y = 0; y < height; y++) {
    for(int x = 0; x < width; x++) {
      image[y * width + x].x = fminf(fmaxf(image[y * width + x].x, 0.0f), 1.0f);
      image[y * width + x].y = fminf(fmaxf(image[y * width + x].y, 0.0f), 1.0f);
      image[y * width + x].z = fminf(fmaxf(image[y * width + x].z, 0.0f), 1.0f);
    }
  }
}

/**
 * Helper function to save an image to a ppm file
 * 
 * @param image The image to save as a 1D array of vec3<float>
 * @param width The width of the image
 * @param height The height of the image
 * @param filename The name of the file to save the image to
 */
void save_to_ppm(const bvh::vec3<float>* image, int width, int height, const char* filename) {
  FILE* file = fopen(filename, "w");
  fprintf(file, "P3\n%d %d\n255\n", width, height);
  for(int y = 0; y < height; y++) {
    for(int x = 0; x < width; x++) {
      int r = (int)(255.0f * image[y * width + x].x);
      int g = (int)(255.0f * image[y * width + x].y);
      int b = (int)(255.0f * image[y * width + x].z);
      fprintf(file, "%d %d %d ", r, g, b);
    }
    fprintf(file, "\n");
  }
  fclose(file);
}

/**
 * Helper function to generate the eye rays for the ray tracing algorithm
 * 
 * @param width The width of the image
 * @param height The height of the image
 * @param fov The vertical field of view of the camera (in degrees)
 * @param camera_pos The position of the camera
 * @param camera_up The up vector of the camera
 * @param camera_dir The direction vector of the camera (not the point it's looking at)
 * 
 * @return A 1D array of vec3<float> representing the eye ray directions
 * with the origin at the top left going row by row
 * (origins are the camera position)
 */
bvh::vec3<float>* generate_eye_ray_directions(
  int width,
  int height,
  float fov,
  bvh::vec3<float> camera_pos,
  bvh::vec3<float> camera_up,
  bvh::vec3<float> camera_dir
) {
  // Allocate the array of eye ray directions
  bvh::vec3<float>* eye_ray_directions = new bvh::vec3<float>[width * height];

  // Compute the ndc vectors
  bvh::vec3<float> z_ndc = camera_dir;  //bvh::vec3<float>(-camera_dir.x, -camera_dir.y, -camera_dir.z);
  bvh::vec3<float> x_ndc = bvh::vec3<float>::normalize(bvh::vec3<float>::cross(z_ndc, camera_up));
  bvh::vec3<float> y_ndc = bvh::vec3<float>::normalize(bvh::vec3<float>::cross(x_ndc, z_ndc));

  float y_size = tan(fov * M_PI / 360.0f);
  float x_size = y_size * (float)width / (float)height;
  y_ndc = y_ndc * y_size;
  x_ndc = x_ndc * x_size;

  // Compute the offsets
  float x_offset = 2.0f / (float)width;
  float y_offset = 2.0f / (float)height;

  // Compute the eye ray directions
  for(int y = 0; y < height; y++) {
    for(int x = 0; x < width; x++) {
      // Compute the ndc coordinates
      float x_ndc_coord = -1.0f + (x + 0.5f) * x_offset;
      float y_ndc_coord = 1.0f - (y + 0.5f) * y_offset;

      // Compute the eye ray direction
      eye_ray_directions[y * width + x] = bvh::vec3<float>::normalize(
        x_ndc * x_ndc_coord + y_ndc * y_ndc_coord + z_ndc
      );
    }
  }

  return eye_ray_directions;
}

/**
 * Helper function to compute the intersection between a ray and a triangle
 * 
 * @param ray_pos The position of the ray
 * @param ray_dir The direction of the ray
 * @param triangle The triangle to intersect with
 * 
 * @return The distance to the intersection point or +INF if there is no intersection
 */
float intersect(bvh::vec3<float> ray_pos, bvh::vec3<float> ray_dir, const bvh::Triangle& triangle) {
  const float EPSILON = 1e-6;

  // Define the triangle's edges
  bvh::vec3<float> edge1 = triangle.vertices[1] - triangle.vertices[0];
  bvh::vec3<float> edge2 = triangle.vertices[2] - triangle.vertices[0];
  
  // Calculate the determinant
  bvh::vec3<float> h = bvh::vec3<float>::cross(ray_dir, edge2);
  float a = bvh::vec3<float>::dot(edge1, h);
  
  // If the determinant is near zero, the ray is parallel to the triangle
  if (abs(a) < EPSILON) {
    return std::numeric_limits<float>::infinity();
  }

  // Calculate the distance from the ray origin to triangle vertex 1
  float f = 1.0 / a;
  bvh::vec3<float> s = ray_pos - triangle.vertices[0];
  float u = f * bvh::vec3<float>::dot(s, h);
  
  // Check if the intersection is outside the triangle
  if (u < 0.0f || u > 1.0f) {
    return std::numeric_limits<float>::infinity();
  }
  
  // Prepare to test the v coordinate
  bvh::vec3<float> q = bvh::vec3<float>::cross(s, edge1);
  float v = f * bvh::vec3<float>::dot(ray_dir, q);
  
  // Check if the intersection is outside the triangle
  if (v < 0.0f || u + v > 1.0f) {
    return std::numeric_limits<float>::infinity();
  }
  // Calculate the intersection point
  float t = f * bvh::vec3<float>::dot(edge2, q);
  
  // If t is greater than a small epsilon, the ray intersects the triangle
  if (t > EPSILON) {
    return t;
  } else {
    return std::numeric_limits<float>::infinity();
  }
}

void bvh::tests::rt_classroom() {

  char* CLASSROOM = "../tests/data/classroom.obj";
  // char* CLASSROOM_BVH = "./classroom.bvh";
  char* CLASSROOM_BVH = "../tests/data/node.bvh";
  char* OUT_EYE_DIR = "./eye_ray_directions.ppm";
  char* OUT_HIT_NORM = "./hit_normals.ppm";

  const int WIDTH = 80;
  const int HEIGHT = 60;
  const float FOV = 90.0f;
  const bvh::vec3<float> CAMERA_POS(0, 0, 0);
  const bvh::vec3<float> CAMERA_UP(0, 1, 0);
  const bvh::vec3<float> CAMERA_DIR(0, 0, -1);
  
  // Precompute the bvh of the scene
  // std::cout << "Building BVH" << std::endl;
  // bvh::Object::build_bvh(CLASSROOM, CLASSROOM_BVH);
  // std::cout << "BVH built" << std::endl;

  // Load the scene
  std::cout << "Loading scene" << std::endl;
  bvh::Object* classroom = bvh::Object::load(CLASSROOM, CLASSROOM_BVH);
  assert(classroom != nullptr, "Failed to load the scene");
  std::cout << "Scene loaded" << std::endl;

  // Render the scene (brute-force)
  std::cout << "Rendering scene" << std::endl;
  std::cout << "Generating eye ray directions" << std::endl;
  bvh::vec3<float>* eye_ray_directions = generate_eye_ray_directions(
    WIDTH, HEIGHT, FOV, CAMERA_POS, CAMERA_UP, CAMERA_DIR
  );
  std::cout << "Eye ray directions generated" << std::endl;

  float* hit_distances = new float[WIDTH * HEIGHT];
  for (int i = 0; i < WIDTH * HEIGHT; i++) {
    hit_distances[i] = std::numeric_limits<float>::infinity();
  }
  bvh::vec3<float>* hit_normals = new bvh::vec3<float>[WIDTH * HEIGHT];

  for(int i = 0; i < WIDTH * HEIGHT; i++) {
    std::cout << "Processing pixel: (" << i % WIDTH << ", " << i / WIDTH << ")" << std::endl;
    for(int j = 0; j < classroom->num_triangles; j++) {
      // Compute the intersection
      float new_hit_distance = intersect(CAMERA_POS, eye_ray_directions[i], classroom->triangles[j]);

      // Update the hit information
      if (new_hit_distance < hit_distances[i]) {
        hit_distances[i] = new_hit_distance;
        hit_normals[i] = classroom->triangles[j].normals[0];
      }
    }
  }

  clamp_zero_one(hit_normals, WIDTH, HEIGHT);
  save_to_ppm(hit_normals, WIDTH, HEIGHT, OUT_HIT_NORM);

  clamp_zero_one(eye_ray_directions, WIDTH, HEIGHT);
  save_to_ppm(eye_ray_directions, WIDTH, HEIGHT, OUT_EYE_DIR);

  delete[] eye_ray_directions;
  delete[] hit_distances;
  delete[] hit_normals;

  // Render the scene (bvh)

}
