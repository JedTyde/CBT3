#pragma once

#include <Wiwa/Renderer3D.h>

struct ShadingView
{
	const char* name;
	Wiwa::Renderer3D::Options glValue;
	bool active = false;

	ShadingView() = default;

	ShadingView(const char* Name, Wiwa::Renderer3D::Options GlValue, bool Active)
		: name(Name), glValue(GlValue), active(Active) {}
};