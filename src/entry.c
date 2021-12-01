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
  SIZE_T cls_index = 0;

  while (cls_index < il2cpp_image_get_class_count(assembly->image)) {
    PVOID cls = il2cpp_image_get_class(assembly->image, cls_index);

    if (cls != NULL) {
      PVOID iterator = NULL;
      PMETHOD_INFO method = NULL;

      while ((method = il2cpp_class_get_methods(cls, &iterator)) != 0) {
        if (strcmp(method->method_name, "GetUnlockedSkins") == 0)
          hook(method->method_pointer, (PVOID)&get_unlocked_skins);

        if (strcmp(method->method_name, "GetUnlockedHats") == 0)
          hook(method->method_pointer, (PVOID)&get_unlocked_hats);

        if (strcmp(method->method_name, "GetUnlockedPets") == 0)
          hook(method->method_pointer, (PVOID)&get_unlocked_pets);
      }
    }

    ++cls_index;
  }

  return EXIT_SUCCESS;
}

BOOL APIENTRY DllMain(PVOID instance, DWORD reason, PVOID reserved) {
  if (reason == DLL_PROCESS_ATTACH)
    CreateThread(NULL, 0, (LPTHREAD_START_ROUTINE)startup_thread, NULL, 0, NULL);

  return TRUE;
}