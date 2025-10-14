
#define STB_IMAGE_WRITE_STATIC
#include "lib/ImageEditor.h" 
#include "imgui/imgui.h"
#include "imgui/backends/imgui_impl_glfw.h"
#include "imgui/backends/imgui_impl_opengl3.h"
#include "imgui/ImGuiFileDialog.h"
#include <GLFW/glfw3.h>
#include <GL/gl.h>
#include <filesystem>
#include <vector>
#include <string>
#include <algorithm>
#include <iostream>

// ===== UITheme Class =====
class UITheme {
public:
    static void ApplyModernTheme();
};

void UITheme::ApplyModernTheme()
{
    ImGuiStyle& style = ImGui::GetStyle();
    ImVec4* colors = style.Colors;

    // --- Modern Color Palette (Dark with accent colors) ---
    colors[ImGuiCol_WindowBg] = ImVec4(0.09f, 0.09f, 0.11f, 0.98f);
    colors[ImGuiCol_ChildBg] = ImVec4(0.12f, 0.12f, 0.14f, 0.95f);
    colors[ImGuiCol_PopupBg] = ImVec4(0.11f, 0.11f, 0.13f, 0.98f);
    colors[ImGuiCol_Border] = ImVec4(0.25f, 0.25f, 0.28f, 0.50f);
    colors[ImGuiCol_BorderShadow] = ImVec4(0.00f, 0.00f, 0.00f, 0.00f);
    
    // Frame backgrounds
    colors[ImGuiCol_FrameBg] = ImVec4(0.16f, 0.16f, 0.18f, 1.0f);
    colors[ImGuiCol_FrameBgHovered] = ImVec4(0.22f, 0.22f, 0.25f, 1.0f);
    colors[ImGuiCol_FrameBgActive] = ImVec4(0.28f, 0.28f, 0.32f, 1.0f);
    
    // Title bar
    colors[ImGuiCol_TitleBg] = ImVec4(0.08f, 0.08f, 0.10f, 1.0f);
    colors[ImGuiCol_TitleBgActive] = ImVec4(0.10f, 0.10f, 0.12f, 1.0f);
    colors[ImGuiCol_TitleBgCollapsed] = ImVec4(0.08f, 0.08f, 0.10f, 0.75f);
    
    // Menu bar
    colors[ImGuiCol_MenuBarBg] = ImVec4(0.10f, 0.10f, 0.12f, 1.0f);
    
    // Scrollbar
    colors[ImGuiCol_ScrollbarBg] = ImVec4(0.10f, 0.10f, 0.12f, 0.53f);
    colors[ImGuiCol_ScrollbarGrab] = ImVec4(0.25f, 0.25f, 0.28f, 1.0f);
    colors[ImGuiCol_ScrollbarGrabHovered] = ImVec4(0.35f, 0.35f, 0.38f, 1.0f);
    colors[ImGuiCol_ScrollbarGrabActive] = ImVec4(0.45f, 0.45f, 0.48f, 1.0f);
    
    // Checkboxes & Radio buttons
    colors[ImGuiCol_CheckMark] = ImVec4(0.45f, 0.70f, 1.00f, 1.0f);
    
    // Sliders
    colors[ImGuiCol_SliderGrab] = ImVec4(0.40f, 0.60f, 0.95f, 1.0f);
    colors[ImGuiCol_SliderGrabActive] = ImVec4(0.50f, 0.70f, 1.00f, 1.0f);
    
    // Buttons - Accent blue gradient
    colors[ImGuiCol_Button] = ImVec4(0.26f, 0.45f, 0.75f, 1.0f);
    colors[ImGuiCol_ButtonHovered] = ImVec4(0.35f, 0.55f, 0.85f, 1.0f);
    colors[ImGuiCol_ButtonActive] = ImVec4(0.45f, 0.65f, 0.95f, 1.0f);
    
    // Headers (collapsible sections)
    colors[ImGuiCol_Header] = ImVec4(0.22f, 0.40f, 0.70f, 0.80f);
    colors[ImGuiCol_HeaderHovered] = ImVec4(0.30f, 0.50f, 0.80f, 0.90f);
    colors[ImGuiCol_HeaderActive] = ImVec4(0.38f, 0.60f, 0.90f, 1.0f);
    
    // Separator
    colors[ImGuiCol_Separator] = ImVec4(0.25f, 0.25f, 0.28f, 0.60f);
    colors[ImGuiCol_SeparatorHovered] = ImVec4(0.35f, 0.55f, 0.85f, 0.78f);
    colors[ImGuiCol_SeparatorActive] = ImVec4(0.45f, 0.65f, 0.95f, 1.0f);
    
    // Resize grip
    colors[ImGuiCol_ResizeGrip] = ImVec4(0.26f, 0.45f, 0.75f, 0.40f);
    colors[ImGuiCol_ResizeGripHovered] = ImVec4(0.35f, 0.55f, 0.85f, 0.70f);
    colors[ImGuiCol_ResizeGripActive] = ImVec4(0.45f, 0.65f, 0.95f, 0.90f);
    
    // Tabs
    colors[ImGuiCol_Tab] = ImVec4(0.16f, 0.28f, 0.48f, 0.86f);
    colors[ImGuiCol_TabHovered] = ImVec4(0.35f, 0.55f, 0.85f, 0.80f);
    colors[ImGuiCol_TabActive] = ImVec4(0.28f, 0.48f, 0.78f, 1.0f);
    colors[ImGuiCol_TabUnfocused] = ImVec4(0.12f, 0.20f, 0.35f, 0.97f);
    colors[ImGuiCol_TabUnfocusedActive] = ImVec4(0.18f, 0.32f, 0.55f, 1.0f);
    
    // Text
    colors[ImGuiCol_Text] = ImVec4(0.95f, 0.95f, 0.97f, 1.0f);
    colors[ImGuiCol_TextDisabled] = ImVec4(0.50f, 0.50f, 0.53f, 1.0f);
    colors[ImGuiCol_TextSelectedBg] = ImVec4(0.35f, 0.55f, 0.85f, 0.43f);
    
    // Modern rounded style
    style.WindowRounding = 10.0f;
    style.ChildRounding = 8.0f;
    style.FrameRounding = 6.0f;
    style.PopupRounding = 6.0f;
    style.ScrollbarRounding = 9.0f;
    style.GrabRounding = 5.0f;
    style.TabRounding = 6.0f;
    
    // Spacing & padding for better UX
    style.WindowPadding = ImVec2(14, 14);
    style.FramePadding = ImVec2(10, 6);
    style.ItemSpacing = ImVec2(10, 8);
    style.ItemInnerSpacing = ImVec2(8, 6);
    style.IndentSpacing = 22.0f;
    style.ScrollbarSize = 14.0f;
    style.GrabMinSize = 12.0f;
    
    // Borders
    style.WindowBorderSize = 1.0f;
    style.ChildBorderSize = 1.0f;
    style.PopupBorderSize = 1.0f;
    style.FrameBorderSize = 0.0f;
    style.TabBorderSize = 0.0f;
}

