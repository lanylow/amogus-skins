#pragma once

#include <windows.h>

typedef struct _METHOD_INFO {
  PVOID method_pointer;
  PVOID invoker_method;
  PCSTR method_name;
} METHOD_INFO, *PMETHOD_INFO;

typedef struct _IL2CPP_ASSEMBLY {
  PVOID image;
} IL2CPP_ASSEMBLY, *PIL2CPP_ASSEMBLY;

typedef struct _LIST {
  BYTE pad[8];
  PVOID array;
} LIST, *PLIST;

typedef struct _HAT_MANAGER {
  BYTE pad[24];
  PLIST all_pets;
  PLIST all_hats;
  PLIST all_skins;
} HAT_MANAGER, *PHAT_MANAGER;

#define DECLARE_FUNC(ret, name, args) \
	typedef ret (*tmp_##name) args; \
	tmp_##name name;

DECLARE_FUNC(PVOID, il2cpp_domain_get, ())
DECLARE_FUNC(PIL2CPP_ASSEMBLY, il2cpp_domain_assembly_open, (PVOID domain, PCSTR name))
DECLARE_FUNC(SIZE_T, il2cpp_image_get_class_count, (PVOID image))
DECLARE_FUNC(PVOID, il2cpp_image_get_class, (PVOID image, SIZE_T index))
DECLARE_FUNC(PMETHOD_INFO, il2cpp_class_get_methods, (PVOID cls, PVOID *iterator))

BOOL il2cpp_init(VOID) {
  HMODULE game_module = GetModuleHandleA("GameAssembly.dll");

	if (!game_module)
		return FALSE;

	#define INIT_FUNC(name) \
		name = (tmp_##name)GetProcAddress(game_module, #name);

	INIT_FUNC(il2cpp_domain_get)
  INIT_FUNC(il2cpp_domain_assembly_open)
  INIT_FUNC(il2cpp_image_get_class_count)
  INIT_FUNC(il2cpp_image_get_class)
  INIT_FUNC(il2cpp_class_get_methods)

	return TRUE;
}