#pragma once

namespace Geg {
	class App {
	public:
		App();
		virtual ~App();

		void start();
	};

	//client must implement
	App* createApp();
}
