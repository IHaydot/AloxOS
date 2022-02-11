//----------INCLUDES--------------
#include "efi.h"
#include "efilib.h"
#include "writer/writer.h"
#include "sources.h"
#include "extras/extras.h"

//----------PREPROCESSOR------------
#define COZY_DEBUG

//----------CODE--------------------

//----------KERNEL CODE--------------------
EFI_STATUS
EFIAPI
efi_main(EFI_HANDLE image_handle, EFI_SYSTEM_TABLE *system_table){
  InitializeLib(image_handle, system_table);

  Framebuffer* buffer = init_GOP();

  PSF1_FONT* font = LoadFont(NULL, L"zap-light16.psf", image_handle, system_table);
  if(font == NULL){
    Print(L"Invalid font or not found\n\r");
  }else{
    Print(L"Font found, char size: %d\n\r", font->header->size);
  }

  Print(L"Hello kernel!\n");
  #ifdef COZY_DEBUG BUFFER
  Print(L"Base: 0x%x\n\rSize: 0x%x\n\rWidth: %d\n\rHeight: %d\n\rPPS: %d\n\r", buffer->baseAd, buffer->size, buffer->width, buffer->height, buffer->PPS);
  #endif

  write_pixel_to_screen(buffer, 50, 50, 1);

  return EFI_SUCCESS;
}
