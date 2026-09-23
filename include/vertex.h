#pragma once
#include "vec3.h"
#include "vec2.h"
#include "vec4.h"
#include <vulkan/vulkan.h>
#include <array>

// defines the vertex struct and functions to describe the vertex stride and the attributes layout
// tangent and bitangent must exists for normal mapping.
struct Vertex {
    Math::vec3 pos;
    Math::vec3 normal;
    Math::Vec2 uv;
    Math::vec4 color;
    Math::vec3 tangent;
    Math::vec3 bitangent;

    constexpr static VkVertexInputBindingDescription getBindingDescription() {
        VkVertexInputBindingDescription description = {};
        description.binding = 0;
        description.stride = sizeof(Vertex);
        description.inputRate = VK_VERTEX_INPUT_RATE_VERTEX;
        return description;
    }

    constexpr static std::array<VkVertexInputAttributeDescription, 6> getAttributeDescriptions() {
        std::array<VkVertexInputAttributeDescription, 6> attributeDescriptions{};

        /* Position (location 0) */
        attributeDescriptions[0].binding = 0;
        attributeDescriptions[0].location = 0;
        attributeDescriptions[0].format = VK_FORMAT_R32G32B32_SFLOAT;
        attributeDescriptions[0].offset = offsetof(Vertex, pos);

        /* Normal (Location 1) */
        attributeDescriptions[1].binding = 0;
        attributeDescriptions[1].location = 1;
        attributeDescriptions[1].format = VK_FORMAT_R32G32B32_SFLOAT;
        attributeDescriptions[1].offset = offsetof(Vertex, normal);

        /* Texture Coordinates uv (location 2) */
        attributeDescriptions[2].binding = 0;
        attributeDescriptions[2].location = 2;
        attributeDescriptions[2].format = VK_FORMAT_R32G32_SFLOAT;
        attributeDescriptions[2].offset = offsetof(Vertex, uv);

        /* Color (Location 3) */
        attributeDescriptions[3].binding = 0;
        attributeDescriptions[3].location = 3;
        attributeDescriptions[3].format = VK_FORMAT_R32G32B32A32_SFLOAT;
        attributeDescriptions[3].offset = offsetof(Vertex, color);

        /* Tangent (Location 4) */
        attributeDescriptions[4].binding = 0;
        attributeDescriptions[4].location = 4;
        attributeDescriptions[4].format = VK_FORMAT_R32G32B32_SFLOAT;
        attributeDescriptions[4].offset = offsetof(Vertex, tangent);

        /* Bitangent (Location 5) */
        attributeDescriptions[5].binding = 0;
        attributeDescriptions[5].location = 5;
        attributeDescriptions[5].format = VK_FORMAT_R32G32B32_SFLOAT;
        attributeDescriptions[5].offset = offsetof(Vertex, bitangent);

        return attributeDescriptions;
    }
};