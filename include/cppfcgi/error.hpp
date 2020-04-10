#ifndef CPPFCGI_ERROR_HPP
#define CPPFCGI_ERROR_HPP

#include <string>
#include <system_error>
#include <type_traits>

namespace fcgi
{

  enum struct fastcgi_error
  {
    request_complete,
    cant_multiplex_connections,
    overloaded,
    unknown_role,
  };

  struct fastcgi_category_impl : std::error_category
  {
    [[nodiscard]] auto name() const noexcept -> char const * override;
    [[nodiscard]] auto message(int error_value) const -> std::string override;
    [[nodiscard]] auto default_error_condition(int error_value) const noexcept -> std::error_condition override;
  };

  [[nodiscard]] auto fastcgi_category() -> std::error_category const &;

  [[nodiscard]] auto make_error_code(fastcgi_error error) -> std::error_code;

  [[nodiscard]] auto make_error_condition(fastcgi_error error) -> std::error_condition;

}  // namespace fcgi

namespace std
{
  template<>
  struct is_error_code_enum<fcgi::fastcgi_error> : true_type
  {
  };
}  // namespace std

#endif