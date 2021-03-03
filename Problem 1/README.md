##Instructions:
Navigate terminal to directory and use makefile to compile the program from terminal:
`make clean`
`make`

Using any number of your own input files, or the provided A.txt and B.txt, run the executable from terminal:
`./myfiles A.txt B.txt output.txt`

Or, using strace:
`strace ./myfiles A.txt B.txt output.txt`

Output from strace:


    execve("./myfiles", ["./myfiles", "A.txt", "B.txt", "output.txt"], 0x7ffef00413d8 /* 66 vars */) = 0
    brk(NULL)                               = 0x55b1d79d7000
    arch_prctl(0x3001 /* ARCH_??? */, 0x7ffcfb8b3770) = -1 EINVAL (Invalid argument)
    access("/etc/ld.so.preload", R_OK)      = -1 ENOENT (No such file or directory)
    openat(AT_FDCWD, "/etc/ld.so.cache", O_RDONLY|O_CLOEXEC) = 6
    fstat(6, {st_mode=S_IFREG|0644, st_size=82248, ...}) = 0
    mmap(NULL, 82248, PROT_READ, MAP_PRIVATE, 6, 0) = 0x7f0a6d3f1000
    close(6)                                = 0
    openat(AT_FDCWD, "/lib/x86_64-linux-gnu/libc.so.6", O_RDONLY|O_CLOEXEC) = 6
    read(6, "\177ELF\2\1\1\3\0\0\0\0\0\0\0\0\3\0>\0\1\0\0\0\360q\2\0\0\0\0\0"..., 832) = 832
    pread64(6, "\6\0\0\0\4\0\0\0@\0\0\0\0\0\0\0@\0\0\0\0\0\0\0@\0\0\0\0\0\0\0"..., 784, 64) = 784
    pread64(6, "\4\0\0\0\20\0\0\0\5\0\0\0GNU\0\2\0\0\300\4\0\0\0\3\0\0\0\0\0\0\0", 32, 848) = 32
    pread64(6, "\4\0\0\0\24\0\0\0\3\0\0\0GNU\0\t\233\222%\274\260\320\31\331\326\10\204\276X>\263"..., 68, 880) = 68
    fstat(6, {st_mode=S_IFREG|0755, st_size=2029224, ...}) = 0
    mmap(NULL, 8192, PROT_READ|PROT_WRITE, MAP_PRIVATE|MAP_ANONYMOUS, -1, 0) = 0x7f0a6d3ef000
    pread64(6, "\6\0\0\0\4\0\0\0@\0\0\0\0\0\0\0@\0\0\0\0\0\0\0@\0\0\0\0\0\0\0"..., 784, 64) = 784
    pread64(6, "\4\0\0\0\20\0\0\0\5\0\0\0GNU\0\2\0\0\300\4\0\0\0\3\0\0\0\0\0\0\0", 32, 848) = 32
    pread64(6, "\4\0\0\0\24\0\0\0\3\0\0\0GNU\0\t\233\222%\274\260\320\31\331\326\10\204\276X>\263"..., 68, 880) = 68
    mmap(NULL, 2036952, PROT_READ, MAP_PRIVATE|MAP_DENYWRITE, 6, 0) = 0x7f0a6d1fd000
    mprotect(0x7f0a6d222000, 1847296, PROT_NONE) = 0
    mmap(0x7f0a6d222000, 1540096, PROT_READ|PROT_EXEC, MAP_PRIVATE|MAP_FIXED|MAP_DENYWRITE, 6, 0x25000) = 0x7f0a6d222000
    mmap(0x7f0a6d39a000, 303104, PROT_READ, MAP_PRIVATE|MAP_FIXED|MAP_DENYWRITE, 6, 0x19d000) = 0x7f0a6d39a000
    mmap(0x7f0a6d3e5000, 24576, PROT_READ|PROT_WRITE, MAP_PRIVATE|MAP_FIXED|MAP_DENYWRITE, 6, 0x1e7000) = 0x7f0a6d3e5000
    mmap(0x7f0a6d3eb000, 13528, PROT_READ|PROT_WRITE, MAP_PRIVATE|MAP_FIXED|MAP_ANONYMOUS, -1, 0) = 0x7f0a6d3eb000
    close(6)                                = 0
    arch_prctl(ARCH_SET_FS, 0x7f0a6d3f0540) = 0
    mprotect(0x7f0a6d3e5000, 12288, PROT_READ) = 0
    mprotect(0x55b1d6f08000, 4096, PROT_READ) = 0
    mprotect(0x7f0a6d433000, 4096, PROT_READ) = 0
    munmap(0x7f0a6d3f1000, 82248)           = 0
    brk(NULL)                               = 0x55b1d79d7000
    brk(0x55b1d79f8000)                     = 0x55b1d79f8000
    openat(AT_FDCWD, "A.txt", O_RDONLY)     = 6
    openat(AT_FDCWD, "B.txt", O_RDONLY)     = 7
    openat(AT_FDCWD, "output.txt", O_WRONLY|O_CREAT|O_TRUNC, 0666) = 8
    close(8)                                = 0
    lseek(6, 0, SEEK_END)                   = 40
    lseek(6, 0, SEEK_SET)                   = 0
    read(6, "Data for file_in_1 This is OS 43"..., 40) = 40
    openat(AT_FDCWD, "output.txt", O_WRONLY|O_CREAT|O_APPEND, 0666) = 8
    lseek(8, 0, SEEK_END)                   = 0
    write(8, "Data for file_in_1 This is OS 43"..., 40) = 40
    lseek(7, 0, SEEK_END)                   = 40
    lseek(7, 0, SEEK_SET)                   = 0
    read(7, "data for file_in_2 but also OS 5"..., 40) = 40
    openat(AT_FDCWD, "output.txt", O_WRONLY|O_CREAT|O_APPEND, 0666) = 9
    lseek(9, 0, SEEK_END)                   = 40
    write(9, "data for file_in_2 but also OS 5"..., 40) = 40
    close(6)                                = 0
    close(7)                                = 0
    exit_group(0)                           = ?
    +++ exited with 0 +++