// ===== UIComponents Class =====
class UIComponents {
public:
    static void DrawSectionHeader(const char* text);
    static bool IconButton(const char* label, const ImVec2& size, bool enabled = true, ImVec4 color = ImVec4(0,0,0,0));
};

void UIComponents::DrawSectionHeader(const char* text) {
    ImGui::Spacing();
    ImGui::PushStyleColor(ImGuiCol_Text, ImVec4(0.60f, 0.80f, 1.0f, 1.0f));
    ImGui::Text("%s", text);
    ImGui::PopStyleColor();
    ImGui::Separator();
    ImGui::Spacing();
}

bool UIComponents::IconButton(const char* label, const ImVec2& size, bool enabled, ImVec4 color) {
    if (!enabled) {
        ImGui::PushStyleVar(ImGuiStyleVar_Alpha, 0.5f);
    }
    if (color.x != 0 || color.y != 0 || color.z != 0) {
        ImGui::PushStyleColor(ImGuiCol_Button, color);
        ImGui::PushStyleColor(ImGuiCol_ButtonHovered, ImVec4(color.x * 1.2f, color.y * 1.2f, color.z * 1.2f, color.w));
        ImGui::PushStyleColor(ImGuiCol_ButtonActive, ImVec4(color.x * 1.4f, color.y * 1.4f, color.z * 1.4f, color.w));
    }
    
    bool result = enabled && ImGui::Button(label, size);
    
    if (color.x != 0 || color.y != 0 || color.z != 0) {
        ImGui::PopStyleColor(3);
    }
    if (!enabled) {
        ImGui::PopStyleVar();
    }
    return result;
}

// ===== TextureManager Class =====
class TextureManager {
public:
    TextureManager();
    ~TextureManager();
    
    void updateTexture(const Image& image);
    GLuint getTextureID() const { return texture; }
    int getWidth() const { return texW; }
    int getHeight() const { return texH; }
    
private:
    GLuint texture;
    int texW;
    int texH;
    
    Image createPreviewImage(const Image& original);
    
    static const int MAX_PREVIEW_WIDTH = 1920;
    static const int MAX_PREVIEW_HEIGHT = 1080;
};

TextureManager::TextureManager() : texture(0), texW(0), texH(0) {
}

TextureManager::~TextureManager() {
    if (texture) {
        glDeleteTextures(1, &texture);
    }
}

