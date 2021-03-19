#ifndef ISHAREDMEMORYADAPTOR_H
#define ISHAREDMEMORYADAPTOR_H

#include <Windows.h>
#include <string>


namespace Common
{
namespace Shared_Memory
{

const DWORD CreateMapAccess_readOnly = 0x02;
const DWORD CreateMapAccess_readWrite = 0x04;
const DWORD CreateMapAccess_writeCopy = 0x08;

enum SharedMemoryAccess
{
    SharedMemoryAccess_write,
    SharedMemoryAccess_read,
    SharedMemoryAccess_all
};


class iSharedMemoryAdaptor
{
public:

    virtual ~iSharedMemoryAdaptor() {}

    virtual HANDLE Create( DWORD access
                         , unsigned long size
                         , std::string mapName ) = 0;

    virtual HANDLE OpenSharedMemory( SharedMemoryAccess access
                                   , unsigned long size
                                   , std::string mapName ) = 0;

    virtual LPCTSTR MapSharedMemory( HANDLE mappedObject 
                                   , SharedMemoryAccess access
                                   , unsigned long size ) = 0;

    virtual void UnmapShareView( LPCTSTR memoryBuffer ) = 0;

    virtual void CloseSharedMemory( HANDLE mapFile ) = 0;
};

}
}

#endif
