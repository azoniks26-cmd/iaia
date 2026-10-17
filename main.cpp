#include <cstdio>
#include <cstdint>
#include <cstring>
#include <cstdlib>
#include <vector>
#include <string>
#include <algorithm>
#include <limits>
#include <map>
#include <elf.h>
#include <fcntl.h>
#include <unistd.h>
#include <sys/stat.h>

asm(R"asm(
.text
.macro QR a, b, c, d
mov \b(%rsp), %eax
add %eax, \a(%rsp)
mov \a(%rsp), %eax
xor %eax, \d(%rsp)
mov \d(%rsp), %eax
rol $16, %eax
mov %eax, \d(%rsp)
mov \d(%rsp), %eax
add %eax, \c(%rsp)
mov \c(%rsp), %eax
xor %eax, \b(%rsp)
mov \b(%rsp), %eax
rol $12, %eax
mov %eax, \b(%rsp)
mov \b(%rsp), %eax
add %eax, \a(%rsp)
mov \a(%rsp), %eax
xor %eax, \d(%rsp)
mov \d(%rsp), %eax
rol $8, %eax
mov %eax, \d(%rsp)
mov \d(%rsp), %eax
add %eax, \c(%rsp)
mov \c(%rsp), %eax
xor %eax, \b(%rsp)
mov \b(%rsp), %eax
rol $7, %eax
mov %eax, \b(%rsp)
.endm
.globl xn_stub_start
.type xn_stub_start,@function
xn_stub_start:
call .Lxn_self
.Lxn_self:
pop %rax
sub $5, %rax
push %rbp
push %rbx
push %r12
push %r13
push %r14
push %r15
sub $1032, %rsp
lea xn_cfg_start(%rip), %rbx
movabs $0x584e4542554c4158, %r9
cmp %r9, 0(%rbx)
je .Lxn_magic_ok
lea .Lmsg_bad(%rip), %rsi
mov $(.Lmsg_bad_end - .Lmsg_bad), %edx
jmp .Lxn_die
.Lxn_magic_ok:
mov 8(%rbx), %r9
mov %rax, %r10
sub %r9, %r10
mov %r10, 0(%rsp)
mov 16(%rbx), %r9
mov %r9, 8(%rsp)
mov 24(%rbx), %r9
mov %r9, 16(%rsp)
lea 64(%rbx), %r9
mov %r9, 24(%rsp)
mov $4, %edi
xor %esi, %esi
mov $157, %eax
syscall
call .Lxn_tracer_check
mov $9, %eax
xor %edi, %edi
mov $16384, %esi
mov $3, %edx
mov $0x22, %r10d
mov $-1, %r8
xor %r9d, %r9d
syscall
test %rax, %rax
js .Lxn_mon_fail
mov %rax, %r12
mov %rax, 320(%rsp)
mov 0(%rsp), %r9
mov %r9, 0(%rax)
lea 64(%rbx), %r9
mov %r9, 8(%rax)
mov 24(%rbx), %r9
mov %r9, 16(%rax)
movq $0, 24(%rax)
lea 16384(%rax), %rsi
mov $0x50f00, %edi
xor %edx, %edx
xor %r10d, %r10d
xor %r8d, %r8d
mov $56, %eax
syscall
test %rax, %rax
js .Lxn_mon_fail
jz .Lxn_monitor
.Lxn_dbg_done:
mov 0(%rsp), %rsi
add 8(%rbx), %rsi
mov $(xn_code_end - xn_stub_start), %ecx
lea (%rsi,%rcx,1), %rdi
movabs $0xcbf29ce484222325, %r12
movabs $0x100000001b3, %r13
.Lxn_pad_hash:
movzbl (%rsi), %r9d
xor %r9, %r12
imul %r13, %r12
inc %rsi
cmp %rdi, %rsi
jb .Lxn_pad_hash
xor %r15d, %r15d
.Lxn_key:
lea 1(%r15), %r9
movabs $0x9e3779b97f4a7c15, %r10
imul %r10, %r9
add %r12, %r9
mov %r9, %r10
shr $30, %r10
xor %r10, %r9
movabs $0xbf58476d1ce4e5b9, %r10
imul %r10, %r9
mov %r9, %r10
shr $27, %r10
xor %r10, %r9
movabs $0x94d049bb133111eb, %r10
imul %r10, %r9
mov %r9, %r10
shr $31, %r10
xor %r10, %r9
mov %r15, %r10
shl $3, %r10
mov 32(%rbx,%r10,1), %r11
xor %r9, %r11
mov %r11, 32(%rsp,%r10,1)
inc %r15
cmp $4, %r15
jb .Lxn_key
.Lxn_range_loop:
cmpq $0, 16(%rsp)
je .Lxn_ranges_done
decq 16(%rsp)
mov 24(%rsp), %rsi
mov 0(%rsi), %r9
mov 8(%rsi), %r10
mov 16(%rsi), %r11
add $24, %rsi
mov %rsi, 24(%rsp)
mov %r9, 256(%rsp)
mov %r11, 280(%rsp)
mov %r10, %r11
shr $56, %r11
mov %r11, 272(%rsp)
shl $8, %r10
shr $8, %r10
mov %r10, 264(%rsp)
test %r10, %r10
jz .Lxn_range_loop
mov 0(%rsp), %r8
add %r8, %r9
mov %r9, 288(%rsp)
mov %r9, %rdi
and $-4096, %rdi
mov %r9, %rsi
add %r10, %rsi
add $4095, %rsi
and $-4096, %rsi
sub %rdi, %rsi
mov $3, %edx
mov $10, %eax
syscall
test %rax, %rax
jnz .Lxn_err_mprot
movl $0x61707865, 128(%rsp)
movl $0x3320646e, 132(%rsp)
movl $0x79622d32, 136(%rsp)
movl $0x6b206574, 140(%rsp)
.set xn_i, 0
.rept 4
mov 32+8*xn_i(%rsp), %r9
mov %r9, 144+8*xn_i(%rsp)
.set xn_i, xn_i+1
.endr
movl $0, 176(%rsp)
mov 256(%rsp), %r9
mov %r9d, 180(%rsp)
shr $32, %r9
mov %r9d, 184(%rsp)
mov 264(%rsp), %r9
mov %r9d, 188(%rsp)
mov 264(%rsp), %r9
mov %r9, 304(%rsp)
mov 288(%rsp), %r9
mov %r9, 296(%rsp)
movq $0, 312(%rsp)
.Lxn_block:
mov 312(%rsp), %r9
mov %r9d, 176(%rsp)
.set xn_i, 0
.rept 8
mov 128+8*xn_i(%rsp), %r9
mov %r9, 64+8*xn_i(%rsp)
.set xn_i, xn_i+1
.endr
mov $10, %r15d
.Lxn_round:
QR 64,80,96,112
QR 68,84,100,116
QR 72,88,104,120
QR 76,92,108,124
QR 64,84,104,124
QR 68,88,108,112
QR 72,92,96,116
QR 76,80,100,120
dec %r15d
jnz .Lxn_round
.set xn_i, 0
.rept 16
mov 128+4*xn_i(%rsp), %eax
add %eax, 64+4*xn_i(%rsp)
.set xn_i, xn_i+1
.endr
mov 304(%rsp), %r9
cmp $64, %r9
jbe .Lxn_nok
mov $64, %r9d
.Lxn_nok:
xor %ecx, %ecx
mov 296(%rsp), %rdi
.Lxn_xor:
cmp %r9, %rcx
jae .Lxn_xdone
movzbl 64(%rsp,%rcx,1), %eax
xor %al, (%rdi,%rcx,1)
inc %rcx
jmp .Lxn_xor
.Lxn_xdone:
add %r9, 296(%rsp)
sub %r9, 304(%rsp)
incq 312(%rsp)
cmpq $0, 304(%rsp)
jne .Lxn_block
mov 288(%rsp), %rsi
mov 264(%rsp), %rdi
add %rsi, %rdi
movabs $0xcbf29ce484222325, %r12
movabs $0x100000001b3, %r13
.Lxn_hash:
cmp %rdi, %rsi
jae .Lxn_hash_done
movzbl (%rsi), %r9d
xor %r9, %r12
imul %r13, %r12
inc %rsi
jmp .Lxn_hash
.Lxn_hash_done:
cmp 280(%rsp), %r12
jne .Lxn_err_integrity
mov 288(%rsp), %rdi
and $-4096, %rdi
mov 288(%rsp), %rsi
add 264(%rsp), %rsi
add $4095, %rsi
and $-4096, %rsi
sub %rdi, %rsi
mov 272(%rsp), %rdx
mov $10, %eax
syscall
test %rax, %rax
jnz .Lxn_err_mprot
jmp .Lxn_range_loop
.Lxn_ranges_done:
mov 320(%rsp), %r9
movq $1, 24(%r9)
mov 8(%rsp), %r9
test %r9, %r9
jz .Lxn_done
add 0(%rsp), %r9
call *%r9
.Lxn_done:
add $1032, %rsp
pop %r15
pop %r14
pop %r13
pop %r12
pop %rbx
pop %rbp
ret
.Lxn_err_mprot:
lea .Lmsg_mprot(%rip), %rsi
mov $(.Lmsg_mprot_end - .Lmsg_mprot), %edx
jmp .Lxn_die
.Lxn_err_integrity:
lea .Lmsg_integ(%rip), %rsi
mov $(.Lmsg_integ_end - .Lmsg_integ), %edx
jmp .Lxn_die
.Lxn_die:
mov $2, %edi
mov $1, %eax
syscall
mov $1, %edi
mov $231, %eax
syscall
.Lxn_tracer_check:
sub $2120, %rsp
lea .Lpath_task(%rip), %rsi
mov $-100, %rdi
mov $0x10000, %edx
mov $257, %eax
syscall
test %rax, %rax
js .Lxn_status_fail
mov %rax, 2048(%rsp)
.Lxn_tc_read:
mov 2048(%rsp), %rdi
mov %rsp, %rsi
mov $1024, %edx
mov $217, %eax
syscall
test %rax, %rax
js .Lxn_status_fail
jz .Lxn_tc_close
mov %rax, 2080(%rsp)
movq $0, 2072(%rsp)
.Lxn_tc_ent:
mov 2072(%rsp), %rax
cmp 2080(%rsp), %rax
jae .Lxn_tc_read
lea (%rsp,%rax,1), %r8
movzwl 16(%r8), %ecx
test %ecx, %ecx
jz .Lxn_status_fail
add %rcx, %rax
mov %rax, 2072(%rsp)
movzbl 19(%r8), %eax
cmp $48, %al
jb .Lxn_tc_ent
cmp $57, %al
ja .Lxn_tc_ent
mov 2048(%rsp), %rdi
lea 19(%r8), %rsi
mov $0x10000, %edx
mov $257, %eax
syscall
test %rax, %rax
js .Lxn_tc_ent
mov %rax, 2056(%rsp)
mov %rax, %rdi
lea .Lname_status(%rip), %rsi
xor %edx, %edx
mov $257, %eax
syscall
test %rax, %rax
js .Lxn_tc_close_tid
mov %rax, 2064(%rsp)
mov %rax, %rdi
lea 1024(%rsp), %rsi
mov $1023, %edx
xor %eax, %eax
syscall
mov %rax, 2088(%rsp)
mov 2064(%rsp), %rdi
mov $3, %eax
syscall
mov 2088(%rsp), %rdx
test %rdx, %rdx
jle .Lxn_tc_close_tid
lea 1024(%rsp), %rsi
add %rsi, %rdx
call .Lxn_scan_tracer
.Lxn_tc_close_tid:
mov 2056(%rsp), %rdi
mov $3, %eax
syscall
jmp .Lxn_tc_ent
.Lxn_tc_close:
mov 2048(%rsp), %rdi
mov $3, %eax
syscall
add $2120, %rsp
ret
.Lxn_scan_tracer:
lea .Lpat(%rip), %r8
.Lxn_sc_scan:
lea 10(%rsi), %r9
cmp %rdx, %r9
ja .Lxn_status_fail
xor %ecx, %ecx
.Lxn_sc_cmp:
cmp $10, %rcx
jae .Lxn_sc_found
movzbl (%rsi,%rcx,1), %eax
cmpb (%r8,%rcx,1), %al
jne .Lxn_sc_next
inc %rcx
jmp .Lxn_sc_cmp
.Lxn_sc_next:
inc %rsi
jmp .Lxn_sc_scan
.Lxn_sc_found:
add $10, %rsi
.Lxn_sc_ws:
cmp %rdx, %rsi
jae .Lxn_status_fail
movzbl (%rsi), %eax
cmp $32, %al
je .Lxn_sc_ws_next
cmp $9, %al
je .Lxn_sc_ws_next
cmp $48, %al
jne .Lxn_dbg_detected
ret
.Lxn_sc_ws_next:
inc %rsi
jmp .Lxn_sc_ws
.Lxn_status_fail:
lea .Lmsg_status(%rip), %rsi
mov $(.Lmsg_status_end - .Lmsg_status), %edx
jmp .Lxn_die
.Lxn_dbg_detected:
lea .Lmsg_dbg(%rip), %rsi
mov $(.Lmsg_dbg_end - .Lmsg_dbg), %edx
jmp .Lxn_die
.Lxn_mon_fail:
lea .Lmsg_mon(%rip), %rsi
mov $(.Lmsg_mon_end - .Lmsg_mon), %edx
jmp .Lxn_die
.Lxn_monitor:
sub $16, %rsp
movq $0, 0(%rsp)
movq $500000000, 8(%rsp)
.Lxn_mon_loop:
call .Lxn_tracer_check
cmpq $0, 24(%r12)
je .Lxn_mon_sleep
call .Lxn_integrity_scan
.Lxn_mon_sleep:
mov %rsp, %rdi
xor %esi, %esi
mov $35, %eax
syscall
jmp .Lxn_mon_loop
.Lxn_integrity_scan:
mov 8(%r12), %r13
mov 16(%r12), %r14
xor %r15d, %r15d
.Lxn_is_loop:
cmp %r14, %r15
jae .Lxn_is_done
lea (%r15,%r15,2), %rax
mov (%r13,%rax,8), %rsi
add (%r12), %rsi
mov 8(%r13,%rax,8), %rcx
shl $8, %rcx
shr $8, %rcx
movabs $0xcbf29ce484222325, %r8
movabs $0x100000001b3, %r9
.Lxn_is_hash:
test %rcx, %rcx
jz .Lxn_is_cmp
movzbl (%rsi), %edx
xor %rdx, %r8
imul %r9, %r8
inc %rsi
dec %rcx
jmp .Lxn_is_hash
.Lxn_is_cmp:
cmp 16(%r13,%rax,8), %r8
jne .Lxn_err_integrity
inc %r15
jmp .Lxn_is_loop
.Lxn_is_done:
ret
.Lmsg_bad:
.ascii "XNebula: bad configuration\n"
.Lmsg_bad_end:
.Lmsg_dbg:
.ascii "XNebula: debugger detected\n"
.Lmsg_dbg_end:
.Lmsg_mprot:
.ascii "XNebula: mprotect refused (SELinux or W^X policy)\n"
.Lmsg_mprot_end:
.Lmsg_integ:
.ascii "XNebula: code integrity check failed\n"
.Lmsg_integ_end:
.Lmsg_status:
.ascii "XNebula: process state unavailable\n"
.Lmsg_status_end:
.Lmsg_mon:
.ascii "XNebula: monitor unavailable\n"
.Lmsg_mon_end:
.Lpath_task:
.asciz "/proc/self/task"
.Lname_status:
.asciz "status"
.Lpat:
.ascii "TracerPid:"
.balign 16
.globl xn_code_end
xn_code_end:
.balign 16
.globl xn_cfg_start
xn_cfg_start:
.skip 64
.globl xn_stub_end
xn_stub_end:
)asm");

