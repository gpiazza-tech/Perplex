#include <Perplex/pch.h>
#include <Perplex/Perpixel/PerpixelInstance.h>

#include <Perplex/Perpixel/PerpixelSpawnShape.h>
#include <c/perplex_pixel.h>
#include <pxr/pxr.h>

#include <vector>

namespace Perplex
{
	void PerpixelInstance::SetSpawnShape(const PerpixelShape& shape)
	{
        Asset spriteAsset = shape.Info.SpriteAsset;
        if (!spriteAsset)
            return;

        Ref<pxr::Sprite> colorSprite = spriteAsset.GetData<pxr::Sprite>();
        glm::u8vec4* colorPixels = new glm::u8vec4[colorSprite->PixelWidth * colorSprite->PixelHeight];
        pxr::SpriteRegistry::FetchPixels(*colorSprite.get(), colorPixels);

        // TODO: add emission z
        //glm::u8vec4* emissionPixels = new glm::u8vec4[emissionSprite.PixelWidth * emissionSprite.PixelHeight];
        //pxr::SpriteRegistry::FetchPixels(emissionSprite, emissionPixels);

        float pixelsPerUnit = 16.0f;
        glm::vec2 center = pxr::MakePixelPerfect(glm::vec3{ (float)colorSprite->PixelWidth / pixelsPerUnit / 2.0f, (float)colorSprite->PixelHeight / pixelsPerUnit / 2.0f, 0.0f }, static_cast<int>(pixelsPerUnit));

        m_SpawnPixels.clear();
        m_SpawnPixels.reserve(colorSprite->PixelWidth * colorSprite->PixelHeight);
        glm::vec2 positionOffset = {  };
        for (size_t i = 0; i < colorSprite->PixelWidth * colorSprite->PixelHeight; i++)
        {
            float aColor = colorPixels[i].a / 255.0f;
            if (aColor == 0.0f)
                continue;

            float rColor = colorPixels[i].r / 255.0f;
            float gColor = colorPixels[i].g / 255.0f;
            float bColor = colorPixels[i].b / 255.0f;

            /*
            float rEmit = emissionPixels[i].r / 255.0f;
            float gEmit = emissionPixels[i].g / 255.0f;
            float bEmit = emissionPixels[i].b / 255.0f;
            float aEmit = emissionPixels[i].a / 255.0f;
            */

            // Particle
            pixel pxl{};
            pxl.Color = { rColor, gColor, bColor, aColor };
            pxl.Position = { (i % colorSprite->PixelWidth / pixelsPerUnit) - center.x, (i / colorSprite->PixelWidth / pixelsPerUnit) - center.y };
            pxl.Emission = 0;

            m_SpawnPixels.emplace_back(pxl);
        }

        delete[] colorPixels;
        // delete[] emissionPixels;

        m_SpawnShape = shape;
	}
}