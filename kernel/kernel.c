#include "efi.h"
#include "efilib.h"
#include "writer/writer.h"

EFIAPI
EFI_STATUS efi_main(EFI_HANDLE image_handle, EFI_SYSTEM_TABLE *system_table){
  EFI_STATUS status;
  EFI_INPUT_KEY key;
  ST = system_table;
  INIT_GOP(*ST);
  if(EFI_ERROR(status)){
    return status;
  }
  status = ST->ConIn->Reset(ST->ConIn, FALSE);
  if(EFI_ERROR(status)){
    return status;
  }
  while((status = ST->ConIn->ReadKeyStroke(ST->ConIn, &key)) != EFI_NOT_READY);
  

  return status;

}

