#include "stdafx.h"
#include "ProcessorImp.h"
#include <iostream>
#include <algorithm>
#include "..\Interface\YapInterfaces.h"
#include <assert.h>

using namespace std;
using namespace Yap;

CProcessorImp::CProcessorImp()
{
}


CProcessorImp::~CProcessorImp()
{
}

IPortEnumerator * CProcessorImp::GetInputPortEnumerator()
{
	return &_input_ports;
}

IPortEnumerator * CProcessorImp::GetOutputPortEnumerator()
{
	return &_output_ports;
}

bool CProcessorImp::Init()
{
	return true;
}

bool CProcessorImp::Input(const wchar_t * port, IData * data)
{

	return true;
}


bool CProcessorImp::Link(const wchar_t * out_port, IProcessor* next_processor, const wchar_t * in_port)
{
	_links.insert(std::make_pair(out_port, Anchor(next_processor, in_port)));
	return true;
}

bool CProcessorImp::AddInputPort(const wchar_t * name, unsigned int dimensions, DataType data_type)
{
	try
	{
		return _input_ports.AddPort(new CPort(name, dimensions, data_type));
	}
	catch (std::bad_alloc&)
	{
		return false;
	}
}

bool CProcessorImp::AddOutputPort(const wchar_t * name, unsigned int dimensions, DataType data_type)
{
	try
	{
		return _output_ports.AddPort(new CPort(name, dimensions, data_type));
	}
	catch (std::bad_alloc&)
	{
		return false;
	}
}

void CProcessorImp::Feed(const wchar_t * out_port, IData * data)
{
	auto range = _links.equal_range(out_port);
	for (auto iter = range.first; iter != range.second; ++iter)
	{
		iter->second.processor->Input(iter->second.in_port.c_str(), data);
	}
}

bool CProcessorImp::AddProperty(PropertyType type,
	const wchar_t * name,
	const wchar_t * description)
{
	try
	{
		switch (type)
		{
		case PropertyBool:
			return _properties.AddProperty(new CBoolProperty(name, description));
		case PropertyInt:
			return _properties.AddProperty(new CIntProperty(name, description));
		case PropertyFloat:
			return _properties.AddProperty(new CFloatProperty(name, description));
		case PropertyString:
			return _properties.AddProperty(new CStringProperty(name, description));
		default:
			return false;
		}
	}
	catch (std::bad_alloc&)
	{
		return false;
	}
}

IPropertyEnumerator * CProcessorImp::GetProperties()
{
	return &_properties;
}

void CProcessorImp::SetIntProperty(const wchar_t * name, int value)
{
	assert(name != nullptr && name[0] != 0);

	auto property = _properties.GetProperty(name);
	if (property == nullptr)
		throw PropertyException(name, PropertyException::PropertyNotFound);

	if (property->GetType() != PropertyInt)
		throw PropertyException(name, PropertyException::TypeNotMatch);

	auto int_value = dynamic_cast<IIntValue*>(property);
	assert(int_value != nullptr);
	int_value->SetValue(value);
}

int CProcessorImp::GetIntProperty(const wchar_t * name)
{
	assert(name != nullptr && name[0] != 0);

	auto property = _properties.GetProperty(name);
	if (property == nullptr)
		throw PropertyException(name, PropertyException::PropertyNotFound);

	if (property->GetType() != PropertyInt)
		throw PropertyException(name, PropertyException::TypeNotMatch);

	auto int_value = dynamic_cast<IIntValue*>(property);
	assert(int_value != nullptr);

	return int_value->GetValue();
}

void CProcessorImp::SetFloatProperty(const wchar_t * name, double value)
{
	assert(name != nullptr && name[0] != 0);

	auto property = _properties.GetProperty(name);
	if (property == nullptr)
		throw PropertyException(name, PropertyException::PropertyNotFound);

	if (property->GetType() != PropertyFloat)
		throw PropertyException(name, PropertyException::TypeNotMatch);

	auto float_value = dynamic_cast<IFloatValue*>(property);
	assert(float_value != nullptr);
	float_value->SetValue(value);
}

double CProcessorImp::GetFloatProperty(const wchar_t * name)
{
	assert(name != nullptr && name[0] != 0);

	auto property = _properties.GetProperty(name);
	if (property == nullptr)
		throw PropertyException(name, PropertyException::PropertyNotFound);

	if (property->GetType() != PropertyFloat)
		throw PropertyException(name, PropertyException::TypeNotMatch);

	auto float_value = dynamic_cast<IFloatValue*>(property);
	assert(float_value != nullptr);

	return float_value->GetValue();
}

