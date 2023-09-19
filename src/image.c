#include "image.h"

#include <stdlib.h>

#define STB_IMAGE_IMPLEMENTATION
#include "stb_image.h"

Image* load_image_from_file(const char* filename)
{
    Image* img = malloc(sizeof(Image));
    img->data = stbi_load(filename, &img->w, &img->h, &img->comp, STBI_rgb_alpha);
    return img;
}

void free_image(Image* img)
{
    stbi_image_free(img->data);
    free(img);
}
