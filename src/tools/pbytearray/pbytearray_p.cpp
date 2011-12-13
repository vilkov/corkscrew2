#include "pbytearray_p.h"


DynamicByteArray::DynamicByteArray(size_type size) :
	m_data(static_cast<Data*>(malloc(sizeof(Data) + size)))
{
	if (m_data)
	{
		m_data->ref = 1;
		m_data->size = 0;
		m_data->capacity = size;
		m_data->string[0] = 0;
	}
	else
		new (this) EmptyByteArray();
}

DynamicByteArray::DynamicByteArray(const value_type *string1, size_type size1, const value_type *string2, size_type size2) :
	m_data(static_cast<Data*>(malloc(sizeof(Data) + size1 + size2)))
{
	if (m_data)
	{
		m_data->ref = 1;
		m_data->capacity = 0;
		memcpy(m_data->string, string1, size1);
		memcpy(m_data->string + size1, string2, size2);
		m_data->string[m_data->size = size1 + size2] = 0;
	}
	else
		new (this) EmptyByteArray();
}

DynamicByteArray::DynamicByteArray(const value_type *string, size_type size,
		const value_type *string1,
		const value_type *string2,
		const value_type *string3,
		const value_type *string4,
		const value_type *string5,
		const value_type *string6,
		const value_type *string7,
		const value_type *string8,
		const value_type *string9,
		const value_type *string10)
{
	struct { const value_type *string; size_type size; } strings[MaxNumberOfArgs] =
	{
			{ string1,  string1  ? strlen(string1)  : 0 },
			{ string2,  string2  ? strlen(string2)  : 0 },
			{ string3,  string3  ? strlen(string3)  : 0 },
			{ string4,  string4  ? strlen(string4)  : 0 },
			{ string5,  string5  ? strlen(string5)  : 0 },
			{ string6,  string6  ? strlen(string6)  : 0 },
			{ string7,  string7  ? strlen(string7)  : 0 },
			{ string8,  string8  ? strlen(string8)  : 0 },
			{ string9,  string9  ? strlen(string9)  : 0 },
			{ string10, string10 ? strlen(string10) : 0 },
	};
	size_type totalSize = size;

	for (unsigned char i = 0; i < MaxNumberOfArgs; ++i)
		totalSize += strings[i].size;

	if (m_data = static_cast<Data*>(malloc(sizeof(Data) + totalSize)))
	{
		m_data->ref = 1;
		m_data->size = size;
		m_data->capacity = 0;
		memcpy(m_data->string, string, size);

		for (unsigned char i = 0; i < MaxNumberOfArgs; ++i)
			if (strings[i].size)
			{
				memcpy(m_data->string + m_data->size, strings[i].string, strings[i].size);
				m_data->size += strings[i].size;
			}

		m_data->string[m_data->size] = 0;
	}
	else
		new (this) EmptyByteArray();
}

DynamicByteArray::~DynamicByteArray()
{
	if ((--m_data->ref) == 0)
		free(m_data);
}

DynamicByteArray::size_type DynamicByteArray::size() const
{
	return m_data->size;
}

DynamicByteArray::size_type DynamicByteArray::capacity() const
{
	return m_data->capacity;
}

const DynamicByteArray::value_type *DynamicByteArray::c_str() const
{
	return m_data->string;
}

const DynamicByteArray::value_type *DynamicByteArray::data() const
{
	return m_data->string;
}

DynamicByteArray::value_type *DynamicByteArray::data()
{
	if (m_data->ref > 1)
	{
		this->~DynamicByteArray();
		return (new (this) DynamicByteArray(m_data->string, m_data->size))->data();
	}
	else
		return m_data->string;
}

void DynamicByteArray::copyTo(void *dest) const
{
	new (dest) DynamicByteArray(*this);
}

