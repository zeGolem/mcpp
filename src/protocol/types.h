#pragma once

#include "utils/types.h"
#include <string>
#include <vector>

namespace mcpp
{
namespace protocol
{

template <typename T> class protocol_type
{
  private:
	T m_value;

  public:
	protocol_type(T value) : m_value(value) {}
	protocol_type(): m_value() {}
	const T value() const { return m_value; }

	operator const T() const { return m_value; }

	// Size of the raw data to send through the socket in bytes
	const unsigned int size() const;
};

namespace type
{
typedef protocol_type<int> varint;
typedef protocol_type<long> varlong;
typedef protocol_type<std::string> string;
} // namespace type

} // namespace protocol
} // namespace mcpp