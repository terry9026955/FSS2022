// exception.cc 
//	Entry point into the Nachos kernel from user programs.
//	There are two kinds of things that can cause control to
//	transfer back to here from user code:
//
//	syscall -- The user code explicitly requests to call a procedure
//	in the Nachos kernel.  Right now, the only function we support is
//	"Halt".
//
//	exceptions -- The user code does something that the CPU can't handle.
//	For instance, accessing memory that doesn't exist, arithmetic errors,
//	etc.  
//
//	Interrupts (which can also cause control to transfer from user
//	code into the Nachos kernel) are handled elsewhere.
//
// For now, this only handles the Halt() system call.
// Everything else core dumps.
//
// Copyright (c) 1992-1996 The Regents of the University of California.
// All rights reserved.  See copyright.h for copyright notice and limitation 
// of liability and disclaimer of warranty provisions.

#include "copyright.h"
#include "main.h"
#include "syscall.h"
#include "ksyscall.h"
//----------------------------------------------------------------------
// ExceptionHandler
// 	Entry point into the Nachos kernel.  Called when a user program
//	is executing, and either does a syscall, or generates an addressing
//	or arithmetic exception.
//
// 	For system calls, the following is the calling convention:
//
// 	system call code -- r2
//		arg1 -- r4
//		arg2 -- r5
//		arg3 -- r6
//		arg4 -- r7
//
//	The result of the system call, if any, must be put back into r2. 
//
// If you are handling a system call, don't forget to increment the pc
// before returning. (Or else you'll loop making the same system call forever!)
//
//	"which" is the kind of exception.  The list of possible exceptions 
//	is in machine.h.
//----------------------------------------------------------------------

char itoa(int num)
{
	char c;
	switch(num)
	{	
		case 0:
			c = '0';
			break;
		case 1:
			c = '1';
			break;
		case 2:
			c = '2';
			break;
		case 3:
			c = '3';
			break;		
		case 4:
			c = '4';
			break;			
		case 5:
			c = '5';
			break;
		case 6:
			c = '6';
			break;
		case 7:
			c = '7';
			break;
		case 8:
			c = '8';
			break;
		case 9:
			c = '9';
			break;
	}
		return c;
}


