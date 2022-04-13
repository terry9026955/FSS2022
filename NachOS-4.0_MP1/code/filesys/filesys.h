// filesys.h 
//	Data structures to represent the Nachos file system.
//
//	A file system is a set of files stored on disk, organized
//	into directories.  Operations on the file system have to
//	do with "naming" -- creating, opening, and deleting files,
//	given a textual file name.  Operations on an individual
//	"open" file (read, write, close) are to be found in the OpenFile
//	class (openfile.h).
//
//	We define two separate implementations of the file system. 
//	The "STUB" version just re-defines the Nachos file system 
//	operations as operations on the native UNIX file system on the machine
//	running the Nachos simulation.
//
//	The other version is a "real" file system, built on top of 
//	a disk simulator.  The disk is simulated using the native UNIX 
//	file system (in a file named "DISK"). 
//
//	In the "real" implementation, there are two key data structures used 
//	in the file system.  There is a single "root" directory, listing
//	all of the files in the file system; unlike UNIX, the baseline
//	system does not provide a hierarchical directory structure.  
//	In addition, there is a bitmap for allocating
//	disk sectors.  Both the root directory and the bitmap are themselves
//	stored as files in the Nachos file system -- this causes an interesting
//	bootstrap problem when the simulated disk is initialized. 
//
// Copyright (c) 1992-1993 The Regents of the University of California.
// All rights reserved.  See copyright.h for copyright notice and limitation 
// of liability and disclaimer of warranty provisions.

#ifndef FS_H
#define FS_H

#include "copyright.h"
#include "sysdep.h"
#include "openfile.h"




#ifdef FILESYS_STUB 		// Temporarily implement file system calls as 
				// calls to UNIX, until the real file system
				// implementation is available
class FileSystem {
  public:
    FileSystem() { 
		for (int i = 0; i < 20; i++){
			fileDescriptorTable[i] = NULL; 
		}
	}

    bool Create(char *name) {
	int fileDescriptor = OpenForWrite(name);

	if (fileDescriptor == -1) return FALSE;
	Close(fileDescriptor); 
	return TRUE; 
	}

    OpenFile* Open(char *name) {
	//int Open(char *name) {
	  int fileDescriptor = OpenForReadWrite(name, FALSE);

	  if (fileDescriptor == -1) return NULL;
	  OpenFile* f = new OpenFile(fileDescriptor);
      fileDescriptorTable[fileDescriptor]=f; //將file pointer放入table中
      //return fileDescriptor;
	  return new OpenFile(fileDescriptor);
      }
	
	int Read(int fd, char* buf, int size){
       if(fd<0 || fd>19) //檢查是否有超過filedescriptorteble的界線
              return FALSE;
       OpenFile* fp =  fileDescriptorTable[fd];//取得table中的object
       if(fp==NULL)
              return FALSE;
       else
              return fp->Read(buf,size);//對object進行操作
    }
	

	

	
	//implementation put in here
	//Open!
	int OpenAFile(char *name) {	//OpenFileId change to int
        //open file
		for(int i = 0; i < 20; i++){
			if(fileDescriptorTable[i] == NULL){//can use
				int fileDescriptor = OpenForReadWrite(name, FALSE);
				cout << "OpenAFile fileDescriptor: " << fileDescriptor << endl;
				if(fileDescriptor == -1) return -1;
				id_to_fd[i] = fileDescriptor;

				OpenFile* openfile = new OpenFile(fileDescriptor);
				fileDescriptorTable[i] = openfile;
				return i;		
			}
		}				
		return -1;	
	}
	//Open!

	//Write!
	int Write(char *buffer, int size, int id){//OpenFileId change to int
		if(fileDescriptorTable[id] == NULL) return -1;
		int num_bytes = fileDescriptorTable[id]->Write(buffer, size*sizeof(char));
		return num_bytes/sizeof(char);	
	}
	//Write!

	//ReadFile!
	int ReadFile(char *buffer, int size, int id){//OpenFileId change to int
		if(fileDescriptorTable[id] == NULL) return -1;
		int num_bytes = fileDescriptorTable[id]->Read(buffer, size*sizeof(char));
		return num_bytes/sizeof(char);
	}
	//ReadFile!

	//CloseFile!
	int CloseFile(int id){//OpenFileId change to int
		if(fileDescriptorTable[id] == NULL) return -1;
		fileDescriptorTable[id] = NULL;
		int retval = Close(id_to_fd[id]);
		return retval;
	}
	//CloseFile!

	int id_to_fd[20];	//OpenFileId counter
	int OpenFileTable[20];	

	//implementation put in here
	

    bool Remove(char *name) { return Unlink(name) == 0; }

	OpenFile *fileDescriptorTable[20];
	
};

#else // FILESYS
class FileSystem {
  public:
    FileSystem(bool format);		// Initialize the file system.
					// Must be called *after* "synchDisk" 
					// has been initialized.
    					// If "format", there is nothing on
					// the disk, so initialize the directory
    					// and the bitmap of free blocks.

    bool Create(char *name, int initialSize);  	
					// Create a file (UNIX creat)

    OpenFile* Open(char *name); 	// Open a file (UNIX open)

    bool Remove(char *name);  		// Delete a file (UNIX unlink)

    void List();			// List all the files in the file system

    void Print();			// List all the files and their contents

  private:
   OpenFile* freeMapFile;		// Bit map of free disk blocks,
					// represented as a file
   OpenFile* directoryFile;		// "Root" directory -- list of 
					// file names, represented as a file
};







#endif // FILESYS

#endif // FS_H
