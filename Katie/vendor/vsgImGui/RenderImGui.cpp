/* <editor-fold desc="MIT License">

Copyright(c) 2021 Don Burns, Roland Hill and Robert Osfield.

Permission is hereby granted, free of charge, to any person obtaining a copy of
this software and associated documentation files (the "Software"), to deal in
the Software without restriction, including without limitation the rights to
use, copy, modify, merge, publish, distribute, sublicense, and/or sell copies of
the Software, and to permit persons to whom the Software is furnished to do so,
subject to the following conditions:

The above copyright notice and this permission notice shall be included in all
copies or substantial portions of the Software.

THE SOFTWARE IS PROVIDED "AS IS", WITHOUT WARRANTY OF ANY KIND, EXPRESS OR
IMPLIED, INCLUDING BUT NOT LIMITED TO THE WARRANTIES OF MERCHANTABILITY, FITNESS
FOR A PARTICULAR PURPOSE AND NONINFRINGEMENT. IN NO EVENT SHALL THE AUTHORS OR
COPYRIGHT HOLDERS BE LIABLE FOR ANY CLAIM, DAMAGES OR OTHER LIABILITY, WHETHER
IN AN ACTION OF CONTRACT, TORT OR OTHERWISE, ARISING FROM, OUT OF OR IN
CONNECTION WITH THE SOFTWARE OR THE USE OR OTHER DEALINGS IN THE SOFTWARE.

</editor-fold> */

#include "RenderImGui.h"
#include<imgui.h>

#include "imgui_impl_sdl.h"
#include "imgui_impl_vulkan.h"

#include <vsg/all.h>

#include <SDL2/SDL.h>

#include <SDL2/SDL_vulkan.h>

#include<filesystem>

using namespace vsgImGui;

namespace
{
    void check_vk_result(VkResult err)
    {
        if (err == 0) return;

        vsg::error("[vulkan] Error: VkResult = ", err);
    }
} // namespace

RenderImGui::RenderImGui(const vsg::ref_ptr<vsg::Window>& window, bool useClearAttachments)
{
    _init(window);
    _uploadFonts();

    if (useClearAttachments)
    {
        // clear the depth buffer before view2 gets rendered
        VkClearValue clearValue{};
        clearValue.depthStencil = { 1.0f, 0 };
        VkClearAttachment attachment{ VK_IMAGE_ASPECT_DEPTH_BIT, 1, clearValue };
        VkClearRect rect{ VkRect2D{VkOffset2D{0, 0}, VkExtent2D{window->extent2D().width, window->extent2D().height}}, 0, 1 };
        _clearAttachments = vsg::ClearAttachments::create(vsg::ClearAttachments::Attachments{ attachment }, vsg::ClearAttachments::Rects{ rect });
    }
}

RenderImGui::~RenderImGui()
{
    ImGui_ImplVulkan_Shutdown();
    ImGui::DestroyContext();
}

