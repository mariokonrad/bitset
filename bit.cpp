// bit.cpp

#include <bitset.hpp>
#include <stdint.h>
#include <cstdio>

template <typename T> static void dump(T v, int b = sizeof(T) * 8)
{
	for (int i = 0; i < b; ++i) {
		if (i % bitset<T>::BITS_PER_BYTE == 0) printf(" ");
		printf("%d", (v >> (b - i - 1)) & 0x1);
	}
}

int main(int, char **)
{
	bitset<uint8_t> s;

	uint8_t w = 0;
	uint16_t x = 0;

	s.append(0, 16);
	s.set(0xff, 3, 8);
	s.append(0, 8);
	s.append(0xff, 8);
	s.append(1, 1);

	s.get(w, 6, 5);
	s.get(x, 10);

	printf("w =   %02x : ", w); dump(w); printf("\n");
	printf("x = %04x : ", x); dump(x); printf("\n");

/*
	s.append(0xff, 8);
	s.append(0xff, 3);
	s.append(0, 5);
	s.append(0xffff, 16);
	s.append(1, 1);
	s.append(0, 1);
	s.append(1, 1);

	s.set(1, 40, 1);
*/
	printf("\n %u / %u : ", s.size(), s.capacity());
	for (bitset<uint8_t>::size_type i = 0; i < s.size(); ++i) {
		if (i % bitset<uint8_t>::BITS_PER_BYTE == 0) printf(" ");
		printf("%d", s[i] ? 1 : 0);
	}
	printf("\n");
	printf("\n");

	return 0;
}

