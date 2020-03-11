# bf-interpreter
An interpreter for the esoteric programming language Brainf\*ck, written in C. 

Currently, this interpreter does not perform any semantic checks. It can only interpret valid BF programs.

Running this interpreter on non-valid BF programs results in indeterminant behavior. Likely, this interpreter will attempt to interpret the initial part of the program that looks like a valid BF program and then give up when the program appears to no longer be a BF program.

Written in less than one day while my university canceled in-person classes due to COVID-19.
