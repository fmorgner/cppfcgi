#include "cppfcgi/error_suite.hpp"

#include <cute/cute.h>
#include <cute/cute_runner.h>
#include <cute/tap_listener.h>

#include <algorithm>
#include <cstdlib>
#include <iostream>
#include <iterator>
#include <numeric>
#include <string>
#include <utility>
#include <vector>

using suite_list = std::vector<std::pair<cute::suite, std::string>>;

auto get_test_selectors(suite_list const & suites) -> std::vector<std::string>
{
  auto selectors = std::vector<std::string>{};

  for_each(cbegin(suites), cend(suites), [&](auto descriptor) {
    auto const & [suite, name] = descriptor;
    transform(cbegin(suite), cend(suite), std::back_inserter(selectors), [&, name = name](auto test) {
      return name + "#" + test.name();
    });
  });

  return selectors;
}

auto do_run_tests(suite_list const & suites, int argc, char ** argv) -> bool
{
  auto listener = cute::tap_listener<>{};
  auto runner = cute::makeRunner(listener, argc, argv);

  return accumulate(cbegin(suites), cend(suites), true, [&](auto accumulator, auto const & descriptor) {
    auto const & [suite, name] = descriptor;
    return accumulator && runner(suite, name.c_str());
  });
}

int main(int argc, char ** argv)
{
  using namespace std::string_literals;

  auto suites = std::vector{
      error_suite(),
  };

  auto selectors = get_test_selectors(suites);

  auto list_tests = argc == 2 && argv[1] == "--tests"s;
  auto selected_tests = std::vector<std::string>{};

  if (list_tests)
  {
    copy(cbegin(selectors), cend(selectors), std::ostream_iterator<std::string>{std::cout, "\n"});
  }
  else
  {
    return do_run_tests(suites, argc, argv) ? EXIT_SUCCESS : EXIT_FAILURE;
  }
}