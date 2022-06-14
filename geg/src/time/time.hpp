#pragma once

namespace geg{
	class Time {
	public:
		static	void updateTime();
		static const long& getTime();
		static const long& getTimeMs();
		static const double& getgegTime();
		static const double& getgegTimeMs();
		static const float& getDeltaTime();
	};
}		 // namespace geg::Time
