bitset
======

This is a dynamic bitset, supporting various operations.


Supported Operations
--------------------

Container operations:

    // tells the capacity of the current bitset in bits
    size_type capacity() const noexcept;

    // tells the current size of the bitset in bits
    size_type size() const noexcept;

    // reserves number of blocks
    void reserve(size_type blocks);

    // clears all bits from the bitset
    void clear();


Iterator operations:

	// const iterator
    const_iterator begin() const;
    const_iterator end() const;
    const_iterator cbegin() const;
    const_iterator cend() const;


Append to the bitset:

    // appends another bitset
    template <class U>
    void append(const bitset<U> & other);

    // appends a number of bits to the bitset
    template <class T>
    void append(T value, size_type bits);


Random access write:

    // sets another bitset at the specific position
    template <class U>
    void set(const bitset<U> & bs, size_type ofs);

    // sets a number of bits from the specified value at the specified position
    template <typename T>
    void set(T v, size_type ofs, size_type bits);

    // resets a bit at the specified position
    void reset(size_type index);

    // resets the entire bitset
    void reset() noexcept;

    // sets a bit value at the speicifed position
    void set_bit(size_type i, bool value)


Random access read:

    // reads a bit value at the specified position
    bool get_bit(size_type i) const

    // reads a bit value at the specified position
    bool test(size_type i) const;

    // returns the value at the specified position using the specified number of bits.
    // T must be an integral or enum type.
    template <class T>
    T get(size_type ofs, size_type bits) const;

    // reads into the value from the specified position using specified number of bits.
    template <class T>
    void get(T & value, size_type ofs, size_type bits) const;

    // returns the bit value at the specified position.
    bool operator[](size_type i) const;


Comparison:

    // comparison for equal and inequal. works with bitsets of different block type as well.
    bool operator==(const bitset & other) const;
    bool operator!=(const bitset & other) const;

    // comparison for less, less or equal, greater, greater or equal.
    bool operator<(const bitset & other) const;
    bool operator<=(const bitset & other) const;
    bool operator>(const bitset & other) const;
    bool operator>=(const bitset & other) const;


Arithmetic operations:

    // increment
    bitset & operator++();
    bitset operator++(int);

    // decrement
    bitset & operator--();
    bitset operator--(int);


Bit shift:

    // shift left number of bits
    bitset & shl(size_type bits);
    bitset & operator<<=(size_type bits);
    bitset operator<<(size_type bits) const;


Logic operations:

    // logical or
    bitset & operator|=(const bitset & other);
    bitset operator|(const bitset & other) const;

    // logical and
    bitset & operator&=(const bitset & other);
    bitset operator&(const bitset & other) const;

    // logical xor
    bitset & operator^=(const bitset & other);
    bitset operator^(const bitset & other) const;


Other functions:

    // flips a bit at the specified position.
    void flip(size_type i);

    // returns true if all bits are set.
    bool all() const noexcept;

    // returns true if any bits are set, overall or within the range.
    bool any() const noexcept;
    bool any(const_iterator first, const_iterator last) const noexcept;

    // returns true if no bits are set, overall or within the range.
    bool none() const noexcept;
    bool none(const_iterator first, const_iterator last) const noexcept;

    // returns the number of bits set, overall or within the range.
    size_type count() const noexcept;
    size_type count(const_iterator first, const_iterator last) const noexcept;


LICENSE
=======

See LICENSE

Status
======

[![Build Status](https://travis-ci.org/mariokonrad/bitset.svg?branch=master)](https://travis-ci.org/mariokonrad/bitset)

Build
=====

The source is completely as header file. If you like to build examples
and unit tests:
~~~~~~~~~~
	make
~~~~~~~~~~

