#ifndef STAGE
#define STAGE
#include <iostream>
#include <fstream>
#include <unordered_map>
#include "data.h"
#include "control.h"
#include "datapath.h"
const uint32_t MAXMEMSIZE(0x10000);
const uint32_t MAXSTACKSIZE(0x1000);
const uint32_t mask(0x1);
inline void binary(uint32_t number,size_t bits){
	for (int i=bits-1;i>=0;i--){
		std::cout<<( ((mask << i) & number) !=0 ? '1' : '0');
	}
}
class InstructionFetch : public Data, public ControlData
{
	public:
		InstructionFetch(char* filename):Data(),ControlData(){
			std::ifstream inFile(filename,std::ios::in);
			if (!inFile){
				std::cout<<"failed to open the file."<<std::endl;
				exit(1);
			}
			uint32_t pc = 0x0;
			std::string line;
			while (getline(inFile,line)){
				for (int i=0;i<4;i++){
					InstructionMemory[pc + i] = 0x0;
					for (int j=0;j<8;j++){
						InstructionMemory[pc + i] |= line[8*i+j] == '1' ? mask << (7-j) : 0x0; 	
					}
				}
				pc +=4;
			}
			for (;pc<MAXMEMSIZE;pc++){
				InstructionMemory[pc] = 0x0;
			}
		}
		void Check(){
			for (uint32_t i=0;i<MAXMEMSIZE;i++){
				std::cout<<"address: ";
				binary(i,32);
				std::cout<<" InstructionMemory[address]: ";
				binary(InstructionMemory[i],8);
				std::cout<<std::endl;
			}
		}
		uint32_t Operate(const uint32_t& nowPC,const ControlUnit& nowControl,const DataPath& nowDatapath);
	private:
		//Instruction memory
		std::unordered_map<uint32_t,uint8_t> InstructionMemory;
};
class InstructionDecode : public Data, public ControlData
{
	public:
		InstructionDecode():Data(),ControlData(){
			for (uint32_t ind = 0; ind < 32;ind++){
				Reg[ind] = 0x0;
			}
			//sp is x2
			Reg[2] = MAXMEMSIZE+MAXSTACKSIZE;
		}
		void Check(){
			for (uint32_t i=0;i<32;i++){
				std::cout<<"index: "<< i << " Reg[index]: ";
				binary(Reg[i],32);
				std::cout<<std::endl;
			}
		}
		void Check(std::string regname){
			if (regname.length()<2 || regname.length()>3 || regname[0]!='x'){
				std::cout<<"invalid argument: register name"<<std::endl;
				exit(1);
			}
			int32_t index = std::stoi(regname.substr(1,regname.length()-1));
			if (index < 0 || index > 31){
				std::cout<<"wrong register number"<<std::endl;
				exit(1);
			}
			std::cout<<" binary: ";
			binary(Reg[index],32);
			std::cout<<"  decimal(signed): "<<(int32_t) Reg[index]<<std::endl;
		}
		void Operate(const ControlUnit& nowControl,const DataPath& nowDatapath);
	private:
		//register file
		std::unordered_map<uint32_t,uint32_t> Reg;
};
class Execute : public Data, public ControlData
{
	public:
		Execute():Data(),ControlData(){;}	
		void Operate(const ControlUnit& nowControl,const DataPath& nowDatapath);
};
class Memory : public Data, public ControlData
{
	public:
		Memory():Data(),ControlData(){
			//memory and stack-memory
			for (uint32_t addr=0;addr<MAXMEMSIZE+MAXSTACKSIZE;addr++){
				DataMemory[addr] = 0x0;	
			}	
		}	
		void Check(){
			for (uint32_t i=0;i<MAXMEMSIZE;i++){
				std::cout<<"address: ";
				binary(i,32);
				std::cout<<" DataMemory[address]: ";
				binary(DataMemory[i],8);
				std::cout<<std::endl;
			}
		}
		bool Operate(const ControlUnit& nowControl,const DataPath& nowDatapath);
	private:
		std::unordered_map<uint32_t,uint8_t> DataMemory;
};
class WriteBack : public Data, public ControlData
{
	public:
		WriteBack():Data(),ControlData(){;}
		void Operate(const ControlUnit& nowControl,const DataPath& nowDatapath);
};






#endif