static const uint8_t xn_a64_stub[] = {
    0xfd, 0x7b, 0xb9, 0xa9, 0xf3, 0x53, 0x01, 0xa9, 0xf5, 0x5b, 0x02, 0xa9,
    0xf7, 0x63, 0x03, 0xa9, 0xf9, 0x6b, 0x04, 0xa9, 0xfb, 0x73, 0x05, 0xa9,
    0xff, 0x03, 0x11, 0xd1, 0xe0, 0x83, 0x00, 0xf9, 0xe1, 0x87, 0x00, 0xf9,
    0xe2, 0x8b, 0x00, 0xf9, 0xd3, 0xfe, 0xff, 0x10, 0xb4, 0x52, 0x00, 0x10,
    0x09, 0x2b, 0x88, 0xd2, 0x89, 0xa9, 0xaa, 0xf2, 0x49, 0xa8, 0xc8, 0xf2,
    0xc9, 0x09, 0xeb, 0xf2, 0x8a, 0x02, 0x40, 0xf9, 0x5f, 0x01, 0x09, 0xeb,
    0x80, 0x00, 0x00, 0x54, 0x21, 0x4a, 0x00, 0x10, 0x62, 0x03, 0x80, 0xd2,
    0xac, 0x01, 0x00, 0x14, 0x89, 0x06, 0x40, 0xf9, 0x6a, 0x02, 0x09, 0xcb,
    0xea, 0x03, 0x00, 0xf9, 0x89, 0x0a, 0x40, 0xf9, 0xe9, 0x07, 0x00, 0xf9,
    0x89, 0x0e, 0x40, 0xf9, 0xe9, 0x0b, 0x00, 0xf9, 0x89, 0x02, 0x01, 0x91,
    0xe9, 0x0f, 0x00, 0xf9, 0x80, 0x00, 0x80, 0xd2, 0x01, 0x00, 0x80, 0xd2,
    0xe8, 0x14, 0x80, 0xd2, 0x01, 0x00, 0x00, 0xd4, 0xa4, 0x01, 0x00, 0x94,
    0x00, 0x00, 0x80, 0xd2, 0x01, 0x00, 0x88, 0xd2, 0x62, 0x00, 0x80, 0xd2,
    0x43, 0x04, 0x80, 0xd2, 0x04, 0x00, 0x80, 0x92, 0x05, 0x00, 0x80, 0xd2,
    0xc8, 0x1b, 0x80, 0xd2, 0x01, 0x00, 0x00, 0xd4, 0xa0, 0x40, 0xf8, 0xb7,
    0xf6, 0x03, 0x00, 0xaa, 0xf6, 0x93, 0x00, 0xf9, 0xe9, 0x03, 0x40, 0xf9,
    0xc9, 0x02, 0x00, 0xf9, 0x89, 0x02, 0x01, 0x91, 0xc9, 0x06, 0x00, 0xf9,
    0x89, 0x0e, 0x40, 0xf9, 0xc9, 0x0a, 0x00, 0xf9, 0xdf, 0x0e, 0x00, 0xf9,
    0x01, 0x00, 0x88, 0xd2, 0xc1, 0x02, 0x01, 0x8b, 0x00, 0xe0, 0x81, 0x52,
    0xa0, 0x00, 0xa0, 0x72, 0x02, 0x00, 0x80, 0xd2, 0x03, 0x00, 0x80, 0xd2,
    0x04, 0x00, 0x80, 0xd2, 0x88, 0x1b, 0x80, 0xd2, 0x01, 0x00, 0x00, 0xd4,
    0x40, 0x3e, 0xf8, 0xb7, 0x80, 0x3e, 0x00, 0xb4, 0xed, 0x03, 0x13, 0xaa,
    0x0c, 0x50, 0x81, 0xd2, 0x6c, 0x02, 0x0c, 0x8b, 0xae, 0x64, 0x84, 0xd2,
    0x4e, 0x84, 0xb0, 0xf2, 0x8e, 0x9c, 0xd3, 0xf2, 0x4e, 0x7e, 0xf9, 0xf2,
    0x6f, 0x36, 0x80, 0xd2, 0x0f, 0x00, 0xa0, 0xf2, 0x0f, 0x20, 0xc0, 0xf2,
    0x0f, 0x00, 0xe0, 0xf2, 0xa9, 0x15, 0x40, 0x38, 0xce, 0x01, 0x09, 0xca,
    0xce, 0x7d, 0x0f, 0x9b, 0xbf, 0x01, 0x0c, 0xeb, 0x83, 0xff, 0xff, 0x54,
    0x15, 0x00, 0x80, 0xd2, 0xa9, 0x06, 0x00, 0x91, 0xaa, 0x82, 0x8f, 0xd2,
    0x4a, 0xe9, 0xaf, 0xf2, 0x2a, 0x37, 0xcf, 0xf2, 0xea, 0xc6, 0xf3, 0xf2,
    0x29, 0x7d, 0x0a, 0x9b, 0x29, 0x01, 0x0e, 0x8b, 0x2a, 0xfd, 0x5e, 0xd3,
    0x29, 0x01, 0x0a, 0xca, 0x2a, 0xb7, 0x9c, 0xd2, 0x8a, 0x9c, 0xa3, 0xf2,
    0xaa, 0xed, 0xc8, 0xf2, 0x0a, 0xeb, 0xf7, 0xf2, 0x29, 0x7d, 0x0a, 0x9b,
    0x2a, 0xfd, 0x5b, 0xd3, 0x29, 0x01, 0x0a, 0xca, 0x6a, 0x3d, 0x82, 0xd2,
    0x2a, 0x66, 0xa2, 0xf2, 0x6a, 0x37, 0xc9, 0xf2, 0x0a, 0x9a, 0xf2, 0xf2,
    0x29, 0x7d, 0x0a, 0x9b, 0x2a, 0xfd, 0x5f, 0xd3, 0x29, 0x01, 0x0a, 0xca,
    0xaa, 0xf2, 0x7d, 0xd3, 0x8b, 0x02, 0x0a, 0x8b, 0x6b, 0x11, 0x40, 0xf9,
    0x6b, 0x01, 0x09, 0xca, 0xec, 0x83, 0x00, 0x91, 0x8c, 0x01, 0x0a, 0x8b,
    0x8b, 0x01, 0x00, 0xf9, 0xb5, 0x06, 0x00, 0x91, 0xbf, 0x12, 0x00, 0xf1,
    0x03, 0xfc, 0xff, 0x54, 0xe9, 0x13, 0x40, 0xf9, 0xe9, 0x4b, 0x00, 0xf9,
    0xe9, 0x17, 0x40, 0xf9, 0xe9, 0x4f, 0x00, 0xf9, 0xe9, 0x1b, 0x40, 0xf9,
    0xe9, 0x53, 0x00, 0xf9, 0xe9, 0x1f, 0x40, 0xf9, 0xe9, 0x57, 0x00, 0xf9,
    0xa9, 0x0c, 0x8f, 0x52, 0x09, 0x2e, 0xac, 0x72, 0xe9, 0x83, 0x00, 0xb9,
    0xc9, 0x8d, 0x8c, 0x52, 0x09, 0x64, 0xa6, 0x72, 0xe9, 0x87, 0x00, 0xb9,
    0x49, 0xa6, 0x85, 0x52, 0x49, 0x2c, 0xaf, 0x72, 0xe9, 0x8b, 0x00, 0xb9,
    0x89, 0xae, 0x8c, 0x52, 0x09, 0x64, 0xad, 0x72, 0xe9, 0x8f, 0x00, 0xb9,
    0xe9, 0x0b, 0x40, 0xf9, 0xe9, 0x23, 0x00, 0xb4, 0x29, 0x05, 0x00, 0xd1,
    0xe9, 0x0b, 0x00, 0xf9, 0xe9, 0x0f, 0x40, 0xf9, 0x2a, 0x01, 0x40, 0xf9,
    0x2b, 0x05, 0x40, 0xf9, 0x2c, 0x09, 0x40, 0xf9, 0x29, 0x61, 0x00, 0x91,
    0xe9, 0x0f, 0x00, 0xf9, 0xea, 0x63, 0x00, 0xf9, 0xec, 0x6f, 0x00, 0xf9,
    0x6d, 0xfd, 0x78, 0xd3, 0xed, 0x6b, 0x00, 0xf9, 0x6b, 0xdd, 0x78, 0xd3,
    0x6b, 0xfd, 0x48, 0xd3, 0xeb, 0x67, 0x00, 0xf9, 0xeb, 0xfd, 0xff, 0xb4,
    0xed, 0x03, 0x40, 0xf9, 0x4e, 0x01, 0x0d, 0x8b, 0xee, 0x73, 0x00, 0xf9,
    0xcf, 0xfd, 0x4c, 0xd3, 0xef, 0xcd, 0x74, 0xd3, 0xd0, 0x01, 0x0b, 0x8b,
    0x10, 0xfe, 0x3f, 0x91, 0x10, 0xfe, 0x4c, 0xd3, 0x10, 0xce, 0x74, 0xd3,
    0x01, 0x02, 0x0f, 0xcb, 0xe0, 0x03, 0x0f, 0xaa, 0x62, 0x00, 0x80, 0xd2,
    0x48, 0x1c, 0x80, 0xd2, 0x01, 0x00, 0x00, 0xd4, 0x80, 0x22, 0x00, 0xb5,
    0xe9, 0x63, 0x40, 0xf9, 0xe9, 0xb7, 0x00, 0xb9, 0x2a, 0xfd, 0x60, 0xd3,
    0xea, 0xbb, 0x00, 0xb9, 0xeb, 0x67, 0x40, 0xf9, 0xeb, 0xbf, 0x00, 0xb9,
    0xff, 0xb3, 0x00, 0xb9, 0xe9, 0x67, 0x40, 0xf9, 0xe9, 0x77, 0x00, 0xf9,
    0xe9, 0x73, 0x40, 0xf9, 0xe9, 0x7b, 0x00, 0xf9, 0xe0, 0x83, 0x40, 0xb9,
    0xe1, 0x87, 0x40, 0xb9, 0xe2, 0x8b, 0x40, 0xb9, 0xe3, 0x8f, 0x40, 0xb9,
    0xe4, 0x93, 0x40, 0xb9, 0xe5, 0x97, 0x40, 0xb9, 0xe6, 0x9b, 0x40, 0xb9,
    0xe7, 0x9f, 0x40, 0xb9, 0xe8, 0xa3, 0x40, 0xb9, 0xe9, 0xa7, 0x40, 0xb9,
    0xea, 0xab, 0x40, 0xb9, 0xeb, 0xaf, 0x40, 0xb9, 0xec, 0xb3, 0x40, 0xb9,
    0xed, 0xb7, 0x40, 0xb9, 0xee, 0xbb, 0x40, 0xb9, 0xef, 0xbf, 0x40, 0xb9,
    0x51, 0x01, 0x80, 0x52, 0x00, 0x00, 0x04, 0x0b, 0x8c, 0x01, 0x00, 0x4a,
    0x8c, 0x41, 0x8c, 0x13, 0x08, 0x01, 0x0c, 0x0b, 0x84, 0x00, 0x08, 0x4a,
    0x84, 0x50, 0x84, 0x13, 0x00, 0x00, 0x04, 0x0b, 0x8c, 0x01, 0x00, 0x4a,
    0x8c, 0x61, 0x8c, 0x13, 0x08, 0x01, 0x0c, 0x0b, 0x84, 0x00, 0x08, 0x4a,
    0x84, 0x64, 0x84, 0x13, 0x21, 0x00, 0x05, 0x0b, 0xad, 0x01, 0x01, 0x4a,
    0xad, 0x41, 0x8d, 0x13, 0x29, 0x01, 0x0d, 0x0b, 0xa5, 0x00, 0x09, 0x4a,
    0xa5, 0x50, 0x85, 0x13, 0x21, 0x00, 0x05, 0x0b, 0xad, 0x01, 0x01, 0x4a,
    0xad, 0x61, 0x8d, 0x13, 0x29, 0x01, 0x0d, 0x0b, 0xa5, 0x00, 0x09, 0x4a,
    0xa5, 0x64, 0x85, 0x13, 0x42, 0x00, 0x06, 0x0b, 0xce, 0x01, 0x02, 0x4a,
    0xce, 0x41, 0x8e, 0x13, 0x4a, 0x01, 0x0e, 0x0b, 0xc6, 0x00, 0x0a, 0x4a,
    0xc6, 0x50, 0x86, 0x13, 0x42, 0x00, 0x06, 0x0b, 0xce, 0x01, 0x02, 0x4a,
    0xce, 0x61, 0x8e, 0x13, 0x4a, 0x01, 0x0e, 0x0b, 0xc6, 0x00, 0x0a, 0x4a,
    0xc6, 0x64, 0x86, 0x13, 0x63, 0x00, 0x07, 0x0b, 0xef, 0x01, 0x03, 0x4a,
    0xef, 0x41, 0x8f, 0x13, 0x6b, 0x01, 0x0f, 0x0b, 0xe7, 0x00, 0x0b, 0x4a,
    0xe7, 0x50, 0x87, 0x13, 0x63, 0x00, 0x07, 0x0b, 0xef, 0x01, 0x03, 0x4a,
    0xef, 0x61, 0x8f, 0x13, 0x6b, 0x01, 0x0f, 0x0b, 0xe7, 0x00, 0x0b, 0x4a,
    0xe7, 0x64, 0x87, 0x13, 0x00, 0x00, 0x05, 0x0b, 0xef, 0x01, 0x00, 0x4a,
    0xef, 0x41, 0x8f, 0x13, 0x4a, 0x01, 0x0f, 0x0b, 0xa5, 0x00, 0x0a, 0x4a,
    0xa5, 0x50, 0x85, 0x13, 0x00, 0x00, 0x05, 0x0b, 0xef, 0x01, 0x00, 0x4a,
    0xef, 0x61, 0x8f, 0x13, 0x4a, 0x01, 0x0f, 0x0b, 0xa5, 0x00, 0x0a, 0x4a,
    0xa5, 0x64, 0x85, 0x13, 0x21, 0x00, 0x06, 0x0b, 0x8c, 0x01, 0x01, 0x4a,
    0x8c, 0x41, 0x8c, 0x13, 0x6b, 0x01, 0x0c, 0x0b, 0xc6, 0x00, 0x0b, 0x4a,
    0xc6, 0x50, 0x86, 0x13, 0x21, 0x00, 0x06, 0x0b, 0x8c, 0x01, 0x01, 0x4a,
    0x8c, 0x61, 0x8c, 0x13, 0x6b, 0x01, 0x0c, 0x0b, 0xc6, 0x00, 0x0b, 0x4a,
    0xc6, 0x64, 0x86, 0x13, 0x42, 0x00, 0x07, 0x0b, 0xad, 0x01, 0x02, 0x4a,
    0xad, 0x41, 0x8d, 0x13, 0x08, 0x01, 0x0d, 0x0b, 0xe7, 0x00, 0x08, 0x4a,
    0xe7, 0x50, 0x87, 0x13, 0x42, 0x00, 0x07, 0x0b, 0xad, 0x01, 0x02, 0x4a,
    0xad, 0x61, 0x8d, 0x13, 0x08, 0x01, 0x0d, 0x0b, 0xe7, 0x00, 0x08, 0x4a,
    0xe7, 0x64, 0x87, 0x13, 0x63, 0x00, 0x04, 0x0b, 0xce, 0x01, 0x03, 0x4a,
    0xce, 0x41, 0x8e, 0x13, 0x29, 0x01, 0x0e, 0x0b, 0x84, 0x00, 0x09, 0x4a,
    0x84, 0x50, 0x84, 0x13, 0x63, 0x00, 0x04, 0x0b, 0xce, 0x01, 0x03, 0x4a,
    0xce, 0x61, 0x8e, 0x13, 0x29, 0x01, 0x0e, 0x0b, 0x84, 0x00, 0x09, 0x4a,
    0x84, 0x64, 0x84, 0x13, 0x31, 0x06, 0x00, 0x71, 0xe1, 0xf3, 0xff, 0x54,
    0xf0, 0x83, 0x40, 0xb9, 0x00, 0x00, 0x10, 0x0b, 0xf0, 0x87, 0x40, 0xb9,
    0x21, 0x00, 0x10, 0x0b, 0xf0, 0x8b, 0x40, 0xb9, 0x42, 0x00, 0x10, 0x0b,
    0xf0, 0x8f, 0x40, 0xb9, 0x63, 0x00, 0x10, 0x0b, 0xf0, 0x93, 0x40, 0xb9,
    0x84, 0x00, 0x10, 0x0b, 0xf0, 0x97, 0x40, 0xb9, 0xa5, 0x00, 0x10, 0x0b,
    0xf0, 0x9b, 0x40, 0xb9, 0xc6, 0x00, 0x10, 0x0b, 0xf0, 0x9f, 0x40, 0xb9,
    0xe7, 0x00, 0x10, 0x0b, 0xf0, 0xa3, 0x40, 0xb9, 0x08, 0x01, 0x10, 0x0b,
    0xf0, 0xa7, 0x40, 0xb9, 0x29, 0x01, 0x10, 0x0b, 0xf0, 0xab, 0x40, 0xb9,
    0x4a, 0x01, 0x10, 0x0b, 0xf0, 0xaf, 0x40, 0xb9, 0x6b, 0x01, 0x10, 0x0b,
    0xf0, 0xb3, 0x40, 0xb9, 0x8c, 0x01, 0x10, 0x0b, 0xf0, 0xb7, 0x40, 0xb9,
    0xad, 0x01, 0x10, 0x0b, 0xf0, 0xbb, 0x40, 0xb9, 0xce, 0x01, 0x10, 0x0b,
    0xf0, 0xbf, 0x40, 0xb9, 0xef, 0x01, 0x10, 0x0b, 0xe0, 0x43, 0x00, 0xb9,
    0xe1, 0x47, 0x00, 0xb9, 0xe2, 0x4b, 0x00, 0xb9, 0xe3, 0x4f, 0x00, 0xb9,
    0xe4, 0x53, 0x00, 0xb9, 0xe5, 0x57, 0x00, 0xb9, 0xe6, 0x5b, 0x00, 0xb9,
    0xe7, 0x5f, 0x00, 0xb9, 0xe8, 0x63, 0x00, 0xb9, 0xe9, 0x67, 0x00, 0xb9,
    0xea, 0x6b, 0x00, 0xb9, 0xeb, 0x6f, 0x00, 0xb9, 0xec, 0x73, 0x00, 0xb9,
    0xed, 0x77, 0x00, 0xb9, 0xee, 0x7b, 0x00, 0xb9, 0xef, 0x7f, 0x00, 0xb9,
    0xe9, 0x77, 0x40, 0xf9, 0x0a, 0x08, 0x80, 0xd2, 0x3f, 0x01, 0x0a, 0xeb,
    0x2a, 0x31, 0x8a, 0x9a, 0xeb, 0x7b, 0x40, 0xf9, 0xee, 0x03, 0x01, 0x91,
    0xec, 0x03, 0x0a, 0xaa, 0xcd, 0x15, 0x40, 0x38, 0x6f, 0x01, 0x40, 0x39,
    0xef, 0x01, 0x0d, 0x4a, 0x6f, 0x15, 0x00, 0x38, 0x8c, 0x05, 0x00, 0xf1,
    0x61, 0xff, 0xff, 0x54, 0xeb, 0x7b, 0x00, 0xf9, 0xe9, 0x77, 0x40, 0xf9,
    0x29, 0x01, 0x0a, 0xcb, 0xe9, 0x77, 0x00, 0xf9, 0xec, 0xb3, 0x40, 0xb9,
    0x8c, 0x05, 0x00, 0x11, 0xec, 0xb3, 0x00, 0xb9, 0x29, 0xe9, 0xff, 0xb5,
    0xe9, 0x73, 0x40, 0xf9, 0xea, 0x67, 0x40, 0xf9, 0x2a, 0x01, 0x0a, 0x8b,
    0xab, 0x64, 0x84, 0xd2, 0x4b, 0x84, 0xb0, 0xf2, 0x8b, 0x9c, 0xd3, 0xf2,
    0x4b, 0x7e, 0xf9, 0xf2, 0x6c, 0x36, 0x80, 0xd2, 0x0c, 0x00, 0xa0, 0xf2,
    0x0c, 0x20, 0xc0, 0xf2, 0x0c, 0x00, 0xe0, 0xf2, 0x3f, 0x01, 0x0a, 0xeb,
    0xa2, 0x00, 0x00, 0x54, 0x2d, 0x15, 0x40, 0x38, 0x6b, 0x01, 0x0d, 0xca,
    0x6b, 0x7d, 0x0c, 0x9b, 0xfb, 0xff, 0xff, 0x17, 0xed, 0x6f, 0x40, 0xf9,
    0x7f, 0x01, 0x0d, 0xeb, 0x01, 0x08, 0x00, 0x54, 0xe9, 0x73, 0x40, 0xf9,
    0xeb, 0x67, 0x40, 0xf9, 0x2a, 0xfd, 0x4c, 0xd3, 0x4a, 0xcd, 0x74, 0xd3,
    0x2c, 0x01, 0x0b, 0x8b, 0x8c, 0xfd, 0x3f, 0x91, 0x8c, 0xfd, 0x4c, 0xd3,
    0x8c, 0xcd, 0x74, 0xd3, 0x81, 0x01, 0x0a, 0xcb, 0xe0, 0x03, 0x0a, 0xaa,
    0xe2, 0x6b, 0x40, 0xf9, 0x48, 0x1c, 0x80, 0xd2, 0x01, 0x00, 0x00, 0xd4,
    0xe0, 0x05, 0x00, 0xb5, 0xe9, 0x73, 0x40, 0xf9, 0xeb, 0x67, 0x40, 0xf9,
    0x2c, 0x01, 0x0b, 0x8b, 0x2e, 0x00, 0x3b, 0xd5, 0xcf, 0x4d, 0x50, 0xd3,
    0x90, 0x00, 0x80, 0xd2, 0x0f, 0x22, 0xcf, 0x9a, 0xf1, 0x05, 0x00, 0xd1,
    0x2d, 0x01, 0x31, 0x8a, 0x2d, 0x7b, 0x0b, 0xd5, 0xad, 0x01, 0x0f, 0x8b,
    0xbf, 0x01, 0x0c, 0xeb, 0xa3, 0xff, 0xff, 0x54, 0x9f, 0x3b, 0x03, 0xd5,
    0xcf, 0x0d, 0x40, 0x92, 0x90, 0x00, 0x80, 0xd2, 0x0f, 0x22, 0xcf, 0x9a,
    0xf1, 0x05, 0x00, 0xd1, 0x2d, 0x01, 0x31, 0x8a, 0x2d, 0x75, 0x0b, 0xd5,
    0xad, 0x01, 0x0f, 0x8b, 0xbf, 0x01, 0x0c, 0xeb, 0xa3, 0xff, 0xff, 0x54,
    0x9f, 0x3b, 0x03, 0xd5, 0xdf, 0x3f, 0x03, 0xd5, 0xe1, 0xfe, 0xff, 0x17,
    0xe9, 0x93, 0x40, 0xf9, 0x29, 0x61, 0x00, 0x91, 0x2a, 0x00, 0x80, 0xd2,
    0x2a, 0xfd, 0x9f, 0xc8, 0xe9, 0x07, 0x40, 0xf9, 0xe9, 0x00, 0x00, 0xb4,
    0xea, 0x03, 0x40, 0xf9, 0x29, 0x01, 0x0a, 0x8b, 0xe0, 0x83, 0x40, 0xf9,
    0xe1, 0x87, 0x40, 0xf9, 0xe2, 0x8b, 0x40, 0xf9, 0x20, 0x01, 0x3f, 0xd6,
    0xff, 0x03, 0x11, 0x91, 0xfb, 0x73, 0x45, 0xa9, 0xf9, 0x6b, 0x44, 0xa9,
    0xf7, 0x63, 0x43, 0xa9, 0xf5, 0x5b, 0x42, 0xa9, 0xf3, 0x53, 0x41, 0xa9,
    0xfd, 0x7b, 0xc7, 0xa8, 0xc0, 0x03, 0x5f, 0xd6, 0xc1, 0x16, 0x00, 0x50,
    0x42, 0x06, 0x80, 0xd2, 0x04, 0x00, 0x00, 0x14, 0x01, 0x18, 0x00, 0x10,
    0xa2, 0x04, 0x80, 0xd2, 0x01, 0x00, 0x00, 0x14, 0x40, 0x00, 0x80, 0xd2,
    0x08, 0x08, 0x80, 0xd2, 0x01, 0x00, 0x00, 0xd4, 0x20, 0x00, 0x80, 0xd2,
    0xc8, 0x0b, 0x80, 0xd2, 0x01, 0x00, 0x00, 0xd4, 0xff, 0x03, 0x21, 0xd1,
    0xfe, 0x1b, 0x04, 0xf9, 0x60, 0x0c, 0x80, 0x92, 0xa1, 0x19, 0x00, 0x30,
    0x02, 0x00, 0x88, 0xd2, 0x08, 0x07, 0x80, 0xd2, 0x01, 0x00, 0x00, 0xd4,
    0xa0, 0x0b, 0xf8, 0xb7, 0xe0, 0x03, 0x04, 0xf9, 0xe0, 0x03, 0x44, 0xf9,
    0xe1, 0x03, 0x00, 0x91, 0x02, 0x80, 0x80, 0xd2, 0xa8, 0x07, 0x80, 0xd2,
    0x01, 0x00, 0x00, 0xd4, 0xc0, 0x0a, 0xf8, 0xb7, 0x60, 0x06, 0x00, 0xb4,
    0xe0, 0x13, 0x04, 0xf9, 0xff, 0x0f, 0x04, 0xf9, 0xe9, 0x0f, 0x44, 0xf9,
    0xea, 0x13, 0x44, 0xf9, 0x3f, 0x01, 0x0a, 0xeb, 0x82, 0xfe, 0xff, 0x54,
    0xeb, 0x03, 0x00, 0x91, 0x6b, 0x01, 0x09, 0x8b, 0x6c, 0x21, 0x40, 0x79,
    0x6c, 0x09, 0x00, 0x34, 0x29, 0x01, 0x0c, 0x8b, 0xe9, 0x0f, 0x04, 0xf9,
    0x6d, 0x4d, 0x40, 0x39, 0xbf, 0xc1, 0x00, 0x71, 0x83, 0xfe, 0xff, 0x54,
    0xbf, 0xe5, 0x00, 0x71, 0x48, 0xfe, 0xff, 0x54, 0xe0, 0x03, 0x44, 0xf9,
    0x61, 0x4d, 0x00, 0x91, 0x02, 0x00, 0x88, 0xd2, 0x08, 0x07, 0x80, 0xd2,
    0x01, 0x00, 0x00, 0xd4, 0x80, 0xfd, 0xff, 0xb7, 0xe0, 0x07, 0x04, 0xf9,
    0xe0, 0x07, 0x44, 0xf9, 0x61, 0x15, 0x00, 0x30, 0x02, 0x00, 0x80, 0xd2,
    0x03, 0x00, 0x80, 0xd2, 0x08, 0x07, 0x80, 0xd2, 0x01, 0x00, 0x00, 0xd4,
    0x00, 0x02, 0xf8, 0xb7, 0xe0, 0x0b, 0x04, 0xf9, 0xe1, 0x03, 0x10, 0x91,
    0xe2, 0x7f, 0x80, 0xd2, 0xe8, 0x07, 0x80, 0xd2, 0x01, 0x00, 0x00, 0xd4,
    0xe0, 0x17, 0x04, 0xf9, 0xe0, 0x0b, 0x44, 0xf9, 0x28, 0x07, 0x80, 0xd2,
    0x01, 0x00, 0x00, 0xd4, 0xe2, 0x17, 0x44, 0xf9, 0x5f, 0x00, 0x00, 0xf1,
    0x8d, 0x00, 0x00, 0x54, 0xe1, 0x03, 0x10, 0x91, 0x22, 0x00, 0x02, 0x8b,
    0x0b, 0x00, 0x00, 0x94, 0xe0, 0x07, 0x44, 0xf9, 0x28, 0x07, 0x80, 0xd2,
    0x01, 0x00, 0x00, 0xd4, 0xd1, 0xff, 0xff, 0x17, 0xe0, 0x03, 0x44, 0xf9,
    0x28, 0x07, 0x80, 0xd2, 0x01, 0x00, 0x00, 0xd4, 0xfe, 0x1b, 0x44, 0xf9,
    0xff, 0x03, 0x21, 0x91, 0xc0, 0x03, 0x5f, 0xd6, 0xc5, 0x11, 0x00, 0x10,
    0x29, 0x28, 0x00, 0x91, 0x3f, 0x01, 0x02, 0xeb, 0x28, 0x03, 0x00, 0x54,
    0x0a, 0x00, 0x80, 0xd2, 0x5f, 0x29, 0x00, 0xf1, 0x22, 0x01, 0x00, 0x54,
    0x2b, 0x68, 0x6a, 0x38, 0xac, 0x68, 0x6a, 0x38, 0x7f, 0x01, 0x0c, 0x6b,
    0x61, 0x00, 0x00, 0x54, 0x4a, 0x05, 0x00, 0x91, 0xf9, 0xff, 0xff, 0x17,
    0x21, 0x04, 0x00, 0x91, 0xf3, 0xff, 0xff, 0x17, 0x21, 0x28, 0x00, 0x91,
    0x3f, 0x00, 0x02, 0xeb, 0x62, 0x01, 0x00, 0x54, 0x2b, 0x00, 0x40, 0x39,
    0x7f, 0x81, 0x00, 0x71, 0xc0, 0x00, 0x00, 0x54, 0x7f, 0x25, 0x00, 0x71,
    0x80, 0x00, 0x00, 0x54, 0x7f, 0xc1, 0x00, 0x71, 0xe1, 0x00, 0x00, 0x54,
    0xc0, 0x03, 0x5f, 0xd6, 0x21, 0x04, 0x00, 0x91, 0xf5, 0xff, 0xff, 0x17,
    0x81, 0x0b, 0x00, 0x30, 0x62, 0x04, 0x80, 0xd2, 0x94, 0xff, 0xff, 0x17,
    0x81, 0x07, 0x00, 0x70, 0x62, 0x03, 0x80, 0xd2, 0x91, 0xff, 0xff, 0x17,
    0xe1, 0x0b, 0x00, 0x10, 0xa2, 0x03, 0x80, 0xd2, 0x8e, 0xff, 0xff, 0x17,
    0xff, 0x43, 0x00, 0xd1, 0xff, 0x03, 0x00, 0xf9, 0x09, 0xa0, 0x8c, 0xd2,
    0xa9, 0xb9, 0xa3, 0xf2, 0xe9, 0x07, 0x00, 0xf9, 0x8e, 0xff, 0xff, 0x97,
    0xc9, 0x62, 0x00, 0x91, 0x29, 0xfd, 0xdf, 0xc8, 0x49, 0x00, 0x00, 0xb4,
    0x06, 0x00, 0x00, 0x94, 0xe0, 0x03, 0x00, 0x91, 0x01, 0x00, 0x80, 0xd2,
    0xa8, 0x0c, 0x80, 0xd2, 0x01, 0x00, 0x00, 0xd4, 0xf7, 0xff, 0xff, 0x17,
    0xcd, 0x06, 0x40, 0xf9, 0xce, 0x0a, 0x40, 0xf9, 0x0f, 0x00, 0x80, 0xd2,
    0xff, 0x01, 0x0e, 0xeb, 0x82, 0x03, 0x00, 0x54, 0xe9, 0x05, 0x0f, 0x8b,
    0xaa, 0x0d, 0x09, 0x8b, 0x4b, 0x01, 0x40, 0xf9, 0xcc, 0x02, 0x40, 0xf9,
    0x6b, 0x01, 0x0c, 0x8b, 0x4c, 0x05, 0x40, 0xf9, 0x8c, 0xdd, 0x78, 0xd3,
    0x8c, 0xfd, 0x48, 0xd3, 0xb0, 0x64, 0x84, 0xd2, 0x50, 0x84, 0xb0, 0xf2,
    0x90, 0x9c, 0xd3, 0xf2, 0x50, 0x7e, 0xf9, 0xf2, 0x71, 0x36, 0x80, 0xd2,
    0x11, 0x00, 0xa0, 0xf2, 0x11, 0x20, 0xc0, 0xf2, 0x11, 0x00, 0xe0, 0xf2,
    0xcc, 0x00, 0x00, 0xb4, 0x69, 0x15, 0x40, 0x38, 0x10, 0x02, 0x09, 0xca,
    0x10, 0x7e, 0x11, 0x9b, 0x8c, 0x05, 0x00, 0xd1, 0xfb, 0xff, 0xff, 0x17,
    0x49, 0x09, 0x40, 0xf9, 0x1f, 0x02, 0x09, 0xeb, 0xc1, 0xeb, 0xff, 0x54,
    0xef, 0x05, 0x00, 0x91, 0xe4, 0xff, 0xff, 0x17, 0xc0, 0x03, 0x5f, 0xd6,
    0x58, 0x4e, 0x65, 0x62, 0x75, 0x6c, 0x61, 0x3a, 0x20, 0x62, 0x61, 0x64,
    0x20, 0x63, 0x6f, 0x6e, 0x66, 0x69, 0x67, 0x75, 0x72, 0x61, 0x74, 0x69,
    0x6f, 0x6e, 0x0a, 0x58, 0x4e, 0x65, 0x62, 0x75, 0x6c, 0x61, 0x3a, 0x20,
    0x64, 0x65, 0x62, 0x75, 0x67, 0x67, 0x65, 0x72, 0x20, 0x64, 0x65, 0x74,
    0x65, 0x63, 0x74, 0x65, 0x64, 0x0a, 0x58, 0x4e, 0x65, 0x62, 0x75, 0x6c,
    0x61, 0x3a, 0x20, 0x6d, 0x70, 0x72, 0x6f, 0x74, 0x65, 0x63, 0x74, 0x20,
    0x72, 0x65, 0x66, 0x75, 0x73, 0x65, 0x64, 0x20, 0x28, 0x53, 0x45, 0x4c,
    0x69, 0x6e, 0x75, 0x78, 0x20, 0x6f, 0x72, 0x20, 0x57, 0x5e, 0x58, 0x20,
    0x70, 0x6f, 0x6c, 0x69, 0x63, 0x79, 0x29, 0x0a, 0x58, 0x4e, 0x65, 0x62,
    0x75, 0x6c, 0x61, 0x3a, 0x20, 0x63, 0x6f, 0x64, 0x65, 0x20, 0x69, 0x6e,
    0x74, 0x65, 0x67, 0x72, 0x69, 0x74, 0x79, 0x20, 0x63, 0x68, 0x65, 0x63,
    0x6b, 0x20, 0x66, 0x61, 0x69, 0x6c, 0x65, 0x64, 0x0a, 0x58, 0x4e, 0x65,
    0x62, 0x75, 0x6c, 0x61, 0x3a, 0x20, 0x70, 0x72, 0x6f, 0x63, 0x65, 0x73,
    0x73, 0x20, 0x73, 0x74, 0x61, 0x74, 0x65, 0x20, 0x75, 0x6e, 0x61, 0x76,
    0x61, 0x69, 0x6c, 0x61, 0x62, 0x6c, 0x65, 0x0a, 0x58, 0x4e, 0x65, 0x62,
    0x75, 0x6c, 0x61, 0x3a, 0x20, 0x6d, 0x6f, 0x6e, 0x69, 0x74, 0x6f, 0x72,
    0x20, 0x75, 0x6e, 0x61, 0x76, 0x61, 0x69, 0x6c, 0x61, 0x62, 0x6c, 0x65,
    0x0a, 0x2f, 0x70, 0x72, 0x6f, 0x63, 0x2f, 0x73, 0x65, 0x6c, 0x66, 0x2f,
    0x74, 0x61, 0x73, 0x6b, 0x00, 0x73, 0x74, 0x61, 0x74, 0x75, 0x73, 0x00,
    0x54, 0x72, 0x61, 0x63, 0x65, 0x72, 0x50, 0x69, 0x64, 0x3a, 0x00, 0x00,
    0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00,
    0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00,
    0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00,
    0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00,
    0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00,
    0x00, 0x00, 0x00, 0x00,
};
constexpr uint64_t XN_A64_CODE_LEN = 0xa80;

