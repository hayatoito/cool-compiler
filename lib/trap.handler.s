	.kdata
__m1_:	.asciiz "  Exception "
__m2_:	.asciiz " occurred and ignored\n"
__e0_:	.asciiz "  [Interrupt] "
__e1_:	.asciiz	""
__e2_:	.asciiz	""
__e3_:	.asciiz	""
__e4_:	.asciiz	"  [Unaligned address in inst/data fetch] "
__e5_:	.asciiz	"  [Unaligned address in store] "
__e6_:	.asciiz	"  [Bad address in text read] "
__e7_:	.asciiz	"  [Bad address in data/stack read] "
__e8_:	.asciiz	"  [Error in syscall] "
__e9_:	.asciiz	"  [Breakpoint] "
__e10_:	.asciiz	"  [Reserved instruction] "
__e11_:	.asciiz	""
__e12_:	.asciiz	"  [Arithmetic overflow] "
__e13_:	.asciiz	"  [Inexact floating point result] "
__e14_:	.asciiz	"  [Invalid floating point result] "
__e15_:	.asciiz	"  [Divide by 0] "
__e16_:	.asciiz	"  [Floating point overflow] "
__e17_:	.asciiz	"  [Floating point underflow] "
__excp:	.word __e0_,__e1_,__e2_,__e3_,__e4_,__e5_,__e6_,__e7_,__e8_,__e9_
	.word __e10_,__e11_,__e12_,__e13_,__e14_,__e15_,__e16_,__e17_
s1:	.word 0
s2:	.word 0

    .ktext 0x80000080
	.set noat
	# Because we are running in the kernel, we can use $k0/$k1 without
	# saving their old values.
	move $k1 $at	# Save $at
	.set at
	sw $v0 s1	# Not re-entrent and we can't trust $sp
	sw $a0 s2
	mfc0 $k0 $13	# Cause
        sgt $v0 $k0 0x44 # ignore interrupt exceptions
        bgtz $v0 ret
        addu $0 $0 0
	li $v0 4	# syscall 4 (print_str)
	la $a0 __m1_
	syscall
	li $v0 1	# syscall 1 (print_int)
        srl $a0 $k0 2	# shift Cause reg
	syscall
	li $v0 4	# syscall 4 (print_str)
	lw $a0 __excp($k0)
	syscall
	bne $k0 0x18 ok_pc # Bad PC requires special checks
	mfc0 $a0, $14	# EPC
	and $a0, $a0, 0x3 # Is EPC word-aligned?
	beq $a0, 0, ok_pc
	li $v0 10	# Exit on really bad PC (out of text)
	syscall

ok_pc:
    li $v0 4	# syscall 4 (print_str)
	la $a0 __m2_
	syscall
	mtc0 $0, $13	# Clear Cause register
ret:	lw $v0 s1
	lw $a0 s2
	mfc0 $k0 $14	# EPC
	.set noat
	move $at $k1	# Restore $at
	.set at
	rfe		# Return from exception handler
	addiu $k0 $k0 4 # Return to next instruction
	jr $k0


    .data

# heap memory block constants
MEM_META_COUNT=2
MEM_HDR_AVAILABLE=1
MEM_HDR_UNAVAILABLE=0
MEM_HDR_AVAILABILITY=0
MEM_HDR_SIZE=4
MEM_RAW_START=8

# misc constants
WORD_SIZE=4

# object layout constants
OBJ_HDR_TAG=0
OBJ_HDR_SIZE=4
OBJ_HDR_DISP=8
OBJ_HDR_COUNT=3
OBJ_ATTRIB_START=12

# variable that holds address of start of heap (dynamically allocated portion)
heap_start:
    .word 0

# variable to keep track of current break
# used by the memory manager when allocating memory
heap_break:
    .word 0

	.text
	.globl __start
__start:
	lw $a0, 0($sp)	# argc
	addiu $a1, $sp, 4 # argv
	addiu $a2, $a1, 4 # envp
	sll $v0, $a0, 2
	addu $a2, $a2, $v0
    jal memmgr_init
	jal Main.main
	li $v0 10
	syscall		# syscall 10 (exit)

memmgr_init:
    move $a0, $zero
    li $v0, 9
    syscall
    sw $v0, heap_start
    sw $v0, heap_break
    jr $ra
memmgr_alloc:
    lw $t1, heap_start               
    lw $t4, heap_break
memmgr_get_chunk:
    beq $t1, $t4, memmgr_req_mem     # if current heap pointer is equal to current break, we need to ask kernel for more
    lw $t2, MEM_HDR_AVAILABILITY($t1)   # store availability value to $t2
    seq $t2, $t2, 1        
    lw $t3, MEM_HDR_SIZE($t1)         
    sge $t3, $t3, $a0                # checks that the current chunk's size >= requested size
    and $t2, $t2, $t3
    move $v0, $t1
    beq $t2, 1, memmgr_allocated     # if chunk is available && size >= requested size, return chunk
    lw $t2, MEM_HDR_SIZE($t1)
    addiu $t2, $t2, MEM_META_COUNT   
    mul $t2, $t2, WORD_SIZE
    add $t1, $t1, $t2                # if none of the conditions satisfied, move current heap position to next chunk
    b memmgr_get_chunk
memmgr_req_mem:
    move $t1, $a0                    # store requested # of words to $t1  
    addiu $a0, $a0, MEM_META_COUNT   # add 2 extra words to account for metadata (availability & allocation size)
    mul $a0, $a0, WORD_SIZE          # multiply by word size to get total number of bytes needed 
    li $v0, 9                        # load syscall number of sbrk
    syscall       
    add $t2, $v0, $a0                
    sw $t2, heap_break               # increment break to point to one past valid dynamically allocated memory
    sw $t1, MEM_HDR_SIZE($v0)        # store block size (not including metadata) to newly allocated memory
memmgr_allocated:
    sw $zero, MEM_HDR_AVAILABILITY($v0) # mark the newly allocated chunk of memory as unavailable
    la $v0, MEM_RAW_START($v0)       # store pointer to new memory to $a0
    jr $ra

    .globl Object.copy
Object.copy:
    addiu $sp, $sp, -8
    sw $a0, 4($sp)
    sw $ra, 8($sp)
    lw $a0, OBJ_HDR_SIZE($a0)        # get size of object and store to acc
    jal memmgr_alloc                 # allocate memory
    lw $t1, 4($sp)
    lw $t2, OBJ_HDR_SIZE($t1)
    move $t4, $zero
__copy_attrib:
    lw $t3, 0($t1)
    sw $t3, 0($v0) 
    addiu $t1, $t1, WORD_SIZE
    addiu $v0, $v0, WORD_SIZE
    addiu $t4, $t4, 1
    bne $t4, $t2, __copy_attrib
    mul $t2, $t2, WORD_SIZE
    sub $v0, $v0, $t2 
    lw $ra, 8($sp)
    addiu $sp, $sp, 8
    jr $ra
