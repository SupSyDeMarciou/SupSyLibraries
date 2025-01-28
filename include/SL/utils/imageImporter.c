#include "../../SL.h"
#include <stdio.h>
#include <stdlib.h>
#include <memory.h>

#include "puff.h"

void destroyImage2D(image2D toFree) {
    free(toFree.rawData);
}



/////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////
/////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////
////////////////////////////////////////////////////////////// * * *  IMPORTING  * * * //////////////////////////////////////////////////////////////
/////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////
/////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////



///////////////////////////////////////////////////////////////////////////////////
//////////////////////////////////// PNG FORMAT ///////////////////////////////////
///////////////////////////////////////////////////////////////////////////////////


static uint read4Bytes(const uint8* data, uint64* l) {
    uint val = data[*l + 3] + 256 * (data[*l + 2] + 256 * (data[*l + 1] + 256 * data[*l + 0]));
    *l += 4;
    return val;
}

static void readBlockHeader(const uint8* data, uint64* l, uint* size, char* type) {
    *size = read4Bytes(data, l);
    type[0] = data[*l + 0]; type[1] = data[*l + 1]; type[2] = data[*l + 2]; type[3] = data[*l + 3];
    *l += 4;
}

static int isType(char* type, char comp[5]) {
    return (type[0] == comp[0] && type[1] == comp[1] && type[2] == comp[2] && type[3] == comp[3]);
}

