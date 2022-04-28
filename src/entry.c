#include "il2cpp.h"

VOID hook(PVOID target, PVOID detour) {
  DWORD old;
  VirtualProtect(target, 5, PAGE_EXECUTE_READWRITE, &old);
  *(PBYTE)(target) = 0xE9;
  *(PUINT)((PBYTE)target + 1) = ((UINT)detour - (UINT)target - 5);
  VirtualProtect(target, 5, old, &old);
}

BOOLEAN get_purchase(PVOID item, PVOID bundle, PVOID info) {
  return TRUE;
}

DWORD WINAPI startup_thread(VOID) {
  if (!il2cpp_init())
    return EXIT_FAILURE;

  PVOID domain = il2cpp_domain_get();
  PIL2CPP_ASSEMBLY assembly = il2cpp_domain_assembly_open(domain, "Assembly-CSharp");
  PVOID save_manager = il2cpp_class_from_name(assembly->image, "", "SaveManager");

  hook(il2cpp_class_get_method_from_name(save_manager, "GetPurchase", 2)->method_pointer, (PVOID)&get_purchase);

  return EXIT_SUCCESS;
}

BOOL APIENTRY DllMain(PVOID instance, DWORD reason, PVOID reserved) {
  if (reason == DLL_PROCESS_ATTACH)
    CreateThread(NULL, 0, (LPTHREAD_START_ROUTINE)startup_thread, NULL, 0, NULL);

  return TRUE;
}