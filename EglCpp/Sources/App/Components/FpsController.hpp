#pragma once

#include "../IApplicationComponent.hpp"

namespace Egliss
{
	class FpsController : public IApplicationComponent
	{
	public:

		virtual void Initialize() override;
		virtual void Finalize() override;

		void Wait();


		// fps < 0 is unlimited
		int TargetFps() const { return this->_fps; }
		void TargetFps(int fps);
		long long FrameCount() const { return this->_frameCount; }
		virtual std::string Name() const override { return "FpsController"; }

	private:
		int _fps;
		long long _frameCount;
	};
}