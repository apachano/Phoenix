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

#include <Client/Player.hpp>

#include <Common/Actor.hpp>
#include <Common/Movement.hpp>
#include <Common/PlayerView.hpp>

using namespace phx;

Player::Player(entt::registry* registry) : m_registry(registry)
{
	m_entity = ActorSystem::registerActor(registry);
}

void Player::registerAPI(cms::ModManager* manager)
{
	manager->registerFunction("core.player.getSpeed", [this]() {
		return m_registry->get<Movement>(m_entity).moveSpeed;
	});

	manager->registerFunction("core.player.setSpeed", [this](int speed) {
		m_registry->get<Movement>(m_entity).moveSpeed = speed;
	});

	manager->registerFunction("core.player.getPosition", [this]() {
		sol::table pos;
		pos["x"] = m_registry->get<Position>(m_entity).position.x;
		pos["y"] = m_registry->get<Position>(m_entity).position.y;
		pos["z"] = m_registry->get<Position>(m_entity).position.z;
		return pos;
	});

	manager->registerFunction(
	    "core.player.setPosition", [this](int posx, int posy, int posz) {
		    m_registry->get<Position>(m_entity).position = {posx, posy, posz};
	    });
}