Image TextureManager::createPreviewImage(const Image& original) {
    int width = original.width;
    int height = original.height;
    
    float scaleX = 1.0f;
    float scaleY = 1.0f;
    
    if (width > MAX_PREVIEW_WIDTH) {
        scaleX = (float)MAX_PREVIEW_WIDTH / width;
    }
    if (height > MAX_PREVIEW_HEIGHT) {
        scaleY = (float)MAX_PREVIEW_HEIGHT / height;
    }
    
    float scale = std::min(scaleX, scaleY);
    
    // If image is small enough, return copy of original
    if (scale >= 1.0f) {
        return original;
    }
    
    // Calculate new dimensions
    int newWidth = (int)(width * scale);
    int newHeight = (int)(height * scale);
    
    // Create downsampled image using nearest neighbor (fast)
    Image preview(newWidth, newHeight);
    
    for (int y = 0; y < newHeight; y++) {
        for (int x = 0; x < newWidth; x++) {
            int srcX = (int)(x / scale);
            int srcY = (int)(y / scale);
            
            // Clamp to avoid out of bounds
            srcX = std::min(srcX, width - 1);
            srcY = std::min(srcY, height - 1);
            
            for (int c = 0; c < 3; c++) {
                preview(x, y, c) = original(srcX, srcY, c);
            }
        }
    }
    
    return preview;
}

void TextureManager::updateTexture(const Image& image) {
    if (texture == 0) {
        glGenTextures(1, &texture);
    }

    // Create preview version if image is too large
    Image previewImg = createPreviewImage(image);
    
    glBindTexture(GL_TEXTURE_2D, texture);
    glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MIN_FILTER, GL_LINEAR);
    glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MAG_FILTER, GL_LINEAR);
    glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_WRAP_S, GL_CLAMP);
    glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_WRAP_T, GL_CLAMP);

    // Convert RGB to RGBA if needed
    std::vector<unsigned char> rgbaData;
    if (previewImg.channels == 3) {
        rgbaData.resize(previewImg.width * previewImg.height * 4);
        for (int i = 0; i < previewImg.width * previewImg.height; ++i) {
            rgbaData[i * 4 + 0] = previewImg.imageData[i * 3 + 0];
            rgbaData[i * 4 + 1] = previewImg.imageData[i * 3 + 1];
            rgbaData[i * 4 + 2] = previewImg.imageData[i * 3 + 2];
            rgbaData[i * 4 + 3] = 255;
        }
        glTexImage2D(GL_TEXTURE_2D, 0, GL_RGBA, previewImg.width, previewImg.height, 0, GL_RGBA, GL_UNSIGNED_BYTE, rgbaData.data());
    } else {
        glTexImage2D(GL_TEXTURE_2D, 0, GL_RGBA, previewImg.width, previewImg.height, 0, GL_RGBA, GL_UNSIGNED_BYTE, previewImg.imageData);
    }
    
    texW = previewImg.width;
    texH = previewImg.height;
}



