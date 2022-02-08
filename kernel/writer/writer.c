#include <efi.h>
#include <efilib.h>

void INIT_GOP(EFI_SYSTEM_TABLE *table){
    EFI_STATUS status;
    EFI_GUID gopGUID = EFI_GRAPHICS_OUTPUT_PROTOCOL_GUID;
    EFI_GRAPHICS_OUTPUT_PROTOCOL *gop;
    status = uefi_call_wrapper(BS->LocateProtocol, 3, &gopGUID, NULL, (void**)&gop);
    if(EFI_ERROR(status)){
        
    }

}