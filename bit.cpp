// bit.cpp

#include <stdint.h>
#include <vector>
#include <cstdio>

class bitset
{
	public:
		typedef uint8_t block_type;
	private:
		enum { BLOCK_BITS = sizeof(block_type) * 8 };
		typedef std::vector<block_type> Data;
	public:
		typedef Data::size_type size_type;
	private:
		Data data;
		size_type pos; // bits
	private:
		void extend(size_type bits)
		{
			// TODO: using reserve (because of potentially large values of 'bits')
			while (bits > capacity() - pos) {
				data.push_back(block_type());
			}
		}

		// number of bits must be between 0 and BLOCK_BITS
		void append_block(block_type v, size_type bits = BLOCK_BITS)
		{
			if (bits == 0) return;
			extend(bits);
			size_type i = pos / BLOCK_BITS; // index of current block
			size_type u_bits = BLOCK_BITS - (pos % BLOCK_BITS); // number of bits unused within the current block
			if (u_bits >= bits) {
				// enough room within current block
				data[i] |= v << (u_bits - bits);
			} else {
				// not enough room, split value to current and next block
				data[i+0] |= v >> (bits - u_bits);
				data[i+1] |= v << (BLOCK_BITS - (bits - u_bits));
			}
			pos += bits;
		}

		void set_block(block_type v, size_type ofs, size_type bits = BLOCK_BITS)
		{
			if (bits == 0) return;
			if (ofs + bits > capacity()) extend(ofs + bits - capacity());
			size_type i = ofs / BLOCK_BITS; // index of current block
			size_type u_bits = BLOCK_BITS - (ofs % BLOCK_BITS); // number of bits unused within the current block
			if (u_bits >= bits) {
				// enough room within current block
				block_type mask = ~((1 << (u_bits - bits)) - 1);
				data[i] = (data[i] & mask) | v << (u_bits - bits);
			} else {
				// not enough room, split value to current and next block
				block_type mask0 = ~((1 << (bits - u_bits)) - 1);
				block_type mask1 = (1 << (BLOCK_BITS - (bits - u_bits))) - 1;

				data[i+0] = (data[i+0] & mask0) | v >> (bits - u_bits);
				data[i+1] = (data[i+1] & mask1) | v << (BLOCK_BITS - (bits - u_bits));
			}
			if (ofs + bits > pos) pos = ofs + bits;
		}

public:
		void get_block(block_type & v, size_type ofs, size_type bits = BLOCK_BITS)
		{
			if (bits == 0) return;
			if (ofs + bits > size()) return;
			size_type i = ofs / BLOCK_BITS; // index of current block
			size_type u_bits = BLOCK_BITS - (ofs % BLOCK_BITS); // number of bits unused within the current block
			if (u_bits >= bits) {
				// desired data fully within the current block
				block_type mask = (1 << u_bits) - 1;
				v = (data[i] & mask) >> (u_bits - bits);
			} else {
				// desired value is part from current block and part from next
				block_type mask0 = (1 << u_bits) - 1;
				block_type mask1 = ((1 << (BLOCK_BITS - (bits - u_bits))) - 1) << (bits - u_bits);
				v = (data[i+0] & mask0) << (bits - u_bits) | (data[i+1] & mask1) >> (BLOCK_BITS - (bits - u_bits));
			}
		}
	public:
		bitset()
			: pos(0)
		{}

		size_type capacity() const
		{
			return data.size() * BLOCK_BITS;
		}

		size_type size() const
		{
			return pos;
		}

		void reserve(size_type bits)
		{
			extend(bits);
		}

		bool operator[](size_type i) const
		{
			return get_bit(i);
		}

		bool get_bit(size_type i) const
		{
			if (i > size()) return false;
			size_type n_bit = BLOCK_BITS - (i % BLOCK_BITS) - 1; // bit within the block to be read
			return (data[i / BLOCK_BITS] >> n_bit) & 1 ? true : false;
		}

		template <typename T>
		void append(T v, size_type bits = sizeof(T) * 8)
		{
			if (bits == 0) return;
			if (bits > sizeof(v) * 8) return; // TODO: no padding supported
			block_type * p = reinterpret_cast<block_type *>(&v);
			size_type n_bits = bits % BLOCK_BITS; // incomplete blocks
			if (n_bits != 0) {
				append_block(*p, n_bits);
				bits -= n_bits;
			}
			for (; bits > 0; bits -= BLOCK_BITS, ++p) {
				append_block(*p);
			}
		}

		template <typename T>
		void set(T v, size_type ofs, size_type bits = sizeof(T) * 8)
		{
			if (bits == 0) return;
			if (bits > sizeof(v) * 8) return; // TODO: no padding supported
			if (ofs + bits > capacity()) extend(ofs + bits - capacity());
			block_type * p = reinterpret_cast<block_type *>(&v);
			size_type n_bits = bits % BLOCK_BITS; // incomplete block
			if (n_bits != 0) {
				set_block(*p, ofs, n_bits);
				ofs += n_bits;
				bits -= n_bits;
			}
			for (; bits > 0; bits -= BLOCK_BITS, ++p) {
				set_block(*p, ofs);
				ofs += BLOCK_BITS;
			}
		}

		template <typename T>
		void get(T & v, size_type ofs, size_type bits = sizeof(T) * 8)
		{
			if (bits == 0) return;
			if (bits > sizeof(T)) return; // impossible to read more bits than the specified container can hold
			if (ofs + bits > pos) return; // impossible to ream more than written

			size_type prefix_v_bits = sizeof(T) * 8 - bits; // bits within v to be left alone

			size_type u_bits = BLOCK_BITS - (ofs % BLOCK_BITS); // number of bits unused within the current block
			size_type q_bits = BLOCK_BITS - ((ofs + bits) % BLOCK_BITS); // number of bits unused within the last block

			// TODO
		}
};

int main(int, char **)
{
	bitset s;

	uint8_t v = 0;

	s.append(0, 16);
	s.set(0xff, 3, 8);

	s.get_block(v, 6, 5);

	printf("v = %02x\n", v);

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
	for (bitset::size_type i = 0; i < s.size(); ++i) {
		if (i % 8 == 0) printf(" ");
		printf("%d", s[i] ? 1 : 0);
	}
	printf("\n");
	printf("\n");

	return 0;
}

