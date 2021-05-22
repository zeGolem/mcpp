#include "types.h"
#include "utils/types.h"
#include <vector>

using namespace mcpp;
using namespace protocol;

template <> const unsigned int type::varint::size() const
{
	unsigned int size = 0;
	int value_tmp = value();
	do {
		value_tmp >>= 7;
		size++;
	} while (value_tmp != 0);

	return size;
}

template <> const unsigned int type::string::size() const
{
	auto string_size = value().size();
	auto varint_size = type::varint(string_size).size();

	return string_size + varint_size;
}

template <> const unsigned int type::uuid::size() const
{
	return 16;
}
