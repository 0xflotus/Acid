#include "RendererParticles.hpp"

#include "Maths/Maths.hpp"
#include "Models/VertexModel.hpp"

namespace acid
{
	RendererParticles::RendererParticles(const GraphicsStage &graphicsStage) :
		IRenderer(graphicsStage),
		m_uniformScene(UniformHandler()),
		m_pipeline(Pipeline(graphicsStage, PipelineCreate({"Shaders/Particles/Particle.vert", "Shaders/Particles/Particle.frag"}, {VertexModel::GetVertexInput()},
			PIPELINE_MODE_POLYGON, PIPELINE_DEPTH_READ, VK_POLYGON_MODE_FILL, VK_CULL_MODE_BACK_BIT, {{"MAX_INSTANCES", String::To(ParticleType::MAX_TYPE_INSTANCES)}})))
	{
	}

	void RendererParticles::Render(const CommandBuffer &commandBuffer, const Vector4 &clipPlane, const ICamera &camera)
	{
		m_uniformScene.Push("projection", camera.GetProjectionMatrix());
		m_uniformScene.Push("view", camera.GetViewMatrix());

		auto particles = Particles::Get()->GetParticles();

		m_pipeline.BindPipeline(commandBuffer);

		for (auto &[type, particles] : particles)
		{
			type->CmdRender(commandBuffer, m_pipeline, m_uniformScene);
		}
	}
}
