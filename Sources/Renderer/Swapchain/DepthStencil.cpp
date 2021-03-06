#include "DepthStencil.hpp"

#include <cassert>
#include "Display/Display.hpp"
#include "Textures/Texture.hpp"

namespace acid
{
	static const std::vector<VkFormat> TRY_FORMATS =
	{
		VK_FORMAT_D32_SFLOAT_S8_UINT,
		VK_FORMAT_D32_SFLOAT,
		VK_FORMAT_D24_UNORM_S8_UINT,
		VK_FORMAT_D16_UNORM_S8_UINT,
		VK_FORMAT_D16_UNORM
	};

	DepthStencil::DepthStencil(const uint32_t &width, const uint32_t &height, const VkSampleCountFlagBits &samples) :
		IDescriptor(),
		Buffer(width * height * 4, VK_BUFFER_USAGE_TRANSFER_DST_BIT | VK_IMAGE_USAGE_SAMPLED_BIT, VK_MEMORY_PROPERTY_DEVICE_LOCAL_BIT),
		m_width(width),
		m_height(height),
		m_image(VK_NULL_HANDLE),
		m_imageView(VK_NULL_HANDLE),
		m_sampler(VK_NULL_HANDLE),
		m_format(VK_FORMAT_UNDEFINED),
		m_imageInfo({})
	{
		auto physicalDevice = Display::Get()->GetPhysicalDevice();

		for (auto &format : TRY_FORMATS)
		{
			VkFormatProperties formatProperties = {};
			vkGetPhysicalDeviceFormatProperties(physicalDevice, format, &formatProperties);

			if (formatProperties.optimalTilingFeatures & VK_FORMAT_FEATURE_DEPTH_STENCIL_ATTACHMENT_BIT)
			{
				m_format = format;
				break;
			}
		}

		if (m_format == VK_FORMAT_UNDEFINED)
		{
			assert(false && "Vulkan runtime error, depth stencil format not selected!");
		}

		Texture::CreateImage(m_image, m_bufferMemory, m_width, m_height, VK_IMAGE_TYPE_2D, samples, 1, m_format, VK_IMAGE_TILING_OPTIMAL,
			VK_IMAGE_USAGE_DEPTH_STENCIL_ATTACHMENT_BIT | VK_IMAGE_USAGE_SAMPLED_BIT, VK_MEMORY_PROPERTY_DEVICE_LOCAL_BIT, 1);
		Texture::TransitionImageLayout(m_image, m_format, VK_IMAGE_LAYOUT_UNDEFINED, VK_IMAGE_LAYOUT_DEPTH_STENCIL_ATTACHMENT_OPTIMAL, 1, 0, 1);
		Texture::CreateImageSampler(m_sampler, VK_FILTER_LINEAR, VK_SAMPLER_ADDRESS_MODE_CLAMP_TO_EDGE, false, 1);
		Texture::CreateImageView(m_image, m_imageView, VK_IMAGE_VIEW_TYPE_2D, m_format, VK_IMAGE_ASPECT_DEPTH_BIT, 1, 0, 1);

		m_imageInfo.imageLayout = VK_IMAGE_LAYOUT_DEPTH_STENCIL_ATTACHMENT_OPTIMAL;
		m_imageInfo.imageView = m_imageView;
		m_imageInfo.sampler = m_sampler;
	}

	DepthStencil::~DepthStencil()
	{
		auto logicalDevice = Display::Get()->GetLogicalDevice();

		vkDestroySampler(logicalDevice, m_sampler, nullptr);
		vkDestroyImageView(logicalDevice, m_imageView, nullptr);
		vkDestroyImage(logicalDevice, m_image, nullptr);
	}

	DescriptorType DepthStencil::CreateDescriptor(const uint32_t &binding, const VkDescriptorType &descriptorType, const VkShaderStageFlags &stage)
	{
		VkDescriptorSetLayoutBinding descriptorSetLayoutBinding = {};
		descriptorSetLayoutBinding.binding = binding;
		descriptorSetLayoutBinding.descriptorCount = 1;
		descriptorSetLayoutBinding.descriptorType = descriptorType;
		descriptorSetLayoutBinding.pImmutableSamplers = nullptr;
		descriptorSetLayoutBinding.stageFlags = stage;

		VkDescriptorPoolSize descriptorPoolSize = {};
		descriptorPoolSize.type = descriptorType;
		descriptorPoolSize.descriptorCount = 1;

		return DescriptorType(binding, stage, descriptorSetLayoutBinding, descriptorPoolSize);
	}

	VkWriteDescriptorSet DepthStencil::GetWriteDescriptor(const uint32_t &binding, const VkDescriptorType &descriptorType, const DescriptorSet &descriptorSet) const
	{
		VkWriteDescriptorSet descriptorWrite = {};
		descriptorWrite.sType = VK_STRUCTURE_TYPE_WRITE_DESCRIPTOR_SET;
		descriptorWrite.dstSet = descriptorSet.GetDescriptorSet();
		descriptorWrite.dstBinding = binding;
		descriptorWrite.dstArrayElement = 0;
		descriptorWrite.descriptorType = descriptorType;
		descriptorWrite.descriptorCount = 1;
		descriptorWrite.pImageInfo = &m_imageInfo;

		return descriptorWrite;
	}
}
