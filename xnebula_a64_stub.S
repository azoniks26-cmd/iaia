.text
.macro MOV32 r, v
movz \r, #((\v) & 0xffff)
movk \r, #(((\v) >> 16) & 0xffff), lsl #16
.endm
.macro MOV64 r, v
movz \r, #((\v) & 0xffff)
movk \r, #(((\v) >> 16) & 0xffff), lsl #16
movk \r, #(((\v) >> 32) & 0xffff), lsl #32
movk \r, #(((\v) >> 48) & 0xffff), lsl #48
.endm
.macro QRR qa, qb, qc, qd
add \qa, \qa, \qb
eor \qd, \qd, \qa
ror \qd, \qd, #16
add \qc, \qc, \qd
eor \qb, \qb, \qc
ror \qb, \qb, #20
add \qa, \qa, \qb
eor \qd, \qd, \qa
ror \qd, \qd, #24
add \qc, \qc, \qd
eor \qb, \qb, \qc
ror \qb, \qb, #25
.endm
.globl xn_a64_stub_start
.type xn_a64_stub_start,@function
xn_a64_stub_start:
.Lxn_a64_stub_start:
stp x29, x30, [sp, #-112]!
stp x19, x20, [sp, #16]
stp x21, x22, [sp, #32]
stp x23, x24, [sp, #48]
stp x25, x26, [sp, #64]
stp x27, x28, [sp, #80]
sub sp, sp, #1088
str x0, [sp, #256]
str x1, [sp, #264]
str x2, [sp, #272]
adr x19, .Lxn_a64_stub_start
adr x20, .Lxn_a64_cfg_start
MOV64 x9, 0x584e4542554c4158
ldr x10, [x20]
cmp x10, x9
b.eq .Lxn_a64_magic_ok
adr x1, .Lxn_a64_msg_bad
movz x2, #(.Lxn_a64_msg_bad_end - .Lxn_a64_msg_bad)
b .Lxn_a64_die
.Lxn_a64_magic_ok:
ldr x9, [x20, #8]
sub x10, x19, x9
str x10, [sp, #0]
ldr x9, [x20, #16]
str x9, [sp, #8]
ldr x9, [x20, #24]
str x9, [sp, #16]
add x9, x20, #64
str x9, [sp, #24]
mov x0, #4
mov x1, #0
mov x8, #167
svc #0
bl .Lxn_a64_tracer_check
mov x0, #0
mov x1, #8192
mov x2, #3
mov x3, #0x22
mov x4, #-1
mov x5, #0
mov x8, #222
svc #0
tbnz x0, #63, .Lxn_a64_mon_fail
add x1, x0, #8192
MOV32 w0, 0x50f00
mov x2, #0
mov x3, #0
mov x4, #0
mov x8, #220
svc #0
tbnz x0, #63, .Lxn_a64_mon_fail
cbz x0, .Lxn_a64_monitor
.Lxn_a64_dbg_done:
mov x13, x19
movz x12, #(xn_a64_code_end - xn_a64_stub_start)
add x12, x19, x12
MOV64 x14, 0xcbf29ce484222325
MOV64 x15, 0x100000001b3
.Lxn_a64_pad_hash:
ldrb w9, [x13], #1
eor x14, x14, x9
mul x14, x14, x15
cmp x13, x12
b.lo .Lxn_a64_pad_hash
mov x21, #0
.Lxn_a64_key:
add x9, x21, #1
MOV64 x10, 0x9e3779b97f4a7c15
mul x9, x9, x10
add x9, x9, x14
lsr x10, x9, #30
eor x9, x9, x10
MOV64 x10, 0xbf58476d1ce4e5b9
mul x9, x9, x10
lsr x10, x9, #27
eor x9, x9, x10
MOV64 x10, 0x94d049bb133111eb
mul x9, x9, x10
lsr x10, x9, #31
eor x9, x9, x10
lsl x10, x21, #3
add x11, x20, x10
ldr x11, [x11, #32]
eor x11, x11, x9
add x12, sp, #32
add x12, x12, x10
str x11, [x12]
add x21, x21, #1
cmp x21, #4
b.lo .Lxn_a64_key
ldr x9, [sp, #32]
str x9, [sp, #144]
ldr x9, [sp, #40]
str x9, [sp, #152]
ldr x9, [sp, #48]
str x9, [sp, #160]
ldr x9, [sp, #56]
str x9, [sp, #168]
MOV32 w9, 0x61707865
str w9, [sp, #128]
MOV32 w9, 0x3320646e
str w9, [sp, #132]
MOV32 w9, 0x79622d32
str w9, [sp, #136]
MOV32 w9, 0x6b206574
str w9, [sp, #140]
.Lxn_a64_range_loop:
ldr x9, [sp, #16]
cbz x9, .Lxn_a64_ranges_done
sub x9, x9, #1
str x9, [sp, #16]
ldr x9, [sp, #24]
ldr x10, [x9]
ldr x11, [x9, #8]
ldr x12, [x9, #16]
add x9, x9, #24
str x9, [sp, #24]
str x10, [sp, #192]
str x12, [sp, #216]
lsr x13, x11, #56
str x13, [sp, #208]
lsl x11, x11, #8
lsr x11, x11, #8
str x11, [sp, #200]
cbz x11, .Lxn_a64_range_loop
ldr x13, [sp, #0]
add x14, x10, x13
str x14, [sp, #224]
lsr x15, x14, #12
lsl x15, x15, #12
add x16, x14, x11
add x16, x16, #4095
lsr x16, x16, #12
lsl x16, x16, #12
sub x1, x16, x15
mov x0, x15
mov x2, #3
mov x8, #226
svc #0
cbnz x0, .Lxn_a64_err_mprot
ldr x9, [sp, #192]
str w9, [sp, #180]
lsr x10, x9, #32
str w10, [sp, #184]
ldr x11, [sp, #200]
str w11, [sp, #188]
str wzr, [sp, #176]
ldr x9, [sp, #200]
str x9, [sp, #232]
ldr x9, [sp, #224]
str x9, [sp, #240]
.Lxn_a64_block:
ldr w0, [sp, #128]
ldr w1, [sp, #132]
ldr w2, [sp, #136]
ldr w3, [sp, #140]
ldr w4, [sp, #144]
ldr w5, [sp, #148]
ldr w6, [sp, #152]
ldr w7, [sp, #156]
ldr w8, [sp, #160]
ldr w9, [sp, #164]
ldr w10, [sp, #168]
ldr w11, [sp, #172]
ldr w12, [sp, #176]
ldr w13, [sp, #180]
ldr w14, [sp, #184]
ldr w15, [sp, #188]
mov w17, #10
.Lxn_a64_round:
QRR w0, w4, w8, w12
QRR w1, w5, w9, w13
QRR w2, w6, w10, w14
QRR w3, w7, w11, w15
QRR w0, w5, w10, w15
QRR w1, w6, w11, w12
QRR w2, w7, w8, w13
QRR w3, w4, w9, w14
subs w17, w17, #1
b.ne .Lxn_a64_round
ldr w16, [sp, #128]
add w0, w0, w16
ldr w16, [sp, #132]
add w1, w1, w16
ldr w16, [sp, #136]
add w2, w2, w16
ldr w16, [sp, #140]
add w3, w3, w16
ldr w16, [sp, #144]
add w4, w4, w16
ldr w16, [sp, #148]
add w5, w5, w16
ldr w16, [sp, #152]
add w6, w6, w16
ldr w16, [sp, #156]
add w7, w7, w16
ldr w16, [sp, #160]
add w8, w8, w16
ldr w16, [sp, #164]
add w9, w9, w16
ldr w16, [sp, #168]
add w10, w10, w16
ldr w16, [sp, #172]
add w11, w11, w16
ldr w16, [sp, #176]
add w12, w12, w16
ldr w16, [sp, #180]
add w13, w13, w16
ldr w16, [sp, #184]
add w14, w14, w16
ldr w16, [sp, #188]
add w15, w15, w16
str w0, [sp, #64]
str w1, [sp, #68]
str w2, [sp, #72]
str w3, [sp, #76]
str w4, [sp, #80]
str w5, [sp, #84]
str w6, [sp, #88]
str w7, [sp, #92]
str w8, [sp, #96]
str w9, [sp, #100]
str w10, [sp, #104]
str w11, [sp, #108]
str w12, [sp, #112]
str w13, [sp, #116]
str w14, [sp, #120]
str w15, [sp, #124]
ldr x9, [sp, #232]
mov x10, #64
cmp x9, x10
csel x10, x9, x10, lo
ldr x11, [sp, #240]
add x14, sp, #64
mov x12, x10
.Lxn_a64_xor:
ldrb w13, [x14], #1
ldrb w15, [x11]
eor w15, w15, w13
strb w15, [x11], #1
subs x12, x12, #1
b.ne .Lxn_a64_xor
str x11, [sp, #240]
ldr x9, [sp, #232]
sub x9, x9, x10
str x9, [sp, #232]
ldr w12, [sp, #176]
add w12, w12, #1
str w12, [sp, #176]
cbnz x9, .Lxn_a64_block
ldr x9, [sp, #224]
ldr x10, [sp, #200]
add x10, x9, x10
MOV64 x11, 0xcbf29ce484222325
MOV64 x12, 0x100000001b3
.Lxn_a64_hash:
cmp x9, x10
b.hs .Lxn_a64_hash_done
ldrb w13, [x9], #1
eor x11, x11, x13
mul x11, x11, x12
b .Lxn_a64_hash
.Lxn_a64_hash_done:
ldr x13, [sp, #216]
cmp x11, x13
b.ne .Lxn_a64_err_integrity
ldr x9, [sp, #224]
ldr x11, [sp, #200]
lsr x10, x9, #12
lsl x10, x10, #12
add x12, x9, x11
add x12, x12, #4095
lsr x12, x12, #12
lsl x12, x12, #12
sub x1, x12, x10
mov x0, x10
ldr x2, [sp, #208]
mov x8, #226
svc #0
cbnz x0, .Lxn_a64_err_mprot
ldr x9, [sp, #224]
ldr x11, [sp, #200]
add x12, x9, x11
mrs x14, ctr_el0
ubfx x15, x14, #16, #4
mov x16, #4
lsl x15, x16, x15
sub x17, x15, #1
bic x13, x9, x17
.Lxn_a64_dc:
dc cvau, x13
add x13, x13, x15
cmp x13, x12
b.lo .Lxn_a64_dc
dsb ish
and x15, x14, #15
mov x16, #4
lsl x15, x16, x15
sub x17, x15, #1
bic x13, x9, x17
.Lxn_a64_ic:
ic ivau, x13
add x13, x13, x15
cmp x13, x12
b.lo .Lxn_a64_ic
dsb ish
isb
b .Lxn_a64_range_loop
.Lxn_a64_ranges_done:
ldr x9, [sp, #8]
cbz x9, .Lxn_a64_done
ldr x10, [sp, #0]
add x9, x9, x10
ldr x0, [sp, #256]
ldr x1, [sp, #264]
ldr x2, [sp, #272]
blr x9
.Lxn_a64_done:
add sp, sp, #1088
ldp x27, x28, [sp, #80]
ldp x25, x26, [sp, #64]
ldp x23, x24, [sp, #48]
ldp x21, x22, [sp, #32]
ldp x19, x20, [sp, #16]
ldp x29, x30, [sp], #112
ret
.Lxn_a64_err_mprot:
adr x1, .Lxn_a64_msg_mprot
movz x2, #(.Lxn_a64_msg_mprot_end - .Lxn_a64_msg_mprot)
b .Lxn_a64_die
.Lxn_a64_err_integrity:
adr x1, .Lxn_a64_msg_integ
movz x2, #(.Lxn_a64_msg_integ_end - .Lxn_a64_msg_integ)
b .Lxn_a64_die
.Lxn_a64_die:
mov x0, #2
mov x8, #64
svc #0
mov x0, #1
mov x8, #94
svc #0
.Lxn_a64_tracer_check:
sub sp, sp, #1056
mov x0, #-100
adr x1, .Lxn_a64_path
mov x2, #0
mov x3, #0
mov x8, #56
svc #0
tbnz x0, #63, .Lxn_a64_status_fail
str x0, [sp, #1024]
mov x1, sp
mov x2, #1023
mov x8, #63
svc #0
str x0, [sp, #1032]
ldr x0, [sp, #1024]
mov x8, #57
svc #0
ldr x2, [sp, #1032]
cmp x2, #0
b.le .Lxn_a64_status_fail
mov x3, sp
add x2, x3, x2
adr x5, .Lxn_a64_pat
.Lxn_a64_chk_scan:
add x9, x3, #10
cmp x9, x2
b.hi .Lxn_a64_status_fail
mov x10, #0
.Lxn_a64_chk_cmp:
cmp x10, #10
b.hs .Lxn_a64_chk_found
ldrb w11, [x3, x10]
ldrb w12, [x5, x10]
cmp w11, w12
b.ne .Lxn_a64_chk_next
add x10, x10, #1
b .Lxn_a64_chk_cmp
.Lxn_a64_chk_next:
add x3, x3, #1
b .Lxn_a64_chk_scan
.Lxn_a64_chk_found:
add x3, x3, #10
.Lxn_a64_chk_ws:
cmp x3, x2
b.hs .Lxn_a64_status_fail
ldrb w11, [x3]
cmp w11, #32
b.eq .Lxn_a64_chk_ws_next
cmp w11, #9
b.eq .Lxn_a64_chk_ws_next
cmp w11, #48
b.ne .Lxn_a64_dbg_detected
add sp, sp, #1056
ret
.Lxn_a64_chk_ws_next:
add x3, x3, #1
b .Lxn_a64_chk_ws
.Lxn_a64_status_fail:
adr x1, .Lxn_a64_msg_status
movz x2, #(.Lxn_a64_msg_status_end - .Lxn_a64_msg_status)
b .Lxn_a64_die
.Lxn_a64_dbg_detected:
adr x1, .Lxn_a64_msg_dbg
movz x2, #(.Lxn_a64_msg_dbg_end - .Lxn_a64_msg_dbg)
b .Lxn_a64_die
.Lxn_a64_mon_fail:
adr x1, .Lxn_a64_msg_mon
movz x2, #(.Lxn_a64_msg_mon_end - .Lxn_a64_msg_mon)
b .Lxn_a64_die
.Lxn_a64_monitor:
sub sp, sp, #16
str xzr, [sp, #0]
MOV32 x9, 100000000
str x9, [sp, #8]
.Lxn_a64_mon_loop:
bl .Lxn_a64_tracer_check
mov x0, sp
mov x1, #0
mov x8, #101
svc #0
b .Lxn_a64_mon_loop
.Lxn_a64_msg_bad:
.ascii "XNebula: bad configuration\n"
.Lxn_a64_msg_bad_end:
.Lxn_a64_msg_dbg:
.ascii "XNebula: debugger detected\n"
.Lxn_a64_msg_dbg_end:
.Lxn_a64_msg_mprot:
.ascii "XNebula: mprotect refused (SELinux or W^X policy)\n"
.Lxn_a64_msg_mprot_end:
.Lxn_a64_msg_integ:
.ascii "XNebula: code integrity check failed\n"
.Lxn_a64_msg_integ_end:
.Lxn_a64_msg_status:
.ascii "XNebula: process state unavailable\n"
.Lxn_a64_msg_status_end:
.Lxn_a64_msg_mon:
.ascii "XNebula: monitor unavailable\n"
.Lxn_a64_msg_mon_end:
.Lxn_a64_path:
.asciz "/proc/self/status"
.Lxn_a64_pat:
.ascii "TracerPid:"
.balign 16
.balign 16
.globl xn_a64_code_end
xn_a64_code_end:
.balign 16
.globl xn_a64_cfg_start
xn_a64_cfg_start:
.Lxn_a64_cfg_start:
.skip 64
.globl xn_a64_stub_end
xn_a64_stub_end: