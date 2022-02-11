C_SOURCES = $(wildcard kernel/*.c kernel/writer/*.c)
C_OBJ = ${C_SOURCES:.c=.o}

%.o: %.c
	x86_64-elf-gcc $< -c -fno-stack-protector -fpic -fshort-wchar -mno-red-zone -I gnu-efi/inc -I gnu-efi/inc/x86_64 -DEFI_FUNCTION_WRAPPER -ffreestanding -o $@
hexaos-amd64.efi: $(C_OBJ)
	x86_64-elf-ld $(C_OBJ) gnu-efi/x86_64/gnuefi/crt0-efi-x86_64.o -nostdlib -znocombreloc -T gnu-efi/gnuefi/elf_x86_64_efi.lds -shared -Bsymbolic -L /usr/lib -l:libgnuefi.a -l:libefi.a -o kernel.so
	objcopy -j .text -j .sdata -j .data -j .dynamic -j .dynsym -j .rel -j .rela -j .reloc --target=efi-app-x86_64 kernel.so hexaos-amd64.efi 
os-image.iso: hexaos-amd64.efi
	dd if=/dev/zero of=uefi.img bs=512 count=93750
	parted uefi.img -s -a minimal mklabel gpt
	parted uefi.img -s -a minimal mkpart EFI FAT16 2048s 93716s
	parted uefi.img -s -a minimal toggle 1 boot
	dd if=/dev/zero of=part.img bs=512 count=91669
	mformat -i part.img -h 32 -t 32 -n 64 -c 1
	mcopy -i part.img hexaos-amd64.efi ::
	dd if=part.img of=uefi.img bs=512 count=91669 seek=2048 conv=notrunc
run: os-image.iso
	qemu-system-x86_64 -cpu qemu64 -bios /usr/share/ovmf/OVMF.fd -drive file=uefi.img,if=ide -net none