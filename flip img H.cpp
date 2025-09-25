void Flip(image &img){

    for (int i = 0; i < img.height; i++) {
        for (int j = 0; j < img.width / 2; j++) {
            swap(img(j, i, 0), img(img.width - j - 1, i, 0));
            swap(img(j, i, 1), img(img.width - j - 1, i, 1));
            swap(img(j, i, 2), img(img.width - j - 1, i, 2));
        }
    }


}