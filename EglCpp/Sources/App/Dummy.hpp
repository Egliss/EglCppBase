#pragma once

#include "IApplicationComponent.hpp"

namespace Egliss
{
	class Dummy : public IApplicationComponent
	{
		virtual void Initialize() override;
		virtual void Update() override;
		virtual void Finalize() override;
		virtual std::string Name() const override;
	};
}