C_SOURCE_FILES = $(wildcard kernel/writer/*.c kernel/*.c)
C_OBJ = ${C_SOURCE_FILES:.c=.o} 
%.o: %.c
	x86_64-elf-gcc -Ignu-efi/inc -fpic -ffreestanding -fno-stack-protector -fno-stack-check -fshort-wchar -mno-red-zone -maccumulate-outgoing-args -c $< -o $@
HEXAOS-AMD64.EFI: kernel/kernel.c $(C_OBJ)
	x86_64-elf-ld -shared -Bsymbolic -Lgnu-efi -Tgnu-efi/gnuefi/elf_x86_64_efi.lds crt0-efi-x86_64.o kernel/kernel.o $(C_OBJ) -o build/kernel.so -lgnuefi -lefi
	objcopy -j .text -j .sdata -j .data -j .dynamic -j .dynsym  -j .rel -j .rela -j .rel.* -j .rela.* -j .reloc --target efi-app-x86_64 --subsystem=10 build/kernel.so HEXAOS-AMD64.EFI
os-image.iso: HEXAOS-AMD64.EFI
	dd if=/dev/zero of=fat.img bs=1k count=1440
	mformat -i fat.img -f 1440 ::
	mmd -i fat.img ::/EFI
	mmd -i fat.img ::/EFI/BOOT
	mcopy -i fat.img HEXAOS-AMD64.EFI ::/EFI/BOOT
	cp fat.img iso
	xorriso -as mkisofs -R -f -e fat.img -no-emul-boot -o os-image.iso iso
run: os-image.iso
	@echo "Success!"
	qemu-system-x86_64 -L /usr/share/qemu/ -pflash /usr/share/qemu/OVMF.fd -cdrom $< -net none
  
  
  
