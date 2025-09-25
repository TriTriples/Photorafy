#ifndef IMAGEEDITOR_H
#define IMAGEEDITOR_H

#include <string>
#include <stack>
#include "lib/Image_Class.h"

class ImageEditor {
private:
    Image originalImage;       
    Image currentImage;
    std::stack<Image> history;

public:
    ImageEditor(const std::string& path);

    void flipVertical();
    void flipHorizontal();
    void blackAndWhite();
    void grayscale();

    void undo();
    void save(const std::string& path);
    void preview(const std::string& path);


    const Image& getImage() const { return currentImage; }
};

#endif
