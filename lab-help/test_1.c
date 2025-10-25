```c
#include "stdio.h"

void function(int a, int b, int c) {
         // let us have some pointer which we want later to point to that location in stack we are searching.  
int *ret=33;
    // ^ Initializes a pointer with the integer literal 33 (0x21). This is not a valid
    //   pointer in normal programs; compilers warn about “making pointer from integer”.
    //   If ever dereferenced, this would be undefined behavior (likely a crash).
    //   This line exists only so 'ret' has some value before it’s reassigned below.

   // find the address in stack where the function return address is stored.
  
 ret=&ret + 2;
    // ^^^^^^^^^^^
    // '&ret' is the address *of the local variable 'ret'*. Its type is 'int **'
    // (pointer to pointer-to-int), because 'ret' itself is an 'int *'.
    //
    // Pointer arithmetic scales by the size of the pointed-to type:
    //   - '&ret' has type 'int **'
    //   - adding 1 advances by sizeof(int*) bytes
    //   - adding 2 advances by 2 * sizeof(int*) bytes
    //
    // On a 64-bit system: sizeof(int*) == 8, so +2 == +16 bytes.
    // On a 32-bit system: sizeof(int*) == 4, so +2 == +8 bytes.
    //
    //
    // With a classic 64-bit frame that looks like this:
    //   [rbp+8 ]  saved return address
    //   [rbp+0 ]  saved old rbp
    //   [rbp-8 ]  local 'ret'
    // the address-of 'ret' is (rbp-8). Adding 16 bytes (+2 * 8) lands at (rbp+8),
    // which *in that specific build* is where the saved return address resides.
    // Key takeaway: the '2' here is a *count of int*-pointer-sized steps, not bytes.
    // '+ 2' means "+ 2 * sizeof(int*) bytes".

         // when you found it you have to modify it.
         // whati is X? change the address in a way that "x = 1" will never be executed.
  
 (*ret) += 7;
 // Explanation of the '+ 7' using the disassembled file obtained by `objdump -dS test`:
 // In your disassembly, the return site in main for the line `x = 1;` is at address 0x118f,
 // and the next line `printf("%d\n", x);` starts at 0x1196.
 // The difference is 0x1196 - 0x118f = 0x7 (hex), which is 7 in decimal.
 // So the literal 7 here corresponds to that 0x7-byte offset between the two instruction addresses.
 // Below is my disassembly
  /*
0000000000001139 <function>:
objdump: Warning: source file /home/ani/comporg25/stack_smashing/test.c is more recent than object file
#include "stdio.h"

void function(int a, int b, int c) {
1139:  55                    push   %rbp
113a:  48 89 e5              mov    %rsp,%rbp
113d:  89 7d ec              mov    %edi,-0x14(%rbp)
1140:  89 75 e8              mov    %esi,-0x18(%rbp)
1143:  89 55 e4              mov    %edx,-0x1c(%rbp)
// let us have some pointer which we want later to point to that location in stack we are searching.
//char str[8];
int *ret=33;
1146:  48 c7 45 f8 21 00 00  movq   $0x21,-0x8(%rbp)
114d:  00
// find the address in stack where the function return address is stored.
ret=&ret + 2;
ret = (int*)((char*)&a + 28);
114e:  48 8d 45 ec           lea    -0x14(%rbp),%rax
1152:  48 83 c0 1c           add    $0x1c,%rax
1156:  48 89 45 f8           mov    %rax,-0x8(%rbp)
// when you found it you have to modify it.
// whati is X? change the address in a way that "x = 1" will never be executed.
(*ret) += 7;
115a:  48 8b 45 f8           mov    -0x8(%rbp),%rax
115e:  8b 00                 mov    (%rax),%eax
1160:  8d 50 07              lea    0x7(%rax),%edx
1163:  48 8b 45 f8           mov    -0x8(%rbp),%rax
1167:  89 10                 mov    %edx,(%rax)
}
1169:  90                    nop
116a:  5d                    pop    %rbp
116b:  c3                    ret

000000000000116c <main>:

void main() {
116c:  55                    push   %rbp
116d:  48 89 e5              mov    %rsp,%rbp
1170:  48 83 ec 10           sub    $0x10,%rsp
int x;

x = 0;
1174:  c7 45 fc 00 00 00 00  movl   $0x0,-0x4(%rbp)
function(1,2,3);
117b:  ba 03 00 00 00        mov    $0x3,%edx
1180:  be 02 00 00 00        mov    $0x2,%esi
1185:  bf 01 00 00 00        mov    $0x1,%edi
118a:  e8 aa ff ff ff        call   1139 <function>
x = 1;
118f:  c7 45 fc 01 00 00 00  movl   $0x1,-0x4(%rbp)
printf("%d\n",x);
1196:  8b 45 fc              mov    -0x4(%rbp),%eax
1199:  89 c6                 mov    %eax,%esi
119b:  48 8d 05 62 0e 00 00  lea    0xe62(%rip),%rax        # 2004 <_IO_stdin_used+0x4>
11a2:  48 89 c7              mov    %rax,%rdi
11a5:  b8 00 00 00 00        mov    $0x0,%eax
11aa:  e8 81 fe ff ff        call   1030 [printf@plt](mailto:printf@plt)
}
11af:  90                    nop
11b0:  c9                    leave
11b1:  c3                    ret
*/


}

void main() {
  int x;

  x = 0;
  function(1,2,3);
  x = 1;
  printf("%d\n",x);
}
```
