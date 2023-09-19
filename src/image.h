#ifndef IMAGE_H
#define IMAGE_H

typedef struct
{
    unsigned char* data;
    int w;
    int h;
    int comp;
} Image;

Image* load_image_from_file(const char* filename);
void free_image(Image* img);

#endif // IMAGE_H