void RenderImGui::_init(const vsg::ref_ptr<vsg::Window>& window)
{
    IMGUI_CHECKVERSION();
    ImGui::CreateContext();

    // io set
    ImGuiIO& io = ImGui::GetIO(); (void)io;

    //font 
    auto fontPath = std::filesystem::path("C:/dev/MyFirstVsgApplication/build/myfirstvsgapplication.dir/fonts/Roboto-Regular.ttf");
    ImFontConfig font_cfg;
    font_cfg.GlyphExtraSpacing.x = 1.5f;
    
    io.Fonts->AddFontFromFileTTF(fontPath.string().c_str(), 18, &font_cfg);

    io.ConfigFlags |= ImGuiConfigFlags_NavEnableKeyboard;       // Enable Keyboard Controls
    //io.ConfigFlags |= ImGuiConfigFlags_NavEnableGamepad;      // Enable Gamepad Controls
    io.ConfigFlags |= ImGuiConfigFlags_DockingEnable;           // Enable Docking
    io.ConfigFlags |= ImGuiConfigFlags_ViewportsEnable;         // Enable Multi-Viewport / Platform Windows
    //io.ConfigFlags |= ImGuiConfigFlags_ViewportsNoTaskBarIcons;
    //io.ConfigFlags |= ImGuiConfigFlags_ViewportsNoMerge;

    ImGuiStyle& style = ImGui::GetStyle();

    style.Alpha = 1.0;
    style.DisabledAlpha = 0.4000000059604645;
    style.WindowPadding = ImVec2(10.0, 10.0);
    style.WindowRounding = 4.0;
    style.WindowBorderSize = 0.0;
    style.WindowMinSize = ImVec2(50.0, 50.0);
    style.WindowTitleAlign = ImVec2(0.5, 0.5);
    style.WindowMenuButtonPosition = ImGuiDir_Left;
    style.ChildRounding = 0.0;
    style.ChildBorderSize = 1.0;
    style.PopupRounding = 1.0;
    style.PopupBorderSize = 1.0;
    style.FramePadding = ImVec2(5.0, 3.0);
    style.FrameRounding = 3.0;
    style.FrameBorderSize = 0.0;
    style.ItemSpacing = ImVec2(6.0, 6.0);
    style.ItemInnerSpacing = ImVec2(3.0, 2.0);
    style.CellPadding = ImVec2(3.0, 3.0);
    style.IndentSpacing = 6.0;
    style.ColumnsMinSpacing = 6.0;
    style.ScrollbarSize = 13.0;
    style.ScrollbarRounding = 16.0;
    style.GrabMinSize = 20.0;
    style.GrabRounding = 4.0;
    style.TabRounding = 4.0;
    style.TabBorderSize = 1.0;
    style.TabMinWidthForCloseButton = 0.0;
    style.ColorButtonPosition = ImGuiDir_Right;
    style.ButtonTextAlign = ImVec2(0.5, 0.5);
    style.SelectableTextAlign = ImVec2(0.0, 0.0);

    style.Colors[ImGuiCol_Text] = ImVec4(0.8980392217636108, 0.8980392217636108, 0.8980392217636108, 1.0);
    style.Colors[ImGuiCol_TextDisabled] = ImVec4(0.6000000238418579, 0.6000000238418579, 0.6000000238418579, 1.0);
    style.Colors[ImGuiCol_WindowBg] = ImVec4(0.08235294371843338, 0.08235294371843338, 0.1333333402872086, 1.0);
    style.Colors[ImGuiCol_ChildBg] = ImVec4(0.0, 0.0, 0.0, 0.0);
    style.Colors[ImGuiCol_PopupBg] = ImVec4(0.08235294371843338, 0.08235294371843338, 0.1333333402872086, 1.0);
    style.Colors[ImGuiCol_Border] = ImVec4(0.4980392158031464, 0.4980392158031464, 0.4980392158031464, 0.5);
    style.Colors[ImGuiCol_BorderShadow] = ImVec4(0.0, 0.0, 0.0, 0.0);
    style.Colors[ImGuiCol_FrameBg] = ImVec4(0.4274509847164154, 0.4274509847164154, 0.4274509847164154, 0.3899999856948853);
    style.Colors[ImGuiCol_FrameBgHovered] = ImVec4(0.1529411822557449, 0.1490196138620377, 0.2235294133424759, 1.0);
    style.Colors[ImGuiCol_FrameBgActive] = ImVec4(0.1529411822557449, 0.1490196138620377, 0.2235294133424759, 1.0);
    style.Colors[ImGuiCol_TitleBg] = ImVec4(0.1890319883823395, 0.1845125257968903, 0.270386278629303, 1.0);
    style.Colors[ImGuiCol_TitleBgActive] = ImVec4(0.1340093016624451, 0.1304868459701538, 0.1974248886108398, 1.0);
    style.Colors[ImGuiCol_TitleBgCollapsed] = ImVec4(0.1529411822557449, 0.1490196138620377, 0.2235294133424759, 1.0);
    style.Colors[ImGuiCol_MenuBarBg] = ImVec4(0.1529411822557449, 0.1490196138620377, 0.2235294133424759, 1.0);
    style.Colors[ImGuiCol_ScrollbarBg] = ImVec4(0.1529411822557449, 0.1490196138620377, 0.2235294133424759, 1.0);
    style.Colors[ImGuiCol_ScrollbarGrab] = ImVec4(0.4102674424648285, 0.4051649272441864, 0.5021458864212036, 0.3648068904876709);
    style.Colors[ImGuiCol_ScrollbarGrabHovered] = ImVec4(0.4117647111415863, 0.4039215743541718, 0.501960813999176, 1.0);
    style.Colors[ImGuiCol_ScrollbarGrabActive] = ImVec4(0.4117647111415863, 0.4039215743541718, 0.501960813999176, 1.0);
    style.Colors[ImGuiCol_CheckMark] = ImVec4(0.8499497175216675, 0.8369098901748657, 1.0, 1.0);
    style.Colors[ImGuiCol_SliderGrab] = ImVec4(0.2666666805744171, 0.2666666805744171, 0.3764705955982208, 1.0);
    style.Colors[ImGuiCol_SliderGrabActive] = ImVec4(0.2254600375890732, 0.2254601716995239, 0.3090128898620605, 1.0);
    style.Colors[ImGuiCol_Button] = ImVec4(0.2666666805744171, 0.2666666805744171, 0.3764705955982208, 0.7854077219963074);
    style.Colors[ImGuiCol_ButtonHovered] = ImVec4(0.2249074280261993, 0.2249074429273605, 0.3175965547561646, 0.7843137383460999);
    style.Colors[ImGuiCol_ButtonActive] = ImVec4(0.3589677512645721, 0.3589678108692169, 0.4978540539741516, 0.7843137383460999);
    style.Colors[ImGuiCol_Header] = ImVec4(0.1882352977991104, 0.1843137294054031, 0.2705882489681244, 1.0);
    style.Colors[ImGuiCol_HeaderHovered] = ImVec4(0.4470588266849518, 0.4470588266849518, 0.8980392217636108, 0.800000011920929);
    style.Colors[ImGuiCol_HeaderActive] = ImVec4(0.529411792755127, 0.529411792755127, 0.8666666746139526, 0.800000011920929);
    style.Colors[ImGuiCol_Separator] = ImVec4(0.4980392158031464, 0.4980392158031464, 0.4980392158031464, 0.6000000238418579);
    style.Colors[ImGuiCol_SeparatorHovered] = ImVec4(0.6000000238418579, 0.6000000238418579, 0.6980392336845398, 1.0);
    style.Colors[ImGuiCol_SeparatorActive] = ImVec4(0.6980392336845398, 0.6980392336845398, 0.8980392217636108, 1.0);
    style.Colors[ImGuiCol_ResizeGrip] = ImVec4(1.0, 1.0, 1.0, 0.1000000014901161);
    style.Colors[ImGuiCol_ResizeGripHovered] = ImVec4(0.7764706015586853, 0.8196078538894653, 1.0, 0.6000000238418579);
    style.Colors[ImGuiCol_ResizeGripActive] = ImVec4(0.7764706015586853, 0.8196078538894653, 1.0, 0.8999999761581421);
    style.Colors[ImGuiCol_Tab] = ImVec4(0.3333333432674408, 0.3333333432674408, 0.6823529601097107, 0.7860000133514404);
    style.Colors[ImGuiCol_TabHovered] = ImVec4(0.4470588266849518, 0.4470588266849518, 0.8980392217636108, 0.800000011920929);
    style.Colors[ImGuiCol_TabActive] = ImVec4(0.4039215743541718, 0.4039215743541718, 0.7254902124404907, 0.8420000076293945);
    style.Colors[ImGuiCol_TabUnfocused] = ImVec4(0.2823529541492462, 0.2823529541492462, 0.5686274766921997, 0.8212000131607056);
    style.Colors[ImGuiCol_TabUnfocusedActive] = ImVec4(0.3490196168422699, 0.3490196168422699, 0.6509804129600525, 0.8371999859809875);
    style.Colors[ImGuiCol_PlotLines] = ImVec4(1.0, 1.0, 1.0, 1.0);
    style.Colors[ImGuiCol_PlotLinesHovered] = ImVec4(0.8980392217636108, 0.6980392336845398, 0.0, 1.0);
    style.Colors[ImGuiCol_PlotHistogram] = ImVec4(0.8980392217636108, 0.6980392336845398, 0.0, 1.0);
    style.Colors[ImGuiCol_PlotHistogramHovered] = ImVec4(1.0, 0.6000000238418579, 0.0, 1.0);
    style.Colors[ImGuiCol_TableHeaderBg] = ImVec4(0.2666666805744171, 0.2666666805744171, 0.3764705955982208, 1.0);
    style.Colors[ImGuiCol_TableBorderStrong] = ImVec4(0.3098039329051971, 0.3098039329051971, 0.4470588266849518, 1.0);
    style.Colors[ImGuiCol_TableBorderLight] = ImVec4(0.2588235437870026, 0.2588235437870026, 0.2784313857555389, 1.0);
    style.Colors[ImGuiCol_TableRowBg] = ImVec4(0.0, 0.0, 0.0, 0.0);
    style.Colors[ImGuiCol_TableRowBgAlt] = ImVec4(1.0, 1.0, 1.0, 0.07000000029802322);
    style.Colors[ImGuiCol_TextSelectedBg] = ImVec4(0.9999899864196777, 0.9999899864196777, 1.0, 0.2103004455566406);
    style.Colors[ImGuiCol_DragDropTarget] = ImVec4(1.0, 1.0, 0.0, 0.8999999761581421);
    style.Colors[ImGuiCol_NavHighlight] = ImVec4(0.4470588266849518, 0.4470588266849518, 0.8980392217636108, 0.800000011920929);
    style.Colors[ImGuiCol_NavWindowingHighlight] = ImVec4(1.0, 1.0, 1.0, 0.699999988079071);
    style.Colors[ImGuiCol_NavWindowingDimBg] = ImVec4(0.800000011920929, 0.800000011920929, 0.800000011920929, 0.2000000029802322);
    style.Colors[ImGuiCol_ModalWindowDimBg] = ImVec4(0.2000000029802322, 0.2000000029802322, 0.2000000029802322, 0.3499999940395355);

    _device = window->getOrCreateDevice();

    std::tie(_queueFamily, std::ignore) = _device->getPhysicalDevice()->getQueueFamily(window->traits()->queueFlags, window->getSurface());
    _queue = _device->getQueue(_queueFamily);

    ImGui_ImplSDL2_InitForVulkan((*window).getSDLwindow());
    ImGui_ImplVulkan_InitInfo init_info = {};
    init_info.Instance = *(_device->getInstance());
    init_info.PhysicalDevice = *(_device->getPhysicalDevice());
    init_info.Device = *(_device);
    init_info.QueueFamily = _queueFamily;
    init_info.Queue = *(_queue);  // ImGui doesn't use the queue so we shouldn't need to assign it, but it has an IM_ASSERT requiring it during debug build.
    init_info.PipelineCache = VK_NULL_HANDLE;

    // Create Descriptor Pool
    vsg::DescriptorPoolSizes pool_sizes = {
        {VK_DESCRIPTOR_TYPE_SAMPLER, 1000},
        {VK_DESCRIPTOR_TYPE_COMBINED_IMAGE_SAMPLER, 1000},
        {VK_DESCRIPTOR_TYPE_SAMPLED_IMAGE, 1000},
        {VK_DESCRIPTOR_TYPE_STORAGE_IMAGE, 1000},
        {VK_DESCRIPTOR_TYPE_UNIFORM_TEXEL_BUFFER, 1000},
        {VK_DESCRIPTOR_TYPE_STORAGE_TEXEL_BUFFER, 1000},
        {VK_DESCRIPTOR_TYPE_UNIFORM_BUFFER, 1000},
        {VK_DESCRIPTOR_TYPE_STORAGE_BUFFER, 1000},
        {VK_DESCRIPTOR_TYPE_UNIFORM_BUFFER_DYNAMIC, 1000},
        {VK_DESCRIPTOR_TYPE_STORAGE_BUFFER_DYNAMIC, 1000},
        {VK_DESCRIPTOR_TYPE_INPUT_ATTACHMENT, 1000} };

    uint32_t maxSets = 1000 * pool_sizes.size();
    _descriptorPool = vsg::DescriptorPool::create(_device, maxSets, pool_sizes);

    VkSurfaceCapabilitiesKHR capabilities;
    vkGetPhysicalDeviceSurfaceCapabilitiesKHR(init_info.PhysicalDevice,
        *(window->getSurface()),
        &capabilities);
    uint32_t imageCount = 3;
    imageCount =
        std::max(imageCount,
            capabilities.minImageCount); // Vulkan spec requires
                                         // minImageCount to be 1 or greater
    if (capabilities.maxImageCount > 0)
        imageCount = std::min(
            imageCount,
            capabilities.maxImageCount); // Vulkan spec specifies 0 as being
                                         // unlimited number of images

    init_info.DescriptorPool = *(_descriptorPool);
    init_info.Allocator = nullptr;
    init_info.MinImageCount = capabilities.minImageCount;
    init_info.ImageCount = imageCount;
    init_info.CheckVkResultFn = check_vk_result;

    ImGui_ImplVulkan_Init(&init_info, *window->getOrCreateRenderPass());
}