void
ExceptionHandler(ExceptionType which)
{
	int type = kernel->machine->ReadRegister(2);
	int val;
	int status, exit, threadID, programID;
	OpenFile* file;
	DEBUG(dbgSys, "Received Exception " << which << " type: " << type << "\n");
	/*int type = kernel->machine->ReadRegister(2);
	int status, exit, threadID, programID, fileID, numChar;
	char ch;
	int val, filename_addr, buf_addr, size_addr, id_addr;
	OpenFileId id;	//change OpenFileId to int
	int size;*/
	//DEBUG(dbgSys, "Received Exception " << which << " type: " << type << "\n");
	//DEBUG(dbgTraCode, "In ExceptionHandler(), Reveived Exception " << witch << "\n");
    switch (which) {
    case SyscallException:
		switch(type) 
		{
		case SC_Open:
			val = kernel->machine->ReadRegister(4);
			{
				char *filename = &(kernel->machine->mainMemory[val]);
				status = OpenForReadWrite(filename, false);
				kernel->machine->WriteRegister(2, (int)status);
			}
			kernel->machine->WriteRegister(PrevPCReg, kernel->machine->ReadRegister(PCReg));
			kernel->machine->WriteRegister(PCReg, kernel->machine->ReadRegister(PCReg) + 4);
			kernel->machine->WriteRegister(NextPCReg, kernel->machine->ReadRegister(PCReg) + 4);
			return;
			ASSERTNOTREACHED();
			break;
			/*DEBUG(dbgSys, "SC_Open, open a file and return OpenFileid. \n");
			filename_addr = kernel->machine->ReadRegister(4);
			{
			char *filename = &(kernel->machine->mainMemory[filename_addr]);
			cout << "Open filename: " << filename << endl;
			id = SysOpen(filename);
			kernel->machine->WriteRegister(2,id);
			}
			kernel->machine->WriteRegister(PrevPCReg, kernel->machine->ReadRegister(PCReg));
			kernel->machine->WriteRegister(PCReg, kernel->machine->ReadRegister(PCReg) + 4);
			kernel->machine->WriteRegister(NextPCReg, kernel->machine->ReadRegister(PCReg) + 4);
			return;
			ASSERTNOTREACHED();
			break;*/
			/*val = kernel->machine->ReadRegister(4);
                        {
                        char *filename = &(kernel->machine->mainMemory[val]);
                        status = SysOpen(filename);
                        kernel->machine->WriteRegister(2, (int) status);//file address
                        }
                        kernel->machine->WriteRegister(PrevPCReg, kernel->machine->ReadRegister(PCReg));
                        kernel->machine->WriteRegister(PCReg, kernel->machine->ReadRegister(PCReg) + 4);
                        kernel->machine->WriteRegister(NextPCReg, kernel->machine->ReadRegister(PCReg)+4);
                        return;
                        ASSERTNOTREACHED();
                        break;*/
		case SC_Read:
			{
			val = kernel->machine->ReadRegister(4);
			char *buffer = &(kernel->machine->mainMemory[val]);
			int size = (int)kernel->machine->ReadRegister(5);
			file = new OpenFile((int)kernel->machine->ReadRegister(6));
			int count = file->Read(buffer, size);
			status = (count == size) ? size : -1;
			kernel->machine->WriteRegister(2, (int)status);
			}
			kernel->machine->WriteRegister(PrevPCReg, kernel->machine->ReadRegister(PCReg));
			kernel->machine->WriteRegister(PCReg, kernel->machine->ReadRegister(PCReg) + 4);
			kernel->machine->WriteRegister(NextPCReg, kernel->machine->ReadRegister(PCReg) + 4);
			return;
			ASSERTNOTREACHED();
			break;
			/*val = kernel->machine->ReadRegister(4);
                        {
                        int s = kernel->machine->ReadRegister(5); //把size從reg5讀取出來
                        int fid = kernel->machine->ReadRegister(6); //把fileID從reg6讀取出來
                        char *buf = &(kernel->machine->mainMemory[val]); //把要存入的buffer的位址取出
                        status = SysRead(fid,buf,s);//將參數傳送給ksyscall
                        kernel->machine->WriteRegister(2, (int) status);//file address
                        }
                        kernel->machine->WriteRegister(PrevPCReg, kernel->machine->ReadRegister(PCReg));
                        kernel->machine->WriteRegister(PCReg, kernel->machine->ReadRegister(PCReg) + 4);
                        kernel->machine->WriteRegister(NextPCReg, kernel->machine->ReadRegister(PCReg)+4);
                        return;
                        ASSERTNOTREACHED();
                        break;
			*/
			/*buf_addr = kernel->machine->ReadRegister(4);
			size = kernel->machine->ReadRegister(5);
			id = kernel->machine->ReadRegister(6);		
			{
			char *buf = &(kernel->machine->mainMemory[buf_addr]);
			cout << "Read " << size << " char from file with id: " << id << endl;
			numChar = SysRead(buf, size, id);
			cout << "return number of char: " << numChar << endl;
			kernel->machine->WriteRegister(2, (int) numChar);
			}
			kernel->machine->WriteRegister(PrevPCReg, kernel->machine->ReadRegister(PCReg));
			kernel->machine->WriteRegister(PCReg, kernel->machine->ReadRegister(PCReg) + 4);
			kernel->machine->WriteRegister(NextPCReg, kernel->machine->ReadRegister(PCReg) + 4);
			return;
			ASSERTNOTREACHED();
			break;*/
		case SC_Write:
			{
			val = kernel->machine->ReadRegister(4);
			char *buffer = &(kernel->machine->mainMemory[val]);
			int size = (int)kernel->machine->ReadRegister(5);
			file = new OpenFile((int)kernel->machine->ReadRegister(6));
			int count = file->WriteAt(buffer, size, file->Length());
			status = (count == size) ? size : -1;
			kernel->machine->WriteRegister(2, (int)status);
			}
			kernel->machine->WriteRegister(PrevPCReg, kernel->machine->ReadRegister(PCReg));
			kernel->machine->WriteRegister(PCReg, kernel->machine->ReadRegister(PCReg) + 4);
			kernel->machine->WriteRegister(NextPCReg, kernel->machine->ReadRegister(PCReg) + 4);
			return;
			ASSERTNOTREACHED();
			break;
			/*DEBUG(dbgSys, "SC_Write, write buf into a file(id), with size of element(each sizeof(char)).\n");
			buf_addr = kernel->machine->ReadRegister(4);
			size = kernel->machine->ReadRegister(5);
			id = kernel->machine->ReadRegister(6);	
			{
			char *buf = &(kernel->machine->mainMemory[buf_addr]);
			cout << "Write " << size << " char into file with id: " << id << endl;
			status = SysWrite(buf, size, id);
			cout << "return number of char: " << status << endl;
			kernel->machine->WriteRegister(2, (int) status);
			}
			kernel->machine->WriteRegister(PrevPCReg, kernel->machine->ReadRegister(PCReg));
			kernel->machine->WriteRegister(PCReg, kernel->machine->ReadRegister(PCReg) + 4);
			kernel->machine->WriteRegister(NextPCReg, kernel->machine->ReadRegister(PCReg) + 4);
			return;
			ASSERTNOTREACHED();
			break;*/
		case SC_Close:
			val = kernel->machine->ReadRegister(4);
			{
			status = (Close(val) == 0) ? 1 : 0;
			kernel->machine->WriteRegister(2, (int)status);
			}
			kernel->machine->WriteRegister(PrevPCReg, kernel->machine->ReadRegister(PCReg));
			kernel->machine->WriteRegister(PCReg, kernel->machine->ReadRegister(PCReg) + 4);
			kernel->machine->WriteRegister(NextPCReg, kernel->machine->ReadRegister(PCReg) + 4);
			return;
			ASSERTNOTREACHED();
			break;
			/*DEBUG(dbgSys, "SC_Close, close a file with OpenFileid.\n");
			id = kernel->machine->ReadRegister(4);
			{
			status = SysClose(id);
			if(status >= 0) status = 1;
			else status = -1;
			kernel->machine->WriteRegister(2, (int) status);
			}
			kernel->machine->WriteRegister(PrevPCReg, kernel->machine->ReadRegister(PCReg));
			kernel->machine->WriteRegister(PCReg, kernel->machine->ReadRegister(PCReg) + 4);
			kernel->machine->WriteRegister(NextPCReg, kernel->machine->ReadRegister(PCReg) + 4);
			return;
			ASSERTNOTREACHED();
			break;*/
		case SC_Halt:
			DEBUG(dbgSys, "Shutdown, initiated by user program.\n");
			SysHalt();
            		cout<<"in exception\n";
			ASSERTNOTREACHED();
			break;
		case SC_MSG:
			DEBUG(dbgSys, "Message received.\n");
			val = kernel->machine->ReadRegister(4);
			{
			char *msg = &(kernel->machine->mainMemory[val]);
			cout << msg << endl;
			}
			SysHalt();
			ASSERTNOTREACHED();
			break;
		case SC_Create:
			val = kernel->machine->ReadRegister(4);
			{
			char *filename = &(kernel->machine->mainMemory[val]);
			//cout << filename << endl;
			status = SysCreate(filename);
			kernel->machine->WriteRegister(2, (int) status);
			}
			kernel->machine->WriteRegister(PrevPCReg, kernel->machine->ReadRegister(PCReg));
			kernel->machine->WriteRegister(PCReg, kernel->machine->ReadRegister(PCReg) + 4);
			kernel->machine->WriteRegister(NextPCReg, kernel->machine->ReadRegister(PCReg)+4);
			return;
			ASSERTNOTREACHED();
            		break;
      		case SC_Add:
			DEBUG(dbgSys, "Add " << kernel->machine->ReadRegister(4) << " + " << kernel->machine->ReadRegister(5) << "\n");
			/* Process SysAdd Systemcall*/
			int result;
			result = SysAdd(/* int op1 */(int)kernel->machine->ReadRegister(4),
			/* int op2 */(int)kernel->machine->ReadRegister(5));
			DEBUG(dbgSys, "Add returning with " << result << "\n");
			/* Prepare Result */
			kernel->machine->WriteRegister(2, (int)result);	
			/* Modify return point */
			{
	  		/* set previous programm counter (debugging only)*/
	  		kernel->machine->WriteRegister(PrevPCReg, kernel->machine->ReadRegister(PCReg));
	  			
			/* set programm counter to next instruction (all Instructions are 4 byte wide)*/
	  		kernel->machine->WriteRegister(PCReg, kernel->machine->ReadRegister(PCReg) + 4);
	  
	 		/* set next programm counter for brach execution */
	 		kernel->machine->WriteRegister(NextPCReg, kernel->machine->ReadRegister(PCReg)+4);
			}
			cout << "result is " << result << "\n";	
			return;	
			ASSERTNOTREACHED();
            		break;
		case SC_PrintInt:
			val = kernel->machine->ReadRegister(4);
			{
				while(val/10>0)
				{
					kernel->synchConsoleOut->PutChar(itoa(val%10));
					val = val/10;
					kernel->synchConsoleOut->PutChar('\n');
				}
				kernel->synchConsoleOut->PutChar(itoa(val%10));
				kernel->synchConsoleOut->PutChar('\n');
			}
			kernel->machine->WriteRegister(PrevPCReg, kernel->machine->ReadRegister(PCReg));
			kernel->machine->WriteRegister(PCReg, kernel->machine->ReadRegister(PCReg)+4);
			kernel->machine->WriteRegister(NextPCReg, kernel->machine->ReadRegister(PCReg)+4);
			return;
			ASSERTNOTREACHED();
			break;
		case SC_Exit:
			DEBUG(dbgAddr, "Program exit\n");
            		val=kernel->machine->ReadRegister(4);
            		cout << "return value:" << val << endl;
			kernel->currentThread->Finish();
            		break;
      		default:
			cerr << "Unexpected system call " << type << "\n";
			break;
		}
		break;
    default:
		cerr << "Unexpected user mode exception " << (int)which << "\n";
		break;
    }
    ASSERTNOTREACHED();
}