extern "C" {
extern uint8_t xn_stub_start[];
extern uint8_t xn_code_end[];
extern uint8_t xn_cfg_start[];
extern uint8_t xn_stub_end[];
}

constexpr uint64_t XN_MAGIC = 0x584e4542554c4158ULL;
constexpr uint64_t XN_CFG_HDR = 64;
constexpr uint64_t XN_ENTRY = 24;
constexpr uint64_t XN_FNV_OFFSET = 0xcbf29ce484222325ULL;
constexpr uint64_t XN_FNV_PRIME = 0x100000001b3ULL;
constexpr uint64_t XN_GOLDEN = 0x9e3779b97f4a7c15ULL;
constexpr uint64_t XN_MIX1 = 0xbf58476d1ce4e5b9ULL;
constexpr uint64_t XN_MIX2 = 0x94d049bb133111ebULL;
constexpr uint64_t XN_DT_SYMTAB_SHNDX = 0x6ffffef6ULL;
constexpr uint64_t XN_DT_RELR = 36;
constexpr uint64_t XN_DT_RELRSZ = 35;

struct Interval {
    uint64_t addr;
    uint64_t size;
};

struct ProtRange {
    uint64_t addr;
    uint64_t size;
    uint64_t prot;
    uint64_t foff;
    uint64_t hash;
};

