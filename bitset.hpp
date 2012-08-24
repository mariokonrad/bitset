#ifndef __BITSET__HPP__
#define __BITSET__HPP__

#include <vector>

/// @TODO: support for iterator
/// @TODO: support for const_iterator (partially prepared)
/// @TODO: support for ranges:
///        - copy_constructor(const_iterator begin_inclusive, const_iterator end_exclusive)
///        - append(bitset)
///        - set(bitset)
/// @TODO: test also for big-endian
/// @TODO: padding for 'append' and 'set'
/// @TODO: really ignoring the failure of 'set', 'append' and 'get'?
/// @TODO: documentation
template <typename Block, class Container = std::vector<Block> > class bitset
{
	public:
		typedef Block block_type;
		enum { BITS_PER_BYTE = 8 };
		enum { BITS_PER_BLOCK = sizeof(block_type) * BITS_PER_BYTE };
	public:
		typedef typename Container::size_type size_type;
		typedef typename Container::const_iterator data_const_iterator;

		class const_iterator
		{
				friend class bitset;
			private:
				const bitset * const bs;
				size_type pos;

				const_iterator(const bitset * const bs)
					: bs(bs)
					, pos(0)
				{}
			public:
				const_iterator()
					: bs(NULL)
					, pos(0)
				{}

				// TODO
		};
	private:
		Container data;
		size_type pos; // number of bits contained within the set
	private:

		/// Extends the container by the specified number of bits.
		/// Extension is always one block.
		void extend(size_type bits)
		{
			if (bits <= 0) return;
			size_type n_blocks = (pos + bits + BITS_PER_BLOCK - 1) / BITS_PER_BLOCK;
			if (n_blocks > data.capacity()) {
				data.reserve(n_blocks);
				while (bits > capacity() - pos) {
					data.push_back(block_type());
				}
			}
		}

		/// Appends the specified block to the data. The bitset is automatically extended
		/// to hold all the data.
		///
		/// @param[in] v The data to append to the bitset.
		/// @param[in] bits The number of bits to append. This value must be
		///            between 0 and BITS_PER_BLOCK. If not all bits are being
		///            appended, only the least significant bits are being taken.
		void append_block(block_type v, size_type bits = BITS_PER_BLOCK)
		{
			if (bits <= 0) return;
			extend(bits);
			size_type i = pos / BITS_PER_BLOCK; // index of current block
			size_type u_bits = BITS_PER_BLOCK - (pos % BITS_PER_BLOCK); // number of bits unused within the current block
			if (u_bits >= bits) {
				// enough room within current block
				data[i] |= v << (u_bits - bits);
			} else {
				// not enough room, split value to current and next block
				data[i+0] |= v >> (bits - u_bits);
				data[i+1] |= v << (BITS_PER_BLOCK - (bits - u_bits));
			}
			pos += bits;
		}

		/// Sets the specified block within the bit set. The bitset is automatically
		/// extended to hold the data.
		///
		/// @param[in] v The data to be set.
		/// @param[in] ofs The offset in bits at which the data has to be written.
		/// @param[in] bits The number of bits of the data to be written
		///            If not all bits are being written, only the least significant bits are being taken.
		void set_block(block_type v, size_type ofs, size_type bits = BITS_PER_BLOCK)
		{
			if (bits <= 0) return;
			if (ofs + bits > capacity()) extend(ofs + bits - capacity());
			size_type i = ofs / BITS_PER_BLOCK; // index of current block
			size_type u_bits = BITS_PER_BLOCK - (ofs % BITS_PER_BLOCK); // number of bits unused within the current block
			if (u_bits >= bits) {
				// enough room within current block
				block_type mask = ~((1 << (u_bits - bits)) - 1);
				data[i] = (data[i] & mask) | v << (u_bits - bits);
			} else {
				// not enough room, split value to current and next block
				block_type mask0 = ~((1 << (bits - u_bits)) - 1);
				block_type mask1 = (1 << (BITS_PER_BLOCK - (bits - u_bits))) - 1;

				data[i+0] = (data[i+0] & mask0) | v >> (bits - u_bits);
				data[i+1] = (data[i+1] & mask1) | v << (BITS_PER_BLOCK - (bits - u_bits));
			}
			if (ofs + bits > pos) pos = ofs + bits;
		}

		/// Reads a block from the bit set.
		///
		/// @param[out] v The container to hold the data.
		/// @param[in] ofs The offset in bits at which the data has to be read.
		/// @param[in] bits Number of bits to be read.
		///            If the number of bits is smaller than what the specified data can
		///            hold, only the least significant bits are being set.
		void get_block(block_type & v, size_type ofs, size_type bits = BITS_PER_BLOCK)
		{
			if (bits <= 0) return;
			if (ofs + bits > size()) return;
			size_type i = ofs / BITS_PER_BLOCK; // index of current block
			size_type u_bits = BITS_PER_BLOCK - (ofs % BITS_PER_BLOCK); // number of bits unused within the current block
			if (u_bits >= bits) {
				// desired data fully within the current block
				block_type mask = (1 << u_bits) - 1;
				v = (data[i] & mask) >> (u_bits - bits);
			} else {
				// desired value is part from current block and part from next
				block_type mask0 = (1 << u_bits) - 1;
				block_type mask1 = ((1 << (BITS_PER_BLOCK - (bits - u_bits))) - 1) << (bits - u_bits);
				v = (data[i+0] & mask0) << (bits - u_bits) | (data[i+1] & mask1) >> (BITS_PER_BLOCK - (bits - u_bits));
			}
		}
	public:
		bitset()
			: pos(0)
		{}

		/// Returns the capacity of this bit set. Note: not all bits must have
		/// been occupied.
		size_type capacity() const
		{
			return data.size() * BITS_PER_BLOCK;
		}

		/// Returns the number of used bits.
		size_type size() const
		{
			return pos;
		}

		/// Reserves the number of bits within this set.
		void reserve(size_type bits)
		{
			extend(bits);
		}

		/// Returns the bit at the specified position.
		bool operator[](size_type i) const
		{
			return get_bit(i);
		}

		/// Returns the bit at the specified position. If the index is larger
		/// than the actual number of bits, 'false' will rturn.
		bool get_bit(size_type i) const
		{
			if (i > size()) return false;
			size_type n_bit = BITS_PER_BLOCK - (i % BITS_PER_BLOCK) - 1; // bit within the block to be read
			return (data[i / BITS_PER_BLOCK] >> n_bit) & 1 ? true : false;
		}

		/// Returns a const iterator to the beginning of the data itself.
		/// Note: this iterator accesses the data up to capacity(), some bits
		/// may be unused at the end of the set.
		data_const_iterator data_begin() const
		{
			return data.begin();
		}

		/// Returns a const iterator to the end of the data itself.
		data_const_iterator data_end() const
		{
			return data.end();
		}

		/// Appends the lowest significant bits of the specified data to the
		/// bit set. The set will be extended if necessary.
		/// The second parameter specifies the number of bits to be used from
		/// the given data, beginning at the lowest signnificant bit.
		/// A size of 0 bits will have no effect.
		///
		/// @param[in] v The value to append to the bitset.
		/// @param[in] bits Number of bits from the specified value. This must not
		///            exceed the number of bits provided by the specified data,
		///            padding is not supported.
		template <typename T> void append(T v, size_type bits = sizeof(T) * BITS_PER_BYTE)
		{
			if (bits <= 0) return;
			if (bits > sizeof(v) * BITS_PER_BYTE) return; // TODO: no padding supported
			block_type * p = reinterpret_cast<block_type *>(&v);
			size_type n_bits = bits % BITS_PER_BLOCK; // incomplete blocks
			if (n_bits != 0) {
				append_block(*p, n_bits);
				bits -= n_bits;
			}
			for (; bits > 0; bits -= BITS_PER_BLOCK, ++p) {
				append_block(*p);
			}
		}

		/// Sets bits within the set. The bitset is automatically exteneded to hold the data.
		///
		/// @param[in] v The value to set.
		/// @param[in] ofs The offset (in bits) at which position the value has to be written.
		/// @paran[in] bits The number of bits to write. This must not exceed the number of bits
		///            provided by the specified data, padding is not supported.
		template <typename T> void set(T v, size_type ofs, size_type bits = sizeof(T) * BITS_PER_BYTE)
		{
			if (bits <= 0) return;
			if (bits > sizeof(v) * BITS_PER_BYTE) return; // TODO: no padding supported
			if (ofs + bits > capacity()) extend(ofs + bits - capacity());
			block_type * p = reinterpret_cast<block_type *>(&v);
			size_type n_bits = bits % BITS_PER_BLOCK; // incomplete block
			if (n_bits != 0) {
				set_block(*p, ofs, n_bits);
				ofs += n_bits;
				bits -= n_bits;
			}
			for (; bits > 0; bits -= BITS_PER_BLOCK, ++p) {
				set_block(*p, ofs);
				ofs += BITS_PER_BLOCK;
			}
		}

		/// Reads data from the bit set. There must be enough capacity in either the
		/// bitset to be read as well as the provided data type to contain the desired
		/// number of bits.
		///
		/// @param[out] v The container to hold the data.
		/// @param[in] ofs The offset in bits at which position the data is to be read.
		/// @param[in] bits Number of bits to be read. This must not exceeed the number of
		///            bits the specified data type can hold.
		///            If the number of bits is smaller than what the specified data can
		///            hold, only the least significant bits are being set.
		template <typename T> void get(T & v, size_type ofs, size_type bits = sizeof(T) * BITS_PER_BYTE)
		{
			if (bits <= 0) return;
			if (bits > sizeof(T) * BITS_PER_BYTE) return; // impossible to read more bits than the specified container can hold
			if (ofs + bits > pos) return; // impossible to read more than available

			v = T(); // clear result

			size_type u_bits = BITS_PER_BLOCK - (ofs % BITS_PER_BLOCK); // number of bits unused within the current block
			size_type q_bits = (ofs + bits) % BITS_PER_BLOCK; // number of bits unused within the last block

			block_type block;

			if (u_bits > 0) {
				get_block(block, ofs, u_bits);
				v =+ block;
				ofs += u_bits;
				bits -= u_bits;
			}

			for (; bits > q_bits; bits -= BITS_PER_BLOCK) {
				get_block(block, ofs);
				v <<= BITS_PER_BLOCK;
				v += block;
				ofs += BITS_PER_BLOCK;
			}

			if (q_bits > 0) {
				get_block(block, ofs, q_bits);
				v <<= q_bits;
				v += block;
			}
		}
};


#endif
