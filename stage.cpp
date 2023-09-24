#include <iostream>
#include "stage.h"
const uint32_t bits20(0b11111111111111111111);
const uint32_t bits12(0b111111111111);
const uint32_t bits8(0b11111111);
const uint32_t bits7(0b1111111);
const uint32_t bits5(0b11111);
const uint32_t bits3(0b111);
const uint32_t RTYPE(0b0110011);
const uint32_t IMM(0b0010011);
const uint32_t LOAD(0b0000011);
const uint32_t STORE(0b0100011);
const uint32_t BRANCH(0b1100011);
const uint32_t END(0b1000000);
const uint32_t NOP(0b0000000);
//R-type lw sw beq 
//need load instructions into Memory
uint32_t InstructionFetch::Operate(const uint32_t& nowPC,const ControlUnit& nowControl,const DataPath& nowDatapath){
	CleanControlData();
	CleanData();
	SetPC(nowControl.GetPCSrc() ? nowDatapath.GetBranchTarget() : nowPC); //need to bubble before execute (hazard)
	uint32_t op = 0x0;
	for (uint32_t i = 0; i < 4; ++i){
		op |= ((uint32_t) InstructionMemory[GetPC() + i]) << (8*(3-i));
	}
	SetOP(op);
	return GetPC() + 4;
}

void InstructionDecode::Operate(const ControlUnit& nowControl,const DataPath& nowDatapath){
	//decode
	uint32_t opcode = GetOP() & bits7;
	uint32_t funct7,funct3;
	switch (opcode){
		case RTYPE: //Rtype
			SetRegWrite(true);		
			SetALUSrc(false);
			SetBranch(false);
			SetMemWrite(false);
			SetMemRead(false);
			SetMemToReg(false);
			SetEnd(false);
			funct7 = (GetOP() >> 25) & bits7;
			funct3 = (GetOP() >> 12) & bits3;
			if (funct7 == 0 && funct3 == 0){SetALUop(0,0);}//add
			else if (funct7 == 32 && funct3 == 0){SetALUop(0,1);}//sub
			else if (funct7 == 0 && funct3 == 7){SetALUop(1,0);}//and
			else if (funct7 == 0 && funct3 == 6){SetALUop(1,1);}//or
			else{
				std::cout<<"unrecognize operation"<<std::endl;
				exit(1);
			}	
			break;
		case IMM:
			SetRegWrite(true);		
			SetALUSrc(true);
			SetBranch(false);
			SetMemWrite(false);
			SetMemRead(false);
			SetMemToReg(false);
			SetEnd(false);
			
			funct3 = (GetOP() >> 12) & bits3;
			if (funct3 == 0){SetALUop(0,0);}
			else if (funct3 == 7){SetALUop(1,0);}
			else if (funct3 == 6){SetALUop(1,1);}
			else{
				std::cout<<"unrecognize operation"<<std::endl;
				exit(1);
			}	
			break;
		case LOAD:
			SetRegWrite(true);		
			SetALUSrc(true);
			SetBranch(false);
			SetMemWrite(false);
			SetMemRead(true);
			SetMemToReg(true);
			SetEnd(false);

			SetALUop(0,0);
			break;
		case STORE:
			SetRegWrite(false);		
			SetALUSrc(true);
			SetBranch(false);
			SetMemWrite(true);
			SetMemRead(false);
			SetMemToReg(false);
			SetEnd(false);

			SetALUop(0,0);
			break;
		case BRANCH:
			SetRegWrite(false);		
			SetALUSrc(false);
			SetBranch(true);
			SetMemWrite(false);
			SetMemRead(false);
			SetMemToReg(false);
			SetEnd(false);
			SetALUop(0,1);
			break;	
		case END: //end
			SetEnd(true);
			break;
		case NOP: //label, nop
			break;
		default: 
			std::cout<<"unrecognize opcode"<<std::endl;	
			exit(1);
	}
	//write register
	uint32_t writeback = (nowControl.GetRegWrite() ? nowDatapath.GetWriteBackData() : Reg[nowDatapath.GetRd()]);	
	if (nowDatapath.GetRd() == 2 && (writeback > MAXMEMSIZE+MAXSTACKSIZE || writeback < MAXMEMSIZE)){
		std::cout<<"sp_stack overflow."<<std::endl;			
		exit(1);
	}
	Reg[nowDatapath.GetRd()] = writeback;
	//read
	SetRs1Data(Reg[((GetOP() >> 15) & bits5)]);	
	SetRs2Data(Reg[((GetOP() >> 20) & bits5)]);
	SetRd((GetOP() >> 7) & bits5);
	uint32_t constant = ((GetOP() >> 20) & bits12) & ~bits5;
	constant |= (opcode == LOAD || opcode == IMM) ? (GetOP() >> 20) & bits5 : GetRd();
	constant |= (constant & (mask << 11)) ? (bits20 << 12) : 0x0;
	SetConst(constant);	
}

