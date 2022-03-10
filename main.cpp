#include <stdio.h>
#include <stdlib.h>
#include <stdint.h>

int main(int argc, char *argv[])
{
    if (argc != 2)
    {
        printf("Usage: %s image\n", argv[0]);      //No command-line input
        return 1;
    }
	
    FILE *file = fopen(argv[1], "r");
    FILE *img = NULL;
    uint8_t image[512];
    int image_num = 0;
    // Allocate 7 bytes of memory for string (1 extra byte for null terminator)
    // image_name here is 001.jpg - 7 characters + 1 null terminator
    char *image_name = malloc((7 + 1));

    while (fread(image, 1, 512, file))
    {
        // If new jpeg image found
        if (image[0] == 0xff && image[1] == 0xd8 && image[2] == 0xff && (image[3] & 0xf0) == 0xe0)
        {
            //If an image is already opened, then close it
            if (img != NULL)
            {
                fclose(img);
                image_num++;
            }
            // Open a new image and write into it
            sprintf(image_name, "%03i.jpg", image_num);
            img = fopen(image_name, "w");
            fwrite(image, 1, 512, img);
        }
        else
        {
            //If there's already an image opened, then write into it
            if (img != NULL)
            {
                fwrite(image, 1, 512, img);
            }
        }
    }
	
    if (img != NULL)
    {
        fclose(img);
    }
    fclose(file);
    free(image_name);

    return 0;
}