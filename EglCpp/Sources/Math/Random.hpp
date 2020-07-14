#pragma once
#include <memory>

namespace Egliss
{
	class Random
	{
	public:
		~Random();

		static void Initialize(unsigned int seed);
		static void Finalize();
		static float Range(float min, float max);
	private:
		class _Impl;

		static std::unique_ptr<Random> _instance;
		_Impl* _impl;
	};
}