static void xn_fail(const char *msg) {
    std::fprintf(stderr, "XNebula Protector: %s\n", msg);
    std::exit(1);
}

static uint64_t xn_end(uint64_t a, uint64_t s) {
    if (a >= ~0ULL) return ~0ULL;
    if (s > ~0ULL - a) return ~0ULL;
    return a + s;
}

static uint64_t align_up(uint64_t v, uint64_t a) {
    uint64_t m = a - 1;
    if (v > ~0ULL - m) return ~0ULL;
    return (v + m) & ~m;
}

static uint64_t align_down(uint64_t v, uint64_t a) {
    return v & ~(a - 1);
}

static std::vector<Interval> xn_normalize(std::vector<Interval> v) {
    std::sort(v.begin(), v.end(), [](const Interval &a, const Interval &b) {
        if (a.addr != b.addr) return a.addr < b.addr;
        return a.size < b.size;
    });

    std::vector<Interval> out;
    for (auto x : v) {
        if (x.size == 0) continue;
        uint64_t e = xn_end(x.addr, x.size);

        if (out.empty()) {
            out.push_back(x);
            continue;
        }

        uint64_t ce = xn_end(out.back().addr, out.back().size);
        if (x.addr > ce) {
            out.push_back(x);
        } else if (e > ce) {
            out.back().size = e - out.back().addr;
        }
    }

    return out;
}

