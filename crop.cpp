void crop(image &img, int r1, int c1, int r2, int c2){

    int h = r2-r1+1;
    int w = c2-c1+1;
    image crop(w,h,3);
    for (int i = 0; i < h; i++){
        for (int j = 0; j < w; j++){
            crop(i,j,0) = img(i+r1,j+c1,0);
            crop(i,j,1) = img(i+r1,j+c1,1);
            crop(i,j,2) = img(i+r1,j+c1,2);
        }
    }
    img = crop;


      
}