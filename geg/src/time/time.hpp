#pragma once

namespace Geg{
	class Time {
	public:
		static	void updateTime();
		static const long& getTime();
		static const long& getTimeMs();
		static const double& getGegTime();
		static const double& getGegTimeMs();
		static const float& getDeltaTime();
	};
}		 // namespace Geg::Time
