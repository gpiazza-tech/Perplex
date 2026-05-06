#pragma once

#include <entt.hpp>

namespace Holloware
{
	struct Dynamic : entt::type_list<>
	{
		template<typename Base>
		struct type : Base
		{
			void OnPlay() { this->template invoke<0>(*this); }
			void OnStop() { this->template invoke<1>(*this); }
		};

		template<typename Type>
		using impl = entt::value_list<&Type::OnPlay, &Type::OnStop>;
	};
}