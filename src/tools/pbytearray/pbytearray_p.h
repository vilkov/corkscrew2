#ifndef PBYTEARRAY_P_H_
#define PBYTEARRAY_P_H_

#include "pbytearray_imp.h"
#include <stdlib.h>
#include <string.h>
#include <new>


class DynamicByteArray : public PByteArrayImplementation
{
private:
	struct Data
	{
		unsigned int ref;
		size_type size;
		size_type capacity;
		value_type string[1];
	};
	enum { MaxNumberOfArgs = 10 };

public:
	enum { DefaultSize = 4 };

public:
	DynamicByteArray(size_type size = DefaultSize);
	DynamicByteArray(const value_type *string1, size_type size1, const value_type *string2, size_type size2);
	DynamicByteArray(const value_type *string, size_type size,
			const value_type *string1 = NULL,
			const value_type *string2 = NULL,
			const value_type *string3 = NULL,
			const value_type *string4 = NULL,
			const value_type *string5 = NULL,
			const value_type *string6 = NULL,
			const value_type *string7 = NULL,
			const value_type *string8 = NULL,
			const value_type *string9 = NULL,
			const value_type *string10 = NULL);
	virtual ~DynamicByteArray();

	virtual size_type size() const;
	virtual size_type capacity() const;
	virtual const value_type *c_str() const;
	virtual const value_type *data() const;
	virtual value_type *data();
	virtual void copyTo(void *dest) const;

	virtual void truncate(size_type pos);
	virtual void append(const value_type *string, size_type size);
	virtual void prepend(const value_type *string, size_type size);

private:
	DynamicByteArray(const DynamicByteArray &other);

private:
	Data *m_data;
};


class ReferenceByteArray : public PByteArrayImplementation
{
public:
	ReferenceByteArray(value_type *string, size_type size);

	virtual size_type size() const;
	virtual size_type capacity() const;
	virtual const value_type *c_str() const;
	virtual const value_type *data() const;
	virtual value_type *data();
	virtual void copyTo(void *dest) const;

	virtual void truncate(size_type pos);
	virtual void append(const value_type *string, size_type size);
	virtual void prepend(const value_type *string, size_type size);

private:
	value_type *m_string;
	size_type m_size;
};


class ConstReferenceByteArray : public PByteArrayImplementation
{
public:
	ConstReferenceByteArray(const value_type *string, size_type size);

	virtual size_type size() const;
	virtual size_type capacity() const;
	virtual const value_type *c_str() const;
	virtual const value_type *data() const;
	virtual value_type *data();
	virtual void copyTo(void *dest) const;

	virtual void truncate(size_type pos);
	virtual void append(const value_type *string, size_type size);
	virtual void prepend(const value_type *string, size_type size);

private:
	const value_type *m_string;
	size_type m_size;
};


class LiteralByteArray : public PByteArrayImplementation
{
public:
	LiteralByteArray(const value_type *string, size_type size);

	virtual size_type size() const;
	virtual size_type capacity() const;
	virtual const value_type *c_str() const;
	virtual const value_type *data() const;
	virtual value_type *data();
	virtual void copyTo(void *dest) const;

	virtual void truncate(size_type pos);
	virtual void append(const value_type *string, size_type size);
	virtual void prepend(const value_type *string, size_type size);

private:
	const value_type *m_string;
	size_type m_size;
};


class EmptyByteArray : public PByteArrayImplementation
{
public:
	EmptyByteArray();

	virtual size_type size() const;
	virtual size_type capacity() const;
	virtual const value_type *c_str() const;
	virtual const value_type *data() const;
	virtual value_type *data();
	virtual void copyTo(void *dest) const;

	virtual void truncate(size_type count);
	virtual void append(const value_type *string, size_type size);
	virtual void prepend(const value_type *string, size_type size);

private:
	value_type m_string[1];
};

#endif /* PBYTEARRAY_P_H_ */