int main()
{
    // --- Setup GLFW + ImGui ---
    if (!glfwInit()) return -1;
    
    glfwWindowHint(GLFW_CONTEXT_VERSION_MAJOR, 3);
    glfwWindowHint(GLFW_CONTEXT_VERSION_MINOR, 3);
    glfwWindowHint(GLFW_OPENGL_PROFILE, GLFW_OPENGL_CORE_PROFILE);
    glfwWindowHint(GLFW_MAXIMIZED, GLFW_FALSE); // Start windowed but allow maximizing
    
    GLFWwindow* window = glfwCreateWindow(1600, 900, "Image Editor Pro", NULL, NULL);
    if (!window) {
        glfwTerminate();
        return -1;
    }
    glfwMakeContextCurrent(window);
    glfwSwapInterval(1);

    IMGUI_CHECKVERSION();
    ImGui::CreateContext();
    ImGuiIO& io = ImGui::GetIO();
    io.ConfigFlags |= ImGuiConfigFlags_NavEnableKeyboard;
    
    // Load better fonts if available
    ImFont* mainFont = io.Fonts->AddFontFromFileTTF("C:\\Windows\\Fonts\\segoeui.ttf", 16.0f);
    ImFont* headerFont = io.Fonts->AddFontFromFileTTF("C:\\Windows\\Fonts\\segoeui.ttf", 20.0f);
    if (!mainFont) {
        io.Fonts->AddFontDefault();
    }
    
    UITheme::ApplyModernTheme();
    ImGui_ImplGlfw_InitForOpenGL(window, true);
    ImGui_ImplOpenGL3_Init("#version 330");

    ImageEditor* editor = nullptr;
    TextureManager textureManager;
    std::string currentFile;
    int originalWidth = 0, originalHeight = 0;
    bool needsTextureUpdate = false;

    // --- UI variables ---
    static char loadPath[256] = "images/luffy.jpg";
    static char savePath[256] = "generated/output.jpg";
    double brightnessFactor = 1.0;
    int blurRadius = 5;
    int rotationAngle = 90;
    double sunlightIntensity = 0.02;
    double skewDegree = 15.0;
    int oilRadius = 3;
    int oilLevels = 10;
    bool decorated = false;
    float frameColor[3] = {0.83f, 0.69f, 0.21f}; // gold
    bool imageLoaded = false;
    std::string statusMessage = "";
    ImVec4 statusColor = ImVec4(1.0f, 1.0f, 1.0f, 1.0f);
    
    IGFD::FileDialog fileDialog;
    
    // Responsive layout constants
    const float leftPanelWidthRatio = 0.30f; // 30% of screen width
    const float minLeftPanelWidth = 400.0f;
    const float maxLeftPanelWidth = 550.0f;

    while (!glfwWindowShouldClose(window))
    {
        glfwPollEvents();
        ImGui_ImplOpenGL3_NewFrame();
        ImGui_ImplGlfw_NewFrame();
        ImGui::NewFrame();

        // Auto-update texture when image changes
        if (imageLoaded && needsTextureUpdate && editor) {
            textureManager.updateTexture(editor->getCurrentImage());
            needsTextureUpdate = false;
        }

        // Get current window size for responsive layout
        int display_w, display_h;
        glfwGetFramebufferSize(window, &display_w, &display_h);
        float windowWidth = static_cast<float>(display_w);
        float windowHeight = static_cast<float>(display_h);
        
        // Calculate responsive left panel width
        float leftPanelWidth = windowWidth * leftPanelWidthRatio;
        leftPanelWidth = std::max(minLeftPanelWidth, std::min(maxLeftPanelWidth, leftPanelWidth));
        
        // Left Panel - Controls
        ImGui::SetNextWindowPos(ImVec2(0, 0));
        ImGui::SetNextWindowSize(ImVec2(leftPanelWidth, windowHeight));
        ImGui::Begin("Controls", nullptr,
                     ImGuiWindowFlags_NoResize | ImGuiWindowFlags_NoMove | ImGuiWindowFlags_NoCollapse);

        // Header with title
        ImGui::PushStyleColor(ImGuiCol_Text, ImVec4(0.60f, 0.80f, 1.0f, 1.0f));
        ImGui::SetWindowFontScale(1.3f);
        ImGui::Text("  Image Editor Pro");
        ImGui::SetWindowFontScale(1.0f);
        ImGui::PopStyleColor();
        ImGui::Separator();
        ImGui::Spacing();

        // File Operations Section
        UIComponents::DrawSectionHeader("  File Operations");
        
        // Load Image
        ImGui::PushStyleColor(ImGuiCol_FrameBg, ImVec4(0.14f, 0.14f, 0.16f, 1.0f));
        ImGui::SetNextItemWidth(-130);
        ImGui::InputTextWithHint("##LoadPath", "Enter image path...", loadPath, IM_ARRAYSIZE(loadPath));
        ImGui::PopStyleColor();
        ImGui::SameLine();
        if (ImGui::Button(" Browse... ", ImVec2(110, 0))) {
            ImGuiFileDialog::Instance()->OpenDialog("ChooseFileLoad", "Choose Image File", "Image files (*.jpg *.jpeg *.png *.bmp *.tga){.jpg,.jpeg,.png,.bmp,.tga},.*");
        }
        
        ImGui::Spacing();
        ImVec4 loadButtonColor = ImVec4(0.20f, 0.55f, 0.30f, 1.0f);
        if (UIComponents::IconButton("  Load Image", ImVec2(-1, 40), true, loadButtonColor)) {
            if (std::filesystem::exists(loadPath)) {
                delete editor;
                editor = new ImageEditor(loadPath);
                currentFile = loadPath;
                originalWidth = editor->getCurrentImage().width;
                originalHeight = editor->getCurrentImage().height;
                imageLoaded = true;
                needsTextureUpdate = true;
                statusMessage = "  Image loaded successfully!";
                statusColor = ImVec4(0.3f, 0.9f, 0.3f, 1.0f);
            } else {
                statusMessage = "  Failed to load image!";
                statusColor = ImVec4(1.0f, 0.3f, 0.3f, 1.0f);
            }
        }
        
        ImGui::Spacing();
        ImGui::Spacing();

        // Save Image
        ImGui::PushStyleColor(ImGuiCol_FrameBg, ImVec4(0.14f, 0.14f, 0.16f, 1.0f));
        ImGui::SetNextItemWidth(-130);
        ImGui::InputTextWithHint("##SavePath", "Enter save path...", savePath, IM_ARRAYSIZE(savePath));
        ImGui::PopStyleColor();
        ImGui::SameLine();
        if (ImGui::Button(" Save As... ", ImVec2(110, 0))) {
            ImGuiFileDialog::Instance()->OpenDialog("ChooseFileSave", "Save Image File", "Image files (*.jpg *.jpeg *.png *.bmp *.tga){.jpg,.jpeg,.png,.bmp,.tga},.*");
        }
        
        ImGui::Spacing();
        ImVec4 saveButtonColor = ImVec4(0.25f, 0.45f, 0.75f, 1.0f);
        if (UIComponents::IconButton("  Save Image", ImVec2(-1, 40), imageLoaded, saveButtonColor)) {
            if (editor->save(savePath), true) {
                statusMessage = "  Image saved successfully!";
                statusColor = ImVec4(0.3f, 0.9f, 0.3f, 1.0f);
            }
        }
        
        ImGui::Spacing();
        ImGui::Spacing();

        // Filters Section with Tabs
        if (!imageLoaded) {
            ImGui::Spacing();
            ImGui::PushStyleColor(ImGuiCol_Text, ImVec4(1.0f, 0.8f, 0.2f, 1.0f));
            ImGui::TextWrapped("  Load an image to begin editing and unlock all filters and adjustments.");
            ImGui::PopStyleColor();
        } else {
            UIComponents::DrawSectionHeader("  Filters & Effects");
            
            // Tabs for organized filters
            if (ImGui::BeginTabBar("FilterTabs", ImGuiTabBarFlags_None)) {
                
                // BASIC TAB
                if (ImGui::BeginTabItem("  Basic  ")) {
                    ImGui::Spacing();
                    
                    // Grid layout for basic filters (responsive)
                    float availWidth = ImGui::GetContentRegionAvail().x;
                    float buttonWidth = (availWidth - 20) / 3.0f;
                    
                    if (ImGui::Button(" Grayscale", ImVec2(buttonWidth, 38))) {
                        editor->grayscale();
                        needsTextureUpdate = true;
                        statusMessage = "  Grayscale applied!";
                        statusColor = ImVec4(0.3f, 0.9f, 0.3f, 1.0f);
                    }
                    ImGui::SameLine();
                    if (ImGui::Button("Black & White", ImVec2(buttonWidth, 38))) {
                        editor->blackAndWhite();
                        needsTextureUpdate = true;
                        statusMessage = "  B&W applied!";
                        statusColor = ImVec4(0.3f, 0.9f, 0.3f, 1.0f);
                    }
                    ImGui::SameLine();
                    if (ImGui::Button(" Invert", ImVec2(buttonWidth, 38))) {
                        editor->invert();
                        needsTextureUpdate = true;
                        statusMessage = "  Colors inverted!";
                        statusColor = ImVec4(0.3f, 0.9f, 0.3f, 1.0f);
                    }

                    if (ImGui::Button("Edge Detection", ImVec2(buttonWidth, 38))) {
                        editor->edgeDetection();
                        needsTextureUpdate = true;
                        statusMessage = "  Edge detection applied!";
                        statusColor = ImVec4(0.3f, 0.9f, 0.3f, 1.0f);
                    }
                    ImGui::SameLine();
                    if (ImGui::Button(" Flip Vertical", ImVec2(buttonWidth, 38))) {
                        editor->flipVertical();
                        needsTextureUpdate = true;
                        statusMessage = "  Flipped vertically!";
                        statusColor = ImVec4(0.3f, 0.9f, 0.3f, 1.0f);
                    }
                    ImGui::SameLine();
                    if (ImGui::Button("Flip Horizontal", ImVec2(buttonWidth, 38))) {
                        editor->flipHorizontal();
                        needsTextureUpdate = true;
                        statusMessage = "  Flipped horizontally!";
                        statusColor = ImVec4(0.3f, 0.9f, 0.3f, 1.0f);
                    }
                    
                    ImGui::EndTabItem();
                }

                // ADJUSTMENTS TAB
                if (ImGui::BeginTabItem("  Adjust  ")) {
                    ImGui::Spacing();

                    // Brightness
                    ImGui::Text("  Brightness");
                    float brightnessFactorF = static_cast<float>(brightnessFactor);
                    ImGui::SetNextItemWidth(-1);
                    if (ImGui::SliderFloat("##Brightness", &brightnessFactorF, 0.1f, 2.5f, "%.2f")) {
                        brightnessFactor = brightnessFactorF;
                        editor->changeBrightness(brightnessFactor);
                        needsTextureUpdate = true;
                        statusMessage = "  Brightness adjusted!";
                        statusColor = ImVec4(0.3f, 0.9f, 0.3f, 1.0f);
                    }
                    ImGui::Spacing();

                    // Blur
                    ImGui::Text("  Blur");
                    float availWidth = ImGui::GetContentRegionAvail().x;
                    float applyBtnWidth = std::min(90.0f, availWidth * 0.25f);
                    ImGui::SetNextItemWidth(availWidth - applyBtnWidth - 10);
                    ImGui::SliderInt("##BlurRadius", &blurRadius, 1, 50, "Radius: %d");
                    ImGui::SameLine();
                    if (ImGui::Button(" Apply ", ImVec2(applyBtnWidth, 0))) {
                        editor->boxBlur(blurRadius);
                        needsTextureUpdate = true;
                        statusMessage = "  Blur applied!";
                        statusColor = ImVec4(0.3f, 0.9f, 0.3f, 1.0f);
                    }
                    ImGui::Spacing();

                    // Rotation
                    ImGui::Text("  Rotation");
                    const char* rotationOptions[] = {"90°", "180°", "270°"};
                    static int rotationIdx = 0;
                    availWidth = ImGui::GetContentRegionAvail().x;
                    float rotateBtnWidth = std::min(90.0f, availWidth * 0.25f);
                    ImGui::SetNextItemWidth(availWidth - rotateBtnWidth - 10);
                    ImGui::Combo("##Rotation", &rotationIdx, rotationOptions, 3);
                    ImGui::SameLine();
                    if (ImGui::Button(" Rotate ", ImVec2(rotateBtnWidth, 0))) {
                        int angles[] = {90, 180, 270};
                        editor->rotate(angles[rotationIdx]);
                        originalWidth = editor->getCurrentImage().width;
                        originalHeight = editor->getCurrentImage().height;
                        needsTextureUpdate = true;
                        statusMessage = "  Rotated!";
                        statusColor = ImVec4(0.3f, 0.9f, 0.3f, 1.0f);
                    }
                    
                    ImGui::EndTabItem();
                }

                // ARTISTIC TAB
                if (ImGui::BeginTabItem("  Artistic  ")) {
                    ImGui::Spacing();

                    float availWidth = ImGui::GetContentRegionAvail().x;
                    float buttonWidth = (availWidth - 20) / 3.0f;
                    
                    if (ImGui::Button(" Purple", ImVec2(buttonWidth, 38))) {
                        editor->purpleFilter();
                        needsTextureUpdate = true;
                        statusMessage = "  Purple filter applied!";
                        statusColor = ImVec4(0.3f, 0.9f, 0.3f, 1.0f);
                    }
                    ImGui::SameLine();
                    if (ImGui::Button(" Sunlight", ImVec2(buttonWidth, 38))) {
                        editor->sunlightFilter(sunlightIntensity);
                        needsTextureUpdate = true;
                        statusMessage = "  Sunlight filter applied!";
                        statusColor = ImVec4(0.3f, 0.9f, 0.3f, 1.0f);
                    }
                    ImGui::SameLine();
                    if (ImGui::Button(" Old TV", ImVec2(buttonWidth, 38))) {
                        editor->oldTVFilter();
                        needsTextureUpdate = true;
                        statusMessage = "  Old TV filter applied!";
                        statusColor = ImVec4(0.3f, 0.9f, 0.3f, 1.0f);
                    }

                    if (ImGui::Button("Wave Distortion", ImVec2(-1, 38))) {
                        editor->waveDistortion();
                        needsTextureUpdate = true;
                        statusMessage = "  Wave distortion applied!";
                        statusColor = ImVec4(0.3f, 0.9f, 0.3f, 1.0f);
                    }

                    if (ImGui::Button(" Infrared", ImVec2(-1, 38))) {
                        editor->infrared();
                        needsTextureUpdate = true;
                        statusMessage = "  Infrared filter applied!";
                        statusColor = ImVec4(0.3f, 0.9f, 0.3f, 1.0f);
                    }
                    
                    
                    ImGui::Spacing();
                    ImGui::Separator();
                    ImGui::Spacing();

                    // Oil Painting
                    ImGui::Text("  Oil Painting");
                    ImGui::SetNextItemWidth(-1);
                    ImGui::SliderInt("##OilRadius", &oilRadius, 1, 10, "Radius: %d");
                    ImGui::SetNextItemWidth(-1);
                    ImGui::SliderInt("##OilLevels", &oilLevels, 5, 50, "Levels: %d");
                    if (ImGui::Button("Apply Oil Painting", ImVec2(-1, 35))) {
                        editor->oilPainting(oilRadius, oilLevels);
                        needsTextureUpdate = true;
                        statusMessage = "  Oil painting applied!";
                        statusColor = ImVec4(0.3f, 0.9f, 0.3f, 1.0f);
                    }
                    
                    ImGui::EndTabItem();
                }
                
                // EFFECTS TAB
                if (ImGui::BeginTabItem("  Effects  ")) {
                    ImGui::Spacing();

                    // Glitch Effect
                    static float glitchIntensity = 1.2f;
                    ImGui::Text("  Glitch");
                    if (ImGui::Button("Apply Glitch Effect", ImVec2(-1, 38))) {
                        editor->glitch(glitchIntensity);
                        needsTextureUpdate = true;
                        statusMessage = "  Glitch effect applied!";
                        statusColor = ImVec4(0.3f, 0.9f, 0.3f, 1.0f);
                    }
                    
                    ImGui::Spacing();
                    ImGui::Separator();
                    ImGui::Spacing();

                    // Reflection
                    static float reflectFade = 0.6f;
                    static bool reflectRipple = true;
                    static int reflectDirection = 0;
                    ImGui::Text("  Reflection");
                    ImGui::SetNextItemWidth(-1);
                    ImGui::SliderFloat("##ReflectFade", &reflectFade, 0.0f, 1.0f, "Fade: %.2f");
                    ImGui::Checkbox("  Add Ripple Effect", &reflectRipple);
                    ImGui::RadioButton("  Horizontal", &reflectDirection, 0); 
                    ImGui::SameLine();
                    ImGui::RadioButton("  Vertical", &reflectDirection, 1);
                    if (ImGui::Button("Apply Reflection", ImVec2(-1, 35))) {
                        editor->reflect(reflectFade, reflectRipple, reflectDirection == 0);
                        originalWidth = editor->getCurrentImage().width;
                        originalHeight = editor->getCurrentImage().height;
                        needsTextureUpdate = true;
                        statusMessage = "  Reflection applied!";
                        statusColor = ImVec4(0.3f, 0.9f, 0.3f, 1.0f);
                    }
                    
                    ImGui::Spacing();
                    ImGui::Separator();
                    ImGui::Spacing();

                    // Skew
                    ImGui::Text("  Skew Transform");
                    float skewDegreeF = static_cast<float>(skewDegree);
                    ImGui::SetNextItemWidth(-1);
                    if (ImGui::SliderFloat("##Skew", &skewDegreeF, -45.0f, 45.0f, "%.1f°")) {
                        skewDegree = skewDegreeF;
                    }
                    if (ImGui::Button("Apply Skew", ImVec2(-1, 35))) {
                        editor->skewImage(skewDegree);
                        originalWidth = editor->getCurrentImage().width;
                        originalHeight = editor->getCurrentImage().height;
                        needsTextureUpdate = true;
                        statusMessage = "  Skew applied!";
                        statusColor = ImVec4(0.3f, 0.9f, 0.3f, 1.0f);
                    }
                    
                    ImGui::EndTabItem();
                }
                
                // FRAME TAB
                if (ImGui::BeginTabItem("  Frame  ")) {
                    ImGui::Spacing();
                    
                    ImGui::Text("  Add Custom Frame");
                    ImGui::Spacing();
                    ImGui::ColorEdit3("  Frame Color", frameColor, ImGuiColorEditFlags_NoInputs);
                    ImGui::Checkbox("  Decorated Style", &decorated);
                    ImGui::Spacing();
                    if (ImGui::Button("Add Frame", ImVec2(-1, 40))) {
                        unsigned char r = (unsigned char)(frameColor[0] * 255);
                        unsigned char g = (unsigned char)(frameColor[1] * 255);
                        unsigned char b = (unsigned char)(frameColor[2] * 255);
                        editor->addFrame(editor->calculateSuitableThickness(), r, g, b, decorated);
                        needsTextureUpdate = true;
                        statusMessage = "  Frame added!";
                        statusColor = ImVec4(0.3f, 0.9f, 0.3f, 1.0f);
                    }
                    
                    ImGui::EndTabItem();
                }
                
                ImGui::EndTabBar();
            }
            
            ImGui::Spacing();
        }

        // Bottom section - Undo and Status
        ImGui::Spacing();
        ImGui::Separator();
        ImGui::Spacing();
        
        // Undo button
        ImVec4 undoColor = ImVec4(0.80f, 0.50f, 0.20f, 1.0f);
        if (UIComponents::IconButton("  Undo Last Change", ImVec2(-1, 40), imageLoaded && editor && editor->canUndo(), undoColor)) {
            editor->undo();
            originalWidth = editor->getCurrentImage().width;
            originalHeight = editor->getCurrentImage().height;
            needsTextureUpdate = true;
            statusMessage = "  Undo successful!";
            statusColor = ImVec4(1.0f, 0.8f, 0.2f, 1.0f);
        }
        
        ImGui::Spacing();
        ImGui::Separator();
        
        // Status message box
        if (!statusMessage.empty()) {
            ImGui::PushStyleColor(ImGuiCol_ChildBg, ImVec4(0.10f, 0.10f, 0.12f, 0.95f));
            ImGui::BeginChild("StatusBox", ImVec2(-1, 35), true);
            ImGui::TextColored(statusColor, "%s", statusMessage.c_str());
            ImGui::EndChild();
            ImGui::PopStyleColor();
        }

        // Image Info
        if (imageLoaded) {
            ImGui::Spacing();
            ImGui::PushStyleColor(ImGuiCol_ChildBg, ImVec4(0.10f, 0.10f, 0.12f, 0.95f));
            ImGui::BeginChild("ImageInfo", ImVec2(-1, 65), true);
            ImGui::PushStyleColor(ImGuiCol_Text, ImVec4(0.7f, 0.7f, 0.7f, 1.0f));
            ImGui::Text("  Image Information");
            ImGui::Separator();
            ImGui::Text("  Original: %dx%d  |  Preview: %dx%d", originalWidth, originalHeight, 
                       textureManager.getWidth(), textureManager.getHeight());
            ImGui::PopStyleColor();
            ImGui::EndChild();
            ImGui::PopStyleColor();
        }

        ImGui::End();

        // Right Panel - Preview (responsive)
        float previewPanelX = leftPanelWidth + 10;
        float previewPanelWidth = windowWidth - leftPanelWidth - 10;
        
        ImGui::SetNextWindowPos(ImVec2(previewPanelX, 0));
        ImGui::SetNextWindowSize(ImVec2(previewPanelWidth, windowHeight));
        ImGui::Begin("Preview", nullptr,
                     ImGuiWindowFlags_NoResize | ImGuiWindowFlags_NoMove | ImGuiWindowFlags_NoCollapse);

        if (imageLoaded && textureManager.getTextureID()) {
            // Dark background for image preview
            ImGui::PushStyleColor(ImGuiCol_ChildBg, ImVec4(0.08f, 0.08f, 0.09f, 1.0f));
            ImGui::BeginChild("PreviewArea", ImVec2(-1, -1), true, ImGuiWindowFlags_NoScrollbar);
            
            // Calculate aspect-ratio-preserving size
            float availWidth = ImGui::GetContentRegionAvail().x - 40;
            float availHeight = ImGui::GetContentRegionAvail().y - 40;
            
            float aspectRatio = (float)textureManager.getWidth() / textureManager.getHeight();
            float displayWidth = availWidth;
            float displayHeight = availWidth / aspectRatio;
            
            // If height exceeds available space, scale based on height
            if (displayHeight > availHeight) {
                displayHeight = availHeight;
                displayWidth = availHeight * aspectRatio;
            }
            
            // Center the image
            float offsetX = (availWidth - displayWidth) * 0.5f + 20;
            float offsetY = (availHeight - displayHeight) * 0.5f + 20;
            
            ImGui::SetCursorPos(ImVec2(offsetX, offsetY));
            
            // Add subtle border around image
            ImVec2 imagePos = ImGui::GetCursorScreenPos();
            ImVec2 imageSize(displayWidth, displayHeight);
            
            ImDrawList* drawList = ImGui::GetWindowDrawList();
            drawList->AddRect(
                ImVec2(imagePos.x - 2, imagePos.y - 2),
                ImVec2(imagePos.x + imageSize.x + 2, imagePos.y + imageSize.y + 2),
                IM_COL32(60, 60, 70, 255),
                4.0f,
                0,
                2.0f
            );
            
            // Display image with calculated size
            ImGui::Image((ImTextureID)(intptr_t)textureManager.getTextureID(), imageSize);
            
            ImGui::EndChild();
            ImGui::PopStyleColor();
        } else {
            // Welcome screen
            ImGui::PushStyleColor(ImGuiCol_ChildBg, ImVec4(0.08f, 0.08f, 0.09f, 1.0f));
            ImGui::BeginChild("WelcomeArea", ImVec2(-1, -1), true, ImGuiWindowFlags_NoScrollbar);
            
            ImVec2 windowSize = ImGui::GetContentRegionAvail();
            ImVec2 textSize = ImGui::CalcTextSize("  No Image Loaded");
            ImGui::SetCursorPos(ImVec2((windowSize.x - textSize.x) * 0.5f, windowSize.y * 0.45f));
            ImGui::PushStyleColor(ImGuiCol_Text, ImVec4(0.5f, 0.5f, 0.55f, 1.0f));
            ImGui::Text("  No Image Loaded");
            ImGui::PopStyleColor();
            
            textSize = ImGui::CalcTextSize("Load an image from the left panel to begin editing");
            ImGui::SetCursorPosX((windowSize.x - textSize.x) * 0.5f);
            ImGui::PushStyleColor(ImGuiCol_Text, ImVec4(0.4f, 0.4f, 0.45f, 1.0f));
            ImGui::Text("Load an image from the left panel to begin editing");
            ImGui::PopStyleColor();
            
            ImGui::EndChild();
            ImGui::PopStyleColor();
        }

        ImGui::End();

        // ImGuiFileDialog
        if (ImGuiFileDialog::Instance()->Display("ChooseFileLoad")) {
            if (ImGuiFileDialog::Instance()->IsOk()) {
                std::string filePathName = ImGuiFileDialog::Instance()->GetFilePathName();
                strcpy(loadPath, filePathName.c_str());
            }
            ImGuiFileDialog::Instance()->Close();
        }
        if (ImGuiFileDialog::Instance()->Display("ChooseFileSave")) {
            if (ImGuiFileDialog::Instance()->IsOk()) {
                std::string filePathName = ImGuiFileDialog::Instance()->GetFilePathName();
                strcpy(savePath, filePathName.c_str());
            }
            ImGuiFileDialog::Instance()->Close();
        }

        ImGui::Render();
        glViewport(0, 0, display_w, display_h);
        glClearColor(0.15f, 0.15f, 0.18f, 1.0f);
        glClear(GL_COLOR_BUFFER_BIT);
        ImGui_ImplOpenGL3_RenderDrawData(ImGui::GetDrawData());
        glfwSwapBuffers(window);
    }

    delete editor;
    ImGui_ImplOpenGL3_Shutdown();
    ImGui_ImplGlfw_Shutdown();
    ImGui::DestroyContext();
    glfwDestroyWindow(window);
    glfwTerminate();
    return 0;
}