static std::vector<Interval> xn_subtract(std::vector<Interval> bases, std::vector<Interval> subs) {
    bases = xn_normalize(bases);
    subs = xn_normalize(subs);

    std::vector<Interval> out;
    for (auto b : bases) {
        uint64_t cur = b.addr;
        uint64_t end = xn_end(cur, b.size);
        if (cur >= end) continue;

        for (const auto &s : subs) {
            uint64_t se = xn_end(s.addr, s.size);
            if (s.addr >= end) break;
            if (se <= cur) continue;

            if (s.addr > cur) {
                uint64_t nend = std::min(s.addr, end);
                if (nend > cur) out.push_back({cur, nend - cur});
            }

            if (se > cur) cur = se;
            if (cur >= end) break;
        }

        if (cur < end) out.push_back({cur, end - cur});
    }

    return xn_normalize(out);
}

static bool xn_merge_prot(std::vector<ProtRange> &v) {
    std::sort(v.begin(), v.end(), [](const ProtRange &a, const ProtRange &b) {
        if (a.addr != b.addr) return a.addr < b.addr;
        if (a.size != b.size) return a.size < b.size;
        return a.foff < b.foff;
    });

    std::vector<ProtRange> out;
    for (auto x : v) {
        if (x.size == 0) continue;

        if (out.empty()) {
            out.push_back(x);
            continue;
        }

        uint64_t end = xn_end(out.back().addr, out.back().size);
        uint64_t fend = xn_end(out.back().foff, out.back().size);
        if (end == ~0ULL || fend == ~0ULL) return false;
        if (x.addr < end) return false;

        if (x.prot == out.back().prot && x.addr == end && x.foff == fend) {
            if (out.back().size > ~0ULL - x.size) return false;
            uint64_t ns = out.back().size + x.size;
            if (xn_end(out.back().addr, ns) == ~0ULL) return false;
            if (xn_end(out.back().foff, ns) == ~0ULL) return false;
            out.back().size = ns;
        } else {
            out.push_back(x);
        }
    }

    v.swap(out);
    return true;
}

static uint64_t xn_fnv1a(const uint8_t *p, uint64_t n) {
    uint64_t h = XN_FNV_OFFSET;
    for (uint64_t i = 0; i < n; ++i) {
        h ^= p[i];
        h *= XN_FNV_PRIME;
    }
    return h;
}

