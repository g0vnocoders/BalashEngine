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
mask: .quad 0x7fffffffffffffff
.section .text
asmmath_abs:   
    andpd mask(%rip)/*64bit assembly hack for dealing with PIE*/,%xmm0
    ret
asmmath_vec3cos:
    fldl (%rdi)
    fcos
    fstpl (%rdi)
    fldl 8(%rdi)
    fcos
    fstpl 8(%rdi)
    fldl 16(%rdi)
    fcos
    fstpl 16(%rdi)
    movq %rdi,%rax
    ret
asmmath_vec3sin:
    fldl (%rdi)
    fsin
    fstpl (%rdi)
    fldl 8(%rdi)
    fsin
    fstpl 8(%rdi)
    fldl 16(%rdi)
    fsin
    fstpl 16(%rdi)
    movq %rdi,%rax
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
    comisd %xmm1,%xmm0
    jbe retc
    movq %xmm1, %xmm0
asmmath_max:
    comisd %xmm0,%xmm1
    jbe retc
    movq %xmm1, %xmm0
    