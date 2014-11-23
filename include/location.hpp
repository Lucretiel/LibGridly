#pragma once

#include <array>
#include <type_traits>

enum class Direction {up, down, left, right};

template<class T=long>
class Location
{
private:
	static_assert(std::is_integral<T>::value,
		"Location must be of integral type");

public:
	T row = 0;
	T column = 0;

	Location() {}

	Location(T row, T column):
		row(row),
		column(column)
	{}

	Location operator+(const Location& rhs) const
	{ return Location{row + rhs.row, column + rhs.column}; }

	Location operator-(const Location& rhs) const
	{ return Location{row - rhs.row, column - rhs.column}; }

	Location operator*(T factor) const
	{
		return Location{row * factor, column * factor};
	}

	Location above(T distance=1) const
	{ return Location{row - distance, column}; }

	Location below(T distance=1) const
	{ return Location{row + distance, column}; }

	Location left(T distance=1) const
	{ return Location{row, column - distance}; }

	Location right(T distance=1) const
	{ return Location{row, column + distance}; }

	Location relative(T rows, T columns)
	{ return Location{row + rows, column + columns}; }

	Location relative(Direction dir, T distance=1)
	{
		switch(dir)
		{
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

	std::array<Location, 4> adjacent() const
	{ return {above(), below(), left(), right()}; }

	std::array<Location, 4> diagonals() const
	{
		return {
			relative(1, 1),
			relative(-1, 1),
			relative(1, -1),
			relative(-1, -1)
		};
	}

	std::array<Location, 8> surrounding() const
	{
		return {
			relative(1, 1),
			relative(0, 1),
			relative(-1, 1),
			relative(-1, 0),
			relative(-1, -1),
			relative(0, -1),
			relative(1, -1),
			relative(1, 0)
		};
	}

	bool operator==(const Location& rhs) const
	{ return row == rhs.row && column == rhs.column; }

	bool operator!=(const Location& rhs) const
	{ return row != rhs.row || column != rhs.column; }

	#define CMP(OP) \
		bool operator OP (const Location& rhs) const \
		{ return (row OP rhs.row) ? true : \
			((row == rhs.row) && (column OP rhs.column)); }

	CMP(<)
	CMP(>)
	CMP(<=)
	CMP(>=)

	#undef CMP
};

typedef Location<long> Vector;

template<class U, class T>
Location<U> location_cast(const Location<T>& loc)
{
	return Location<U>{
		static_cast<U>(loc.row),
		static_cast<U>(loc.column)};
}
