#include "stdio.h"

void function(int a, int b, int c) {
	 // let us have some pointer which we want later to point to that location in stack we are searching.
int *ret=33;
   // ^ Initializes a pointer with the integer literal 33 (0x21). This isn’t a valid pointer
   //   in a normal hosted program; it’s immediately overwritten below.

   // find the address in stack where the function return address is stored.
ret = (int*)((char*)&a + 28);
   // ^ Treat &a as a byte pointer (char*) and add 28 bytes (0x1C), then cast to int*.
   //   Explanation based on the disassembly of this build:
   //   - The first integer argument arrives in the 32-bit register %edi (which is the low part of %rdi on x86-64).
   //   - At -O0 with a frame pointer, the compiler spills it to the stack with:
   //       mov    %edi, -0x14(%rbp)
   //     Here 0x14 == 20 decimal, so 'a' resides at [rbp - 20].
   //   - In a classic frame, the saved return address is at [rbp + 8].
   //   - To go from [rbp - 20] (location of 'a') to [rbp + 8] (saved return address),
   //     you need to move forward by 28 bytes:  (+20 to reach rbp) + (+8 to reach rbp+8) = 28.
   //   - That is why we add 28 to &a in this specific build to land at the saved return address.

	 // when you found it you have to modify it.
	 // whati is X? change the address in a way that "x = 1" will never be executed.
 (*ret) += 7;
/*
   Why “+ 7” here?

   From the disassembly of this build (objdump -dS), the call site in main looks like:

       118a:  e8 aa ff ff ff        call   1139 <function>
       118f:  c7 45 fc 01 00 00 00  movl   $0x1,-0x4(%rbp)     ; x = 1;
       1196:  8b 45 fc              mov    -0x4(%rbp),%eax     ; printf("%d\n", x);

   The instruction address for `x = 1;` is 0x118f, and the next statement (`printf`) starts at 0x1196.
   The difference is:

       0x1196 - 0x118f = 0x07   →  7 (decimal)

   In the generated machine code for this line you can also see the constant 7 encoded directly:

       115e: 8b 00                 mov    (%rax),%eax     ; load *ret
       1160: 8d 50 07              lea    0x7(%rax),%edx  ; add 7 to the loaded value
       1167: 89 10                 mov    %edx,(%rax)     ; store back

   So `(*ret) += 7;` corresponds to adding that 7-byte delta observed between the instruction
   addresses 0x118f and 0x1196 in this specific build.

*/
}

void main() {
  int x;

  x = 0;
  function(1,2,3);
  x = 1;
  printf("%d\n",x);
}
