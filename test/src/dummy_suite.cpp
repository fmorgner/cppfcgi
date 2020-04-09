#include "cppfcgi/dummy_suite.hpp"

#include <cute/cute.h>

#include <kawaii/kawaii.hpp>

auto fail_because_there_are_no_tests_yet() -> void
{
  FAILM("Start writing tests!");
}

auto dummy_suite() -> kawaii::suite
{
  return {{
              KAWAII(fail_because_there_are_no_tests_yet),
          },
          "Dummy Suite"};
}
