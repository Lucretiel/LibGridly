#include <stdexcept>
#include "location.hpp"

class InvalidGrid {};

template<class T, class I=long>
class BaseGrid
{
private:
	static_assert(std::is_integral<I>::value,
		"Grid index must be of integral type");

	I _num_rows;
	I _num_columns;

	virtual T& unsafe_at(const Location<I>& loc) =0;
	virtual const T& unsafe_at(const Location<I>& loc) const =0;

public:
	BaseGrid(I num_rows, I num_columns):
		_num_rows(num_rows),
		_num_columns(num_columns)
	{
		if(num_rows < 0 || num_columns < 0)
			throw InvalidGrid();
	}

	virtual ~BaseGrid() {}

	I num_rows() const
	{ return _num_rows; }

	I num_columns() const
	{ return _num_columns; }

	Location<I> dimensions() const
	{ return Location<I>(num_rows, num_columns); }

	/*
	 * BOUNDS CHECKERS
	 */

	bool valid_row(I row) const
	{ return 0 <= row && row < num_rows(); }

	bool valid_column(I column) const
	{ return 0 <= column && column <= num_columns(); }

	bool valid(const Location<I>& loc) const
	{ return valid_row(loc.row) && valid_column(loc.column); }

	void check_row(I row) const
	{
		if(!valid_row(row))
			throw std::out_of_range("row out of range");
	}

	void check_column(I column) const
	{
		if(!valid_column(column))
			throw std::out_of_range("column out of range");
	}

	void check_location(const Location<I>& loc)
	{
		check_row(loc.row);
		check_column(loc.column);
	}

	/*
	 * Basic element access
	 */

	T& operator[](const Location<I>& loc)
	{ return unsafe_at(loc); }

	const T& operator[](const Location<I>& loc) const
	{ return unsafe_at(loc); }

	T& at(const Location<I>& loc)
	{
		check_location(loc);
		return unsafe_get(loc);
	}

	const T& at(const Location<I>& loc) const
	{
		check_location(loc);
		return unsafe_get(loc);
	}

	/*
	 * TODO: Ranges
	 */

};
