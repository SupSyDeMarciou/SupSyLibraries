#include "inout.h"

char* SL_readFile(const char* fileName) {
    FILE* file = fopen(fileName, "rb");
    if (!file) SL_throwError("File \'%s\' not found.", fileName);
    fseek(file, 0L, SEEK_END);
    long size = ftell(file);
    fseek(file, 0L, SEEK_SET);

    char* buffer = (char*)malloc(size + 1); // + '\0'
    fread(buffer, 1, size, file);
    buffer[size] = '\0';

    fclose(file);
    return buffer;
}

int SL_readNum(const char* line, uint i) {

    // Find number start position
    int start = i;
    while (start > 0 && SL_isNum(line[--start]));

    int tot = 0;
    int val = 0;
    // Read value
    if (line[start] == '-') for (int val = 0; SL_isFigure(val); val = SL_toInt(line[start]), start++) tot = 10 * tot - val;
    else for (int val = 0; SL_isFigure(val); val = SL_toInt(line[start]), start++) tot = 10 * tot + val;
    return tot;
}

bool SL_match(const char* a, const char* b) {
    while (*a && *b && *a == *b) a++, b++;
    return !*a && !*b;
}
bool SL_match_NoCase(const char* a, const char* b) {
    for (char A = *a, B = *b; *a && *b; A = *++a, B = *++b) {
        if (A != B) {
            if ('A' <= A && A <= 'Z') A += 'a' - 'A';
            if ('A' <= B && B <= 'Z') A += 'a' - 'A';
            if (A != B) return false;
        }
    }
    return *a == *b;
}
bool SL_match_Start(const char* a, const char* b) {
    while (*a && *b && *a == *b) a++, b++;
    return !*b;
}
bool SL_match_Factor(const char* text, const char* factor) {
    do {
        const char *f = factor, *t = text;
        while (*t && *f && *t == *f) ++t, ++f;
        if (!*f) return true;
        if (!*t) return false;
    } while (*++text);
    return false;
}

#include <memory.h>
void* SL_realloc(void* data, size_t lastSize, size_t newSize) {
    if (lastSize == newSize) return data;
    
    void* newData = malloc(newSize);
    if (newData) memcpy(newData, data, lastSize < newSize ? lastSize : newSize);
    
    free(data);
    return newData;
}

typedef struct SL_Path {
    char path[1024];
    uint pathLen;

    struct {
        char *start, *end;
    } sections[32];
    uint sectionCount;

    bool isFolder;
    bool lastSectionIsExt;
} SL_path;