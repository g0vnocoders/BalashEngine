.globl asmmath_floor
.globl asmmath_vec3cos
.globl asmmath_vec3sin
.globl asmmath_sin
.globl asmmath_cos
.globl asmmath_sqrt
.globl asmmath_abs
.globl asmmath_min
.globl asmmath_max
.globl asmmath_max_simd
.globl asmmath_min_simd
.globl asmmath_sub_SSE_V2
.globl tmp
asmmath_sub_SSE_V2:
    movupd (%rsi),%xmm1
    movupd (%rdi),%xmm2
    subpd %xmm2,%xmm1
    movupd %xmm1,%xmm0
    ret
asmmath_floor:
    roundsd $9,%xmm0,%xmm0
    ret

.globl asmmath_vec3cos
.section .data
tmp: .quad 0, 0
.align 16
mask: .quad 0x7fffffffffffffff
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

asmmath_min:
    minpd %xmm0,%xmm1
    movq %xmm1,%xmm0
    ret
asmmath_max:
    maxpd %xmm0,%xmm1
    movq %xmm1,%xmm0
    ret
asmmath_min_simd:

    movq %xmm1,tmp
    movhpd tmp,%xmm0
    movq %xmm3,tmp
    movhpd tmp,%xmm2
    minpd %xmm0,%xmm2
    movdqa %xmm2,tmp
    movq $tmp,%rax
    ret
asmmath_max_simd:

    movq %xmm1,tmp
    movhpd tmp,%xmm0
    movq %xmm3,tmp
    movhpd tmp,%xmm2
    maxpd %xmm0,%xmm2
    movdqa %xmm2,tmp
    movq $tmp,%rax
    ret