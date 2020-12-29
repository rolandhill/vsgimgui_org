#include "imguiNode.h"
#include "drawImgui.h"

#include "imgui/imgui.h"
#include "imgui/imgui_impl_vulkan.h"

static void check_vk_result(VkResult err)
{
    if (err == 0)
        return;
    std::cout << "[vulkan] Error: VkResult = " << err << std::endl;
    if (err < 0)
        abort();
}

imguiNode::imguiNode(vsg::ref_ptr<vsg::Window> window):
    _window(window)
{
    _time = std::chrono::system_clock::now();

    // Setup Dear ImGui context
    IMGUI_CHECKVERSION();
    ImGui::CreateContext();
    ImGuiIO& io = ImGui::GetIO(); (void)io;
    //io.ConfigFlags |= ImGuiConfigFlags_NavEnableKeyboard;     // Enable Keyboard Controls
    //io.ConfigFlags |= ImGuiConfigFlags_NavEnableGamepad;      // Enable Gamepad Controls

    // Setup Dear ImGui style
    ImGui::StyleColorsDark();
    //ImGui::StyleColorsClassic();

    // Load Fonts
    // - If no fonts are loaded, dear imgui will use the default font. You can also load multiple fonts and use ImGui::PushFont()/PopFont() to select them.
    // - AddFontFromFileTTF() will return the ImFont* so you can store it if you need to select the font among multiple.
    // - If the file cannot be loaded, the function will return NULL. Please handle those errors in your application (e.g. use an assertion, or display an error and quit).
    // - The fonts will be rasterized at a given size (w/ oversampling) and stored into a texture when calling ImFontAtlas::Build()/GetTexDataAsXXXX(), which ImGui_ImplXXXX_NewFrame below will call.
    // - Read 'docs/FONTS.md' for more instructions and details.
    // - Remember that in C/C++ if you want to include a backslash \ in a string literal you need to write a double backslash \\ !
    //io.Fonts->AddFontDefault();
    //io.Fonts->AddFontFromFileTTF("../../misc/fonts/Roboto-Medium.ttf", 16.0f);
    //io.Fonts->AddFontFromFileTTF("../../misc/fonts/Cousine-Regular.ttf", 15.0f);
    //io.Fonts->AddFontFromFileTTF("../../misc/fonts/DroidSans.ttf", 16.0f);
    //io.Fonts->AddFontFromFileTTF("../../misc/fonts/ProggyTiny.ttf", 10.0f);
    //ImFont* font = io.Fonts->AddFontFromFileTTF("c:\\Windows\\Fonts\\ArialUni.ttf", 18.0f, NULL, io.Fonts->GetGlyphRangesJapanese());
    //IM_ASSERT(font != NULL);


    // Upload Fonts
    // {
    //     // Use any command queue
    //     VkCommandPool command_pool = wd->Frames[wd->FrameIndex].CommandPool;
    //     VkCommandBuffer command_buffer = wd->Frames[wd->FrameIndex].CommandBuffer;

    //     err = vkResetCommandPool(g_Device, command_pool, 0);
    //     check_vk_result(err);
    //     VkCommandBufferBeginInfo begin_info = {};
    //     begin_info.sType = VK_STRUCTURE_TYPE_COMMAND_BUFFER_BEGIN_INFO;
    //     begin_info.flags |= VK_COMMAND_BUFFER_USAGE_ONE_TIME_SUBMIT_BIT;
    //     err = vkBeginCommandBuffer(command_buffer, &begin_info);
    //     check_vk_result(err);

    //     ImGui_ImplVulkan_CreateFontsTexture(command_buffer);

    //     VkSubmitInfo end_info = {};
    //     end_info.sType = VK_STRUCTURE_TYPE_SUBMIT_INFO;
    //     end_info.commandBufferCount = 1;
    //     end_info.pCommandBuffers = &command_buffer;
    //     err = vkEndCommandBuffer(command_buffer);
    //     check_vk_result(err);
    //     err = vkQueueSubmit(g_Queue, 1, &end_info, VK_NULL_HANDLE);
    //     check_vk_result(err);

    //     err = vkDeviceWaitIdle(g_Device);
    //     check_vk_result(err);
    //     ImGui_ImplVulkan_DestroyFontUploadObjects();
    // }
    unsigned char* pixels;
    int width, height;
    io.Fonts->GetTexDataAsRGBA32(&pixels, &width, &height);

    vsg::vec4* vdata = reinterpret_cast<vsg::vec4*>(pixels);
    // auto textureData = vsg::vec4Array2D::create(width, height, vdata);
    auto textureData = vsg::vec4Array2D::create(width, height);
    // textureData->assign(width, height, vdata);
    textureData->getLayout().format = VK_FORMAT_R32G32B32A32_SFLOAT;
    size_t i = 0;
    for(size_t r = 0; r < height; r++)
    {
        for(size_t c=0; c< width; c++)
        {
            textureData->set(c, r, vdata[i++]);
        }
    }
    
    // set up graphics pipeline
    vsg::DescriptorSetLayoutBindings descriptorBindings
    {
        {0, VK_DESCRIPTOR_TYPE_COMBINED_IMAGE_SAMPLER, 1, VK_SHADER_STAGE_FRAGMENT_BIT, nullptr} // { binding, descriptorTpe, descriptorCount, stageFlags, pImmutableSamplers}
    };

    auto descriptorSetLayout = vsg::DescriptorSetLayout::create(descriptorBindings);

    vsg::PushConstantRanges pushConstantRanges
    {
        {VK_SHADER_STAGE_VERTEX_BIT, 0, 128} // projection view, and model matrices, actual push constant calls autoaatically provided by the VSG's DispatchTraversal
        // {VK_SHADER_STAGE_VERTEX_BIT, 0, 16} // projection view, and model matrices, actual push constant calls autoaatically provided by the VSG's DispatchTraversal
    };

    vsg::VertexInputState::Bindings vertexBindingsDescriptions
    {
        VkVertexInputBindingDescription{0, sizeof(vsg::vec2), VK_VERTEX_INPUT_RATE_VERTEX}, // vertex data
        VkVertexInputBindingDescription{1, sizeof(vsg::vec2), VK_VERTEX_INPUT_RATE_VERTEX},  // tex coord data
        VkVertexInputBindingDescription{2, sizeof(vsg::vec4), VK_VERTEX_INPUT_RATE_VERTEX} // colour data
    };

    vsg::VertexInputState::Attributes vertexAttributeDescriptions
    {
        VkVertexInputAttributeDescription{0, 0, VK_FORMAT_R32G32_SFLOAT, 0}, // vertex data
        VkVertexInputAttributeDescription{1, 1, VK_FORMAT_R32G32_SFLOAT, 0},    // tex coord data
        VkVertexInputAttributeDescription{2, 2, VK_FORMAT_R32G32B32A32_SFLOAT, 0} // colour data
    };

    vsg::GraphicsPipelineStates pipelineStates
    {
        vsg::VertexInputState::create( vertexBindingsDescriptions, vertexAttributeDescriptions ),
        vsg::InputAssemblyState::create(),
        vsg::RasterizationState::create(),
        vsg::MultisampleState::create(),
        vsg::ColorBlendState::create(),
        vsg::DepthStencilState::create()
    };

    // load shaders
    vsg::ref_ptr<vsg::ShaderStage> vertexShader = vsg::ShaderStage::read(VK_SHADER_STAGE_VERTEX_BIT, "main", "/home/roland/Data/Source/sigui/external/imgui/vulkan/vert_imgui.spv");
    vsg::ref_ptr<vsg::ShaderStage> fragmentShader = vsg::ShaderStage::read(VK_SHADER_STAGE_FRAGMENT_BIT, "main", "/home/roland/Data/Source/sigui/external/imgui/vulkan/frag_imgui.spv");
    if (!vertexShader || !fragmentShader)
    {
        std::cout<<"Could not create shaders."<<std::endl;
        return;
    }

    auto pipelineLayout = vsg::PipelineLayout::create(vsg::DescriptorSetLayouts{descriptorSetLayout}, pushConstantRanges);
    auto graphicsPipeline = vsg::GraphicsPipeline::create(pipelineLayout, vsg::ShaderStages{vertexShader, fragmentShader}, pipelineStates);
    auto bindGraphicsPipeline = vsg::BindGraphicsPipeline::create(graphicsPipeline);

    // create texture image and associated DescriptorSets and binding
    auto texture = vsg::DescriptorImage::create(vsg::Sampler::create(), textureData, 0, 0, VK_DESCRIPTOR_TYPE_COMBINED_IMAGE_SAMPLER);
    // Store our identifier
    // io.Fonts->TexID = (ImTextureID)(intptr_t)g_FontImage;
    _imageInfo = texture->imageInfoList[0];

    _descriptorSet = vsg::DescriptorSet::create(descriptorSetLayout, vsg::Descriptors{texture});
    auto bindDescriptorSet = vsg::BindDescriptorSet::create(VK_PIPELINE_BIND_POINT_GRAPHICS, graphicsPipeline->layout, 0, _descriptorSet);

    // create StateGroup as the root of the scene/command graph to hold the GraphicsProgram, and binding of Descriptors to decorate the whole graph
    add(bindGraphicsPipeline);
    add(bindDescriptorSet);

}

