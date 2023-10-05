HMODULE getKernel32Address()
{
  // gs: [0x30] 指向 TEB
  // gs: [0x60] 指向 PEB
  PVOID64 Peb = GetPe();
  PVOID64 LDR_DATA_Addr = *(PVOID64**)((BYTE*)Peb + 0x18); // 0x18是LDR相对的PEB偏移，存放着LDR的基地址
  UNICODE_STRING* FullName;
  HMODULE hKernel32 = NULL;
  LIST_ENTRY* pNode = NULL;
  PNode = (LIST_ENTRY*)(*(PVOID64**)((BYTE*)LDR_DATA_Addr + 0x30));// 偏移到InInitializationOrderModuleList
  while (true)
  {
    FullName = (UNICODE_STRING*)((BYTE*)pNode + 0x38); // BaseDllName基于InInitializationOrderModuleList的偏移
    if (*(FullName->Buffer + 12) == '\0')
    {
      hKernel32 = (HMODULE)(*(ULONG64*)((BYTE*)pNode + 0x10))); // DllBase
      break;
    }
    pNode = pNode->Flink;
  }
}
