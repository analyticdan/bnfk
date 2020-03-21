# bfnk
This repo contains an interpreter and a compiler for the esoteric programming language Brainf\*ck, both written in C. The compiler compiles BF files to C files.

Currently, neither the interpreter nor the compiler perform any semantic checks. It can only interpret or compile valid BF programs.

Running these programs on non-valid BF programs result in indeterminant behavior. Likely, the interpreter will attempt to interpret the initial part of a BF program that looks like a valid BF program and then give up when the program appears to no longer be a BF program. The compiler will likely produce an invalid C program.

Both the interpreter and the compiler were written in less than one day each while my university canceled in-person classes due to COVID-19.
