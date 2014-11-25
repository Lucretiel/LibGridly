#pragma once

#include <array>
#include <type_traits>
#include <functional>

enum class Direction {none, up, down, left, right};

inline static constexpr Direction reverse_dir(const Direction dir)
{
	return dir == Direction::up ? Direction::down :
		dir == Direction:: down ? Direction::up :
		dir == Direction::left ? Direction::right :
		dir == Direction::right ? Direction::left :
		Direction::none;
}

#define ASSERT_INT(TYPE, THING) static_assert(std::is_integral<TYPE>::value, THING " must be of integral type");

template<class T>
class BaseLocation
{
private:
	ASSERT_INT(T, "BaseLocation")

public:
	T row = 0;
	T column = 0;

	BaseLocation() {}

	BaseLocation(T row, T column):
		row(row),
		column(column)
	{}

	template<class U>
	explicit operator BaseLocation<U>() const
	{ return BaseLocation<T>{static_cast<U>(row), static_cast<U>(column)}; }

	template<class U>
	BaseLocation operator+(const BaseLocation<U>& rhs) const
	{ return BaseLocation{row + rhs.row, column + rhs.column}; }

	template<class U>
	BaseLocation operator-(const BaseLocation<U>& rhs) const
	{ return BaseLocation{row - rhs.row, column - rhs.column}; }

	template<class U>
	BaseLocation operator*(U factor) const
	{
		ASSERT_INT(U, "factor");
		return BaseLocation{row * factor, column * factor};
	}

	template<class U=T>
	BaseLocation above(U distance=1) const
	{
		ASSERT_INT(U, "distance");
		return BaseLocation{row - distance, column};
	}

	template<class U=T>
	BaseLocation below(U distance=1) const
	{
		ASSERT_INT(U, "distance");
		return BaseLocation{row + distance, column};
	}

	template<class U=T>
	BaseLocation left(U distance=1) const
	{
		ASSERT_INT(U, "distance");
		return BaseLocation{row, column - distance};
	}

	template<class U=T>
	BaseLocation right(U distance=1) const
	{
		ASSERT_INT(U, "distance");
		return BaseLocation{row, column + distance};
	}

	template<class U>
	BaseLocation relative(U rows, U columns) const
	{
		ASSERT_INT(U, "relative movement")
		return BaseLocation{row + rows, column + columns};
	}

	template<class U=T>
	BaseLocation relative(Direction dir, U distance=1) const
	{
		switch(dir)
		{
		case Direction::none:
			return *this;
		case Direction::up:
			return above(distance);
		case Direction::down:
			return below(distance);
		case Direction::left:
			return left(distance);
		case Direction::right:
			return right(distance);
		}
	}

	std::array<BaseLocation, 4> adjacent() const
	{ return {{above(), below(), left(), right()}}; }

	std::array<BaseLocation, 4> diagonals() const
	{
		return {{
			relative(1, 1),
			relative(-1, 1),
			relative(1, -1),
			relative(-1, -1)
		}};
	}

	std::array<BaseLocation, 8> surrounding() const
	{
		return {{
			relative(1, 1),
			relative(0, 1),
			relative(-1, 1),
			relative(-1, 0),
			relative(-1, -1),
			relative(0, -1),
			relative(1, -1),
			relative(1, 0)
		}};
	}

	template<class U>
	bool operator==(const BaseLocation<U>& rhs) const
	{ return row == rhs.row && column == rhs.column; }

	template<class U>
	bool operator!=(const BaseLocation<U>& rhs) const
	{ return row != rhs.row || column != rhs.column; }

	#define CMP(OP) \
	template<class U> bool operator OP (const BaseLocation<U>& rhs) const \
	{ return (row OP rhs.row) || ((row == rhs.row) && (column OP rhs.column)); }

	CMP(<)
	CMP(>)
	CMP(<=)
	CMP(>=)

	#undef CMP
};

#undef ASSERT_INT

typedef BaseLocation<unsigned long> Location;
typedef BaseLocation<long> Movement;

// Hash implementation
namespace std
{
	template <class T>
	struct hash<BaseLocation<T>>
	{
	private:
		const static std::hash<T> t_hash;

		// From boost::hash
		inline size_t hash_combine(const size_t seed, const T& t)
		{ return seed ^ (t_hash(t) + 0x9e3779b9 + (seed << 6) + (seed >> 2)); }

	public:
		size_t operator()(const BaseLocation<T>& loc) const
		{ return hash_combine(hash_combine(0, loc.row), loc.column); }
	};
};
