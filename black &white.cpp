void black_and_white(image &img)
{
    for (int i =0; i<img.height; i++)
    {
        for (int j =0; j<img.width; j++)
        {
            int r = img (j,i,0);
            int g = img (j,i,1);
            int b = img (j,i,2);
            int avg = (r+g+b)/3;
            if (avg<128)
            {
                img (j,i,0) = 0;
                img (j,i,1) = 0;
                img (j,i,2) = 0;
            }
            else
            {
                img (j,i,0) = 255;
                img (j,i,1) = 255;
                img (j,i,2) = 255;
            }
        
        }
    }
}