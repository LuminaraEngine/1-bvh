#include <test_ray_tracing.hpp>

#include <custom_assert.hpp>
#include <vec3.hpp>
#include <triangle.hpp>
#include <object.hpp>

#include <limits>
#include <math.h>
#include <chrono>

#ifndef M_PI
#define M_PI 3.14159265358979323846
#endif

#define EPSILON 1e-6

/**
 * Helper function to compute the absolute value of an image
 * 
 * @param image The image to compute the absolute value of as a 1D array of vec3<float>
 * @param width The width of the image
 * @param height The height of the image
 */
void img_abs(bvh::vec3<float>* image, int width, int height) {
  for(int y = 0; y < height; y++) {
    for(int x = 0; x < width; x++) {
      image[y * width + x].x = fabs(image[y * width + x].x);
      image[y * width + x].y = fabs(image[y * width + x].y);
      image[y * width + x].z = fabs(image[y * width + x].z);
    }
  }
}

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
float intersect_triangle(bvh::vec3<float> ray_pos, bvh::vec3<float> ray_dir, const bvh::Triangle& triangle) {

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

/**
 * Helper function to render a scene using the brute-force method
 * 
 * @param width The width of the image
 * @param height The height of the image
 * @param eye_origin The origin of the eye rays
 * @param eye_directions The directions of the eye rays
 * @param object The object to render
 * @param output_filename The name of the file to save the image to
 * @param hit_distances (output) The distances to the hit points
 * @param hit_normals (output) The normals at the hit points
 */
void render_brute_force(
  int width,
  int height,
  bvh::vec3<float> eye_origin,
  const bvh::vec3<float>* eye_directions,
  const bvh::Object& object,
  const char* output_filename,
  float** hit_distances,
  bvh::vec3<float>** hit_normals
) {
  *hit_distances = new float[width * height];
  std::fill_n(*hit_distances, width * height, std::numeric_limits<float>::infinity());
  *hit_normals = new bvh::vec3<float>[width * height];

  for(int i = 0; i < width * height; i++) {
    for(int j = 0; j < object.num_triangles; j++) {
      // Compute the intersection
      float new_hit_distance = intersect_triangle(eye_origin, eye_directions[i], object.triangles[j]);

      // Update the hit information
      if (new_hit_distance < (*hit_distances)[i]) {
        (*hit_distances)[i] = new_hit_distance;
        (*hit_normals)[i] = object.triangles[j].normals[0];
      }
    }
  }

  clamp_zero_one(*hit_normals, width, height);
  save_to_ppm(*hit_normals, width, height, output_filename);
}

/**
 * Helper function to compute the intersection between a ray and the closest triangle by traversing the BVH
 * 
 * @param ray_pos The position of the ray
 * @param ray_dir The direction of the ray
 * @param node The current node of the BVH
 * @param triangle_list The list of triangles of the object
 * @param tri_index (output) The index of the closest triangle
 * 
 * @return The distance to the intersection point or +INF if there is no intersection
 */
float intersect_bvh(bvh::vec3<float> ray_pos, bvh::vec3<float> ray_dir, const bvh::BvhNode* node, bvh::Triangle* triangle_list, int &tri_index) {
  // slab test
  bvh::vec3<float> box_min = node->bounding_box.min;
  bvh::vec3<float> box_max = node->bounding_box.max;
  float tx1 = (box_min.x - ray_pos.x) / ray_dir.x, tx2 = (box_max.x - ray_pos.x) / ray_dir.x;
  float ty1 = (box_min.y - ray_pos.y) / ray_dir.y, ty2 = (box_max.y - ray_pos.y) / ray_dir.y;
  float tz1 = (box_min.z - ray_pos.z) / ray_dir.z, tz2 = (box_max.z - ray_pos.z) / ray_dir.z;

  float tmin = fmaxf(fmaxf(fminf(tx1, tx2), fminf(ty1, ty2)), fminf(tz1, tz2));
  float tmax = fminf(fminf(fmaxf(tx1, tx2), fmaxf(ty1, ty2)), fmaxf(tz1, tz2));
  bool ray_intersects_box = (tmax >= tmin && tmax >= 0);

  if (ray_intersects_box) {
    if (node->left == nullptr && node->right == nullptr) { // leaf node
      bvh::BvhLeaf* leaf = (bvh::BvhLeaf*)node;
      float min_distance = std::numeric_limits<float>::infinity();

      for (int i = 0; i < leaf->num_triangles; i++) {
        float new_hit_distance = intersect_triangle(ray_pos, ray_dir, triangle_list[leaf->indices[i]]);
        if (new_hit_distance < min_distance) {
          min_distance = new_hit_distance;
          tri_index = leaf->indices[i];
        }
      }
      return min_distance;

    } else { // recursion to children nodes
      float left_distance = intersect_bvh(ray_pos, ray_dir, node->left, triangle_list, tri_index);
      float right_distance = intersect_bvh(ray_pos, ray_dir, node->right, triangle_list, tri_index);
      return fminf(left_distance, right_distance);
    }
  }

  return std::numeric_limits<float>::infinity();
}

/**
 * Helper function to render a scene using a bvh
 * 
 * @param width The width of the image
 * @param height The height of the image
 * @param eye_origin The origin of the eye rays
 * @param eye_directions The directions of the eye rays
 * @param object The object to render
 * @param output_filename The name of the file to save the image to
 * @param hit_distances (output) The distances to the hit points
 * @param hit_normals (output) The normals at the hit points
 */
void render_bvh(
  int width,
  int height,
  bvh::vec3<float> eye_origin,
  const bvh::vec3<float>* eye_directions,
  const bvh::Object& object,
  const char* output_filename,
  float** hit_distances,
  bvh::vec3<float>** hit_normals
) {
  *hit_distances = new float[width * height];
  std::fill_n(*hit_distances, width * height, std::numeric_limits<float>::infinity());
  *hit_normals = new bvh::vec3<float>[width * height];

  // TODO traverse the bvh and compute the intersections
  for (int i = 0; i < width * height; i++) {
    int tri_index = -1;
    (*hit_distances)[i] = intersect_bvh(eye_origin, eye_directions[i], object.bvh, object.triangles, tri_index);
    (*hit_normals)[i] = object.triangles[tri_index].normals[0];
  }

  clamp_zero_one(*hit_normals, width, height);
  save_to_ppm(*hit_normals, width, height, output_filename);
}

void compare(
  float* hit_distances_bf,
  bvh::vec3<float>* hit_normals_bf,
  float* hit_distances_bvh,
  bvh::vec3<float>* hit_normals_bvh,
  int width,
  int height,
  const char* hit_distances_filename,
  const char* hit_normals_filename
) {

  // Hit distances
  FILE* file_hit_distances = fopen(hit_distances_filename, "w");

  fprintf(file_hit_distances, "P3\n%d %d\n1\n", width, height);

  for(int y = 0; y < height; y++) {
    for(int x = 0; x < width; x++) {
      if (abs(hit_distances_bf[y * width + x] - hit_distances_bvh[y * width + x]) > EPSILON) {
        fprintf(file_hit_distances, "1 0 0 ");  // red
      } else {
        fprintf(file_hit_distances, "0 1 0 ");  // green
      }
    }
    fprintf(file_hit_distances, "\n");
  }
  fclose(file_hit_distances);

  // Hit normals
  bvh::vec3<float>* hit_normal_diff = new bvh::vec3<float>[width * height];

  for(int i = 0; i < width * height; i++) {
    hit_normal_diff[i] = bvh::vec3<float>::normalize(
      bvh::vec3<float>(
        abs(hit_normals_bf[i].x - hit_normals_bvh[i].x),
        abs(hit_normals_bf[i].y - hit_normals_bvh[i].y),
        abs(hit_normals_bf[i].z - hit_normals_bvh[i].z)
      )
    );
  }

  // img_abs(hit_normal_diff, width, height);
  clamp_zero_one(hit_normal_diff, width, height);
  save_to_ppm(hit_normal_diff, width, height, hit_normals_filename);

}

void bvh::tests::rt_classroom() {

  char* CLASSROOM = "../tests/data/final/suzanne.obj";
  char* CLASSROOM_BVH = "./suzanne.bvh";
  char* RENDER_BF = "./suzanne_bf.ppm";
  char* RENDER_BVH = "./suzanne_bvh.ppm";
  char* DIFF_HIT_DISTANCES = "./diff_hit_distances.ppm";
  char* DIFF_HIT_NORMALS = "./diff_hit_normals.ppm";

  // char* CLASSROOM = "../tests/data/classroom.obj";
  // char* CLASSROOM_BVH = "./classroom.bvh";
  // char* RENDER_BF = "./classroom_bf.ppm";
  // char* RENDER_BVH = "./classroom_bvh.ppm";
  // char* DIFF_HIT_DISTANCES = "./diff_hit_distances.ppm";
  // char* DIFF_HIT_NORMALS = "./diff_hit_normals.ppm";

  const int WIDTH = 400;
  const int HEIGHT = 300;
  const float FOV = 60.0f;
  const bvh::vec3<float> CAMERA_POS(0, 0, 3);
  const bvh::vec3<float> CAMERA_UP(0, 1, 0);
  const bvh::vec3<float> CAMERA_DIR = bvh::vec3<float>::normalize(bvh::vec3<float>(0, 0, -1));
  
  // Precompute the bvh of the scene
  std::cout << "Building BVH" << std::endl;
  bvh::Object::build_bvh(CLASSROOM, CLASSROOM_BVH);
  std::cout << "BVH built" << std::endl;

  // Load the scene
  std::cout << "Loading scene" << std::endl;
  bvh::Object* classroom = bvh::Object::load(CLASSROOM, CLASSROOM_BVH);
  assert(classroom != nullptr, "Failed to load the scene");
  std::cout << "Scene loaded" << std::endl;

  // Generate the eye ray directions
  bvh::vec3<float>* eye_ray_directions = generate_eye_ray_directions(
    WIDTH, HEIGHT, FOV, CAMERA_UP, CAMERA_DIR
  );

  // Render the scene (brute-force)
  float* hit_distances_bf;
  bvh::vec3<float>* hit_normals_bf;
  std::cout << "Rendering scene (brute-force)" << std::endl;
  render_brute_force(WIDTH, HEIGHT, CAMERA_POS, eye_ray_directions, *classroom, RENDER_BF, &hit_distances_bf, &hit_normals_bf);
  std::cout << "Scene rendered" << std::endl;
  std::cout << "Hit normals saved to " << RENDER_BF << std::endl;

  // Render the scene (bvh)
  float* hit_distances_bvh;
  bvh::vec3<float>* hit_normals_bvh;
  std::cout << "Rendering scene (bvh)" << std::endl;
  render_bvh(WIDTH, HEIGHT, CAMERA_POS, eye_ray_directions, *classroom, RENDER_BVH, &hit_distances_bvh, &hit_normals_bvh);
  std::cout << "Scene rendered" << std::endl;
  std::cout << "Hit normals saved to " << RENDER_BVH << std::endl;

  // Compare the two images
  compare(hit_distances_bf, hit_normals_bf, hit_distances_bvh, hit_normals_bvh, WIDTH, HEIGHT, DIFF_HIT_DISTANCES, DIFF_HIT_NORMALS);

  // Cleanup
  delete[] eye_ray_directions;
  delete[] hit_distances_bf;
  delete[] hit_normals_bf;
  delete[] hit_distances_bvh;
  delete[] hit_normals_bvh;
  delete classroom;

}
