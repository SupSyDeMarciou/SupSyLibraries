#ifndef __SL_UTILS_IMAGE_IMPORTER
#define __SL_UTILS_IMAGE_IMPORTER

#include "../structures.h"

/// @brief Supported image extensions
// (Lol I wish)
typedef enum ImageExtensions {
    IMG_EXT_PNG,
    IMG_EXT_BMP,
    IMG_EXT_JPEG,
    IMG_EXT_TIFF,
    IMG_EXT_PPM
} img_ext;

/// @brief Raw image data
typedef struct Image2D {
    uvec2 size;
    uint8* rawData;
    uint8 channelCount;
} image2D;

/// @brief Destroy a 2D image
/// @param toDestroy The image to destroy
void destroyImage2D(image2D toDestroy);

/// @brief Import an image
/// @param path The path to the image
/// @param extension The extension of the image
/// @return The data contained in the image
/// @warning ONLY PPM AND PNG ARE CURRENTLY SUPPORTED
image2D importImage(const char* path, img_ext extension);

/// @brief Export an image
/// @param image The source to export
/// @param extension The extension of the exported image
/// @param path Where to store the image
/// @warning ONLY PPM IS CURRENTLY SUPPORTED
void exportImage(image2D image, img_ext extension, const char* path);

#endif