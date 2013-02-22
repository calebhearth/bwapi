#include <cstdio>
#include <string>
#include <windows.h>
#include <vector>

// Typedefs
typedef unsigned __int8  u8 ;
typedef unsigned __int16 u16;
typedef unsigned __int32 u32;
typedef unsigned __int64 u64;

typedef signed   __int8  s8 ;
typedef signed   __int16 s16;
typedef signed   __int32 s32;
typedef signed   __int64 s64;

// Variables
#define __TOSTRING(l) #l
#define _TOSTRING(l) __TOSTRING(l)
#define FILELINE __FILE__ ":" _TOSTRING(__LINE__)

namespace HackUtil
{
  // Imports
  IMAGE_IMPORT_DESCRIPTOR* _GetImportDescriptor(HMODULE module);
  IMAGE_THUNK_DATA32*      _GetImportsList(char* sourceModule, char* importModule);
  DWORD*                   _GetFunctionsList(char* sourceModule, char* importModule);
  /* These functions are not specifically made for public use */

  FARPROC PatchImportOld(char* sourceModule, char* importModule, LPCSTR name, void* patchFunction);

  template <typename T>
  auto PatchImport(char* sourceModule, char* importModule, LPCSTR name, T patchFxn) -> T
  {
    return (T)PatchImportOld(sourceModule, importModule, name, patchFxn);
  }
  template <typename T>
  auto PatchImport(char *importModule, LPCSTR name, T patchFxn) -> T
  {
    return (T)PatchImportOld(nullptr, importModule, name, patchFxn);
  }
  template <typename T>
  auto PatchImport(char* sourceModule, char *importModule, int ordinal, T patchFxn) -> T
  {
    return (T)PatchImportOld(sourceModule, importModule, (LPCSTR)ordinal, patchFxn);
  }
  template <typename T>
  auto PatchImport(char *importModule, int ordinal, T patchFxn) -> T
  {
    return (T)PatchImportOld(nullptr, importModule, (LPCSTR)ordinal, patchFxn);
  }

  /* Creates a detour for the specified import function in any loaded module */

  FARPROC GetImport(char* importModule, LPCSTR name);
  FARPROC GetImport(char* importModule, int ordinal);
  /* Retrieves the address of the imported function from the specified module */

  // Memory
  void WriteNops(void* dest, u32 size);
  void WriteNops(u32 dest, u32 size);
  /* Writes NOPs to the specified destination */

  void WriteMem(void* dest, void* source, u32 size);
  void WriteMem(u32 dest, void* source, u32 size);
  /* Writes data to the specified destination from the source */

  void JmpPatch(void* dest, void* patch);
  void JmpPatch(u32 dest, void* patch);
  /* Writes a jump to the specified patch at the destination */

  void CallPatch(void* dest, void* patch);
  void CallPatch(u32 dest, void* patch);
  /* Writes a call to the specified patch at the destination */

  void Revert(const char *logline);
  /* Reverts all changes made with any function except for WriteMemRaw */

  void WriteMemRaw(void* dest, void* source, u32 size);
  void WriteMemRaw(u32 dest, void* source, u32 size);
  /* Writes to memory and does not include the change in the changes list */
};
