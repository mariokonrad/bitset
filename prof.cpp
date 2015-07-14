#include "bitset.hpp"

using namespace mk;

static void create_and_append_uint32_t(uint64_t times)
{
	static const uint32_t value = 0xaa55aa55;

	for (uint64_t i = 0; i < times; ++i) {
		bitset<uint8_t> b;
		b.append(value);
	}
}

static void set_bits_large_bitset(uint64_t times)
{
	const int SIZE = 1024 * 1024;

	for (uint64_t i = 0; i < times; ++i) {
		bitset<uint8_t> b{SIZE+128};
		for (size_t j = 0; j < SIZE; ++j) {
			b.set(1, j, 1);
		}
	}
}

int main(int, char **)
{
	// create_and_append_uint32_t(1000000);
	set_bits_large_bitset(100);
}
