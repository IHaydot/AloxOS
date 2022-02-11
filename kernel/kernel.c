#include "efi.h"
#include "efilib.h"
#include "writer/writer.h"

EFI_STATUS
EFIAPI
efi_main(EFI_HANDLE image_handle, EFI_SYSTEM_TABLE *system_table){
  InitializeLib(image_handle, system_table);
  init_GOP();
  Print(L"Hello world!\n");
  return EFI_SUCCESS;
}
