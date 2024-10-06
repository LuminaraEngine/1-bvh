#include <ansi_colors.hpp>
#include <custom_assert.hpp>
#include <test_load_bvh.hpp>

#include <iostream>
#include <cstdio>
#include <string>
#include <unordered_map>
#include <chrono>


// Add new tests here
std::unordered_map<std::string, bool (*)()> tests = {
  {"load_bvh", bvh::tests::load_bvh}
};


bool run_one_test(std::string test_name, bool (*test_funct)()) {

  try {
    // Run the test
    auto start = std::chrono::high_resolution_clock::now();
    bool result = test_funct();
    auto end = std::chrono::high_resolution_clock::now();
    std::chrono::duration<double> duration = end - start;

    // Print the result
    if (result) {
      std::cout << BG_GREEN << "TEST" << RESET_BG << GREEN << ' ' << test_name << RESET_COLOR << std::endl;
      std::cout << "PASSED after " << duration.count() << " ms" << std::endl << std::endl;
      return true;
    } else {
      std::cout << BG_RED << "TEST" << RESET_BG << RED << ' ' << test_name << RESET_COLOR << std::endl;
      std::cout << "FAILED after " << duration.count() << " ms" << std::endl << std::endl;
      return false;
    }
  }
  // Catch exceptions
  catch(const AssertionFailedException& e) {
    std::cout << BG_RED << "TEST" << RESET_BG << RED << ' ' << test_name << RESET_COLOR << std::endl;
    std::cout << "FAILED with assertion: " << std::endl;
    std::cout << e.what() << std::endl << std::endl;
    return false;
  }
  catch(const std::exception& e) {
    std::cout << BG_BLUE << "TEST" << RESET_BG << BLUE << ' ' << test_name << RESET_COLOR << std::endl;
    std::cout << "CRASHED with exception: " << std::endl;
    std::cout << e.what() << std::endl << std::endl;
    return false;
  }
  catch(...) {
    std::cout << BG_BLUE << "TEST" << RESET_BG << BLUE << ' ' << test_name << RESET_COLOR << std::endl;
    std::cout << "CRASHED with unknown exception" << std::endl << std::endl;
    return false;
  }
  
}


void run_all_tests() {
  int num_tests = tests.size();
  int num_passed = 0;

  for (auto const& test : tests) {
    if (run_one_test(test.first, test.second)) {
      num_passed++;
    }
  }

  printf("%d/%d tests passed\n", num_passed, num_tests);
}


int main(int argc, char** argv) {
  
  // Print help message
  if (argc == 1) {
    printf("Usage: %s [-a] test_name1 [test_name2 ...]\n", argv[0]);
    return 1;
  }
  if (std::string(argv[1]) == "-h") {
    printf("Usage: %s [-a] test_name1 [test_name2 ...]\n", argv[0]);
    return 1;
  }

  // Print greeting message
  std::cout << BG_CYAN << "CONSOLE" << RESET_BG << std::endl;
  std::cout << "Running BVH tests..." << std::endl << std::endl;

  // Run all tests
  if (std::string(argv[1]) == "-a") {
    run_all_tests();
    return 0;
  }

  int num_tests = argc - 1;
  int num_passed = 0;

  for (int i = 1; i < argc; i++) {
    std::string test_name = argv[i];

    if (tests.find(test_name) == tests.end()) {
      std::cout << BG_YELLOW << "WARNING" << RESET_BG << YELLOW << ' ' << test_name << RESET_COLOR << std::endl;
      std::cout << "Not found" << std::endl << std::endl;
      num_tests--;
      continue;
    }

    if (run_one_test(test_name, tests[test_name])) {
      num_passed++;
    }    
  }

  std::cout << BG_CYAN << "SUMMARY" << RESET_BG << std::endl;
  printf("%d/%d tests passed\n", num_passed, num_tests);

  return 0;

}
