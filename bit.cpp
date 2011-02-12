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
			while (bits > capacity() - pos) {
				data.push_back(block_type());
			}
		}

		// number of bits must be between 0 and BLOCK_BITS
		void append_block(block_type v, size_type bits = BLOCK_BITS)
		{
			if (bits == 0) return;

			extend(bits);

			size_type i_block = pos / BLOCK_BITS; // index of current block
			size_type u_bits = BLOCK_BITS - (pos % BLOCK_BITS); // number of bits unused within the current block

			if (u_bits >= bits) {
				// enough room within current block
				data[i_block] |= v << (u_bits - bits);
			} else {
				// not enough room, split value to current and next block
				data[i_block+0] |= v >> (bits - u_bits);
				data[i_block+1] |= v << (BLOCK_BITS - (bits - u_bits));
			}
			pos += bits;
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
			return get(i);
		}

		bool get(size_type i) const
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
			if (ofs + bits > capacity()) extend(ofs + bits - capacity());

			// TODO
		}
};

int main(int, char **)
{
	bitset s;

	s.append(0xff, 8);
	s.append(0xff, 3);
	s.append(0, 5);
	s.append(0xffff, 16);
	s.append(1, 1);
	s.append(0, 1);
	s.append(1, 1);

	printf("\n %u / %u : ", s.size(), s.capacity());
	for (bitset::size_type i = 0; i < s.size(); ++i) {
		if (i % 8 == 0) printf(" ");
		printf("%d", s[i] ? 1 : 0);
	}
	printf("\n");
	printf("\n");

	return 0;
}

