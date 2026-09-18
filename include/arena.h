#pragma once 

#include <cstddef>
#include <memory>
#include <stdexcept>

class Arena {
public:
	Arena(std::size_t size); 

	template<typename T, typename... Args>
	T* add(Args&&... args) {
		auto* start = static_cast<void*>(buffer + offset); 
		auto* aligned = start; 

		std::size_t space = totalSize - offset;


		if (std::align(alignof(T), sizeof(T), aligned, start) {
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