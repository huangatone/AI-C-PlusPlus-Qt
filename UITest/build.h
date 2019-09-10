#ifndef BUILD_H
#define BUILD_H
#include <string>
using namespace  std;
class Build
{
public:
	Build();
};


class Computer {

private:
	/*CPU*/
	 string CPU;
	/*内存*/
	 string memory;
	/*硬盘*/
	 string hardDisk;
	/*键盘*/
	 string keyboard;
	/*鼠标*/
	 string mouse;
public:
	 string getCPU() {
		return CPU;
	}

	 void setCPU(string cpu) {
		CPU = cpu;
	}

	 string getMemory() {
		return memory;
	}

	 void setMemory(string m) {
		memory = m;
	}

	 string getHardDisk() {
		return hardDisk;
	}

	 void setHardDisk(string hd) {
		hardDisk = hd;
	}

	 string getKeyboard() {
		return keyboard;
	}

	 void setKeyboard(string kb) {
		keyboard = kb;
	}

	 string getMouse() {
		return mouse;
	}

	 void setMouse(string ms) {
		mouse = ms;
	}


	 string tostring() {
		return string("Computer {CPU='") + CPU + '\'' +
				", memory='" + memory + '\'' +
				", hardDisk='" + hardDisk + '\'' +
				", keyboard='" + keyboard + '\'' +
				", mouse='" + mouse + '\'' +
				'}';
	}
};

class  ComputerConfigBuilder
 {
 public:
	 ComputerConfigBuilder();
	 virtual ~ComputerConfigBuilder()
	 {

	 }
	 void setCPU();
	 void setMemery();
	 void setHardDisk();
	 void setKeyboard();
	 void setMouse();
	 Computer getComputer();
 };


class  LowComputeBuilder: public ComputerConfigBuilder
 {
	Computer mComputer;
 public:
	 LowComputeBuilder();
	 virtual ~LowComputeBuilder()
	 {

	 }
	 void setCPU()
	 {
		 mComputer.setCPU("i3");
	 }
	 void setMemery();
	 void setHardDisk();
	 void setKeyboard();
	 void setMouse();
	 Computer getComputer();
 };


class  HiComputeBuilder: public ComputerConfigBuilder
 {
	Computer mComputer;
 public:
	 HiComputeBuilder();
	 virtual ~HiComputeBuilder()
	 {

	 }
	 void setCPU()
	 {
		 mComputer.setCPU("i7");
	 }
	 void setMemery();
	 void setHardDisk();
	 void setKeyboard();
	 void setMouse();
	 Computer getComputer();
 };

class Director {
private:
	ComputerConfigBuilder mBuilder;
public :
	void setBuilder(ComputerConfigBuilder builder){
		mBuilder = builder;
	}
	 void createComputer(){
		mBuilder.setCPU();
		mBuilder.setMemery();
		mBuilder.setHardDisk();
		mBuilder.setKeyboard();
		mBuilder.setMouse();
	}
	 Computer getComputer(){
		return mBuilder.getComputer();
	}
};

#endif // BUILD_H