bool imguiNode::init()
{
    VkDescriptorPool descriptorPool = VK_NULL_HANDLE;
// Create Descriptor Pool
    {
        VkDescriptorPoolSize pool_sizes[] =
        {
            { VK_DESCRIPTOR_TYPE_SAMPLER, 1000 },
            { VK_DESCRIPTOR_TYPE_COMBINED_IMAGE_SAMPLER, 1000 },
            { VK_DESCRIPTOR_TYPE_SAMPLED_IMAGE, 1000 },
            { VK_DESCRIPTOR_TYPE_STORAGE_IMAGE, 1000 },
            { VK_DESCRIPTOR_TYPE_UNIFORM_TEXEL_BUFFER, 1000 },
            { VK_DESCRIPTOR_TYPE_STORAGE_TEXEL_BUFFER, 1000 },
            { VK_DESCRIPTOR_TYPE_UNIFORM_BUFFER, 1000 },
            { VK_DESCRIPTOR_TYPE_STORAGE_BUFFER, 1000 },
            { VK_DESCRIPTOR_TYPE_UNIFORM_BUFFER_DYNAMIC, 1000 },
            { VK_DESCRIPTOR_TYPE_STORAGE_BUFFER_DYNAMIC, 1000 },
            { VK_DESCRIPTOR_TYPE_INPUT_ATTACHMENT, 1000 }
        };
        VkDescriptorPoolCreateInfo pool_info = {};
        pool_info.sType = VK_STRUCTURE_TYPE_DESCRIPTOR_POOL_CREATE_INFO;
        pool_info.flags = VK_DESCRIPTOR_POOL_CREATE_FREE_DESCRIPTOR_SET_BIT;
        pool_info.maxSets = 1000 * IM_ARRAYSIZE(pool_sizes);
        pool_info.poolSizeCount = (uint32_t)IM_ARRAYSIZE(pool_sizes);
        pool_info.pPoolSizes = pool_sizes;
        VkResult err = vkCreateDescriptorPool(_window->getDevice()->getDevice(), &pool_info, NULL, &descriptorPool);
        check_vk_result(err);
    }

    // Setup Platform/Renderer backends
    // ImGui_ImplGlfw_InitForVulkan(window, true);
    int graphicsFamily = -1;
    std::tie(graphicsFamily, std::ignore) = _window->getPhysicalDevice()->getQueueFamily(VK_QUEUE_GRAPHICS_BIT, _window->getSurface());
    auto queue = _window->getDevice()->getQueue(graphicsFamily);

    ImGui_ImplVulkan_InitInfo init_info = {};
    init_info.Instance = _window->getInstance()->getInstance();
    init_info.PhysicalDevice = _window->getPhysicalDevice()->getPhysicalDevice();
    init_info.Device = _window->getDevice()->getDevice();
    init_info.QueueFamily = VK_QUEUE_GRAPHICS_BIT;
    init_info.Queue = *queue;
    init_info.PipelineCache = VK_NULL_HANDLE;
    init_info.DescriptorPool = descriptorPool;
    init_info.Allocator = NULL;
    init_info.MinImageCount = 2;
    init_info.ImageCount = 3; // ************ change
    init_info.CheckVkResultFn = check_vk_result;
    ImGui_ImplVulkan_Init(&init_info, *(_window->getRenderPass()));

    auto draw = drawImgui::create(_window->getDevice(), _descriptorSet, _imageInfo);
    addChild(draw);

    // _initialised = true;

    return true;
}

imguiNode::~imguiNode()
{
    // std::cout << "imguiNode destructor" << std::endl;
}

void imguiNode::render()
{
    ImGuiIO& io = ImGui::GetIO();
    IM_ASSERT(io.Fonts->IsBuilt() && "Font atlas not built! It is generally built by the renderer backend. Missing call to renderer _NewFrame() function? e.g. ImGui_ImplOpenGL3_NewFrame().");

    uint32_t w = _window->extent2D().width;
    uint32_t h = _window->extent2D().height;
    uint32_t display_w = _window->framebuffer(0)->width();
    uint32_t display_h = _window->framebuffer(0)->height();

    io.DisplaySize = ImVec2((float)w, (float)h);
    if (w > 0 && h > 0)
        io.DisplayFramebufferScale = ImVec2((float)display_w / w, (float)display_h / h);

    // Setup time step
    auto current_time = std::chrono::system_clock::now();
    std::chrono::duration<double> diff = current_time-_time;
    io.DeltaTime = diff.count();
    _time = current_time;

    bool open = true;
    ImGui::NewFrame();
    ImGui::ShowDemoWindow(&open);
    ImGui::Render();
}
