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

#include <Common/Voxels/Entity.hpp>
#include <Common/Registry.hpp>

#include <string>

namespace phx::voxels
{
	/**
	 * @brief Acts as a universal pairing between string and entity.
	 *
	 * This class is a universal struct that can be passed around to act as a minimal set of data.
	 *
	 * Each application implements their own EntityRegistry with a few more
	 * pieces of data and lua registration methods.
	 */
	struct EntityReferrer
	{
		/**
		 * @brief Automatically implements the Unknown, Out of Bounds and Air
		 * blocks as necessary.
		 */
		EntityReferrer()
		{
			// this will be 0, which is equivalent to EntityType::UNKNOWN_ENTITY
			auto              uid = referrer.size();
			voxels::BlockType unknown;
			unknown.displayName = "Unknown Entity";
			unknown.id          = "core.unknown";
			unknown.uid         = uid;
			referrer.add(unknown.id, uid);
			entities.add(uid, unknown);

			// by default will return unknown blocks if they don't exist.
			// a tiny bit hacky but solves a lot of problems without writing a
			// bunch of unnecessary code.
			referrer.setUnknownReturnVal(referrer.get("core.unknown"));
			blocks.setUnknownReturnVal(
			    blocks.get(voxels::BlockType::UNKNOWN_ENTITY));
		}

		/**
		 * @brief Gets a BlockType from the registry by its string ID.
		 * @param id The string ID of the BlockType.
		 * @return Pointer to the block matching the string ID.
		 */
		EntityType* getByID(const std::string& id)
		{
			return entities.get(*referrer.get(id));
		};

		// referrer refers a string to int, which in turn is used to get the
		// EntityType.
		Registry<std::string, std::size_t> referrer;
		Registry<std::size_t, EntityType>  entities;
	};
} // namespace phx::voxels
