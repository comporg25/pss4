# Problem Power (original)

Write an x86-64 Linux assembly program that:

1. Reads exactly 2 bytes from standard input into a 2-byte buffer (`inputbuff`).
2. Treats the first byte as the base (`'0'..'9' → 0..9`) and the second byte as the exponent (`'0'..'9' → 0..9`).
3. Computes ( \text{base}^{\text{exponent}} ) via repeated multiplication.
4. Exits the process with the computed value as the exit status (kernel uses only the low 8 bits).

# Modified problem

Refactor the program so the exponentiation logic lives in a **separate function** named `power`. The `_start` entry point should:

* Read the two ASCII digits,
* Convert them to integers,
* Call `power(base, exponent)` using the calling convention (base in `rdi`, exponent in `rsi`),
* Receive the result in `rax`,
* Exit with that result.

