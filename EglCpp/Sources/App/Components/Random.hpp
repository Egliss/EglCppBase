#pragma once

#include "../IApplicationComponent.hpp"

namespace Egliss
{
	class Random : public IApplicationComponent
	{
	public:
		Random() : IApplicationComponent(StaticType<Resources>::Id) {};
		~Random() = default;

		virtual void Initialize() override;
		virtual void Finalize() override;
		virtual std::string Name() const override;

		void Seed(unsigned int seed);
		float Range(float min, float max);
	private:
		class _Impl;

		_Impl* _impl = nullptr;
	};
}