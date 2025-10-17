================================================================================
                                 PHOTORAFY
                       Image Processing & Effects GUI
================================================================================

OVERVIEW:
Photorafy is a image editing application with a graphical
user interface built with Dear ImGui. It provides a wide range of filters,
adjustments, and effects for image manipulation. The application supports
multiple image formats and features an intuitive tabbed interface for easy
navigation between different filter categories.

Photorafy is done as part of CS213 OOP Programming at FCAI, Cairo University under supervision of Dr Mohammad El-Ramly.

Authors:

Mostafa Essam, 20240595 (S22)
Mohamed Osama, 20240466 (S22)
Ahmed Sayed, 20240030 (All-B)

Video: https://drive.google.com/file/d/1myYKezQ1tHTc_CXGG_TtJUAam2dJqFEP/view?usp=drive_link
Shared Drive (code): https://drive.google.com/drive/folders/183av3yQTjuHl7HKOFJ1uYbdqSjbLLbbI?usp=sharing
Github Repo: https://github.com/MrQuartz99/img-filterer
================================================================================
                          TOOLS & VERSIONS USED
================================================================================

Compiler & Build System:
- GCC/G++ (MinGW-w64): 13.2.0 or later
  Download: https://www.msys2.org/
  Installation: Install MSYS2 and run: pacman -S mingw-w64-ucrt64-gcc

- CMake: 3.10 or later
  Download: https://cmake.org/download/
  Installation: Download and run the Windows installer

- C++: C++17

Graphics & UI Libraries:
- OpenGL: 3.3+ (provided by system graphics driver)
- GLFW: 3.3.8
  Location: ./glfw/ directory (included in project)

- Dear ImGui: Latest version
  Location: ./imgui/ directory (included in project)

Image Processing Libraries:
- STB Image (stb_image.h)
- STB Image Write (stb_image_write.h)

================================================================================
                       INSTALLATION & SETUP GUIDE
================================================================================

STEP 1: Install MinGW-w64 and CMake
------------------------------------
1. Download and install MSYS2 from https://www.msys2.org/
2. Open MSYS2 terminal and run:
   pacman -S mingw-w64-ucrt64-gcc mingw-w64-ucrt64-cmake mingw-w64-ucrt64-make

3. Add MinGW-w64 to Windows PATH:
   - Right-click "This PC" > Properties > Advanced system settings
   - Click "Environment Variables"
   - Under "System variables", click "Path" > Edit
   - Add: C:\msys64\ucrt64\bin
   - Click OK and restart your terminal

Verify installation by opening Command Prompt and running:
   g++ --version
   cmake --version
   make --version


STEP 2: Clone or Extract the Project
-------------------------------------
Extract the project files to a directory (e.g., D:\Projects\C++\photorafy)


STEP 3: Build the Project
--------------------------
1. Open Command Prompt or PowerShell
2. Navigate to the project directory:
   cd Path\to\the\project

3. Create a build directory:
   mkdir build
   cd build

4. Generate build files with CMake:
   cmake -G "MinGW Makefiles" ..

5. Build the project:
   cmake --build .

This will create the executable: photorafy.exe in the build directory


STEP 4: Run the Application
----------------------------
Navigate to the build directory and run the .exe file and the GUI will launch.

================================================================================
                            FILTER LIST
================================================================================

1. Grayscale
2. Black & White (Threshold)
3. Invert
4. Edge Detection
5. Flip Vertical
6. Flip Horizontal
7. Brightness
8. Blur (Box Blur Algorithm using 2d prefix sum to speed it up)
9. Rotation
10. Crop
11. Purple Filter
12. Sunlight Filter
13. Old TV Filter

14. Wave Distortion:
    - it applies a horizontal wave distortion to an image.
    • Each horizontal row (y) is shifted left or right based on the sine function sin (y / 
    frequency).
    • The amplitude controls the maximum horizontal shift in pixels.
    • As a result, the image appears to have wavy ripples, similar to water waves moving 
    across it.
    In short: it creates a ripple/wave effect by horizontally displacing rows of pixels according to 
    a sine wave.

15. Infrared Filter
16. Oil Painting
17. Merge with Image

18. Glitch Effect:
    -it applies a random glitch effect to the image.
    Because it uses random numbers, the output changes each time you run it.
    • Color channel shifts:
    Red, Green, and Blue channels are shifted horizontally by different random 
    amounts → creates a misaligned RGB look.
    • Horizontal bands:
    Random horizontal strips of the image are shifted left or right → looks like 
    distorted broken bands.
    • Noise pixels:
    Adds random colored pixels (static/noise) across the image.
    • Random tinted lines:
    Some rows get tinted (red/blue imbalance), making them look corrupted.
    • Scanline effect:
    Darkens every other row slightly to mimic an old CRT/glitchy screen

19. Reflection:
    - it creates a mirror reflection effect of the image, with optional fading and ripple distortion.

    • Horizontal reflection (top → bottom):
    o It flips the image vertically to create a reflection under the original.
    o fadeStrength makes the reflection gradually fade out (dimmer towards 
    the bottom).
    o If addRipple = true, it applies a sine-based ripple distortion to make the 
    reflection look like it’s on water.
    o Adds slight color variation: reduces red a bit and increases blue a bit 
    → makes it more watery.

    • Vertical reflection (left → right):
    o Similar, but flips the image horizontally to the right side.
    o Same fading, ripple, and color adjustments are applied.

20. Skew Transform
21. Custom Frame

================================================================================
                       PROJECT STRUCTURE
================================================================================

Root Directory:
- CMakeLists.txt: Build configuration file
- gui.cpp: Main GUI source code

Subdirectories:

/lib:
- ImageEditor.h: Image editing class header
- ImageEditor.cpp: Image editing implementation
- Image_Class.h: Image data structure
- stb_image.h: STB image loading library
- stb_image_write.h: STB image saving library

/imgui:
- ImGui core files and backends
- ImGuiFileDialog: File dialog component
- OpenGL3 implementation for rendering

/glfw:
- GLFW library headers and libraries
- includes/ and lib-mingw-w64/ directories

/build:
- Generated during build process
- Contains the app img_filterer_gui.exe (executable)

/images
/generated

Architecture:
- Model-View separation: ImageEditor handles logic, GUI handles display
- Undo system: Uses stack-based history (max 10 states)
- Responsive UI: Dynamically adjusts to window size

Code Organization:
- All image processing in lib/ImageEditor.cpp
- All UI logic in gui.cpp
- Modular filter implementations for easy extension

================================================================================
                        TROUBLESHOOTING
================================================================================

Issue: "Failed to load image"
Solution: 
- Verify file path is correct
- Ensure image format is supported (JPG, PNG, BMP, TGA)
- Check file is not corrupted
- Ensure file path doesn't contain special characters


Issue: "CMake not found" during build
Solution:
- Verify CMake is installed and added to PATH
- Restart terminal after installation
- Run: where cmake (should show installation path)

Issue: Build fails with compilation errors
Solution:
- Ensure C++17 compiler is being used
- Delete build folder and rebuild from scratch
- Verify all source files are present
- Check all library files are present in ./lib and ./imgui