static image2D importImagePNG(const char* path, const uint8* data, long length) {

    // Read magic number
    if (!(data[0] == 0x89 && data[1] == 0x50 && data[2] == 0x4E && data[3] == 0x47 &&
          data[4] == 0x0D && data[5] == 0x0A && data[6] == 0x1A && data[7] == 0x0A)) 
        SL_throwError("File \"%s\" is not a PNG image or is corrupted! (First 8 bytes)", path);

    uint64 l = 8;
    uint size = 0;
    char type[4] = {0};

    image2D result;
    uint8 palette[256 * 3];
    bool usingPalette = false;
    uint pixelIdx = 0;

    // Get IHDR block
    readBlockHeader(data, &l, &size, type);
    if (!isType(type, "IHDR")) SL_throwError("File \"%s\" is not a PNG image or is corrupted! (IHDR is not first)", path);

    result.size.x = read4Bytes(data, &l);
    result.size.y = read4Bytes(data, &l);

    uint8 bitDepth = data[l++];
    if (bitDepth < 8) SL_throwError("File \"%s\" is using bit depth less than 8 (%d), which is not supported!", bitDepth);
    uint8 colorType = data[l++];
    result.channelCount = colorType == 0 || colorType == 3 ? 1 : (colorType == 2 ? 3 : (colorType == 6 ? 4 : (colorType == 4 ? 2 : 1)));
    uint8 compressionMethod = data[l++];
    uint8 filterMethod = data[l++];
    if (filterMethod != 0) SL_throwError("File \"%s\" is not a PNG image or is corrupted! (filter method != 0)", path);
    uint8 interlaceMethod = data[l++];
    if (interlaceMethod != 0) SL_throwError("File \"%s\" is using interlaced method 1, which is not supported!", path);

    l += 4; // CRC

    uint pixelSize = bitDepth * result.channelCount / 8;
    uint64 imageWidth = result.size.x * pixelSize;

    uint8* compressedPixels = calloc(1, result.size.y * (imageWidth + 1));
    unsigned long compressedIdx = 0;
    uint8* decompressedPixels = calloc(1, result.size.y * (imageWidth + 1));
    unsigned long decompressedIdx = 0;

    uint IDATcount = 0;
    do {
        readBlockHeader(data, &l, &size, type);

        // Is not critical ? (Critical to decoding the image)
        if (type[0] > 'Z') {
            l += size + 4;
            continue;
        }

        // if (type[1] >= 'A'); // Is public ?
        if (type[2] > 'Z') SL_throwError("File \"%s\" is not a PNG image or is corrupted! (Type '%c%c%c%c')", path, type[0], type[1], type[2], type[3]); // Is not legit PNG (This letter should always be uppercase)
        // if (type[3] >= 'A'); // Is safe to copy ?

        if (isType(type, "PLTE")) { // Palette
            if (size % 3 != 0 || usingPalette) SL_throwError("File \"%s\" is not a PNG image or is corrupted! (Multiple PLTE)", path);
            for (uint i = 0; i < size; i += 3) {
                palette[i + 0] = data[l + i + 0];
                palette[i + 1] = data[l + i + 1];
                palette[i + 2] = data[l + i + 2];
            }
            usingPalette = true;
            printf("  Using Palette\n");
            
            l += size + 4;
        } 
        else if (isType(type, "IDAT")) { // Data block, all data blocks are consecutive in file
            if (IDATcount == 0) {
                for (uint i = 0; i < size - 2; i++) compressedPixels[compressedIdx + i] = data[l + 2 + i]; // Skip first 2 bytes (zlib data)
                compressedIdx += size - 2;
            }
            else {
                for (uint i = 0; i < size; i++) compressedPixels[compressedIdx + i] = data[l + i];
                compressedIdx += size;
            }

            l += size + 4; // CRC
            IDATcount++;
        } 
        else if (isType(type, "IEND")) break; // Image end
        else SL_throwError("File \"%s\" is not a PNG image or is corrupted! (Type '%c%c%c%c')", path, type[0], type[1], type[2], type[3]); // Unrecognised critical chunk

    } while(l < length);
    if (l >= length) SL_throwError("File \"%s\" is not a PNG image or is corrupted! (Missing IEND)", path); // No end of image block
    
    compressedIdx -= 4; // last 4 (zlib checksum)
    unsigned long sourceLen = compressedIdx, destLen = result.size.y * (imageWidth + 1);
    int err = puff(decompressedPixels, &destLen, compressedPixels, &sourceLen);
    if (err) SL_throwError("Puff ended with code %d\n", err);
    free(compressedPixels);

    uint64 idxResult = 0;
    result.rawData = malloc(result.size.y * imageWidth);

    for (uint y = 0; y < result.size.y; y++) {
    // for (int y = result.size.y - 1; y >= 0; y--) {
        uint idxScanLine = y * (imageWidth + 1);
        uint8 filter = decompressedPixels[idxScanLine];

        switch (filter) {
            case 0: // NONE
                NONE:
                memcpy(result.rawData + idxResult, decompressedPixels + idxScanLine + 1, imageWidth);
                idxResult += imageWidth;
                break;

            case 1: // SUB
                SUB:
                for (uint x = 1; x <= pixelSize; x++) result.rawData[idxResult++] = decompressedPixels[idxScanLine + x];
                for (uint x = pixelSize + 1; x <= imageWidth; x++, idxResult++)
                    result.rawData[idxResult] = result.rawData[idxResult - pixelSize] + decompressedPixels[idxScanLine + x];

                break;
            
            case 2: // UP
                if (y == 0) goto NONE; // Scanline "-1" should be treated as full of 0

                for (uint x = 1; x <= imageWidth; x++, idxResult++)
                    result.rawData[idxResult] = result.rawData[idxResult - imageWidth] + decompressedPixels[idxScanLine + x];
                
                break;
            
            case 3: // AVG
                if (y == 0) { // No UP
                    for (uint x = 1; x <= pixelSize; x++) result.rawData[idxResult++] = decompressedPixels[idxScanLine + x];
                    for (uint x = pixelSize + 1; x <= imageWidth; x++, idxResult++)
                        result.rawData[idxResult] = (result.rawData[idxResult - pixelSize] >> 1) + decompressedPixels[idxScanLine + x];
                    
                    break;
                }

                for (uint x = 1; x <= pixelSize; x++, idxResult++)
                    result.rawData[idxResult] = (result.rawData[idxResult - imageWidth] >> 1) + decompressedPixels[idxScanLine + x];
                
                for (uint x = pixelSize + 1; x <= imageWidth; x++, idxResult++)
                    result.rawData[idxResult] = ((result.rawData[idxResult - imageWidth] + (int)result.rawData[idxResult - pixelSize]) >> 1) + decompressedPixels[idxScanLine + x];

                break;

            case 4: // PAETH
                if (y == 0) goto NONE;

                for (uint x = 0; x < pixelSize; x++) {
                    uint b = result.rawData[idxResult - imageWidth];

                    if (!b) result.rawData[idxResult] = 0;
                    else result.rawData[idxResult] = b;

                    result.rawData[idxResult++] += decompressedPixels[idxScanLine + 1 + x];
                }

                for (uint x = pixelSize; x < imageWidth; x++) {
                    
                    uint a = x < pixelSize ? 0 : result.rawData[idxResult - pixelSize];
                    uint b = result.rawData[idxResult - imageWidth];
                    uint c = x < pixelSize ? 0 : result.rawData[idxResult - imageWidth - pixelSize];

                    uint p = a + b - c;
                    uint pa = abs(p - a);
                    uint pb = abs(p - b);
                    uint pc = abs(p - c);

                    if (pa <= pb && pa <= pc) result.rawData[idxResult] = a;
                    else if (pb <= pc) result.rawData[idxResult] = b;
                    else result.rawData[idxResult] = c;

                    result.rawData[idxResult++] += decompressedPixels[idxScanLine + 1 + x];
                }
                break;
        
            default: SL_throwError("File \"%s\" is not a PNG image or is corrupted! (Unrecognized filter %d at scanline %d)", path, filter, y);
        }
    }
    free(decompressedPixels);

    size_t scanlineSize = pixelSize * result.size.x;
    size_t imageSize = result.size.y * scanlineSize / 2;
    void* scanlineBuffer = malloc(scanlineSize);
    for (uint y = 0, yp = (result.size.y - 1) * scanlineSize; y < imageSize; y+=scanlineSize, yp-=scanlineSize) {
        memcpy(scanlineBuffer, result.rawData + y, scanlineSize);
        memcpy(result.rawData + y, result.rawData + yp, scanlineSize);
        memcpy(result.rawData + yp, scanlineBuffer, scanlineSize);
    }
    free(scanlineBuffer);

    return result;
}


