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

#include <Client/Graphics/EntityRenderer.hpp>

using namespace phx::gfx;

EntityRenderer::EntityRenderer(client::EntityRegistry* entityRegistry)
: m_entityRegistry(entityRegistry)
{
	//TODO switch this with entity specific assets
    std::vector<ShaderLayout> layout;
    layout.emplace_back("a_Vertex", 0);
    layout.emplace_back("a_UV", 1);
    m_renderPipeline.prepare("Assets/SimpleWorld.vert",
                             "Assets/SimpleWorld.frag",
                             layout);

    m_renderPipeline.activate();
    const math::mat4 model;
    m_renderPipeline.setMatrix("u_model", model);
}

void EntityRenderer::tick(
    entt::registry*   registry,
    entt::entity      entity,
    const math::mat4& projection,
    const float&      dt)
{
    auto position = registry->get<Position>(entity);
    // temp, will change in the future, based on game time
    static math::vec3 lightDir(0.f, -1.f, 0.f);
    static float      time = 0.f;

    time += dt;

    lightDir.y = std::sin(time);
    lightDir.x = std::cos(time);

    m_renderPipeline.activate();
    m_renderPipeline.setInt("u_TexArray", 0);
    m_renderPipeline.setMatrix("u_view", position.getView());
    m_renderPipeline.setMatrix("u_projection", projection);
    m_renderPipeline.setFloat("u_AmbientStrength", 0.7f);
    m_renderPipeline.setVector3("u_LightDir", lightDir);
    m_renderPipeline.setFloat("u_Brightness", 0.6f);


}