void CProcessorImp::SetBoolProperty(const wchar_t * name, bool value)
{
	assert(name != nullptr && name[0] != 0);

	auto property = _properties.GetProperty(name);
	if (property == nullptr)
		throw PropertyException(name, PropertyException::PropertyNotFound);

	if (property->GetType() != PropertyBool)
		throw PropertyException(name, PropertyException::TypeNotMatch);

	auto bool_value = dynamic_cast<IBoolValue*>(property);
	assert(bool_value != nullptr);
	bool_value->SetValue(value);
}

bool CProcessorImp::GetBoolProperty(const wchar_t * name)
{
	assert(name != nullptr && name[0] != 0);

	auto property = _properties.GetProperty(name);
	if (property == nullptr)
		throw PropertyException(name, PropertyException::PropertyNotFound);

	if (property->GetType() != PropertyBool)
		throw PropertyException(name, PropertyException::TypeNotMatch);

	auto bool_value = dynamic_cast<IBoolValue*>(property);
	assert(bool_value != nullptr);

	return bool_value->GetValue();
}

void CProcessorImp::SetStringProperty(const wchar_t * name, const wchar_t * value)
{
	assert(name != nullptr && name[0] != 0);

	auto property = _properties.GetProperty(name);
	if (property == nullptr)
		throw PropertyException(name, PropertyException::PropertyNotFound);

	if (property->GetType() != PropertyString)
		throw PropertyException(name, PropertyException::TypeNotMatch);

	auto string_value = dynamic_cast<IStringValue*>(property);
	assert(string_value != nullptr);
	string_value->SetValue(value);
}


const wchar_t * CProcessorImp::GetStringProperty(const wchar_t * name)
{
	assert(name != nullptr && name[0] != 0);

	auto property = _properties.GetProperty(name);
	if (property == nullptr)
		throw PropertyException(name, PropertyException::PropertyNotFound);

	if (property->GetType() != PropertyString)
		throw PropertyException(name, PropertyException::TypeNotMatch);

	auto string_value = dynamic_cast<IStringValue*>(property);
	assert(string_value != nullptr);
	return string_value->GetValue();
}

bool Yap::CProcessorImp::CanLink(IProcessor * source,
	const wchar_t * source_output_name,
	IProcessor * next,
	const wchar_t * next_input_name)
{
	NOT_IMPLEMENTED;
	return false;
}

wchar_t * Yap::CProcessorImp::GetId()
{
	throw std::logic_error("The method or operation is not implemented.");
}

bool Yap::CProcessorImp::LinkProperty(const wchar_t * property_id, const wchar_t * param_id)
{
	throw std::logic_error("The method or operation is not implemented.");
}

bool CPortEnumerator::AddPort(IPort* port)
{
	if (port == nullptr)
		return false;

	shared_ptr<IPort> p(port);
	_ports.insert(std::make_pair(port->GetName(), p));

	return true;
}

IPort * CPortEnumerator::GetFirstPort()
{
	_current_port = _ports.begin();
	if (_current_port != _ports.end())
	{
		return _current_port->second.get();
	}

	return nullptr;
}

IPort * CPortEnumerator::GetNextPort()
{
	return (_current_port == _ports.end() || ++_current_port == _ports.end()) ? 
		nullptr : _current_port->second.get();
}

Yap::IPort * Yap::CPortEnumerator::GetPort(const wchar_t * name)
{
	auto iter = _ports.find(name);
	return (iter != _ports.end()) ? iter->second.get() : nullptr;
}

CPort::CPort(const wchar_t * name, unsigned int dimensions, DataType data_type) : 
	_name(name), 
	_dimensions(dimensions), 
	_data_type(data_type)
{
}

const wchar_t * CPort::GetName()
{
	return _name.c_str();
}

unsigned int CPort::GetDimensions()
{
	return _dimensions;
}

DataType CPort::GetDataType()
{
	return _data_type;
}

IProperty * CPropertyEnumerator::GetFirst()
{
	if (_properties.empty())
		return nullptr;

	_current = _properties.begin();

	return _current->second;
}

IProperty * CPropertyEnumerator::GetNext()
{
	return (_current != _properties.end() && ++_current != _properties.end()) ? 
		_current->second : nullptr;
}

IProperty * CPropertyEnumerator::GetProperty(const wchar_t * name)
{
	auto iter = _properties.find(name);
	return (iter != _properties.end()) ? iter->second : nullptr;
}

Yap::CProperty::CProperty(PropertyType type, const wchar_t * name, const wchar_t * description) :
	_type(type),
	_name(name),
	_description(description)
{
}

const wchar_t * Yap::CProperty::GetName()
{
	return _name.c_str();
}

Yap::PropertyType Yap::CProperty::GetType()
{
	return _type;
}

const wchar_t * Yap::CProperty::GetDescription()
{
	return _description.c_str();
}