void RenderImGui::_uploadFonts()
{

    auto commandPool = vsg::CommandPool::create(_device, _queueFamily, VK_COMMAND_POOL_CREATE_TRANSIENT_BIT);
    auto fence = vsg::Fence::create(_device);

    uint64_t timeout = 1000000000;
    vsg::submitCommandsToQueue(commandPool, fence, timeout, _queue, [&](vsg::CommandBuffer& commandBuffer)
        {
            ImGui_ImplVulkan_CreateFontsTexture(commandBuffer);
        });

    VkResult result = fence->status();
    while (result == VK_NOT_READY)
    {
        result = fence->wait(timeout);
    }

    if (result != VK_SUCCESS)
    {
        vsg::error("RenderImGui::_uploadFonts(), fence->state() = ", result);
    }

    ImGui_ImplVulkan_DestroyFontUploadObjects();
}

void RenderImGui::add(const Component& component)
{
    _components.push_back(component);
}

bool RenderImGui::renderComponents() const
{
    ImGui_ImplVulkan_NewFrame();
    ImGui_ImplSDL2_NewFrame();
    ImGui::NewFrame();

    bool visibleComponents = false;
    for (auto& component : _components)
    {
        if (component()) visibleComponents = true;
    }

    ImGui::EndFrame();
    ImGui::Render();

    ImGui::UpdatePlatformWindows();
    ImGui::RenderPlatformWindowsDefault();

    return visibleComponents;
}

void RenderImGui::record(vsg::CommandBuffer& commandBuffer) const
{
    bool visibleComponents = renderComponents();

    if (visibleComponents)
    {
        if (_clearAttachments) _clearAttachments->record(commandBuffer);

        ImDrawData* draw_data = ImGui::GetDrawData();
        if (draw_data)
            ImGui_ImplVulkan_RenderDrawData(draw_data, &(*commandBuffer));
    }
}
