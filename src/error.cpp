#include "cppfcgi/error.hpp"

#include <string>
#include <system_error>

namespace fcgi
{

  auto fastcgi_category_impl::name() const noexcept -> char const *
  {
    return "FastCGI";
  }

  auto fastcgi_category_impl::message(int error_value) const -> std::string
  {
    switch (static_cast<fastcgi_error>(error_value))
    {
    case fastcgi_error::request_complete:
      return "Request complete";
    case fastcgi_error::cant_multiplex_connections:
      return "Connection multiplexing is not supported";
    case fastcgi_error::overloaded:
      return "Application overloaded";
    case fastcgi_error::unknown_role:
      return "Unknown application role";
    default:
      return "Unknown FastCGI error";
    };
  }

  auto fastcgi_category_impl::default_error_condition(int error_value) const noexcept -> std::error_condition
  {
    switch (static_cast<fastcgi_error>(error_value))
    {
    case fastcgi_error::cant_multiplex_connections:
      return std::errc::operation_not_supported;
    case fastcgi_error::overloaded:
      return std::errc::resource_unavailable_try_again;
    case fastcgi_error::unknown_role:
      return std::errc::not_supported;
    default:
      return std::error_condition(error_value, *this);
    }
  }

  auto fastcgi_category() -> std::error_category const &
  {
    auto static const instance = fastcgi_category_impl{};
    return instance;
  }

  [[nodiscard]] auto make_error_code(fastcgi_error error) -> std::error_code
  {
    return std::error_code(static_cast<int>(error), fastcgi_category());
  }

  [[nodiscard]] auto make_error_condition(fastcgi_error error) -> std::error_condition
  {
    return std::error_condition(static_cast<int>(error), fastcgi_category());
  }

}  // namespace fcgi