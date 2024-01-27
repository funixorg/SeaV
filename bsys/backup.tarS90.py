from struct import pack as _uspack
import os

class TarType:
    DIRECTORY = '5'
    FILE      = '0'

class TarHeader:
    def pack(self, name:str, type:str, content:bytes) -> bytes:
        self.name = name         # 100
        self.size = len(content) # 8
        self.typeflag = type     # 1
        self.magic = b"#tars90#" # 8
        self.version = b'00'     # 2
        self.padding = b'\x00'*9 # 9

        result = b""
        result += _uspack("100s", self.name.encode())
        result += _uspack("<Q", self.size)
        result += _uspack("c", self.typeflag.encode())
        result += self.magic+self.version+self.padding
        result += _uspack(f"{self.size}s", content)
        return result

    def _dir_to_headers(source:str) -> bytes:
        archive: bytes = b""
        for item in os.listdir(source):
            archive += TarHeader._ositem_to_header(source, item)
        return archive

    def _ositem_to_header(base:str, path: str) -> bytes:
        header = None
        path = os.path.join(base,path)
        type = TarType.DIRECTORY
        content = b""
        if os.path.isfile(path): 
            type = TarType.FILE
            content = open(path, "rb").read()
        else:
            dirs = len([x[0] for x in os.walk(path)])-1
            files = [len(x) for _,_,x in os.walk(path)][0]
            content = _uspack("ii", dirs, files)
    
        name = '/'+'/'.join(path.split("/")[1:])
        header = TarHeader().pack(name, type, content)

        if not os.path.isfile(path):
            header+=TarHeader._dir_to_headers(path)
        
        return header
    
    def dir_to_archive(source:str, path:str) -> None:
        dirs = len([x for _,x,_ in os.walk(source)][0])
        files = [len(x) for _,_,x in os.walk(source)][0]
        content = _uspack("ii", dirs, files)
        archive = TarHeader().pack("/", TarType.DIRECTORY, content)
        archive += TarHeader._dir_to_headers(source)
        open(path, "wb").write(archive)