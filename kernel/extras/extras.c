#include "efi.h"
#include "efilib.h"

EFI_FILE* LoadFile(EFI_FILE* dir, CHAR16* path, EFI_HANDLE image_handle, EFI_SYSTEM_TABLE* system_table){
    EFI_FILE* loaded;
    EFI_LOADED_IMAGE_PROTOCOL* Limage;

    system_table->BootServices->HandleProtocol(image_handle, &gEfiLoadedImageProtocolGuid, (void**)&Limage);

    EFI_SIMPLE_FILE_SYSTEM_PROTOCOL* filesystem;
    system_table->BootServices->HandleProtocol(Limage->DeviceHandle, &gEfiSimpleFileSystemProtocolGuid, (void**)filesystem);

    if(dir == NULL){
        filesystem->OpenVolume(filesystem, &dir);
    }

    EFI_STATUS status = dir->Open(dir, &loaded, path, EFI_FILE_MODE_READ, EFI_FILE_READ_ONLY);
    if(EFI_ERROR(status)){
        Print(L"Failed to load file...");
        return NULL;
    }
    return loaded;
}