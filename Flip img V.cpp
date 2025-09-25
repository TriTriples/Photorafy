void Flip_V(image &img){

    for (int i = 0; i < img.height / 2; i++){
        for (int j = 0; j < img.width; j++){
            swap(img(j, i, 0), img(j, img.height - i - 1, 0));
            swap(img(j, i, 1), img(j, img.height - i - 1, 1));
            swap(img(j, i, 2), img(j, img.height - i - 1, 2));
        }
    }
}