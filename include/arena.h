#pragma once 

#include <cstddef>
#include <memory>
#include <stdexcept>
#include "defines.h"
#include "config.h"

class Arena {
public:
	Arena(std::size_t size);
	~Arena(); 

	template<typename T, typename... Args>
	T* add(Args&&... args) {
		auto* start = static_cast<void*>(static_cast<char*>(buffer) + offset);
		auto* aligned = start; 

		std::size_t space = totalSize - offset;


		if (std::align(alignof(T), sizeof(T), aligned, space)) {
			const std::size_t padding =
				reinterpret_cast<uintptr_t>(aligned) - reinterpret_cast<uintptr_t>(start);

				offset += padding + sizeof(T);
				return new(aligned) T(std::forward<Args>(args)...);
		}
		throw std::runtime_error("Not enough space to allocate object!");
	}


private:
	std::size_t totalSize;
	std::size_t offset;
	void* buffer; 
};

inline Arena& GetArena() {
	static Arena s_Arena(Config::globalArenaSize);
	return s_Arena;
}