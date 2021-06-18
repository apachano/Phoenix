// Copyright 2019-20 Genten Studios
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

/**
 * @file ChunkRenderer.hpp
 * @brief The ChunkRenderer.
 *
 * @copyright Copyright (c) 2019-20 Genten Studios
 */

#pragma once

#include <Client/Graphics/Camera.hpp>
#include <Client/Graphics/ShaderPipeline.hpp>
#include <Client/Graphics/TexturePacker.hpp>
#include <Client/Voxels/EntityRegistry.hpp>

#include <entt/entt.hpp>

#include <Common/Position.hpp>
#include <unordered_map>
#include <vector>

namespace phx::gfx
{

	/**
	 * @brief Renders all entities in a view
	 */
	class EntityRenderer
	{
	public:
		/// @brief Typedef to make it easier for use outside of this class.
		using AssociativeTextureTable =
		    std::unordered_map<std::string, std::size_t>;

		EntityRenderer(client::EntityRegistry* entityRegistry);

		void prep();

		void tick(
		    entt::registry*   registry,
		    entt::entity      entity,
		    const math::mat4& projection,
		    const float&      dt);

		/**
		 * @brief Gets the shader vertex layout that this renderer requires.
		 * @return The layout that the ShaderPipeline needs to guarantee.
		 */
		static std::vector<ShaderLayout> getRequiredShaderLayout();

		/**
		 * @brief Gets the table telling everything where each texture is in the
		 * GPU-side array.
		 * @return An associative table storing which textures are on which
		 * layers within the texture array.
		 */
		const AssociativeTextureTable& getTextureTable() const;

	private:
		client::EntityRegistry* m_entityRegistry;

		static const int m_vertexAttributeLocation = 0;
		static const int m_uvAttributeLocation     = 1;
		static const int m_normalAttributeLocation = 2;
		static const int m_colorAttributeLocation  = 3;

        gfx::ShaderPipeline m_renderPipeline;
	};
} // namespace phx::gfx
