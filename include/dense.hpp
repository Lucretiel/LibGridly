#include <vector>

#include "base.hpp"

template<class T, class I=unsigned long>
class DenseGrid : public BaseGrid<T, I>
{
private:
	std::vector<T> _content;

	size_t index(const BaseLocation<I>& loc) const
	{
		return (loc.row * this->num_columns()) + loc.column;
	}

	virtual T& unsafe_at(const BaseLocation<I>& loc) override
	{
		return _content[index(loc)];
	}

	virtual const T& unsafe_at(const BaseLocation<I>& loc) const override
	{
		return _content[index(loc)];
	}

public:
	explicit DenseGrid(I num_rows, I num_columns, const T& fill = T()):
		BaseGrid<T, I>(num_rows, num_columns),
		_content(num_rows * num_columns, fill)
	{}

	explicit DenseGrid(BaseLocation<I> dimensions, const T& fill = T()):
		DenseGrid(dimensions.row, dimensions.column, fill)
	{}

	virtual ~DenseGrid() {}
};
