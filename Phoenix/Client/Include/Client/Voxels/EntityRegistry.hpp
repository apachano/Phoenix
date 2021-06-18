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

#pragma once

#include <Client/AudioRegistry.hpp>
#include <Client/Graphics/BlockModel.hpp>
#include <Client/Graphics/TexturePacker.hpp>

#include <Common/CMS/ModManager.hpp>
#include <Common/Voxels/EntityReferrer.hpp>

#include <string>
#include <unordered_map>
#include <vector>

namespace phx::client
{
	/**
	 * @brief Acts as a register for block data throughout the client
	 * application.
	 *
	 * This holds a block referrer which handles default initialized blocks and
	 * then provides an API registration for registering blocks from within Lua.
	 *
	 * We implemented this instead of the original impl because something like
	 * the server does not need to store things like texture paths - which is
	 * quite essential on the client.
	 */
	struct EntityRegistry : public voxels::EntityReferrer
	{
		EntityRegistry(AudioRegistry* audioRegistry)
		    : m_audioRegistry(audioRegistry)
		{
			textureHandles.add(voxels::EntityType::UNKNOWN_ENTITY, {0});
			textureHandles.setUnknownReturnVal(
			    textureHandles.get(voxels::EntityType::UNKNOWN_ENTITY));

			textures.add(voxels::EntityType::UNKNOWN_ENTITY, "unknown.png");
			textures.setUnknownReturnVal(
			    textures.get(voxels::EntityType::UNKNOWN_ENTITY));

			models.add(voxels::EntityType::UNKNOWN_ENTITY,
			           gfx::BlockModel::BLOCK);
			models.setUnknownReturnVal(
			    models.get(voxels::EntityType::UNKNOWN_ENTITY));
		}

		gfx::TexturePacker texturePacker;
		Registry<std::size_t, std::vector<gfx::TexturePacker::Handle>>
		                                                  textureHandles;
		Registry<gfx::TexturePacker::Handle, std::string> textures;
		Registry<std::size_t, gfx::BlockModel> models;

	private:
		AudioRegistry* m_audioRegistry;

	public:
		Registry<std::size_t, SourceGroup> SoundOnBreak;
		Registry<std::size_t, SourceGroup> SoundOnPlace;

		void registerAPI(cms::ModManager* manager)
		{
			manager->registerFunction(
			    "voxel.entity.register", [manager, this](sol::table luaEntity) {
				    voxels::EntityType entity;

				    sol::optional<std::string> name = luaEntity["name"];
				    if (name)
				    {
					    entity.displayName = *name;
					}
					else
					{
					    LOG_FATAL("MODDING")
					        << "The mod at: " << manager->getCurrentModPath()
					        << " attempted to register an entity without "
					        << "specifying a name.";
						return;
					}

					sol::optional<std::string> id = luaEntity["id"];
					if (id)
				    {
					    entity.id = luaEntity.get<std::string>("id");
				    }
				    else
				    {
					    LOG_FATAL("MODDING")
					        << "The mod at: " << manager->getCurrentModPath()
					        << " attempts to register a block ("
					        << entity.displayName << ") without "
					        << "specifying a name.";
					    return;
					}

				    sol::optional<std::vector<std::string>> luaTextures = luaEntity["textures"]; // NOLINT: ugly
				    std::vector<gfx::TexturePacker::Handle> handles;
				    if (luaTextures)
				    {
					    for (const auto& tex : *luaTextures)
					    {
						    auto handle = texturePacker.add(
						        manager->getCurrentModPath() + tex);

					    	handles.push_back(handle);

					    	// don't add the whole path to the real texture names.
						    textures.add(handle, tex);
					    }
				    }

				    entity.uid = referrer.size();
				    referrer.add(entity.id, entity.uid);
				    entities.add(entity.uid, entity);

			    	// if handles are empty, then there clearly aren't any textures so don't waste any memory :)
			    	if (!handles.empty())
			    	{
					    textureHandles.add(entity.uid, handles);
			    	}

				    sol::optional<std::vector<std::string>> luaSoundOnBreak =
                        luaEntity["soundOnBreak"];
				    SourceGroup onBreakSources;
				    if (luaSoundOnBreak)
				    {
					    for (const auto& sourceID : *luaSoundOnBreak)
					    {
						    auto* source = m_audioRegistry->getByID(sourceID);
						    if (source != nullptr)
						    {
							    onBreakSources.push_back(source);
						    }
						    else
						    {
							    LOG_WARNING("MODDING")
							        << "The mod at: "
							        << manager->getCurrentModPath()
							        << " attempted to get unloaded audio file ("
							        << sourceID << ")";
						    }
					    }
					    SoundOnBreak.add(entity.uid, onBreakSources);
				    }

				    sol::optional<std::vector<std::string>> luaSoundOnPlace =
                        luaEntity["soundOnPlace"];
				    SourceGroup onPlaceSources;
				    if (luaSoundOnPlace)
				    {
					    for (const auto& sourceID : *luaSoundOnPlace)
					    {
						    auto* source = m_audioRegistry->getByID(sourceID);
						    if (source != nullptr)
						    {
							    onPlaceSources.emplace_back(source);
						    }
						    else
						    {
							    LOG_WARNING("MODDING")
							        << "The mod at: "
							        << manager->getCurrentModPath()
							        << " attempted to get unloaded audio file ("
							        << sourceID << ")";
						    }
					    }
					    SoundOnPlace.add(entity.uid, onPlaceSources);
				    }
			    });
		}
	};
} // namespace phx::client