static uint64_t xn_pad_word(uint64_t h, uint64_t i) {
    uint64_t z = h + (i + 1) * XN_GOLDEN;
    z = (z ^ (z >> 30)) * XN_MIX1;
    z = (z ^ (z >> 27)) * XN_MIX2;
    return z ^ (z >> 31);
}

static uint32_t xn_rotl(uint32_t v, int n) {
    return (v << n) | (v >> (32 - n));
}

static void xn_qr(uint32_t *x, int a, int b, int c, int d) {
    x[a] += x[b]; x[d] = xn_rotl(x[d] ^ x[a], 16);
    x[c] += x[d]; x[b] = xn_rotl(x[b] ^ x[c], 12);
    x[a] += x[b]; x[d] = xn_rotl(x[d] ^ x[a], 8);
    x[c] += x[d]; x[b] = xn_rotl(x[b] ^ x[c], 7);
}

static void xn_chacha_block(const uint32_t key[8], uint32_t counter, uint32_t n0, uint32_t n1, uint32_t n2, uint8_t out[64]) {
    const uint32_t init[16] = {
        0x61707865u, 0x3320646eu, 0x79622d32u, 0x6b206574u,
        key[0], key[1], key[2], key[3], key[4], key[5], key[6], key[7],
        counter, n0, n1, n2
    };
    uint32_t x[16];
    std::memcpy(x, init, sizeof(x));
    for (int r = 0; r < 10; ++r) {
        xn_qr(x, 0, 4, 8, 12);
        xn_qr(x, 1, 5, 9, 13);
        xn_qr(x, 2, 6, 10, 14);
        xn_qr(x, 3, 7, 11, 15);
        xn_qr(x, 0, 5, 10, 15);
        xn_qr(x, 1, 6, 11, 12);
        xn_qr(x, 2, 7, 8, 13);
        xn_qr(x, 3, 4, 9, 14);
    }
    for (int i = 0; i < 16; ++i) {
        uint32_t v = x[i] + init[i];
        out[4 * i + 0] = (uint8_t)(v);
        out[4 * i + 1] = (uint8_t)(v >> 8);
        out[4 * i + 2] = (uint8_t)(v >> 16);
        out[4 * i + 3] = (uint8_t)(v >> 24);
    }
}

static void xn_crypt(uint8_t *p, uint64_t size, const uint32_t key[8], uint64_t vaddr, uint64_t rsize) {
    uint32_t n0 = (uint32_t)vaddr;
    uint32_t n1 = (uint32_t)(vaddr >> 32);
    uint32_t n2 = (uint32_t)rsize;
    uint8_t ks[64];

    for (uint64_t blk = 0; size > 0; ++blk) {
        xn_chacha_block(key, (uint32_t)blk, n0, n1, n2, ks);
        uint64_t n = size < 64 ? size : 64;
        for (uint64_t j = 0; j < n; ++j) p[j] ^= ks[j];
        p += n;
        size -= n;
    }
}

static std::string xn_base_name(const std::string &p) {
    size_t n = p.find_last_of('/');
    if (n == std::string::npos) return p;
    return p.substr(n + 1);
}

static uint64_t xn_v2o(const std::vector<Elf64_Phdr> &phdrs, uint64_t v, uint64_t need) {
    for (const auto &p : phdrs) {
        if (p.p_type != PT_LOAD || p.p_filesz == 0) continue;
        if (v < p.p_vaddr) continue;
        uint64_t delta = v - p.p_vaddr;
        if (delta >= p.p_filesz) continue;
        if (need > p.p_filesz - delta) return ~0ULL;
        return p.p_offset + delta;
    }
    return ~0ULL;
}

template <typename T>
static bool xn_read(const std::vector<uint8_t> &data, uint64_t off, T &out) {
    if (off > data.size() || sizeof(T) > data.size() - off) return false;
    std::memcpy(&out, data.data() + off, sizeof(T));
    return true;
}

