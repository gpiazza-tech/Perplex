#pragma once

#include <string_view>
#include <string>
#include <functional>

namespace Perplex
{
	class StringPopup
	{
	public:
		StringPopup(const char* label);

		void Open(const std::string defaultValue, std::function<void(const std::string&)> closeCallback);
		void Update();
	private:
		const char* m_Label;
		std::string m_Value;

		bool m_TriggerOpen;
		std::function<void(const std::string&)> m_CloseCallback;
	};
}