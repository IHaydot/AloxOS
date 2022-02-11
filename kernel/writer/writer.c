#include "efi.h"
#include "efilib.h"
#include "../sources.h"
#include "../extras/extras.h"
#define PSF1_MAGIC0 0x36
#define PSF1_MAGIC1 0x04

typedef struct{
    void* baseAd;
    size_t size;
    unsigned int width;
    unsigned int height;
    unsigned int PPS;
} Framebuffer;

typedef struct{
    unsigned char magic[2];
    unsigned char mode;
    unsigned char size;
} PSF1_HEADER;

typedef struct{
    PSF1_HEADER* header;
    void* glyphBuffer;
} PSF1_FONT;

PSF1_FONT* LoadFont(EFI_FILE* dir, CHAR16* path, EFI_HANDLE image_handle, EFI_SYSTEM_TABLE* system_table){
    //EFI_FILE* font = LoadFile(dir, path, image_handle, system_table);
    EFI_FILE* font;
    if(font == NULL){
        return NULL;
    }
    PSF1_HEADER* header;
    system_table->BootServices->AllocatePool(EfiLoaderData, sizeof(PSF1_HEADER), (void**)header);
    UINTN size = sizeof(PSF1_HEADER);
    font->Read(font, &size, header);
    if(header->magic[0] != PSF1_MAGIC0 || header->magic[1] != PSF1_MAGIC1){
        return NULL;
    }
    UINTN GBS = header->size * 256;
    if(header-> mode == 1){
        GBS = header->size * 512;
    }

    void* glyphBuffer;
    {
        font->SetPosition(font, sizeof(PSF1_HEADER));
        system_table->BootServices->AllocatePool(EfiLoaderData, GBS, (void**)glyphBuffer);
        font->Read(font, &GBS, glyphBuffer); 
    }

    PSF1_FONT* finalFont;
    system_table->BootServices->AllocatePool(EfiLoaderData, sizeof(PSF1_FONT), (void**)&finalFont);
    finalFont->header = header;
    finalFont->glyphBuffer = glyphBuffer;
    return finalFont;
}

Framebuffer framebuffer;

Framebuffer* init_GOP(){
    EFI_STATUS status;
    EFI_GUID gopID = EFI_GRAPHICS_OUTPUT_PROTOCOL_GUID;
    EFI_GRAPHICS_OUTPUT_PROTOCOL *gop;
    status = uefi_call_wrapper(BS->LocateProtocol, 3, &gopID, NULL, (void**)&gop);
    if(EFI_ERROR(status)){
        Print(L"Failed to find GOP protocol...\n");
    }else{
        Print(L"Located GOP!\n");
    }

    framebuffer.baseAd = (void*)gop->Mode->FrameBufferBase;
    framebuffer.size = gop->Mode->FrameBufferSize;
    framebuffer.width = gop->Mode->Info->HorizontalResolution;
    framebuffer.height = gop->Mode->Info->VerticalResolution;
    framebuffer.PPS = gop->Mode->Info->PixelsPerScanLine;

    return &framebuffer;
}

void write_pixel_to_screen(Framebuffer *framebuffer, unsigned int x, unsigned int y, uint8_t mode){
    unsigned int Iy = y;
    unsigned int BBP = 4;
    if(mode == 0){
        *(unsigned int*)(x + (Iy * framebuffer->PPS * BBP) + framebuffer->baseAd) = 0xffffffff;
    }else{
        for (unsigned int i = 0; i < x * BBP; i++){
            *(unsigned int*)(i + (Iy * framebuffer->PPS * BBP) + framebuffer->baseAd) = 0xffffffff;
        }
    }
}

void cls(Framebuffer *framebuffer){
    unsigned int BBP = 4;
    for (unsigned int i = 0; i < 5000 * BBP; i++){
        for(unsigned int o = 0; o < 5000 * BBP; o++){
            *(unsigned int*)(i + (o * framebuffer->PPS * BBP) + framebuffer->baseAd) = 0xffffffff;
        }
    }
}