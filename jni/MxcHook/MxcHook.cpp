#include "MxcHook.h"

void *handler = dlopen("libil2cpp.so", RTLD_LAZY | RTLD_GLOBAL | RTLD_NOW);

void *GetModuleBaseAddr(pid_t pid, char* pszModuleName)
{
    FILE *pFileMaps = NULL;
    unsigned long ulBaseValue = 0;
    char szMapFilePath[256] = {0};
    char szFileLineBuffer[1024] = {0};

    if (pid < 0)
    {
        snprintf(szMapFilePath, sizeof(szMapFilePath), "/proc/self/maps");
    }
    else
    {
        snprintf(szMapFilePath, sizeof(szMapFilePath),  "/proc/%d/maps", pid);
    }

    pFileMaps = fopen(szMapFilePath, "r");
    if (NULL == pFileMaps)
    {
        return (void *)ulBaseValue;
    }

    while (fgets(szFileLineBuffer, sizeof(szFileLineBuffer), pFileMaps) != NULL)
    {
        if (strstr(szFileLineBuffer, pszModuleName))
        {
            char *pszModuleAddress = strtok(szFileLineBuffer, "-");
            if (pszModuleAddress)
            {
                ulBaseValue = strtoul(pszModuleAddress, NULL, 16);

                if (ulBaseValue == 0x8000)
                    ulBaseValue = 0;

                break;
            }
        }
    }
    fclose(pFileMaps);
    return (void *)ulBaseValue;
}

double newHurt()
{
    char *nmsl = "nmsl";
    return 0;
}

JNIEXPORT jint JNI_OnLoad(JavaVM *vm, void *reserved) 
{
    
    void *pModuleBaseAddr = GetModuleBaseAddr(-1, "libil2cpp.so");
    void *preHurt = (void *)((uint64_t)pModuleBaseAddr + 0x1CE3818);
    InlineHook(preHurt, (void *)&newHurt);
    return JNI_VERSION_1_6;
}