.section .bss                  # Uninitialized data section

.lcomm inputbuff, 2            # Reserve 2 bytes named 'inputbuff' (no initial value)

.section .text                 # Code section

.globl _start                  # Make _start the entry point

_start:

mov $0, %rax                   # rax = 0 -> sys_read
mov $0, %rdi                   # rdi = 0 -> file descriptor 0 (stdin)
mov $inputbuff, %rsi           # rsi = ADDRESS of inputbuff (buffer pointer)
mov $2, %rdx                   # rdx = 2 -> read exactly 2 bytes
syscall                        # perform read(0, &inputbuff, 2)

convert:
mov inputbuff, %rdi            # rdi = *QWORD at address 'inputbuff'
                               # (NOTE: loads 8 bytes starting at inputbuff; buffer is only 2 bytes.)
sub $48, %rdi                  # rdi -= '0' (48) -> intended: convert ASCII digit to integer (base)

mov inputbuff+1, %rsi          # rsi = *QWORD at address 'inputbuff+1' (starts from the 2nd byte)
sub $48, %rsi                  # rsi -= '0' (48) -> intended: convert ASCII digit to integer (exponent)

call power                     # call power(base in rdi, exponent in rsi); result returned in rax

exit:
mov %rax, %rdi                 # move result to rdi as exit status (low 8 bits used)
mov $60, %rax                  # rax = 60 -> sys_exit
syscall                        # exit(result)

.type power @function          # Declare 'power' as a function symbol

power:

push %rbp                      # function prologue: save old base pointer
movq %rsp, %rbp                # set new base pointer

mov $1, %rax                   # result accumulator = 1

loop:

cmp $0, %rsi                   # compare exponent (rsi) with 0
je return                      # if exponent == 0, done -> return

imul %rdi, %rax                # rax *= rdi -> multiply result by base
dec %rsi                       # rsi-- -> decrement exponent
jmp loop                       # repeat until exponent reaches 0

return:
movq %rbp, %rsp                # function epilogue: restore stack pointer
pop %rbp                       # restore old base pointer
ret                            # return; result is in rax
