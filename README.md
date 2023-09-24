# RISC-V pipeline processor
The processor emulator supports following Risc-v instruction.
**add rd, rs1, rs2**
**sub rd, rs1, rs2**
**and rd, rs1, rs2**
**or rd, rs1, rs2**
**lw rd constant(rs1)**
**addi rd, rs1, constant**
**sw rs2, constant(rs1)**
**beq rs1, rs2, constant**
**bne rs1, rs2, constant**
**end (opcode = 1000000) //at the end of the code**
**nop (opcode = 0000000) //for both label, nop instruction**
p.s. I know it's still very incomplete, but I think its a good start for me.
#Input Format

#Excecution
Your Risc-v assembly-code shoud be run with following command.
**$ make all**
**$ ./main <riscv-assembly-code> <register-name>**
ex. ./main input/recursion x5
