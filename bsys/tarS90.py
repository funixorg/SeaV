from struct import pack as _uspack
import os

"""
DIR:
	entry_size (u32) [DirHeader size in bytes]
	path_size  (u16) [path strlen]
	name_size  (u16) [name strlen]
	path       (str) [path]
	name       (str) [name]
	n_file     (u16) [file count]
	n_dir      (u16) [dir count]
	hfile_size (u32) [size of all files in bytes]
	hdir_size  (u32) [size of all dirs in bytes]
    cont_off   (u16) [offset from header to content]
    {FILE * n_file}  [file header + content]
    {DIR  * n_dir}   [dir header]

FILE:
    entry_size (u32) [FileHeader size in bytes]
	path_size  (u16) [path strlen]
	name_size  (u16) [name strlen]
	path       (str) [path]
	name       (str) [name]
    size       (u32) [content size in bytes]
    cont_off   (u16) [offset from header to content]
    {content}        [file content]
"""

class DirHeader:
    def pack(self, path:str, n_file:int, n_dir:int, hfile_size:int, hdir_size:int, files:bytes, dirs:bytes) -> bytes:
        self.path = "/"+'/'.join(path.split("/")[1:])
        self.name = self.path.split("/")[-1]
        if (self.name == ""): self.name = "/"
        self.path_size = len(self.path)
        self.name_size = len(self.name)
        
        self.n_file = n_file
        self.n_dir  = n_dir
        self.hfile_size = hfile_size
        self.hdir_size  = hdir_size

        header = _uspack(
                f"<HH{self.path_size}s{self.name_size}sHHII", self.path_size, self.name_size, self.path.encode(),
                                                            self.name.encode(), self.n_file, self.n_dir,
                                                            self.hfile_size, self.hdir_size)
        header += _uspack("<H", len(header)+6) # this + entry_size
        head_content = (
            b""
            +header
            +files
            +dirs
        )
        return (
            b""
            +_uspack("<I", len(head_content)+4)
            +head_content
        )

class FileHeader:
    def pack(self, path:str, content:bytes) -> bytes:
        self.path = "/"+'/'.join(path.split("/")[1:])
        self.name = self.path.split("/")[-1]
        self.path_size = len(self.path)
        self.name_size = len(self.name)
        
        self.size = len(content)

        header = _uspack(
                f"<HH{self.path_size}s{self.name_size}sI", self.path_size, self.name_size, self.path.encode(),
                                                            self.name.encode(), self.size)
        header += _uspack("<H", len(header)+6) # this + entry_size
        head_content = (
            b""
            +header
            +content
        )
        return (
            b""
            +_uspack("<I", len(head_content)+4)
            +head_content
        )

class TarArchive:
    def __init__(self, path:str, dest:str):
        self.source = path
        self.dest   = dest
        archive = self._parse_dir(path)
        open(dest, "wb").write(archive)
    
    def _parse_file(self, path:str) -> bytes:
        return FileHeader().pack(path, open(path, "rb").read())
    
    def _parse_dir(self, path:str) -> bytes:
        files: bytes = b""
        n_file: int = 0
        dirs: bytes = b""
        n_dir: int = 0

        for entry in os.listdir(path):
            if (os.path.isfile(os.path.join(path, entry))):
                files += self._parse_file(os.path.join(path, entry))
                n_file += 1
            else:
                dirs += self._parse_dir(os.path.join(path, entry))
                n_dir += 1
        return DirHeader().pack(path, n_file, n_dir, len(files), len(dirs), files, dirs)