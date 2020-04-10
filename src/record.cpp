#include "cppfcgi/record.hpp"

#include <cstdint>
#include <iterator>
#include <numeric>

namespace fcgi
{

  namespace
  {
    auto constexpr bits_per_byte{8};

    auto constexpr version_offset{static_cast<std::size_t>(0)};
    auto constexpr type_offset{version_offset + sizeof(record::header::version_type)};
    auto constexpr request_id_offset{type_offset + sizeof(record::header::type_type)};
    auto constexpr content_length_offset{request_id_offset + sizeof(record::header::request_id_type)};
    auto constexpr padding_length_offset{content_length_offset + sizeof(record::header::content_length_type)};

    template<typename OutputType, typename BufferType>
    [[nodiscard]] auto constexpr parse_value(BufferType const & buffer, std::size_t offset = 0) -> OutputType
    {
      auto value_begin = std::cbegin(buffer) + offset;
      auto value_end = std::next(value_begin, sizeof(OutputType));

      return std::accumulate(value_begin, value_end, OutputType{}, [](auto acc, auto part) {
        return (acc << bits_per_byte) | static_cast<unsigned char>(part);
      });
    }
  }  // namespace

  auto record::header::version() const noexcept -> version_type
  {
    auto raw_version = parse_value<std::uint8_t>(m_raw_data, version_offset);
    return static_cast<version_type>(raw_version);
  }

  auto record::header::type() const noexcept -> type_type
  {
    auto raw_type = parse_value<std::uint8_t>(m_raw_data, type_offset);
    return static_cast<type_type>(raw_type);
  }

  auto record::header::request_id() const noexcept -> request_id_type
  {
    auto raw_request_id = parse_value<std::uint16_t>(m_raw_data, request_id_offset);
    return static_cast<request_id_type>(raw_request_id);
  }

  auto record::header::content_length() const noexcept -> content_length_type
  {
    auto raw_content_length = parse_value<std::uint16_t>(m_raw_data, content_length_offset);
    return static_cast<content_length_type>(raw_content_length);
  }

  auto record::header::padding_length() const noexcept -> padding_length_type
  {
    auto raw_padding_length = parse_value<std::uint16_t>(m_raw_data, padding_length_offset);
    return static_cast<padding_length_type>(raw_padding_length);
  }

  auto record::header::check_version() const -> void
  {
    if (static_cast<unsigned char>(version()) > static_cast<unsigned char>(version_type::_1))
    {
      throw std::invalid_argument{"unsupported version: " + std::to_string(static_cast<int>(version()))};
    }
  }

  auto record::header::check_type() const -> void
  {
    if (static_cast<unsigned char>(type()) > static_cast<unsigned char>(type_type::unknown))
    {
      throw std::invalid_argument{"unsupported type: " + std::to_string(static_cast<int>(type()))};
    }
  }

}  // namespace fcgi