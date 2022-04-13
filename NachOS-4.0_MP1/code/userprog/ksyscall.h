/**************************************************************
 *
 * userprog/ksyscall.h
 *
 * Kernel interface for systemcalls 
 *
 * by Marcus Voelp  (c) Universitaet Karlsruhe
 *
 **************************************************************/

#ifndef __USERPROG_KSYSCALL_H__ 
#define __USERPROG_KSYSCALL_H__ 

#include "kernel.h"

#include "synchconsole.h"


void SysHalt()
{
  kernel->interrupt->Halt();
}

int SysAdd(int op1, int op2)
{
  return op1 + op2;
}

int SysCreate(char *filename)
{
	// return value
	// 1: success
	// 0: failed
	return kernel->interrupt->CreateFile(filename);
}

/*int SysOpen(char *filename)
{
        // return value
        // negtive: failed
        // larger than 0: openfilw object address
        return (int)&(kernel->interrupt->OpenFile(filename));
}*/


OpenFileId SysOpen(char *name)//change OpenFileId to int
{
	return kernel->fileSystem->OpenAFile(name);
}

int SysWrite(char *buffer, int size, OpenFileId id)//change OpenFileId to int
{
	return kernel->fileSystem->Write(buffer, size, id);
}


int SysRead(char *buffer, int size, OpenFileId id)//change OpenFileId to int
{
	return kernel->fileSystem->ReadFile(buffer, size, id);
}


int SysClose(OpenFileId id)//change OpenFileId to int
{
	return kernel->fileSystem->CloseFile(id);
}



#endif /* ! __USERPROG_KSYSCALL_H__ */
