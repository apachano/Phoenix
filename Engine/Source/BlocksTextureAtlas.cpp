// Copyright 2019 Genten Studios
//
// Redistribution and use in source and binary forms, with or without
// modification, are permitted provided that the following conditions are met:
//
// 1. Redistributions of source code must retain the above copyright notice,
// this list of conditions and the following disclaimer.
//
// 2. Redistributions in binary form must reproduce the above copyright notice,
// this list of conditions and the following disclaimer in the documentation
// and/or other materials provided with the distribution.
//
// 3. Neither the name of the copyright holder nor the names of its contributors
// may be used to endorse or promote products derived from this software without
// specific prior written permission.
//
// THIS SOFTWARE IS PROVIDED BY THE COPYRIGHT HOLDERS AND CONTRIBUTORS "AS IS"
// AND ANY EXPRESS OR IMPLIED WARRANTIES, INCLUDING, BUT NOT LIMITED TO, THE
// IMPLIED WARRANTIES OF MERCHANTABILITY AND FITNESS FOR A PARTICULAR PURPOSE
// ARE DISCLAIMED. IN NO EVENT SHALL THE COPYRIGHT HOLDER OR CONTRIBUTORS BE
// LIABLE FOR ANY DIRECT, INDIRECT, INCIDENTAL, SPECIAL, EXEMPLARY, OR
// CONSEQUENTIAL DAMAGES (INCLUDING, BUT NOT LIMITED TO, PROCUREMENT OF
// SUBSTITUTE GOODS OR SERVICES; LOSS OF USE, DATA, OR PROFITS; OR BUSINESS
// INTERRUPTION) HOWEVER CAUSED AND ON ANY THEORY OF LIABILITY, WHETHER IN
// CONTRACT, STRICT LIABILITY, OR TORT (INCLUDING NEGLIGENCE OR OTHERWISE)
// ARISING IN ANY WAY OUT OF THE USE OF THIS SOFTWARE, EVEN IF ADVISED OF THE
// POSSIBILITY OF SUCH DAMAGE.

#include <Quartz2/BlocksTextureAtlas.hpp>

#include <algorithm>
#include <cassert>

#define STB_IMAGE_IMPLEMENTATION
#include <stb_image.h>

#define STB_RECT_PACK_IMPLEMENTATION
#include <stb_rect_pack.h>

using namespace q2;

const BlockTextureAtlas::SpriteID BlockTextureAtlas::INVALID_SPRITE;

BlockTextureAtlas::BlockTextureAtlas(std::size_t spriteWidth,
	std::size_t spriteHeight)
	: m_spriteWidth(spriteWidth), m_spriteHeight(spriteHeight)
{
}

BlockTextureAtlas::BlockTextureAtlas() : m_spriteWidth(0), m_spriteHeight(0) {}

void BlockTextureAtlas::setSpriteWidth(std::size_t w) { m_spriteWidth = w; }

void BlockTextureAtlas::setSpriteHeight(std::size_t h) { m_spriteHeight = h; }

void BlockTextureAtlas::addTextureFile(const char* texturefilepath)
{
	m_textureIDMap.insert(std::make_pair(std::string(texturefilepath),
		BlockTextureAtlas::INVALID_SPRITE));
}

BlockTextureAtlas::SpriteID BlockTextureAtlas::getSpriteIDFromFilepath(
	const char* filepath)
{
	const auto equalsTest =
		[filepath](
			const std::unordered_map<std::string, SpriteID>::value_type a)
		-> bool { return a.first == filepath; };

	if (std::find_if(m_textureIDMap.begin(), m_textureIDMap.end(),
		equalsTest) == m_textureIDMap.end())
		return BlockTextureAtlas::INVALID_SPRITE;

	return m_textureIDMap.at(filepath);
}

RectAABB BlockTextureAtlas::getSpriteFromID(
	BlockTextureAtlas::SpriteID spriteID) const
{
	RectAABB uv;

	const float yPx =
		static_cast<float>(spriteID) * static_cast<float>(m_spriteHeight + 2);
	const float xPx = 0.f;

	const float yUv = (static_cast<float>(yPx) / m_patchedTextureHeight);
	const float xUv = static_cast<float>(xPx) / m_patchedTextureWidth;

	const float widthUv =
		static_cast<float>(m_spriteWidth) / m_patchedTextureWidth;
	const float heightUv =
		(static_cast<float>(m_spriteHeight) / m_patchedTextureHeight);
	
	uv.topLeft = { xUv, yUv };
	uv.topRight = { xUv + widthUv, yUv };
	uv.bottomLeft = { xUv, yUv + heightUv };
	uv.bottomRight = { xUv + widthUv, yUv + heightUv };
	

	/*uv.topLeft = { 0.f, 0.f };
	uv.topRight = { 1.f, 0.f };
	uv.bottomLeft = { 0.f, 1.f};
	uv.bottomRight = { 1.f, 1.f };
	*/

	return uv;
}

void BlockTextureAtlas::patch()
{
	assert(m_spriteWidth != 0);
	assert(m_spriteHeight != 0);

	const std::size_t numTextures = m_textureIDMap.size();

	if (numTextures == 0)
		return;

	const std::size_t textureHeight = (numTextures + 2) * m_spriteHeight;
	const std::size_t textureWidth = m_spriteWidth;

	m_patchedTextureData = new unsigned char[textureWidth * 4 * textureHeight];
	
	for (std::size_t i = 0; i < textureWidth * 4 * textureHeight; i = i + 4)
	{
		m_patchedTextureData[i] = m_patchedTextureData[i + 1] = m_patchedTextureData[i + 2] = 255;
		m_patchedTextureData[i + 3] = 0;
	}

	assert(m_patchedTextureData);

	std::size_t spriteIndex = 0;

	std::size_t gy = 0;

	for (const auto& sprite : m_textureIDMap)
	{
		const std::string& textureFilepath = sprite.first;
		m_textureIDMap[textureFilepath] = SpriteID(spriteIndex);

		int            width = -1, height = -1, nbChannels = -1;
		unsigned char* image =
			stbi_load(textureFilepath.c_str(), &width, &height, &nbChannels, 0);

		// Basic sanity checks on the loaded image
		assert(image);
		assert(width != 0 && height != 0);

		// #todo (bwilks): should have a way of supporting images that don't
		// have 4 channels (maybe support 3 and just pad the alpha with 1?)
		assert(nbChannels == 4);

		std::memcpy(m_patchedTextureData + (m_spriteWidth*nbChannels * gy),image, width * height * nbChannels);

		gy += m_spriteHeight + 2;

		stbi_image_free(image);

		spriteIndex++;
	}

	m_patchedTextureWidth = textureWidth;
	m_patchedTextureHeight = textureHeight;
}

BlockTextureAtlas::~BlockTextureAtlas() { delete[] m_patchedTextureData; }