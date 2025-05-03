

## creating-the-executable

Creating an executable in C* is done via `gcc`. This can be done in one shot via `gcc <filename>` *or* the whole process can be viewed iteratively.

Conceptually, compiling an executable in C looks something like this end-to-end:

1. Generating the *C* source code.
- For the sake of this example, we are talking about one file. However, in the real-world, software projects are typically consisted of many files serving as interdependent modules, so this process would scale for each file accordingly.
2. Preprocessing the *C* code. This is usually done to clean up the source code for the compiler by resolving the header files, macros, and conditionals.
3. The compiler produces the assembly code, the human-readable version of the machine code.
4. The assembler produces the raw machine code, generating an object file.
- Regarding the object file, these are *not* executables. Yes, they are in machine-executable (binary) code. However, object files are missing one final step; they typically have temporary *links*  to the dependent libraries or modules (remember the “in the real-world” example) that are being compiled in parallel. These links must be resolved to insert the machine code of these other files.
5. The linker fully resolves all links within the objects files together into a single machine-executable file.

*Other compiled programming languages vary in their compilation styles.

This is an [informative cheat sheet](https://github.com/funnydman/nasm-assembly-examples/blob/master/gcc-cheat-sheet.md) for `gcc` that helps facilitate generating this process iteratively.

The files in this directory contain a sample program `add.c` with its yielded files from the compilation process using: `gcc -save-temps ../add.c -o add`.

## understanding-the-assembly

Taking a look at the assembly code, I've noted whether the instructed is performed on the CPU core or is a read/write call to memory.

I preferred reading the assembly instructions straight from `objdump` since they provide the addresses for instructions.

## executing-the-process

2 simple programs were executed to help understand the process execution flow.

I noticed that even for simpler programs (`add`), compiler optimization strategies are employed. i.e. deciding not to maintain a frame (base) pointer.
Sometimes, the optimatization strategy actually makes the code *less* optimal. i.e. why did `add.s` need to store `a` and `b` in memory instead of adding from 2 registers directly?

The program's entrypoint was identified using `otool -lv add | grep entry`, giving the offset. When converted to hex, this mapped with the `main` address in `objdump -d --macho add`.

Running the program via `lldb`, I was able to step using `nexti` to see each instruction execute as well as the program counter (`pc`) changes.

```(lldb) register read pc
      pc = 0x0000000100003f5c  multi-function`main + 8
...
(lldb) register read pc
      pc = 0x0000000100003f60  multi-function`main + 12
```

Additionally, the updates to the link register
In a second program (`multi-function.c`), I was able to see how function calls convert into branch instructions. I was able to track the link register (`lr`) updating after th function `adding` was called.
```
(lldb) run
Process 29581 launched: '/Users/ashadahmed/fun/Fundamentals-of-Operating-Systems/anatomy-of-a-process/understanding-the-assembly/multi-function' (arm64)
Process 29581 stopped
* thread #1, queue = 'com.apple.main-thread', stop reason = breakpoint 2.1
    frame #0: 0x0000000100003f84 multi-function`main + 48
multi-function`main:
->  0x100003f84 <+48>: bl     0x100003f2c               ; adding
    0x100003f88 <+52>: mov    x8, x0
    0x100003f8c <+56>: ldr    w0, [sp, #0xc]
    0x100003f90 <+60>: str    w8, [sp, #0x10]
Target 0: (multi-function) stopped.
(lldb) register read lr
      lr = 0x000000019c57f154  dyld`start + 2476
(lldb) next
Process 29581 stopped
* thread #1, queue = 'com.apple.main-thread', stop reason = instruction step over
    frame #0: 0x0000000100003f88 multi-function`main + 52
multi-function`main:
->  0x100003f88 <+52>: mov    x8, x0
    0x100003f8c <+56>: ldr    w0, [sp, #0xc]
    0x100003f90 <+60>: str    w8, [sp, #0x10]
    0x100003f94 <+64>: ldp    x29, x30, [sp, #0x20]
Target 0: (multi-function) stopped.
(lldb) register read lr
      lr = 0x0000000100003f88  multi-function`main + 52 <- Updated link register
```

I was also curious to see if it was possible to replicate a L-Cache miss (due to being 88 bytes away) and see the CPU fetch a new 64-bytes of memory. I did see the function move:
```(lldb) n
Process 47492 stopped
* thread #1, queue = 'com.apple.main-thread', stop reason = instruction step over
    frame #0: 0x0000000100003f84 multi-function`main + 48
multi-function`main:
->  0x100003f84 <+48>: bl     0x100003f2c               ; adding
    0x100003f88 <+52>: mov    x8, x0
    0x100003f8c <+56>: ldr    w0, [sp, #0xc]
    0x100003f90 <+60>: str    w8, [sp, #0x10]
Target 0: (multi-function) stopped.
(lldb) s
Process 47492 stopped
* thread #1, queue = 'com.apple.main-thread', stop reason = instruction step into
    frame #0: 0x0000000100003f2c multi-function`adding
multi-function`adding:
->  0x100003f2c <+0>:  sub    sp, sp, #0x10
    0x100003f30 <+4>:  str    w0, [sp, #0xc]
    0x100003f34 <+8>:  str    w1, [sp, #0x8]
    0x100003f38 <+12>: ldr    w8, [sp, #0xc]
Target 0: (multi-function) stopped.
```

But, it does not seem feasible to see that occur on MacOS without additional tooling.


I wrote a program `stack-overflow.c` to explicitly cause a stack overflow. I was interested to see how the process' allocated stack space fills up.
I first determined the total stack space before letting the seg fault occur.

```Process 12900 launched: '/Users/ashadahmed/fun/Fundamentals-of-Operating-Systems/anatomy-of-a-process/understanding-the-assembly/stack-overflow' (arm64)
Process 12900 stopped
* thread #1, queue = 'com.apple.main-thread', stop reason = breakpoint 6.1
    frame #0: 0x0000000100003f5c stack-overflow`main
stack-overflow`main:
->  0x100003f5c <+0>:  sub    sp, sp, #0x30
    0x100003f60 <+4>:  stp    x29, x30, [sp, #0x20]
    0x100003f64 <+8>:  add    x29, sp, #0x20
    0x100003f68 <+12>: mov    w8, #0x0
Target 0: (stack-overflow) stopped.
(lldb) register read sp
      sp = 0x000000016fdfeed0
(lldb) memory region 0x000000016fdfeed0
[0x000000016f604000-0x000000016fe00000) rw-
Modified memory (dirty) page list provided, 1 entries.
Dirty pages: 0x16fdfc000.```

After letting the program run, I calculated the difference between the initial frame pointer and the stack pointer. The difference indeed covered almost the entire range of the stack space. (~8.36MB)
```
(lldb) register read fp
      fp = 0x000000016fdff0f0
(lldb) run
There is a running process, kill it and restart?: [Y/n] n
(lldb) c
Process 1737 resuming
Process 1737 stopped
* thread #1, queue = 'com.apple.main-thread', stop reason = EXC_BAD_ACCESS (code=2, address=0x16f603ff0)
    frame #0: 0x0000000100003f2c stack-overflow`adding + 4
stack-overflow`adding:
->  0x100003f2c <+4>:  stp    x29, x30, [sp, #0x10]
    0x100003f30 <+8>:  add    x29, sp, #0x10
    0x100003f34 <+12>: stur   w0, [x29, #-0x4]
    0x100003f38 <+16>: str    w1, [sp, #0x8]
Target 0: (stack-overflow) stopped.
(lldb) register read sp
      sp = 0x000000016f603fe0
```
```
>>> 0x000000016fdff0f0 - 0x000000016f603fe0
8368400
>>> 8368400 / 1000
8368.4
>>> 8368400 / 1000 / 1000
8.3684
```