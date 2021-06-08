.globl asmmath_floor
.globl asmmath_vec3cos
.globl asmmath_vec3sin
.globl asmmath_sin
.globl asmmath_cos
.globl asmmath_sqrt
.globl asmmath_abs
.globl asmmath_min
.globl asmmath_max
asmmath_floor:
    roundsd $9,%xmm0,%xmm0
    ret

.globl asmmath_vec3cos
.section .data
tmp: .quad 0, 0
.align 16
mask: .quad 0x7fffffffffffffff //turns out that it needs to be aligned to 16 bytes or else it'll segfault.
.section .text
asmmath_abs:   
    andpd mask,%xmm0
    ret

asmmath_tan:
    movq %xmm0,tmp(%rip)
    fldl tmp(%rip)
    fptan
    fstpl tmp(%rip)
    fstpl tmp(%rip) //need to do this 2 times since fptan pushes 1 too
    movq tmp(%rip),%xmm0
    ret

asmmath_cos:
    movq %xmm0,tmp(%rip)
    fldl tmp(%rip)
    fcos
    fstpl tmp(%rip)
    movq tmp(%rip),%xmm0
    ret
    
    
asmmath_sin:
    movq %xmm0,tmp(%rip)
    fldl tmp(%rip)
    fsin
    fstpl tmp(%rip)
    movq tmp(%rip),%xmm0
    ret

asmmath_sqrt:
    movq %xmm0,tmp(%rip)
    fldl tmp(%rip)
    fsqrt
    fstpl tmp(%rip)
    movq tmp(%rip),%xmm0
    ret
retc:
    ret
asmmath_min:
    minpd %xmm0,%xmm1
    movq %xmm1,%xmm0
    ret
asmmath_max:
    maxpd %xmm0,%xmm1
    movq %xmm1,%xmm0
    ret
    