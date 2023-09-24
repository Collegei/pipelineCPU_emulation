#ifndef DATA
#define DATA
#include <iostream>
class Data{
	public:
		//get
		uint32_t GetPC() const{return PC;}
		uint32_t GetBranchTarget() const{return BranchTarget;}
		uint32_t GetOP() const{return OP;}
		uint32_t GetConst() const{return constant;}
		uint32_t GetRs1Data() const{return rs1_data;}
		uint32_t GetRs2Data() const{return rs2_data;}
		uint32_t GetRd() const{return rd;}
		uint32_t GetWriteBackData() const {return writebackdata;}
		uint32_t GetALUresult() const {return ALUresult;}
		void PassData(const Data* other){
			PC = other->GetPC();
			BranchTarget = other->GetBranchTarget();
			OP = other->GetOP();
			constant = other->GetConst();
			rs1_data = other->GetRs1Data();
			rs2_data = other->GetRs2Data();
			rd = other->GetRd();
			writebackdata = other->GetWriteBackData();
			ALUresult = other->GetALUresult();
		}
		//clean
		void CleanData(void){
			SetPC(0);
			SetBranchTarget(0);
			SetOP(0);
			SetConst(0);
			SetRs1Data(0);
			SetRs2Data(0);
			SetRd(0);	
			SetWriteBackData(0);
			SetALUresult(0);
		}

	protected:
		//set
		void SetPC(uint32_t x){PC = x;}
		void SetBranchTarget(uint32_t x){BranchTarget = x;}
		void SetOP(uint32_t x){OP = x;}
		void SetConst(uint32_t x){constant = x;}
		void SetRs1Data(uint32_t x){rs1_data = x;}
		void SetRs2Data(uint32_t x){rs2_data = x;}
		void SetRd(uint32_t x ){rd = x;}
		void SetWriteBackData(uint32_t x){writebackdata = x;}
		void SetALUresult(uint32_t x){ALUresult = x;}
	private:
		uint32_t PC;
		uint32_t BranchTarget;
		uint32_t OP;
		uint32_t constant;
		uint32_t rs1_data;
		uint32_t rs2_data;
		uint32_t rd;
		uint32_t writebackdata;
		uint32_t ALUresult;
};
class ControlData{
	public:
		void PassControlData(const ControlData* other){
			PCSrc = other->GetPCSrc();
			RegWrite = other->GetRegWrite();
			ALUSrc = other->GetALUSrc();
			ALUop[0] = other->GetALUop()[0];
			ALUop[1] = other->GetALUop()[1];
			Branch = other->GetBranch();
			MemWrite = other->GetMemWrite();
			MemRead = other->GetMemRead();
			MemToReg = other->GetMemToReg();
			End = other->GetEnd();
		}
		//get
		bool GetPCSrc() const{return PCSrc;}
		bool GetRegWrite() const{return RegWrite;}
		bool GetALUSrc() const{return ALUSrc;}
		const bool* GetALUop() const{return ALUop;}
		bool GetBranch() const{return Branch;}
		bool GetMemWrite() const{return MemWrite;}
		bool GetMemRead() const{return MemRead;}
		bool GetMemToReg() const{return MemToReg;}
		bool GetEnd() const{return End;}
		//clean
		void CleanControlData(void){
			SetPCSrc(0);
			SetRegWrite(0);
			SetALUSrc(0);
			SetALUop(0,0);
			SetBranch(0);
			SetMemWrite(0);
			SetMemRead(0);
			SetMemToReg(0); 
			SetEnd(0);
		}
	protected:
		//set 
		void SetPCSrc(bool x) {PCSrc = x;}
		void SetRegWrite(bool x) { RegWrite = x;}
		void SetALUSrc(bool x) {ALUSrc = x;}
		void SetALUop(bool x,bool y) {ALUop[1] = x,ALUop[0] = y;}
		void SetBranch(bool x) {Branch = x;}
		void SetMemWrite(bool x) {MemWrite = x;}
		void SetMemRead(bool x) {MemRead = x;}
		void SetMemToReg(bool x) {MemToReg = x;}
		void SetEnd(bool x){End = x;}
	private:
		bool PCSrc;
		bool RegWrite;
		bool ALUSrc;
		bool ALUop[2]; //+:00  -:01  &:10  |:11
		bool Branch;
		bool MemWrite;
		bool MemRead;
		bool MemToReg;	
		bool End;
};
#endif
