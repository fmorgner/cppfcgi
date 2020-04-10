#include "cppfcgi/record_suite.hpp"

#include "cppfcgi/record.hpp"

#include <cute/cute.h>

#include <kawaii/kawaii.hpp>

#include <array>
#include <stdexcept>

namespace
{
  auto constexpr wireshark_pcap_package_4_header_data = std::array{
      std::byte{0x01},
      std::byte{0x01},
      std::byte{0x00},
      std::byte{0x01},
      std::byte{0x00},
      std::byte{0x08},
      std::byte{0x00},
      std::byte{0x00},
  };

  auto constexpr wireshark_pcap_package_5_header_data = std::array{
      std::byte{0x01},
      std::byte{0x04},
      std::byte{0x00},
      std::byte{0x01},
      std::byte{0x04},
      std::byte{0x17},
      std::byte{0x00},
      std::byte{0x00},
  };

}  // namespace

auto wireshark_fcgi_pcap_package_4_header_parses_version_successfully() -> void
{
  auto header = fcgi::record::header{wireshark_pcap_package_4_header_data};
  ASSERT_EQUAL(fcgi::record::header::version_type::_1, header.version());
}

auto wireshark_fcgi_pcap_package_4_header_parses_type_successfully() -> void
{
  auto header = fcgi::record::header{wireshark_pcap_package_4_header_data};
  ASSERT_EQUAL(fcgi::record::header::type_type::begin_request, header.type());
}

auto wireshark_fcgi_pcap_package_4_header_parses_request_id_successfully() -> void
{
  auto header = fcgi::record::header{wireshark_pcap_package_4_header_data};
  ASSERT_EQUAL(fcgi::record::header::request_id_type{1}, header.request_id());
}

auto wireshark_fcgi_pcap_package_4_header_parses_content_length_successfully() -> void
{
  auto header = fcgi::record::header{wireshark_pcap_package_4_header_data};
  ASSERT_EQUAL(fcgi::record::header::content_length_type{8}, header.content_length());
}

auto wireshark_fcgi_pcap_package_4_header_parses_padding_length_successfully() -> void
{
  auto header = fcgi::record::header{wireshark_pcap_package_4_header_data};
  ASSERT_EQUAL(fcgi::record::header::padding_length_type{0}, header.padding_length());
}

auto wireshark_fcgi_pcap_package_5_header_parses_version_successfully() -> void
{
  auto header = fcgi::record::header{wireshark_pcap_package_5_header_data};
  ASSERT_EQUAL(fcgi::record::header::version_type::_1, header.version());
}

auto wireshark_fcgi_pcap_package_5_header_parses_type_successfully() -> void
{
  auto header = fcgi::record::header{wireshark_pcap_package_5_header_data};
  ASSERT_EQUAL(fcgi::record::header::type_type::parameters, header.type());
}

auto wireshark_fcgi_pcap_package_5_header_parses_request_id_successfully() -> void
{
  auto header = fcgi::record::header{wireshark_pcap_package_5_header_data};
  ASSERT_EQUAL(fcgi::record::header::request_id_type{1}, header.request_id());
}

auto wireshark_fcgi_pcap_package_5_header_parses_content_length_successfully() -> void
{
  auto header = fcgi::record::header{wireshark_pcap_package_5_header_data};
  ASSERT_EQUAL(fcgi::record::header::content_length_type{1047}, header.content_length());
}

auto wireshark_fcgi_pcap_package_5_header_parses_padding_length_successfully() -> void
{
  auto header = fcgi::record::header{wireshark_pcap_package_5_header_data};
  ASSERT_EQUAL(fcgi::record::header::padding_length_type{0}, header.padding_length());
}

auto corrupted_wireshark_fcgi_pcap_package_5_header_fails_to_parse_version_successfully() -> void
{
  auto data = wireshark_pcap_package_5_header_data;
  data[0] = std::byte{42};
  ASSERT_THROWS(fcgi::record::header{data}, std::invalid_argument);
}

auto corrupted_wireshark_fcgi_pcap_package_5_header_fails_to_parse_type_successfully() -> void
{
  auto data = wireshark_pcap_package_5_header_data;
  data[1] = std::byte{21};
  ASSERT_THROWS(fcgi::record::header{data}, std::invalid_argument);
}

auto record_suite() -> kawaii::suite
{
  return {{
              KAWAII(wireshark_fcgi_pcap_package_4_header_parses_version_successfully),
              KAWAII(wireshark_fcgi_pcap_package_4_header_parses_type_successfully),
              KAWAII(wireshark_fcgi_pcap_package_4_header_parses_request_id_successfully),
              KAWAII(wireshark_fcgi_pcap_package_4_header_parses_content_length_successfully),
              KAWAII(wireshark_fcgi_pcap_package_4_header_parses_padding_length_successfully),
              KAWAII(wireshark_fcgi_pcap_package_5_header_parses_version_successfully),
              KAWAII(wireshark_fcgi_pcap_package_5_header_parses_type_successfully),
              KAWAII(wireshark_fcgi_pcap_package_5_header_parses_request_id_successfully),
              KAWAII(wireshark_fcgi_pcap_package_5_header_parses_content_length_successfully),
              KAWAII(wireshark_fcgi_pcap_package_5_header_parses_padding_length_successfully),
              KAWAII(corrupted_wireshark_fcgi_pcap_package_5_header_fails_to_parse_version_successfully),
              KAWAII(corrupted_wireshark_fcgi_pcap_package_5_header_fails_to_parse_type_successfully),
          },
          "Record Suite"};
}
