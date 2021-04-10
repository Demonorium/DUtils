#pragma once

#define DEMONORIUM_CONVERTION_DECLARE(FROM, TO) template<> struct convert< FROM , TO > { static TO cast( const FROM & source) {
#define DEMONORIUM_CONVERTION_TEMPLATE(FROM, TO) struct convert< FROM , TO > { static TO cast( const FROM & source) {
#define DEMONORIUM_CONVERTION_END } }

#include<string>


namespace demonorium
{
	namespace utils
	{
		template<class FROM, class TO>
		struct convert {
			static TO cast(const FROM& value) {
				return (TO)value;
			}
		};

		DEMONORIUM_CONVERTION_DECLARE(std::string, char)
			return std::atoi(source.c_str());
		DEMONORIUM_CONVERTION_END;

		DEMONORIUM_CONVERTION_DECLARE(std::string, short)
			return std::atoi(source.c_str());
		DEMONORIUM_CONVERTION_END;

		DEMONORIUM_CONVERTION_DECLARE(std::string, int)
			return std::atoi(source.c_str());
		DEMONORIUM_CONVERTION_END;

		DEMONORIUM_CONVERTION_DECLARE(std::string, long)
			return std::atol(source.c_str());
		DEMONORIUM_CONVERTION_END;

		DEMONORIUM_CONVERTION_DECLARE(std::string, long long)
			return std::atoll(source.c_str());
		DEMONORIUM_CONVERTION_END;

		DEMONORIUM_CONVERTION_DECLARE(std::string, float)
			return std::atof(source.c_str());
		DEMONORIUM_CONVERTION_END;

		DEMONORIUM_CONVERTION_DECLARE(std::string, double)
			return std::atof(source.c_str());
		DEMONORIUM_CONVERTION_END;

		template<class T>
		DEMONORIUM_CONVERTION_TEMPLATE(T, std::string)
			return std::to_string(source);
		DEMONORIUM_CONVERTION_END;

	}
}