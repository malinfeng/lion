#include "Buffer.h"
#include <assert.h>

using namespace lion;

Buffer::Buffer(unsigned int bufferSize)
	:
	_buffer(defaultPreSize + bufferSize),
	_readIdx(defaultPreSize),
	_writeIdx(defaultPreSize)
{
	assert(bufferSize > 0);
}

Buffer::~Buffer()
{
}


void Buffer::swap(Buffer& changedbf)
{
	_buffer.swap(changedbf._buffer);
	std::swap(_readIdx, changedbf._readIdx);
	std::swap(_writeIdx, changedbf._writeIdx);
}


void Buffer::append(const char* data, unsigned int len)
{
	if (len > writeableBytes() + preAppendableBytes() - defaultPreSize)
	{
		//扩容
		_buffer.resize(2 * _buffer.size() - defaultPreSize);
		moveDataToHead();
		append(data, len);
	}
	else if (len > writeableBytes())
	{
		moveDataToHead();
		append(data, len);
	}
	else
	{
		std::copy(data, data + len, writeBegin());
		_writeIdx += len;
	}
}

bool Buffer::preappend(const char* data, unsigned int len)
{
	//尽量不移动数据，使得Buffer高效
	if (len > preAppendableBytes())
	{
		return false;
	}
	_readIdx -= len;
	std::copy(data, data + len, _buffer.begin() + _readIdx);
	return true;
}

std::string Buffer::getString(unsigned int len)
{
	if (len > readableBytes())
	{
		return std::string();
	}
	std::string ret(readBegin(), len);
	_readIdx += len;
	if (_readIdx > _buffer.size() / 2)
	{
		moveDataToHead();
	}
	return ret;
}

std::string Buffer::getAllString()
{
	assert(_readIdx <= _writeIdx);
	std::string ret(readBegin(), readableBytes());
	_readIdx = defaultPreSize;
	_writeIdx = defaultPreSize;
	return ret;
}

unsigned int Buffer::readableBytes()
{
	return _writeIdx - _readIdx;
}

unsigned int Buffer::writeableBytes()
{
	return _buffer.size() - _writeIdx;
}

unsigned int Buffer::preAppendableBytes()
{
	return _readIdx;
}

char* Buffer::readBegin()
{
	return &*(_buffer.begin() + _readIdx);
}

char* Buffer::writeBegin()
{
	return &*(_buffer.begin() + _writeIdx);
}

void Buffer::moveDataToHead()
{
	unsigned int datalen = readableBytes();
	std::copy(readBegin(), readBegin() + datalen, _buffer.begin() + defaultPreSize);
	_readIdx = defaultPreSize;
	_writeIdx = datalen + defaultPreSize;
}