void DynamicByteArray::truncate(size_type pos)
{
	if (m_data->size == 0)
		if (pos < m_data->capacity)
		{
			m_data->capacity -= pos;
			m_data->string[m_data->size = pos] = 0;
		}
		else
		{
			m_data->size = m_data->capacity;
			m_data->capacity = 0;
			m_data->string[m_data->size] = 0;
		}
	else
		if (pos < m_data->size)
		{
			m_data->capacity += m_data->size - pos;
			m_data->string[m_data->size = pos] = 0;
		}
		else
			if (pos < m_data->size + m_data->capacity)
			{
				m_data->capacity = m_data->size + m_data->capacity - pos;
				m_data->string[m_data->size = pos] = 0;
			}
}

void DynamicByteArray::append(const value_type *string, size_type size)
{
	if (m_data->ref > 1)
	{
		this->~DynamicByteArray();
		new (this) DynamicByteArray(m_data->string, m_data->size, string, size);
	}
	else
		if (m_data->capacity > size)
		{
			m_data->capacity -= size;
			memcpy(m_data->string + m_data->size, string, size);
			m_data->string[m_data->size += size] = 0;
		}
		else
		{
			Data *newData = static_cast<Data*>(realloc(m_data, sizeof(Data) + m_data->size + size));

			if (newData != NULL)
			{
				m_data = newData;
				m_data->capacity = 0;
				memcpy(m_data->string + m_data->size, string, size);
				m_data->string[m_data->size += size] = 0;
			}
		}
}

void DynamicByteArray::prepend(const value_type *string, size_type size)
{
	if (m_data->ref > 1)
	{
		this->~DynamicByteArray();
		new (this) DynamicByteArray(string, size, m_data->string, m_data->size);
	}
	else
		if (m_data->capacity > size)
		{
			m_data->capacity -= size;
			memmove(m_data->string + size, m_data->string, m_data->size);
			memcpy(m_data->string, string, size);
			m_data->string[m_data->size += size] = 0;
		}
		else
		{
			Data *newData = static_cast<Data*>(realloc(m_data, sizeof(Data) + m_data->size + size));

			if (newData != NULL)
			{
				m_data = newData;
				m_data->capacity = 0;
				memmove(m_data->string + size, m_data->string, m_data->size);
				memcpy(m_data->string, string, size);
				m_data->string[m_data->size += size] = 0;
			}
		}
}

DynamicByteArray::DynamicByteArray(const DynamicByteArray &other) :
	m_data(other.m_data)
{
	++m_data->ref;
}


ReferenceByteArray::ReferenceByteArray(value_type *string, size_type size) :
	m_string(string),
	m_size(size)
{}

ReferenceByteArray::size_type ReferenceByteArray::size() const
{
	return m_size;
}

ReferenceByteArray::size_type ReferenceByteArray::capacity() const
{
	return 0;
}

const ReferenceByteArray::value_type *ReferenceByteArray::c_str() const
{
	return m_string;
}

const ReferenceByteArray::value_type *ReferenceByteArray::data() const
{
	return m_string;
}

ReferenceByteArray::value_type *ReferenceByteArray::data()
{
	return m_string;
}

void ReferenceByteArray::copyTo(void *dest) const
{
	new (dest) ReferenceByteArray(*this);
}

void ReferenceByteArray::truncate(size_type pos)
{
	if (pos < m_size)
	{
		m_size = pos;
		m_string[m_size] = 0;
	}
}

void ReferenceByteArray::append(const value_type *string, size_type size)
{
	this->~ReferenceByteArray();
	new (this) DynamicByteArray(m_string, m_size, string, size);
}

void ReferenceByteArray::prepend(const value_type *string, size_type size)
{
	this->~ReferenceByteArray();
	new (this) DynamicByteArray(string, size, m_string, m_size);
}


ConstReferenceByteArray::ConstReferenceByteArray(const value_type *string, size_type size) :
	m_string(string),
	m_size(size)
{}

ConstReferenceByteArray::size_type ConstReferenceByteArray::size() const
{
	return m_size;
}

