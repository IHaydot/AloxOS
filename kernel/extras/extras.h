#pragma once
#include "efi.h"
#include "efilib.h"

EFI_FILE* LoadFile(EFI_FILE* dir, CHAR16* path, EFI_HANDLE image_handle, EFI_SYSTEM_TABLE* system_table);