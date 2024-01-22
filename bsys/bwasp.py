from os import system as _sysrun, walk as _pwalk
from os.path import join as _pjoin
from sys import argv as _argv
import config

class Builder:
    def __init__(self):
        self.src_path = config.Global.SOURCE
    
    def __srcf__(self, exts:list[str]) -> list[(str, str)]:
        sraobj: list[(str, str)] = []
        for (root, dirs,files) in _pwalk(self.src_path):
            for file in files:
                for ext in exts:
                    if file.endswith(ext):
                        sraobj.append(
                            (_pjoin(root, file), _pjoin(config.Global.OBJDIR, file).replace(ext, ".o"))
                        )
        return sraobj
    
    def _cbuild(self) -> list[str]:
        objs:list[str] = []
        for file in self.cfiles:
            print("[CC] {}".format(file[0]))
            ret = _sysrun(' '.join((
                config.CC.CC,
                config.CC.CCFLAGS,
                file[0],
                "-o {}".format(file[1])
            )))
            if ret != 0: exit(ret)
            objs.append(file[1])
        return objs
    
    def _asbuild(self) -> list[str]:
        objs:list[str] = []
        for file in self.afiles:
            print("[AS] {}".format(file[0]))
            ret = _sysrun(' '.join((
                config.AS.AS,
                config.AS.ASFLAGS,
                file[0],
                "-o {}".format(file[1])
            )))
            if ret != 0: exit(ret)
            objs.append(file[1])
        return objs

    def _link(self) -> None:
        print("[LD] {}".format(config.Global.OUTPUT))
        ret = _sysrun(' '.join((
            config.LD.LD,
            config.LD.LDFLAGS,
            ' '.join(self.objects),
            "-o {}".format(config.Global.OUTPUT)
        )))
        if ret != 0: exit(ret)
    
    def _limine(self) -> None:
        print(f"[COPY] -- KERNEL, CONF, LIMINE")
        ret = _sysrun(' '.join((
            "cp",
            config.Global.OUTPUT,
            config.LIMINE.CONF,
            f"{config.LIMINE.DIR}limine-bios.sys",
            f"{config.LIMINE.DIR}limine-bios-cd.bin",
            f"{config.LIMINE.DIR}limine-uefi-cd.bin",
            config.LIMINE.ISODIR
        )))
        if ret != 0: exit(ret)
        _sysrun(' '.join((
            "mkdir -p",
            f"{config.LIMINE.ISODIR}EFI/BOOT/"
        )))
        print(f"[COPY] -- EFI BOOT")
        _sysrun(' '.join((
            "cp",
            f"{config.LIMINE.DIR}BOOTX64.EFI {config.LIMINE.ISODIR}EFI/BOOT/"
        )))
        _sysrun(' '.join((
            "cp",
            f"{config.LIMINE.DIR}BOOTIA32.EFI {config.LIMINE.ISODIR}EFI/BOOT/"
        )))
        print(f"[XORRISO] -- {config.Global.ISO}")
        ret = _sysrun(' '.join((
            config.XORRISO.XR,
            config.XORRISO.XRFLAGS,
            config.LIMINE.ISODIR,
            "-o {}".format(config.Global.ISO),
            ">/dev/null 2>&1"
        )))
        if ret != 0:
            _sysrun(' '.join((
                config.XORRISO.XR,
                config.XORRISO.XRFLAGS,
                config.LIMINE.ISODIR,
                "-o {}".format(config.Global.ISO),
            )))
            exit(ret)
        print(f"[LIMINE] -- {config.Global.ISO}")
        ret = _sysrun(' '.join((
            config.LIMINE.EXEC,
            "bios-install",
            config.Global.ISO,
            ">/dev/null 2>&1"
        )))
        if ret != 0:
            _sysrun(' '.join((
                config.LIMINE.EXEC,
                "bios-install",
                config.Global.ISO,
            )))
            exit(ret)
        

    def build(self) -> None:
        self.cfiles = self.__srcf__(config.Global.CEXT)
        self.afiles = self.__srcf__(config.Global.AEXT)
        self.objects = []

        self.objects += self._cbuild()
        self.objects += self._asbuild()
        if self.objects: 
            self._link()
            self._limine()

class Runner():
    def __init__(self):
        pass

    def run(self):
        print("[QEMU] {}".format(config.Global.OUTPUT))
        ret = _sysrun(' '.join((
            config.QEMU.QM,
            config.QEMU.QMFLAGS,
            "-cdrom {}".format(config.Global.ISO),
        )))
        if ret != 0: exit(ret)


def main():
    if len(_argv)<2: return

    if (_argv[1] == "build"): Builder().build()
    elif (_argv[1] == "run"): Runner().run()

if __name__ == "__main__":
    main()