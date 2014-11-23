#include <vector>

#include "base.hpp"

template<class T, class I=long>
class DenseGrid : BaseGrid<T, I>
{
private:
	std::vector<T> _content;

	size_t index(const Location<I>& loc) const
	{
		return (loc.row * this->num_columns()) + loc.column;
	}

	virtual T& unsafe_at(const Location<I>& loc) override
	{
		return _content[index(loc)];
	}

	virtual const T& unsafe_at(const Location<I>& loc) const override
	{
		return _content[index(loc)];
	}

public:
	DenseGrid(I num_rows, I num_columns, const T& fill = T()):
		BaseGrid<T, I>(num_rows, num_columns),
		_content(num_rows * num_columns, fill)
	{}

	virtual ~DenseGrid() {}
};
