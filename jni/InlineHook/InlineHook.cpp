#include "InlineHook.hpp"

bool ChangePageProperty(void *pAddress, size_t size)
{
    bool bRet = false;
    
    if(pAddress == NULL)
    {
        LOGE("change page property error.");
        return bRet;
    }
    
    unsigned long ulPageSize = sysconf(_SC_PAGESIZE);
    int iProtect = PROT_READ | PROT_WRITE | PROT_EXEC;
    unsigned long ulNewPageStartAddress = (unsigned long)(pAddress) & ~(ulPageSize - 1); //pAddress & 0x1111 0000 0000 0000
    long lPageCount = (size / ulPageSize) + 1;
    
    long l = 0;
    while(l < lPageCount)
    {
        int iRet = mprotect((void *)(ulNewPageStartAddress), ulPageSize, iProtect);
        if(-1 == iRet)
        {
            LOGE("mprotect error:%s", strerror(errno));
            return bRet;
        }
        l++; 
    }
    
    return true;
}

void InlineHook(void *symbol, void *replace) {
    ChangePageProperty(symbol, 28);
    LOGE("inline hook %p successfully!", symbol)
    unsigned char szLdrPCOpcodes[28] = {0xe1, 0x03, 0x3f, 0xa9, 0x40, 0x00, 0x00, 0x58, 0x00, 0x00, 0x1f, 0xd6};
    memcpy(szLdrPCOpcodes + 12, &replace, 8);
    szLdrPCOpcodes[20] = 0xE0;
    szLdrPCOpcodes[21] = 0x83;
    szLdrPCOpcodes[22] = 0x5F;
    szLdrPCOpcodes[23] = 0xF8;
    szLdrPCOpcodes[24] = 0xC0;
    szLdrPCOpcodes[25] = 0x03;
    szLdrPCOpcodes[26] = 0x5F;
    szLdrPCOpcodes[27] = 0xD6;
    
    
    memcpy(symbol, szLdrPCOpcodes, 28);
    //LOGE("%s", (*((char *(*)())(symbol)))());
}
