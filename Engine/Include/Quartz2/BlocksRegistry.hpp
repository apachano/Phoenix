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

#pragma once

#include <Quartz2/BlocksTextureAtlas.hpp>
#include <Quartz2/Singleton.hpp>
#include <list>

namespace q2
{
	enum EBlockCategory
	{
		BLOCK_CATEGORY_AIR,
		BLOCK_CATEGORY_SOLID,
		BLOCK_CATEGORY_LIQUID
	};

	struct BlockType
	{
		const char* displayName;
		const char* id;

		EBlockCategory category;

		struct
		{
			BlockTextureAtlas::SpriteID top, bottom, left, right, front,
				back;

			void setAll(BlockTextureAtlas::SpriteID sprite)
			{
				top = bottom = left = right = front = back = sprite;
			}
		} textures;
	};

	class BlockRegistry : public Singleton<BlockRegistry>
	{
	public:
		BlockType* registerBlock(BlockType blockInfo);
		BlockType* getBlockFromID(const char* id);

		void setAtlas(const std::shared_ptr<BlockTextureAtlas>& atlas);

		BlockTextureAtlas* getAtlas() const
			{ return m_textureAtlas.get(); }

	private:
		// This is a std::list as we don't want to invalidate any pointers
		// when resizing... #todo (bwilks): Maybe use HandleAllocator for
		// this as well??
		std::list<BlockType> m_blocks;

		std::shared_ptr<BlockTextureAtlas> m_textureAtlas;
	};
}