

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

this is jsut using one function, therefore only one stack.

TOOD:
- Where are the memory addresses marked in the assembly file?
- what about multi function?
- conditional?

## executing-the-process

TODO: find out where the initial entry point is within the executable's header

In summary, the absence of an instruction setting the frame pointer in your main function suggests that the compiler has optimized it to use the stack pointer directly for stack frame management due to the function's simplicity. This is a valid optimization strategy. You'll likely see the frame pointer being used in more complex functions you might write or in library code.