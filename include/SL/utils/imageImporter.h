#ifndef __SL_UTILS_IMAGE_IMPORTER_H__
#define __SL_UTILS_IMAGE_IMPORTER_H__

#include "../structures.h"

/// @brief Supported image extensions
// (Lol I wish)
typedef enum ImageExtensions {
    IMG_EXT_DETECT = 0,
    IMG_EXT_PNG,
    IMG_EXT_BMP,
    IMG_EXT_JPG,
    IMG_EXT_TIFF,
    IMG_EXT_PPM,
    IMG_EXT_QOI,
} img_ext;

/// @brief Raw image data
typedef struct Image2D {
    uvec2 size;
    uint8 channelCount;
    uint8 bitDepth;
    uint8* rawData;
} image2D;

/// @brief Destroy a 2D image
/// @param toDestroy The image to destroy
void destroyImage2D(image2D toDestroy);

/// @brief Import an image
/// @param path The path to the image
/// @param extension The extension of the image
/// @return The data contained in the image
image2D importImage(const char* path, img_ext extension);

/// @brief Export an image
/// @param image The source to export
/// @param extension The extension of the exported image
/// @param path Where to store the image
void exportImage(image2D image, img_ext extension, const char* path);

#endif