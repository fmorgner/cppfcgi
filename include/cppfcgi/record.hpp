#ifndef CPPFCGI_RECORD_HPP
#define CPPFCGI_RECORD_HPP

#include <array>
#include <cstddef>
#include <cstdint>
#include <stdexcept>
#include <string>
#include <type_traits>
#include <vector>

namespace fcgi
{

  struct record
  {
    struct header
    {
      enum struct version_type : std::uint8_t
      {
        _1 = 1,
      };

      enum struct type_type : std::uint8_t
      {
        begin_request = 1,
        abort_request,
        end_request,
        parameters,
        standard_input,
        standard_output,
        standard_error,
        data,
        get_values,
        get_values_result,
        unknown,
      };

      using request_id_type = std::uint16_t;
      using content_length_type = std::uint16_t;
      using padding_length_type = std::uint8_t;

      auto static constexpr size{static_cast<std::size_t>(8)};

      explicit constexpr header(std::array<std::byte, size> raw_data)
          : m_raw_data{raw_data}
      {
        check_version();
        check_type();
      }

      [[nodiscard]] auto version() const noexcept -> version_type;
      [[nodiscard]] auto type() const noexcept -> type_type;
      [[nodiscard]] auto request_id() const noexcept -> request_id_type;
      [[nodiscard]] auto content_length() const noexcept -> content_length_type;
      [[nodiscard]] auto padding_length() const noexcept -> padding_length_type;

    private:
      auto check_version() const -> void;
      auto check_type() const -> void;

      std::array<std::byte, size> m_raw_data{};
    };

  private:
    header m_header;
    std::vector<std::byte> m_content_data{};
    std::vector<std::byte> m_padding_data{};
  };

}  // namespace fcgi

#endif