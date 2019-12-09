#pragma once
#include <vector>
#include <string>

namespace lion
{
	//简单的buffer，线程非安全
	class Buffer
	{
	public:
		Buffer(unsigned int bufferSize = defaultSize);
		~Buffer();

		void swap(Buffer& changedbf);

		void append(const char* data, unsigned int len);

		bool preappend(const char* data, unsigned int len);

		std::string getString(unsigned int len);

		std::string getAllString();

		bool empty() { return _readIdx == _writeIdx; }
	private:
		unsigned int readableBytes();
		unsigned int writeableBytes();
		unsigned int preAppendableBytes();
		char* readBegin();
		char* writeBegin();

		void moveDataToHead();
	private:
		static const unsigned int defaultSize = 1024;
		static const unsigned int defaultPreSize = 8;
		std::vector<char> _buffer;
		unsigned int _readIdx;
		unsigned int _writeIdx;
	};

}



