#include "pch.h"
#include "arena.h"
#include <new>

Arena::Arena(std::size_t size)
	: totalSize(size), offset(0)
{
	buffer = static_cast<char*>(::operator new(totalSize));
}

Arena::~Arena()
{
	if (buffer) {
		::operator delete(buffer);
		buffer = nullptr;
		offset = 0;
		totalSize = 0;
	}
}