///////////////////////////////////////////////////////////////////////////////////
//////////////////////////////////// BMP FORMAT ///////////////////////////////////
///////////////////////////////////////////////////////////////////////////////////


static image2D importImageBMP(const char* path, const uint8* data, long length) {
    
}


///////////////////////////////////////////////////////////////////////////////////
//////////////////////////////////// PPM FORMAT ///////////////////////////////////
///////////////////////////////////////////////////////////////////////////////////


static image2D importImagePPM(const char* path, const uint8* data, long length) {
    image2D result;

    uint64 l = 0;
    if (data[l] != 'P' || data[l+1] != '6') SL_throwError("File \"%s\" is not a PPM image or is corrupted!", path);
    l+=3;

    for (; data[l] >= '0' && data[l] <= '9'; l++) result.size.x = 10 * result.size.x + data[l] - '0';
    l++;

    for (; data[l] >= '0' && data[l] <= '9'; l++) result.size.y = 10 * result.size.y + data[l] - '0';
    l++;

    uint16 maxValue = 0;
    for (; data[l] >= '0' && data[l] <= '9'; l++) maxValue = 10 * maxValue + data[l] - '0';
    l++;

    int size = (maxValue >= 256) + 1;
    uint64 nbBytes = size * 3 * result.size.x * result.size.y;
    result.rawData = malloc(nbBytes);

    for (uint64 i = 0; i < nbBytes; i++) {
        result.rawData[i] = (uint)(pow(data[l + i] / (float)maxValue, 2.2) * 255);
    }

    return result;
}

image2D importImage(const char* path, img_ext format) {
    if (!path) SL_throwError("Must provide a path for the image to import", 0);

    FILE* f = fopen(path, "rb");
    if (!f) SL_throwError("Image \"%s\" not found", path);

    fseek(f, 0L, SEEK_END);
    long size = ftell(f);
    fseek(f, 0L, SEEK_SET);

    uint8* data = (uint8*)malloc(sizeof(uint8) * size);
    fread(data, sizeof(uint8), size, f);

    fclose(f);

    image2D result;
    switch (format) {
        case IMG_EXT_PNG:
            result = importImagePNG(path, data, size);
            break;

        case IMG_EXT_PPM:
            result = importImagePPM(path, data, size);
            break;

        default: 
            SL_throwError("Format not supported", 0);
    }

    free(data);
    return result;
}



/////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////
/////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////
////////////////////////////////////////////////////////////// * * *  EXPORTING  * * * //////////////////////////////////////////////////////////////
/////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////
/////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////



///////////////////////////////////////////////////////////////////////////////////
//////////////////////////////////// PPM FORMAT ///////////////////////////////////
///////////////////////////////////////////////////////////////////////////////////


static char* exportImagePPM(image2D data, size_t* stringSize, int doublePrecision) {

    uint size = data.size.x * data.size.y;
    uint16 colorDepth = doublePrecision > 0 ? 65535 : 255;
    uint64 nbBytes = size * sizeof(uint8) * 3 * (1 + (doublePrecision > 0));

    char header[64] = {0};
    sprintf(header, "P6\n%d %d\n%d\n", data.size.x, data.size.y, colorDepth);

    uint headerLength = 0;
    while (header[++headerLength]);

    *stringSize = sizeof(uint8) * (headerLength + nbBytes);
    uint8* bytes = malloc(*stringSize);
    uint64 i = 0;
    for (; i < headerLength; i++) bytes[i] = (uint8)header[i];

    if (doublePrecision) {
        for (uint64 j = 0; j < size; j++) ((uint16*)(&bytes[i]))[j] = pow(((float*)data.rawData)[j] / 255.0, -2.2) * colorDepth;
    } else {
        for (uint64 j = 0; j < size; j++) ((uint8*)(&bytes[i]))[j]  = pow(((float*)data.rawData)[j] / 255.0, -2.2) * colorDepth;
    }

    return bytes;
}

void exportImage(image2D image, img_ext format, const char* path) {
    if (!path) SL_throwError("Must provide a path for the image to export", 0);
    
    char* result = NULL;
    size_t size = 0;
    switch (format) {

        case IMG_EXT_PPM: 
            result = exportImagePPM(image, &size, 1);
            break;

        default: 
            SL_throwError("Format not supported", 0);
    }

    if (!result) SL_throwError("Error while processing data in image export", 0);

    FILE* f = fopen(path, "wb");
    fwrite(result, size, 1, f);
    fclose(f);

    free(result);
}