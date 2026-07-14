#pragma once

namespace Perplex
{
	struct RenderSettings
	{
		bool Postprocessing{ true };

		bool Bloom{ true };
		float BloomThreshold{ 1.0f };
		float BloomFilterRadius{ 0.003f };

		bool Tonemapping{ true };
		bool Pixelate{ true };
	};
}