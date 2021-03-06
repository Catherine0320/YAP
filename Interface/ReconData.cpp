#include "ReconData.h"

using namespace Yap;

/**
	获得相关数据使用的总的维数。
*/
unsigned int Yap::CDimensions::GetDimensionCount() 
{
	return static_cast<unsigned int>(_dimension_info.size());
}

/**
	获得指定的维度的信息。
	@param  dimension_index 指定希望获得其信息的维度的索引，从0开始。
	@param  dimension_type 输出参数，指定索引的类型（意义）。
	@param  start_index 相关的数据在这个维度上的起始位置。
	@param  length 相关的数据在这个维度上的宽度。
	@return
*/
bool Yap::CDimensions::GetDimensionInfo(unsigned int dimension_index, 
												  DimensionType & dimension_type, 
												  unsigned int& start_index, 
												  unsigned int& length)
{
	assert(dimension_index < _dimension_info.size());
	if (dimension_index >= _dimension_info.size())
	{
		return false;
	}

	dimension_type = _dimension_info[dimension_index].type;
	start_index = _dimension_info[dimension_index].start_index;
	length = _dimension_info[dimension_index].length;

	return true;
}

IDimensions * Yap::CDimensions::Clone()
{
	try
	{
		return new CDimensions(*this);
	}
	catch (std::bad_alloc&)
	{
		return nullptr;
	}
}

void Yap::CDimensions::Release()
{
	throw std::logic_error("The method or operation is not implemented.");
}

CDimensions & Yap::CDimensions::operator()(DimensionType type, unsigned int index, unsigned int length)
{
	assert(type != DimensionInvalid);

	_dimension_info.push_back(Dimension(type, index, length));

	return *this;
}

Yap::CDimensions::CDimensions()
{
}

Yap::CDimensions::CDimensions(const CDimensions& source)
{
	_dimension_info = source._dimension_info;
}

Yap::CDimensions::CDimensions(IDimensions * source)
{
	DimensionType type;
	unsigned int index, length;

	for (unsigned int i = 0; i < source->GetDimensionCount(); ++i)
	{
		source->GetDimensionInfo(i, type, index, length);
		_dimension_info.push_back(Dimension(type, index, length));
	}
}

Yap::CDimensions::CDimensions(unsigned int dimension_count)
{
	assert(dimension_count < 16);
	_dimension_info.resize(dimension_count);
}

bool Yap::CDimensions::SetDimensionInfo(unsigned int dimension_index, 
												  DimensionType dimension_type, 
												  unsigned int start_index, 
												  unsigned int length)
{
	assert(dimension_index < _dimension_info.size());
	_dimension_info[dimension_index].type = dimension_type; //type不变的应该用modify

	_dimension_info[dimension_index].start_index = start_index;
	_dimension_info[dimension_index].length = length;

	return true;
}

bool Yap::CDimensions::ModifyDimension(DimensionType type, 
												 unsigned int length,
												 unsigned int start_index)
{
	for (unsigned int i = 0; i < _dimension_info.size(); ++i)
	{
		if (_dimension_info[i].type == type)
		{
			_dimension_info[i].length = length;
			_dimension_info[i].start_index = start_index;
			return true;
		}
	}

	return false;
}

unsigned int Yap::CDimensions::GetLength(unsigned int dimension_index)
{
	assert(dimension_index < _dimension_info.size());

	return _dimension_info[dimension_index].length;
}

Yap::Dimension::Dimension(DimensionType type_, 
	unsigned start_index_, 
	unsigned int length_) :
	type(type_), start_index(start_index_), length(length_)
{
}

Yap::Dimension::Dimension() :
	start_index(0),
	length(0),
	type(DimensionInvalid)
{
}
