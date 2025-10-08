void oil(image &img, int r = 3) {
    image temp = img;

    for (int i = r; i < img.height - r; i++) {
        for (int j = r; j < img.width - r; j++) {
            int total = 0, count = 0;
            for (int k = -r; k <= r; k++) {
                for (int l = -r; l <= r; l++) {
                    int R = img(j + k, i + l, 0);
                    int G = img(j + k, i + l, 1);
                    int B = img(j + k, i + l, 2);
                    total += (R + G + B) / 3;
                    count++;
                }
            }

            int avg = total / count;
            int diff = 256;
            int rc = 0, g = 0, b = 0;

            for (int k = -r; k <= r; k++) {
                for (int l = -r; l <= r; l++) {
                    int R = img(j + k, i + l, 0);
                    int G = img(j + k, i + l, 1);
                    int B = img(j + k, i + l, 2);
                    int intensity = (R + G + B) / 3;
                    int d = abs(intensity - avg);
                    if (d < diff) {
                        diff = d;
                        rc = R;
                        g = G;
                        b = B;
                    }
                }
            }

            temp(j, i, 0) = rc;
            temp(j, i, 1) = g;
            temp(j, i, 2) = b;
        }
    }

    img = temp;
}