ConstReferenceByteArray::size_type ConstReferenceByteArray::capacity() const
{
	return 0;
}

const ConstReferenceByteArray::value_type *ConstReferenceByteArray::c_str() const
{
	this->~ConstReferenceByteArray();
	return (new (const_cast<ConstReferenceByteArray*>(this)) DynamicByteArray(m_string, m_size))->c_str();
}

const ConstReferenceByteArray::value_type *ConstReferenceByteArray::data() const
{
	return m_string;
}

ConstReferenceByteArray::value_type *ConstReferenceByteArray::data()
{
	this->~ConstReferenceByteArray();
	return (new (this) DynamicByteArray(m_string, m_size))->data();
}

void ConstReferenceByteArray::copyTo(void *dest) const
{
	new (dest) ConstReferenceByteArray(*this);
}

void ConstReferenceByteArray::truncate(size_type pos)
{
	if (pos < m_size)
	{
		this->~ConstReferenceByteArray();
		new (this) DynamicByteArray(m_string, pos);
	}
}

void ConstReferenceByteArray::append(const value_type *string, size_type size)
{
	this->~ConstReferenceByteArray();
	new (this) DynamicByteArray(m_string, m_size, string, size);
}

void ConstReferenceByteArray::prepend(const value_type *string, size_type size)
{
	this->~ConstReferenceByteArray();
	new (this) DynamicByteArray(string, size, m_string, m_size);
}


LiteralByteArray::LiteralByteArray(const value_type *string, size_type size) :
	m_string(string),
	m_size(size)
{}

LiteralByteArray::size_type LiteralByteArray::size() const
{
	return m_size;
}

LiteralByteArray::size_type LiteralByteArray::capacity() const
{
	return 0;
}

const LiteralByteArray::value_type *LiteralByteArray::c_str() const
{
	return m_string;
}

const LiteralByteArray::value_type *LiteralByteArray::data() const
{
	return m_string;
}

LiteralByteArray::value_type *LiteralByteArray::data()
{
	this->~LiteralByteArray();
	return (new (this) DynamicByteArray(m_string, m_size))->data();
}

void LiteralByteArray::copyTo(void *dest) const
{
	new (dest) LiteralByteArray(*this);
}

void LiteralByteArray::truncate(size_type pos)
{
	if (pos < m_size)
	{
		this->~LiteralByteArray();
		new (this) DynamicByteArray(m_string, pos);
	}
}

void LiteralByteArray::append(const value_type *string, size_type size)
{
	this->~LiteralByteArray();
	new (this) DynamicByteArray(m_string, m_size, string, size);
}

void LiteralByteArray::prepend(const value_type *string, size_type size)
{
	this->~LiteralByteArray();
	new (this) DynamicByteArray(string, size, m_string, m_size);
}


EmptyByteArray::EmptyByteArray()
{
	m_string[0] = 0;
}

EmptyByteArray::size_type EmptyByteArray::size() const
{
	return 0;
}

EmptyByteArray::size_type EmptyByteArray::capacity() const
{
	return 0;
}

const EmptyByteArray::value_type *EmptyByteArray::c_str() const
{
	return m_string;
}

const EmptyByteArray::value_type *EmptyByteArray::data() const
{
	return m_string;
}

EmptyByteArray::value_type *EmptyByteArray::data()
{
	this->~EmptyByteArray();
	return (new (this) DynamicByteArray())->data();
}

void EmptyByteArray::copyTo(void *dest) const
{
	new (dest) EmptyByteArray(*this);
}

void EmptyByteArray::truncate(size_type count)
{

}

void EmptyByteArray::append(const value_type *string, size_type size)
{
	this->~EmptyByteArray();
	new (this) DynamicByteArray(string, size);
}

void EmptyByteArray::prepend(const value_type *string, size_type size)
{
	this->~EmptyByteArray();
	new (this) DynamicByteArray(string, size);
}
