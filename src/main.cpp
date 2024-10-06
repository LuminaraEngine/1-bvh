#include <test_load_bvh.hpp>

#include <cstdio>
#include <string>
#include <unordered_map>
#include <iostream>
#include <stdlib.h>

using namespace bvh;

std::unordered_map<std::string, bool (*)()> tests = {
  {"test_load_bvh", test_load_bvh}
};

void run_all_tests() {
  int num_tests = tests.size();
  int num_passed = 0;

  for (auto const& test : tests) {

    bool result;
    bool crashed = false;

    try {
      result = test.second();
    }
    catch(const std::exception& e) {
      printf("Test %s crashed\n", test.first.c_str());
      printf("Exception: %s\n", e.what());
      result = false;
      crashed = true;
    }
    catch(...) {
      result = false;
      crashed = true;
    }

    if (result) {
      printf("Test %s passed\n", test.first.c_str());
      num_passed++;
    } else if (!crashed) {
      printf("Test %s failed\n", test.first.c_str());
    }
  }

  printf("%d/%d tests passed\n", num_passed, num_tests);
}

int main(int argc, char** argv) {
  
  if (argc == 1) {
    printf("Usage: %s [-a] test_name1 [test_name2 ...]\n", argv[0]);
    return 1;
  }

  if (std::string(argv[1]) == "-a") {
    run_all_tests();
    return 0;
  }

  int num_tests = argc - 1;
  int num_passed = 0;

  for (int i = 1; i < argc; i++) {
    std::string test_name = argv[i];

    if (tests.find(test_name) == tests.end()) {
      printf("Error: Test %s not found\n", test_name.c_str());
      return 1;
    }

    bool result;
    bool crashed = false;

    try {
      result = tests[test_name]();
    }
    catch(const std::exception& e) {
        printf("Test %s crashed\n", test_name.c_str());
      printf("Exception: %s\n", e.what());
      result = false;
      crashed = true;
    }
    catch(...) {
      result = false;
      crashed = true;
    }

    if (result) {
      printf("Test %s passed\n", test_name.c_str());
      num_passed++;
    } else if (!crashed) {
      printf("Test %s failed\n", test_name.c_str());
    }
  }

  printf("%d/%d tests passed\n", num_passed, num_tests);

  return 0;

}
