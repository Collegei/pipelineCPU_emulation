# RISC-V pipeline processor
The processor emulator supports following Risc-v instruction.
<table>
  <tr>
    <td><strong>Instruction</strong></td>
            <td><strong>Remark</strong></td>
  </tr>
  <tr>
    <td>add rd, rs1, rs2</td>
  </tr>
  <tr>
    <td>sub rd, rs1, rs2</td>
  </tr>
  <tr>
    <td></td>and rd, rs1, rs2</td>
  </tr>
  <tr>
    <td>or rd, rs1, rs2</td>
  </tr>
  <tr>
    <td>lw rd constant(rs1)</td>
  </tr>
  <tr>
    <td>addi rd, rs1, constant</td>
  </tr>
  <tr>
    <td>sw rs2, constant(rs1)</td>
  </tr>
  <tr>
    <td>beq rs1, rs2, constant</td>
  </tr>
  <tr>
    <td>bne rs1, rs2, constant</td>
  </tr>
  <tr>
    <td>end</td>
    <td>opcode = 1000000 (at the end of code)</td>
  </tr>
  <tr>
    <td>nop</td>
    <td>opcode = 0000000 (none)</td>
  </tr> 
</table>
p.s. I know it's very incomplete, but I think it's still sufficient to implement several program.

# Input Format
There are some examples in input/ directory.

# Excecution
Your Risc-v assembly-code shoud be run with following command.
```shell
$ make all
$ ./main <file_name> <register_name>
```
ex. ./main input/recursion x5
