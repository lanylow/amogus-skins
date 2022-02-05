#include "il2cpp.h"

VOID hook(PVOID target, PVOID detour) {
  DWORD old;
  VirtualProtect(target, 5, PAGE_EXECUTE_READWRITE, &old);
  *(PBYTE)(target) = 0xE9;
  *(PUINT)((PBYTE)target + 1) = ((UINT)detour - (UINT)target - 5);
  VirtualProtect(target, 5, old, &old);
}

PVOID get_unlocked_skins(PHAT_MANAGER _this, PVOID info) {
  return _this->all_skins->array;
}

PVOID get_unlocked_hats(PHAT_MANAGER _this, PVOID info) {
  return _this->all_hats->array;
}

PVOID get_unlocked_pets(PHAT_MANAGER _this, PVOID info) {
  return _this->all_pets->array;
}

DWORD WINAPI startup_thread(VOID) {
  if (!il2cpp_init())
    return EXIT_FAILURE;

  PVOID domain = il2cpp_domain_get();
  PIL2CPP_ASSEMBLY assembly = il2cpp_domain_assembly_open(domain, "Assembly-CSharp");
  PVOID hat_manager = il2cpp_class_from_name(assembly->image, "", "HatManager");

  hook(il2cpp_class_get_method_from_name(hat_manager, "GetUnlockedSkins", 0)->method_pointer, (PVOID)&get_unlocked_skins);
  hook(il2cpp_class_get_method_from_name(hat_manager, "GetUnlockedHats", 0)->method_pointer, (PVOID)&get_unlocked_hats);
  hook(il2cpp_class_get_method_from_name(hat_manager, "GetUnlockedPets", 0)->method_pointer, (PVOID)&get_unlocked_pets);

  return EXIT_SUCCESS;
}

BOOL APIENTRY DllMain(PVOID instance, DWORD reason, PVOID reserved) {
  if (reason == DLL_PROCESS_ATTACH)
    CreateThread(NULL, 0, (LPTHREAD_START_ROUTINE)startup_thread, NULL, 0, NULL);

  return TRUE;
}