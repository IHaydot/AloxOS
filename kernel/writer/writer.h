#pragma once
#include "../sources.h"

typedef struct{
    unsigned char magic[2];
    unsigned char mode;
    unsigned char size;
} PSF1_HEADER;

typedef struct{
    PSF1_HEADER* header;
    void* glyphBuffer;
} PSF1_FONT;

typedef struct{
    void* baseAd;
    size_t size;
    unsigned int width;
    unsigned int height;
    unsigned int PPS;
} Framebuffer;

Framebuffer* init_GOP();
void write_pixel_to_screen(Framebuffer *framebuffer, unsigned int x, unsigned int y, uint8_t mode);

PSF1_FONT* LoadFont(EFI_FILE* dir, CHAR16* path, EFI_HANDLE image_handle, EFI_SYSTEM_TABLE* system_table);
void cls(Framebuffer *framebuffer);