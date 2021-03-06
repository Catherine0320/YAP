#pragma once
#include "../Interface/YapInterfaces.h"

#include <map>
#include <string>
#include <memory>
#include "../Utilities/macros.h"
#include "../interface/reconData.h"

namespace Yap
{
	class CPort : public IPort
	{
	public:
		CPort(const wchar_t * name, unsigned int dimensions, DataType data_type);
		virtual const wchar_t * GetName() override;
		virtual unsigned int GetDimensions() override;
		virtual DataType GetDataType() override;

	protected:
		std::wstring _name;
		unsigned int _dimensions;
		DataType _data_type;

	};

	class CPortEnumerator : public IPortEnumerator
	{
	public:
		bool AddPort(IPort* port);;

		virtual IPort * GetFirstPort() override;
		virtual IPort * GetNextPort() override;

		virtual IPort * GetPort(const wchar_t * name) override;

	protected:
		std::map<std::wstring, std::shared_ptr<IPort>> _ports;
		std::map<std::wstring, std::shared_ptr<IPort>>::iterator _current_port;
	};

	struct Anchor
	{
		IProcessor * processor;
		std::wstring in_port;
		Anchor(IProcessor* processor_, const wchar_t * in_port_) : processor(processor_), in_port(in_port_) {}
	};


	class CProperty : public IProperty
	{
	public:
		CProperty(PropertyType type, const wchar_t * name, const wchar_t * description);
		virtual const wchar_t * GetName() override;

		virtual PropertyType GetType() override;

		virtual const wchar_t * GetDescription() override;
	private:
		std::wstring _name;
		std::wstring _description;
		PropertyType _type;
	};

	class CFloatProperty : public CProperty, public IFloatValue
	{
	public:
		CFloatProperty(const wchar_t * name, const wchar_t * description, double value = 0.0)
			: CProperty(PropertyFloat, name, description), _value(value) {}
		virtual double GetValue() { return _value; }
		virtual void SetValue(double value) { _value = value; }

	protected:
		double _value;
	};

	class CIntProperty : public CProperty, public IIntValue
	{
	public:
		CIntProperty(const wchar_t * name, const wchar_t * description, int value = 0)
			: CProperty(PropertyInt, name, description), _value(value) {}
		virtual int GetValue() { return _value; }
		virtual void SetValue(int value) { _value = value; }

	protected:
		int _value;
	};

	class CBoolProperty : public CProperty, public IBoolValue
	{
	public:
		CBoolProperty(const wchar_t * name, const wchar_t * description, bool value = 0)
			: CProperty(PropertyBool, name, description), _value(value) {}
		virtual bool GetValue() { return _value; }
		virtual void SetValue(bool value) { _value = value; }

	protected:
		bool _value;
	};

	class CStringProperty : public CProperty, public IStringValue
	{
	public:
		CStringProperty(const wchar_t * name, const wchar_t * description, const wchar_t * value = nullptr)
			: CProperty(PropertyString, name, description), _value(value) {}
		virtual const wchar_t * GetValue() { return _value.c_str(); }
		virtual void SetValue(const wchar_t * value) { _value = value; }

	protected:
		std::wstring _value;
	};

	class CPropertyEnumerator : public IPropertyEnumerator
	{
	public:
		bool AddProperty(IProperty * property) {
			if (property == nullptr)
				return false;

			if (_properties.find(property->GetName()) != _properties.end())
				return false;

			_properties.insert(std::make_pair(property->GetName(), property));
			return true;
		}

		virtual IProperty * GetFirst() override;
		virtual IProperty * GetNext() override;
		virtual IProperty * GetProperty(const wchar_t * name) override;
	protected:
		std::map<std::wstring, IProperty*> _properties;
		std::map<std::wstring, IProperty*>::iterator _current;

	};


	struct PropertyException
	{
		enum Type
		{
			PropertyNotFound,
			TypeNotMatch,
		};
		std::wstring property_name;
		Type type;
		PropertyException(const wchar_t * name, Type type_) : property_name(name), type(type_) {}
	};

	class CProcessorImp :
		public IProcessor
	{
	public:
		CProcessorImp();
		virtual ~CProcessorImp();

		virtual IPortEnumerator * GetInputPortEnumerator() override;
		virtual IPortEnumerator * GetOutputPortEnumerator() override;

		virtual bool Init() override;
		virtual bool Input(const wchar_t * port, IData * data) override;
		virtual bool Link(const wchar_t * output, IProcessor * next, const wchar_t * next_input) override;

		bool AddInputPort(const wchar_t * name, unsigned int dimensions, DataType data_type);
		bool AddOutputPort(const wchar_t * name, unsigned int dimensions, DataType data_type);

		void Feed(const wchar_t * name, IData * data);

	protected:
		bool CanLink(IProcessor * source, const wchar_t * source_output_name, IProcessor * next, const wchar_t * next_input_name);

		bool AddProperty(PropertyType type, const wchar_t * name, const wchar_t * description);
		void SetIntProperty(const wchar_t * name, int value);
		int GetIntProperty(const wchar_t * name);
		void SetFloatProperty(const wchar_t * name, double value);
		double GetFloatProperty(const wchar_t * name);
		void SetBoolProperty(const wchar_t * name, bool value);
		bool GetBoolProperty(const wchar_t * name);
		void SetStringProperty(const wchar_t * name, const wchar_t * value);
		const wchar_t * GetStringProperty(const wchar_t * name);

		virtual IPropertyEnumerator * GetProperties() override;

		virtual wchar_t * GetId() override;

		virtual bool LinkProperty(const wchar_t * property_id, const wchar_t * param_id) override;

		CPortEnumerator _input_ports;
		CPortEnumerator _output_ports;
		CPropertyEnumerator _properties;

		std::multimap<std::wstring, Anchor> _links;
	};
};

