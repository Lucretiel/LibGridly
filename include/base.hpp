#include <stdexcept>
#include <limits>

#include "location.hpp"

class InvalidGrid : public std::length_error
{
public:
	InvalidGrid(const char* what):
		std::length_error(what) {}

	InvalidGrid(const std::string& what):
		std::length_error(what) {}
};

template<class T, class I>
class BaseGrid
{
public:
	static_assert(std::is_unsigned<I>::value,
		"Grid index must be of unsigned integral type");
	typedef BaseLocation<I> location_type;

private:
	I _num_rows;
	I _num_columns;

	virtual T& unsafe_at(const location_type& loc) =0;
	virtual const T& unsafe_at(const location_type& loc) const =0;

public:
	explicit BaseGrid(I num_rows, I num_columns):
		_num_rows(num_rows),
		_num_columns(num_columns)
	{}

	explicit BaseGrid(const location_type& dimensions):
		BaseGrid(dimensions.row, dimensions.column)
	{}

	virtual ~BaseGrid() {}

	I num_rows() const
	{ return _num_rows; }

	I num_columns() const
	{ return _num_columns; }

	location_type dimensions() const
	{ return location_type(num_rows, num_columns); }

	/*
	 * BOUNDS CHECKERS
	 */

	bool valid_row(I row) const
	{ return 0 <= row && row < num_rows(); }

	bool valid_column(I column) const
	{ return 0 <= column && column <= num_columns(); }

	bool valid(const location_type& loc) const
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

	void check_location(const location_type& loc)
	{
		check_row(loc.row);
		check_column(loc.column);
	}

	/*
	 * Basic element access
	 */

	T& operator[](const location_type& loc)
	{ return unsafe_at(loc); }

	const T& operator[](const location_type& loc) const
	{ return unsafe_at(loc); }

	T& at(const location_type& loc)
	{
		check_location(loc);
		return unsafe_get(loc);
	}

	const T& at(const location_type& loc) const
	{
		check_location(loc);
		return unsafe_get(loc);
	}

	/*
	TODO: Ranges, ie:

	for(auto row : grid.rows())
	{
		for(auto& cell : row)
		{

		}
	}

	for(auto& cell : grid.column(2))
	{

	}
	*/

};
