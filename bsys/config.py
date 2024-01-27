
class Global:
    SOURCE  = "sov/"
    CEXT    = [".cpp", ".c"]
    AEXT    = ".s"
    OBJDIR  = "objects/"
    OUTPUT  = "bin/sov.bin"
    ISO     = "bin/sov.iso"
    RDDIR   = "initsysdir/"
    RDFILE  = "isodir/ramdisk.img"

class LIMINE:
    EXEC = "./limine/limine"
    CONF = "sov/config/limine.cfg"
    DIR  = "limine/"
    ISODIR = "isodir/"

class CC:
    CC = "g++"
    CCFLAGS = ' '.join(
        [
            f"-I./{Global.SOURCE}",
            f"-I./{LIMINE.DIR}",
            f"-I./{Global.SOURCE}cuteng/cstd/",
            "-c",
            "-Wall",
            "-Wextra",
            "-nostdlib",
            "-nodefaultlibs",
            "-nostdinc",
            "-ffreestanding",
            "-fno-exceptions",
            "-fno-lto",
            "-fPIE",
            "-m64",
            "-march=x86-64",
            "-mno-mmx",
            "-mno-sse",
            "-mno-sse2",
            "-mno-red-zone",
            "-fno-stack-protector",
            "-fno-stack-check",
            "-Wno-missing-field-initializers",
            "-Wno-pointer-arith",
            "-Wno-write-strings"
        ]
    )

class AS:
    AS = "as"
    ASFLAGS = ' '.join(
        [
        ]
    )

class LD:
    LD = "ld"
    LDFLAGS = ' '.join(
        [
            "-T sov/config/linker.ld",
            "-nostdlib",
            "-m elf_x86_64",
            "-static",
            "--no-dynamic-linker",
            "-z notext",
            "-z max-page-size=0x1000"
        ]
    )

class XORRISO:
    XR = "xorriso"
    XRFLAGS = ' '.join(
        [
            "-as mkisofs",
            "-b limine-bios-cd.bin",
            "-no-emul-boot",
            "-boot-load-size 4",
            "-boot-info-table",
            "--efi-boot limine-uefi-cd.bin",
            "--efi-boot-part",
            "--efi-boot-image",
            "--protective-msdos-label",
        ]
    )

class QEMU:
    QM = "qemu-system-x86_64"
    QMFLAGS = ' '.join(
        [
            "-smp 2",
            "-m 256",
            "-bios /usr/share/OVMF/x64/OVMF.fd",
            "-serial mon:stdio",
        ]
    )