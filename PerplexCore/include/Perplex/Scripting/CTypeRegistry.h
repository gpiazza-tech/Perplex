#pragma once

#include <Perplex/Assets/Asset.h>
#include <Perplex/Scene/Entity.h>

#include <c/perplex_math.h>
#include <c/perplex_asset.h>

#include <array>
#include <utility>
#include <string_view>

#define PX_C_TYPE_SIZE_DEF(x) std::pair<std::string_view, size_t>(#x, sizeof(x))

#define PX_C_TYPES \
PX_C_TYPE_SIZE_DEF(int),\
PX_C_TYPE_SIZE_DEF(float),\
PX_C_TYPE_SIZE_DEF(double),\
PX_C_TYPE_SIZE_DEF(bool),\
PX_C_TYPE_SIZE_DEF(Vec2),\
PX_C_TYPE_SIZE_DEF(Vec3),\
PX_C_TYPE_SIZE_DEF(Vec4),\
PX_C_TYPE_SIZE_DEF(Color),\
PX_C_TYPE_SIZE_DEF(Entity),\
PX_C_TYPE_SIZE_DEF(Asset),\
PX_C_TYPE_SIZE_DEF(PrefabAsset),\
PX_C_TYPE_SIZE_DEF(SceneAsset),\
PX_C_TYPE_SIZE_DEF(Bounds),\
PX_C_TYPE_SIZE_DEF(Radius),\

namespace Perplex
{
	namespace detail
	{
		template <typename T, std::size_t N>
		constexpr size_t FindLargest(const std::array<T, N>& arr)
		{
			size_t max_val = arr[0].second;
			for (const auto& element : arr) {
				if (element.second > max_val) {
					max_val = element.second;
				}
			}
			return max_val;
		}
	}

	class CTypeRegistry
	{
	public:
		static consteval size_t GetLargestCTypeSize()
		{
			constexpr std::array typeSizeTable{ PX_C_TYPES };

			constexpr size_t largest = detail::FindLargest(typeSizeTable);
			return largest;
		}

		static std::optional<size_t> GetCTypeSize(std::string_view str)
		{
			std::unordered_map typeSizeTable{ PX_C_TYPES };

			if (!typeSizeTable.contains(str))
				return std::nullopt;

			return typeSizeTable.at(str);
		}
	};
}