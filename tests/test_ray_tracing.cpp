#include <test_ray_tracing.hpp>

#include <vec3.hpp>
#include <object.hpp>

#include <chrono>
#include <math.h>

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

void bvh::tests::rt_classroom() {

  char* CLASSROOM = "../tests/data/classroom.obj";
  char* CLASSROOM_BVH = "./classroom.bvh";
  
  // Precompute the bvh of the scene
  // bvh::Object::build_bvh(CLASSROOM, CLASSROOM_BVH);

  // Load the scene
  // bvh::Object* classroom = bvh::Object::load(CLASSROOM, CLASSROOM_BVH);

  // Render the scene (brute-force)
  bvh::vec3<float>* eye_ray_directions = generate_eye_ray_directions(
    80, 60, 90.0f, bvh::vec3<float>(0, 0, 0), bvh::vec3<float>(0, 1, 0), bvh::vec3<float>(0, 0, -1)
  );

  clamp_zero_one(eye_ray_directions, 80, 60);
  save_to_ppm(eye_ray_directions, 80, 60, "eye_ray_directions.ppm");

  // Render the scene (bvh)

}
