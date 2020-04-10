#include "cppfcgi/error_suite.hpp"

#include "cppfcgi/error.hpp"

#include <cute/cute.h>

#include <kawaii/kawaii.hpp>

auto request_complete_error_has_appropriate_message() -> void
{
  auto error = make_error_code(fcgi::fastcgi_error::request_complete);
  ASSERT_EQUAL("Request complete", error.message());
}

auto cant_multiplex_connections_error_has_appropriate_message() -> void
{
  auto error = make_error_code(fcgi::fastcgi_error::cant_multiplex_connections);
  ASSERT_EQUAL("Connection multiplexing is not supported", error.message());
}

auto overloaded_error_has_appropriate_message() -> void
{
  auto error = make_error_code(fcgi::fastcgi_error::overloaded);
  ASSERT_EQUAL("Application overloaded", error.message());
}

auto unknown_role_error_has_appropriate_message() -> void
{
  auto error = make_error_code(fcgi::fastcgi_error::unknown_role);
  ASSERT_EQUAL("Unknown application role", error.message());
}

auto unknown_error_has_appropriate_message() -> void
{
  auto error = make_error_code(static_cast<fcgi::fastcgi_error>(42));
  ASSERT_EQUAL("Unknown FastCGI error", error.message());
}

auto error_suite() -> kawaii::suite
{
  return {{
              KAWAII(request_complete_error_has_appropriate_message),
              KAWAII(cant_multiplex_connections_error_has_appropriate_message),
              KAWAII(overloaded_error_has_appropriate_message),
              KAWAII(unknown_role_error_has_appropriate_message),
              KAWAII(unknown_error_has_appropriate_message),
          },
          "Error Suite"};
}