void Execute::Operate(const ControlUnit& nowControl,const DataPath& nowDatapath){
	//ALU operation
	uint32_t A_rd = (nowDatapath.GetOP_A() >> 7) & bits5;
	uint32_t B_rd = (nowDatapath.GetOP_B() >> 7) & bits5;

	uint32_t rs1 = (GetOP() >> 15) & bits5;
	uint32_t input1 = nowControl.testForwardA(rs1,A_rd) ? nowDatapath.GetForwardA() : (nowControl.testForwardB(rs1,B_rd) ? nowDatapath.GetForwardB() : GetRs1Data());
	
	uint32_t rs2 = (GetOP() >> 20) & bits5;
	uint32_t input2 = nowControl.GetALUSrc() ? GetConst() : (nowControl.testForwardA(rs2,A_rd) ? nowDatapath.GetForwardA() : (nowControl.testForwardB(rs2,B_rd) ? nowDatapath.GetForwardB() : GetRs2Data()));

	if (nowControl.GetALUop()[1] == 0 && nowControl.GetALUop()[0] == 0){SetALUresult(input1 + input2);}
	else if (nowControl.GetALUop()[1] == 0 && nowControl.GetALUop()[0] == 1){SetALUresult(input1 - input2);}
	else if (nowControl.GetALUop()[1] == 1 && nowControl.GetALUop()[0] == 0){SetALUresult(input1 & input2);}
	else{SetALUresult(input1 | input2);} 
	//rs2_data to Memory
	SetRs2Data( nowControl.testForwardA(rs2,A_rd) ? nowDatapath.GetForwardA() : (nowControl.testForwardB(rs2,B_rd) ? nowDatapath.GetForwardB() : GetRs2Data() ) );;
	//branch target
	SetBranchTarget(GetPC() + GetConst());
}

bool Memory::Operate(const ControlUnit& nowControl,const DataPath& nowDatapath){
	//branch or not bne,beq	(jal)
	uint32_t funct3 = (GetOP() >> 12) & bits3;
	SetPCSrc(nowControl.GetBranch() && ((funct3 !=0 ) ? (GetALUresult() != 0) : (GetALUresult() == 0)));
	//data memory access
	uint32_t write_data = GetRs2Data();
	if (nowControl.GetMemWrite()){
		for (int i=0;i<4;i++){
			DataMemory[GetALUresult()+i] = write_data >> 8*(3-i) & bits8; 	
		}
	}
	uint32_t read_data = 0;
	if (nowControl.GetMemRead()){
		for (int i=0;i<4;i++){
			read_data |= ((uint32_t) DataMemory[GetALUresult()+i]) << 8*(3-i);
		}
	}
	//predetermine writebackdata
	SetWriteBackData((nowControl.GetMemToReg() ? read_data : GetALUresult()));
	return GetPCSrc();
}

void WriteBack::Operate(const ControlUnit& nowControl,const DataPath& nowDatapath){
}