int main(int argc, char **argv) {
    if (argc < 3 || std::strcmp(argv[1], "-pso") != 0) {
        std::printf("XNebula Protector\nusage: xnebula -pso <file.so>\n");
        return 1;
    }

    std::string in_path = argv[2];
    FILE *fp = std::fopen(in_path.c_str(), "rb");
    if (!fp) xn_fail("cannot open input file");

    std::fseek(fp, 0, SEEK_END);
    long fs = std::ftell(fp);
    std::fseek(fp, 0, SEEK_SET);
    if (fs <= 0) xn_fail("input file is empty");

    size_t fsize = (size_t)fs;
    std::vector<uint8_t> data(fsize);
    if (std::fread(data.data(), 1, fsize, fp) != fsize) xn_fail("cannot read input file");
    std::fclose(fp);

    if (fsize < sizeof(Elf64_Ehdr)) xn_fail("file too small");

    Elf64_Ehdr ehdr;
    std::memcpy(&ehdr, data.data(), sizeof(ehdr));

    if (ehdr.e_ident[EI_MAG0] != ELFMAG0 ||
        ehdr.e_ident[EI_MAG1] != ELFMAG1 ||
        ehdr.e_ident[EI_MAG2] != ELFMAG2 ||
        ehdr.e_ident[EI_MAG3] != ELFMAG3)
        xn_fail("not ELF");

    if (ehdr.e_ident[EI_CLASS] != ELFCLASS64 ||
        ehdr.e_ident[EI_DATA] != ELFDATA2LSB ||
        ehdr.e_type != ET_DYN ||
        (ehdr.e_machine != EM_X86_64 && ehdr.e_machine != EM_AARCH64))
        xn_fail("only ELF64 LSB x86_64 or AArch64 ET_DYN .so supported");

    if (ehdr.e_phoff == 0 || ehdr.e_phnum == 0 || ehdr.e_phentsize != sizeof(Elf64_Phdr))
        xn_fail("bad program header table");

    const bool is_a64 = ehdr.e_machine == EM_AARCH64;
    const uint8_t *stub_s = is_a64 ? xn_a64_stub : xn_stub_start;
    const uint8_t *code_e = is_a64 ? xn_a64_stub + XN_A64_CODE_LEN : xn_code_end;
    const uint8_t *cfg_s = is_a64 ? xn_a64_stub + XN_A64_CODE_LEN : xn_cfg_start;
    const uint8_t *stub_e = is_a64 ? xn_a64_stub + sizeof(xn_a64_stub) : xn_stub_end;
    const uint64_t irel_type = is_a64 ? (uint64_t)R_AARCH64_IRELATIVE : (uint64_t)R_X86_64_IRELATIVE;

    uint64_t file_size = (uint64_t)data.size();
    uint64_t ph_size = (uint64_t)ehdr.e_phnum * ehdr.e_phentsize;
    if (ehdr.e_phoff > file_size || ph_size > file_size - ehdr.e_phoff)
        xn_fail("program header table truncated");

    std::vector<Elf64_Phdr> phdrs(ehdr.e_phnum);
    std::memcpy(phdrs.data(), data.data() + ehdr.e_phoff, ph_size);

    int dyn_idx = -1;
    int phdr_idx = -1;
    uint64_t max_load = 0;
    uint64_t min_load = ~0ULL;

    for (uint16_t i = 0; i < ehdr.e_phnum; ++i) {
        const Elf64_Phdr &p = phdrs[i];

        if (p.p_type == PT_PHDR && phdr_idx < 0)
            phdr_idx = i;

        if (p.p_type == PT_LOAD) {
            if (p.p_memsz < p.p_filesz)
                xn_fail("bad PT_LOAD memsz");

            uint64_t mem_end = xn_end(p.p_vaddr, p.p_memsz);
            if (mem_end == ~0ULL)
                xn_fail("PT_LOAD virtual range overflow");

            if (p.p_filesz) {
                uint64_t file_end = xn_end(p.p_offset, p.p_filesz);
                if (file_end == ~0ULL || file_end > file_size)
                    xn_fail("PT_LOAD file range truncated");

                uint64_t vfile_end = xn_end(p.p_vaddr, p.p_filesz);
                if (vfile_end == ~0ULL)
                    xn_fail("PT_LOAD virtual file range overflow");
            }

            if (mem_end > max_load) max_load = mem_end;
            if (p.p_vaddr < min_load) min_load = p.p_vaddr;
        }

        if (p.p_type == PT_DYNAMIC && dyn_idx < 0)
            dyn_idx = i;
    }

    if (dyn_idx < 0) xn_fail("no PT_DYNAMIC");
    if (max_load == 0) xn_fail("no PT_LOAD");

    Elf64_Phdr dynph = phdrs[dyn_idx];
    if (dynph.p_filesz == 0 || (dynph.p_filesz % sizeof(Elf64_Dyn)) != 0)
        xn_fail("bad dynamic segment size");

    uint64_t dyn_file_end = xn_end(dynph.p_offset, dynph.p_filesz);
    if (dyn_file_end == ~0ULL || dyn_file_end > file_size)
        xn_fail("dynamic segment truncated");

    auto need_off = [&](uint64_t v, uint64_t n) -> uint64_t {
        uint64_t o = xn_v2o(phdrs, v, n);
        if (o == ~0ULL) xn_fail("table address is not inside a loadable segment");
        return o;
    };

    auto offset_to_vaddr = [&](uint64_t off, uint64_t &v) -> bool {
        for (const auto &p : phdrs) {
            if (p.p_type != PT_LOAD || p.p_filesz == 0) continue;
            uint64_t begin = p.p_offset;
            uint64_t end = xn_end(begin, p.p_filesz);
            if (end == ~0ULL) continue;
            if (off >= begin && off < end) {
                uint64_t delta = off - begin;
                if (p.p_vaddr > ~0ULL - delta) return false;
                v = p.p_vaddr + delta;
                return true;
            }
        }
        return false;
    };

    std::vector<Elf64_Dyn> origdyn;
    uint64_t dcount = dynph.p_filesz / sizeof(Elf64_Dyn);
    for (uint64_t i = 0; i < dcount; ++i) {
        Elf64_Dyn d;
        std::memcpy(&d, data.data() + dynph.p_offset + i * sizeof(Elf64_Dyn), sizeof(d));
        origdyn.push_back(d);
        if (d.d_tag == DT_NULL) break;
    }

    if (origdyn.empty()) xn_fail("dynamic table empty");

    std::map<uint64_t, uint64_t> dv;
    for (const auto &d : origdyn) {
        if ((uint64_t)d.d_tag == DT_NULL) break;
        dv.emplace((uint64_t)d.d_tag, (uint64_t)d.d_un.d_ptr);
    }

    auto dget = [&](uint64_t tag) -> uint64_t {
        auto it = dv.find(tag);
        return it == dv.end() ? 0 : it->second;
    };

    const uint64_t original_init = dget(DT_INIT);
    const uint64_t strtab = dget(DT_STRTAB);
    const uint64_t strsz = dget(DT_STRSZ);
    const uint64_t symtab = dget(DT_SYMTAB);
    const uint64_t hash = dget(DT_HASH);
    const uint64_t gnu_hash = dget(DT_GNU_HASH);
    const uint64_t rela = dget(DT_RELA);
    const uint64_t relasz = dget(DT_RELASZ);
    const uint64_t rel = dget(DT_REL);
    const uint64_t relsz = dget(DT_RELSZ);
    const uint64_t jmprel = dget(DT_JMPREL);
    const uint64_t pltrelsz = dget(DT_PLTRELSZ);
    const uint64_t pltrel = dget(DT_PLTREL);
    const uint64_t initarr = dget(DT_INIT_ARRAY);
    const uint64_t initarrsz = dget(DT_INIT_ARRAYSZ);
    const uint64_t finiarr = dget(DT_FINI_ARRAY);
    const uint64_t finiarrsz = dget(DT_FINI_ARRAYSZ);
    const uint64_t preinit = dget(DT_PREINIT_ARRAY);
    const uint64_t preinitsz = dget(DT_PREINIT_ARRAYSZ);
    const uint64_t versym = dget(DT_VERSYM);
    const uint64_t verdef = dget(DT_VERDEF);
    const uint64_t verdefnum = dget(DT_VERDEFNUM);
    const uint64_t verneed = dget(DT_VERNEED);
    const uint64_t verneednum = dget(DT_VERNEEDNUM);
    const uint64_t pltgot = dget(DT_PLTGOT);
    const uint64_t symtab_shndx = dget(XN_DT_SYMTAB_SHNDX);
    const uint64_t relr = dget(XN_DT_RELR);
    const uint64_t relrsz = dget(XN_DT_RELRSZ);

    if (relr && !relrsz) xn_fail("DT_RELR without DT_RELRSZ");

    const uint64_t plt_ent = (pltrel == DT_REL) ? sizeof(Elf64_Rel) : sizeof(Elf64_Rela);

    uint64_t symcount = 0;
    bool have_count = false;
    uint64_t hash_size = 0;
    uint64_t gnu_size = 0;

    if (hash) {
        uint64_t off = need_off(hash, 8);
        uint32_t nb = 0, nc = 0;
        if (!xn_read(data, off, nb) || !xn_read(data, off + 4, nc))
            xn_fail("bad DT_HASH table");
        hash_size = 8ULL + 4ULL * nb + 4ULL * nc;
        (void)need_off(hash, hash_size);
        symcount = nc;
        have_count = true;
    }

    if (gnu_hash) {
        uint64_t goff = need_off(gnu_hash, 16);
        uint32_t nb = 0, so = 0, bs = 0;
        if (!xn_read(data, goff, nb) || !xn_read(data, goff + 4, so) ||
            !xn_read(data, goff + 8, bs) || nb == 0)
            xn_fail("bad DT_GNU_HASH table");

        uint64_t buckets_off = goff + 16ULL + 8ULL * bs;
        uint64_t chains_off = buckets_off + 4ULL * nb;
        if (chains_off > file_size) xn_fail("bad DT_GNU_HASH table");

        uint32_t maxb = 0;
        for (uint32_t b = 0; b < nb; ++b) {
            uint32_t v = 0;
            if (!xn_read(data, buckets_off + 4ULL * b, v)) xn_fail("bad DT_GNU_HASH table");
            if (v > maxb) maxb = v;
        }

        uint64_t count = so;
        uint64_t chain_n = 0;
        if (maxb >= so) {
            uint64_t idx = maxb;
            for (;;) {
                uint32_t cv = 0;
                if (!xn_read(data, chains_off + 4ULL * (idx - so), cv))
                    xn_fail("bad DT_GNU_HASH chain");
                if (cv & 1) break;
                ++idx;
            }
            count = idx + 1;
            chain_n = count - so;
        }

        gnu_size = 16ULL + 8ULL * bs + 4ULL * nb + 4ULL * chain_n;
        (void)need_off(gnu_hash, gnu_size);
        if (!have_count) {
            symcount = count;
            have_count = true;
        }
    }

    if (!have_count && ehdr.e_shoff && ehdr.e_shentsize == sizeof(Elf64_Shdr)) {
        for (uint64_t i = 0; i < ehdr.e_shnum && !have_count; ++i) {
            Elf64_Shdr sh;
            if (!xn_read(data, ehdr.e_shoff + i * sizeof(Elf64_Shdr), sh)) break;
            if (sh.sh_type == SHT_DYNSYM && sh.sh_entsize == sizeof(Elf64_Sym)) {
                symcount = sh.sh_size / sizeof(Elf64_Sym);
                have_count = true;
            }
        }
    }

    if (!have_count) xn_fail("cannot determine .dynsym size");
    if (symcount > file_size / sizeof(Elf64_Sym)) xn_fail("bad dynamic symbol count");

    uint64_t verdef_size = 0;
    if (verdef) {
        if (!verdefnum) xn_fail("DT_VERDEF without DT_VERDEFNUM");
        uint64_t start = need_off(verdef, sizeof(Elf64_Verdef));
        uint64_t off = start;
        uint64_t end = start;
        for (uint64_t i = 0; i < verdefnum; ++i) {
            Elf64_Verdef vd;
            if (!xn_read(data, off, vd)) xn_fail("bad version definition table");
            end = std::max(end, off + sizeof(vd));
            uint64_t aoff = off + vd.vd_aux;
            for (uint64_t k = 0; k < vd.vd_cnt; ++k) {
                Elf64_Verdaux va;
                if (!xn_read(data, aoff, va)) xn_fail("bad version definition table");
                end = std::max(end, aoff + sizeof(va));
                if (!va.vda_next) break;
                aoff += va.vda_next;
            }
            if (!vd.vd_next) break;
            off += vd.vd_next;
        }
        verdef_size = end - start;
        (void)need_off(verdef, verdef_size);
    }

    uint64_t verneed_size = 0;
    if (verneed) {
        if (!verneednum) xn_fail("DT_VERNEED without DT_VERNEEDNUM");
        uint64_t start = need_off(verneed, sizeof(Elf64_Verneed));
        uint64_t off = start;
        uint64_t end = start;
        for (uint64_t i = 0; i < verneednum; ++i) {
            Elf64_Verneed vn;
            if (!xn_read(data, off, vn)) xn_fail("bad version needed table");
            end = std::max(end, off + sizeof(vn));
            uint64_t aoff = off + vn.vn_aux;
            for (uint64_t k = 0; k < vn.vn_cnt; ++k) {
                Elf64_Vernaux va;
                if (!xn_read(data, aoff, va)) xn_fail("bad version needed table");
                end = std::max(end, aoff + sizeof(va));
                if (!va.vna_next) break;
                aoff += va.vna_next;
            }
            if (!vn.vn_next) break;
            off += vn.vn_next;
        }
        verneed_size = end - start;
        (void)need_off(verneed, verneed_size);
    }

    std::vector<Interval> excludes;

    auto add_excl = [&](uint64_t addr, uint64_t size) {
        if (!addr || !size) return;
        if (size > ~0ULL - addr) size = ~0ULL - addr;
        excludes.push_back({addr, size});
    };

    auto add_excl_at = [&](uint64_t addr, uint64_t size) {
        if (!size) return;
        if (size > ~0ULL - addr) size = ~0ULL - addr;
        excludes.push_back({addr, size});
    };

    auto scan_relocs = [&](uint64_t addr, uint64_t size, uint64_t entsz) {
        if (!addr || !size) return;
        if (size % entsz) xn_fail("relocation table size is not a multiple of its entry size");
        uint64_t off = need_off(addr, size);
        for (uint64_t o = 0; o < size; o += entsz) {
            uint64_t r_offset = 0, r_info = 0;
            std::memcpy(&r_offset, data.data() + off + o, 8);
            std::memcpy(&r_info, data.data() + off + o + 8, 8);
            if ((r_info & 0xffffffffULL) == irel_type)
                xn_fail("IRELATIVE relocations (IFUNC) are not supported: resolvers run before DT_INIT");
            add_excl_at(r_offset, 8);
        }
    };

    if (symcount && symtab) {
        uint64_t soff = need_off(symtab, symcount * sizeof(Elf64_Sym));
        for (uint64_t i = 0; i < symcount; ++i) {
            uint8_t info = data[soff + i * sizeof(Elf64_Sym) + 4];
            if (ELF64_ST_TYPE(info) == STT_GNU_IFUNC)
                xn_fail("IFUNC symbols are not supported: resolvers run before DT_INIT");
        }
    }

    if (min_load != ~0ULL)
        add_excl_at(align_down(min_load, 4096), 4096);

    uint64_t mapped_vaddr = 0;
    if (offset_to_vaddr(0, mapped_vaddr))
        add_excl_at(mapped_vaddr, sizeof(Elf64_Ehdr));

    if (ehdr.e_phoff && offset_to_vaddr(ehdr.e_phoff, mapped_vaddr))
        add_excl_at(mapped_vaddr, ph_size);

    if (ehdr.e_shoff && ehdr.e_shnum && ehdr.e_shentsize) {
        if (ehdr.e_shnum <= UINT64_MAX / ehdr.e_shentsize) {
            uint64_t shsz = (uint64_t)ehdr.e_shnum * ehdr.e_shentsize;
            uint64_t shend = xn_end(ehdr.e_shoff, shsz);
            if (shend != ~0ULL && shend <= file_size) {
                if (offset_to_vaddr(ehdr.e_shoff, mapped_vaddr))
                    add_excl_at(mapped_vaddr, shsz);
            }
        }
    }

    for (const auto &p : phdrs) {
        if (p.p_type == PT_LOAD || p.p_type == PT_NULL) continue;
        if (p.p_type == PT_GNU_RELRO && p.p_memsz) {
            uint64_t rs = align_down(p.p_vaddr, 4096);
            uint64_t re_raw = xn_end(p.p_vaddr, p.p_memsz);
            uint64_t re = re_raw == ~0ULL ? ~0ULL : align_up(re_raw, 4096);
            if (re != ~0ULL && re > rs) add_excl_at(rs, re - rs);
        }
        if (p.p_filesz)
            add_excl(p.p_vaddr, p.p_filesz);
    }

    add_excl(dynph.p_vaddr, dynph.p_filesz);
    add_excl(strtab, strsz);
    add_excl(symtab, symcount * sizeof(Elf64_Sym));
    add_excl(hash, hash_size);
    add_excl(gnu_hash, gnu_size);
    add_excl(rela, relasz);
    add_excl(rel, relsz);
    add_excl(jmprel, pltrelsz);
    scan_relocs(rela, relasz, sizeof(Elf64_Rela));
    scan_relocs(rel, relsz, sizeof(Elf64_Rel));
    scan_relocs(jmprel, pltrelsz, plt_ent);
    add_excl(initarr, initarrsz);
    add_excl(finiarr, finiarrsz);
    add_excl(preinit, preinitsz);
    add_excl(relr, relrsz);
    add_excl(symtab_shndx, symcount * sizeof(uint32_t));
    add_excl(versym, symcount * sizeof(uint16_t));
    add_excl(verdef, verdef_size);
    add_excl(verneed, verneed_size);
    add_excl(pltgot, 8ULL * (3ULL + (plt_ent ? pltrelsz / plt_ent : 0ULL)));

    std::vector<ProtRange> ranges;

    for (const auto &p : phdrs) {
        if (p.p_type != PT_LOAD || p.p_filesz == 0)
            continue;

        std::vector<Interval> base;
        base.push_back({p.p_vaddr, p.p_filesz});

        auto kept = xn_subtract(base, excludes);

        uint64_t prot = 0;
        if (p.p_flags & PF_R) prot |= 1;
        if (p.p_flags & PF_W) prot |= 2;
        if (p.p_flags & PF_X) prot |= 4;

        for (auto r : kept) {
            if (!r.size) continue;
            if (r.addr < p.p_vaddr) continue;

            uint64_t vend = xn_end(p.p_vaddr, p.p_filesz);
            if (vend == ~0ULL || r.addr >= vend) continue;

            uint64_t delta = r.addr - p.p_vaddr;
            if (delta > p.p_filesz) continue;
            if (p.p_offset > ~0ULL - delta) continue;

            uint64_t off = p.p_offset + delta;
            if (off > file_size || r.size > file_size - off)
                xn_fail("encryption range out of file");

            ranges.push_back({r.addr, r.size, prot, off, 0});
        }
    }

    if (ranges.empty())
        xn_fail("no loadable data outside the first page to protect (link with -z separate-code)");

    if (!xn_merge_prot(ranges))
        xn_fail("invalid protected ranges");

    if (ranges.size() > 65536)
        xn_fail("too many protected ranges");

    for (auto &r : ranges) {
        if (r.size >= (1ULL << 36))
            xn_fail("range too large");
    }

    std::vector<ProtRange> file_check = ranges;
    std::sort(file_check.begin(), file_check.end(), [](const ProtRange &a, const ProtRange &b) {
        return a.foff < b.foff;
    });

    uint64_t file_prev_end = 0;
    for (const auto &r : file_check) {
        if (r.foff < file_prev_end)
            xn_fail("overlapping encryption ranges in file");
        uint64_t e = xn_end(r.foff, r.size);
        if (e == ~0ULL)
            xn_fail("encryption range overflow");
        file_prev_end = e;
    }

    const uint64_t stub_len = (uint64_t)(reinterpret_cast<uintptr_t>(stub_e) - reinterpret_cast<uintptr_t>(stub_s));
    const uint64_t code_len = (uint64_t)(reinterpret_cast<uintptr_t>(code_e) - reinterpret_cast<uintptr_t>(stub_s));
    const uint64_t cfg_off = (uint64_t)(reinterpret_cast<uintptr_t>(cfg_s) - reinterpret_cast<uintptr_t>(stub_s));
    if (code_len == 0 || cfg_off + XN_CFG_HDR != stub_len)
        xn_fail("stub layout mismatch");

    const uint64_t stub_hash = xn_fnv1a(stub_s, code_len);

    uint8_t keybytes[32];
    int ufd = open("/dev/urandom", O_RDONLY);
    if (ufd < 0) xn_fail("cannot open /dev/urandom");
    ssize_t got = 0;
    while (got < (ssize_t)sizeof(keybytes)) {
        ssize_t rr = read(ufd, keybytes + got, sizeof(keybytes) - got);
        if (rr <= 0) break;
        got += rr;
    }
    close(ufd);
    if (got != (ssize_t)sizeof(keybytes)) xn_fail("cannot read random key");

    uint32_t kw[8];
    for (int i = 0; i < 8; ++i) {
        kw[i] = (uint32_t)keybytes[4 * i] |
                ((uint32_t)keybytes[4 * i + 1] << 8) |
                ((uint32_t)keybytes[4 * i + 2] << 16) |
                ((uint32_t)keybytes[4 * i + 3] << 24);
    }

    uint64_t stored_key[4];
    for (uint64_t i = 0; i < 4; ++i) {
        uint64_t kq = 0;
        for (int b = 7; b >= 0; --b)
            kq = (kq << 8) | keybytes[8 * i + (uint64_t)b];
        stored_key[i] = kq ^ xn_pad_word(stub_hash, i);
    }

    for (auto &r : ranges) {
        if (r.foff > file_size || r.size > file_size - r.foff)
            xn_fail("encryption range out of file");

        r.hash = xn_fnv1a(data.data() + r.foff, r.size);
        xn_crypt(data.data() + r.foff, r.size, kw, r.addr, r.size);
    }

    uint64_t entry_bytes = ranges.size() * XN_ENTRY;
    if (entry_bytes > UINT64_MAX - stub_len)
        xn_fail("configuration overflow");

    uint64_t config_end = stub_len + entry_bytes;
    uint64_t dyn_offset = align_up(config_end, 4096);
    if (dyn_offset == ~0ULL)
        xn_fail("layout overflow");

    uint64_t seg_vaddr = align_up(max_load, 4096);
    if (seg_vaddr == ~0ULL || seg_vaddr == 0 || seg_vaddr < max_load)
        xn_fail("cannot allocate new segment");

    std::vector<Elf64_Dyn> newdyn;
    bool init_set = false;

    for (auto d : origdyn) {
        uint64_t tag = (uint64_t)d.d_tag;
        if (tag == DT_NULL) break;

        if (tag == DT_INIT) {
            if (!init_set) {
                d.d_un.d_ptr = seg_vaddr;
                init_set = true;
                newdyn.push_back(d);
            }
            continue;
        }

        newdyn.push_back(d);
    }

    if (!init_set) {
        Elf64_Dyn d{};
        d.d_tag = DT_INIT;
        d.d_un.d_ptr = seg_vaddr;
        newdyn.push_back(d);
    }

    Elf64_Dyn nul{};
    nul.d_tag = DT_NULL;
    nul.d_un.d_val = 0;
    newdyn.push_back(nul);

    if (newdyn.size() > UINT64_MAX / sizeof(Elf64_Dyn))
        xn_fail("dynamic table overflow");

    uint64_t dyn_bytes = newdyn.size() * sizeof(Elf64_Dyn);
    if (dyn_offset > UINT64_MAX - dyn_bytes)
        xn_fail("layout overflow");

    uint64_t dyn_end_layout = dyn_offset + dyn_bytes;
    uint64_t phdr_offset = align_up(dyn_end_layout, 16);
    if (phdr_offset == ~0ULL)
        xn_fail("layout overflow");

    uint64_t new_phnum = (uint64_t)ehdr.e_phnum + 1;
    if (new_phnum > 65535)
        xn_fail("too many program headers");

    if (new_phnum > UINT64_MAX / sizeof(Elf64_Phdr))
        xn_fail("program header table overflow");

    uint64_t phdr_bytes = new_phnum * sizeof(Elf64_Phdr);
    if (phdr_offset > UINT64_MAX - phdr_bytes)
        xn_fail("layout overflow");

    uint64_t total_end = phdr_offset + phdr_bytes;
    uint64_t total_size = align_up(total_end, 16);
    if (total_size == ~0ULL)
        xn_fail("layout overflow");

    uint64_t seg_offset = align_up(file_size, 4096);
    if (seg_offset == ~0ULL)
        xn_fail("file offset overflow");

    if (seg_offset > UINT64_MAX - total_size)
        xn_fail("file offset overflow");

    uint64_t final_file_size = seg_offset + total_size;
    if (final_file_size > (uint64_t)std::numeric_limits<size_t>::max())
        xn_fail("output file too large");

    if (seg_vaddr > UINT64_MAX - total_size)
        xn_fail("virtual address overflow");

    if (seg_offset > UINT64_MAX - phdr_offset)
        xn_fail("file offset overflow");

    uint64_t new_phoff = seg_offset + phdr_offset;

    Elf64_Ehdr out_ehdr = ehdr;
    out_ehdr.e_phoff = new_phoff;
    out_ehdr.e_phnum = (uint16_t)new_phnum;

    if (ehdr.e_shoff && ehdr.e_shnum && ehdr.e_shentsize == sizeof(Elf64_Shdr)) {
        uint64_t sh_bytes = (uint64_t)ehdr.e_shnum * sizeof(Elf64_Shdr);
        uint64_t sh_end = xn_end(ehdr.e_shoff, sh_bytes);
        if (sh_end == ~0ULL || sh_end > file_size)
            xn_fail("section table truncated");

        uint64_t tmp_v = 0;
        const bool sh_mapped = offset_to_vaddr(ehdr.e_shoff, tmp_v);

        for (uint64_t i = 0; i < ehdr.e_shnum; ++i) {
            Elf64_Shdr sh;
            std::memcpy(&sh, data.data() + ehdr.e_shoff + i * sizeof(Elf64_Shdr), sizeof(sh));
            if ((sh.sh_flags & SHF_ALLOC) || sh.sh_type == SHT_NOBITS || sh.sh_type == SHT_NULL)
                continue;
            if (sh.sh_size == 0) continue;
            if (sh.sh_offset > file_size || sh.sh_size > file_size - sh.sh_offset)
                xn_fail("section data truncated");
            uint64_t tmp_s = 0;
            if (offset_to_vaddr(sh.sh_offset, tmp_s)) continue;
            std::memset(data.data() + sh.sh_offset, 0, (size_t)sh.sh_size);
        }

        if (!sh_mapped)
            std::memset(data.data() + ehdr.e_shoff, 0, (size_t)sh_bytes);

        out_ehdr.e_shoff = 0;
        out_ehdr.e_shnum = 0;
        out_ehdr.e_shstrndx = 0;
    }

    std::memcpy(data.data(), &out_ehdr, sizeof(out_ehdr));

    data.resize((size_t)final_file_size, 0);

    std::memcpy(data.data() + seg_offset, stub_s, (size_t)stub_len);

    const uint64_t cfg_base = seg_offset + cfg_off;
    auto put64 = [&](uint64_t abs_off, uint64_t v) {
        std::memcpy(data.data() + abs_off, &v, 8);
    };

    put64(cfg_base + 0, XN_MAGIC);
    put64(cfg_base + 8, seg_vaddr);
    put64(cfg_base + 16, original_init);
    put64(cfg_base + 24, (uint64_t)ranges.size());
    for (uint64_t i = 0; i < 4; ++i)
        put64(cfg_base + 32 + 8 * i, stored_key[i]);

    uint64_t ent = seg_offset + stub_len;
    for (const auto &r : ranges) {
        uint64_t packed = r.size | ((r.prot & 0xff) << 56);
        put64(ent + 0, r.addr);
        put64(ent + 8, packed);
        put64(ent + 16, r.hash);
        ent += XN_ENTRY;
    }

    std::memcpy(data.data() + seg_offset + dyn_offset, newdyn.data(), (size_t)dyn_bytes);

    std::vector<Elf64_Phdr> out_phdrs = phdrs;

    Elf64_Phdr &dynout = out_phdrs[dyn_idx];
    dynout.p_type = PT_DYNAMIC;
    dynout.p_flags = PF_R;
    dynout.p_offset = seg_offset + dyn_offset;
    dynout.p_vaddr = seg_vaddr + dyn_offset;
    dynout.p_paddr = seg_vaddr + dyn_offset;
    dynout.p_filesz = dyn_bytes;
    dynout.p_memsz = dyn_bytes;
    dynout.p_align = 8;

    if (phdr_idx >= 0) {
        Elf64_Phdr &phout = out_phdrs[phdr_idx];
        phout.p_type = PT_PHDR;
        phout.p_flags = PF_R;
        phout.p_offset = seg_offset + phdr_offset;
        phout.p_vaddr = seg_vaddr + phdr_offset;
        phout.p_paddr = seg_vaddr + phdr_offset;
        phout.p_filesz = phdr_bytes;
        phout.p_memsz = phdr_bytes;
        phout.p_align = 8;
    }

    Elf64_Phdr newload{};
    newload.p_type = PT_LOAD;
    newload.p_flags = PF_R | PF_X;
    newload.p_offset = seg_offset;
    newload.p_vaddr = seg_vaddr;
    newload.p_paddr = seg_vaddr;
    newload.p_filesz = total_size;
    newload.p_memsz = total_size;
    newload.p_align = 4096;
    out_phdrs.push_back(newload);

    if (out_phdrs.size() != (size_t)new_phnum)
        xn_fail("program header table mismatch");

    std::memcpy(data.data() + seg_offset + phdr_offset, out_phdrs.data(), (size_t)phdr_bytes);

    std::string out_path = xn_base_name(in_path) + ".xn.so";
    FILE *of = std::fopen(out_path.c_str(), "wb");
    if (!of) xn_fail("cannot create output file");

    if (std::fwrite(data.data(), 1, data.size(), of) != data.size()) {
        std::fclose(of);
        xn_fail("cannot write output file");
    }

    std::fclose(of);
    chmod(out_path.c_str(), 0755);

    std::printf("XNebula patch created: %s\n", out_path.c_str());
    return 0;
}
