use winapi::{
  shared::minwindef::*,
  um::{
    libloaderapi::*,
    processthreadsapi::*,
    winnt::*,
    memoryapi::*
  }
};

mod il2cpp;

unsafe extern "system" fn init_thread(_instance: LPVOID) -> DWORD {
  let domain = il2cpp::domain_get();
  let assembly = il2cpp::domain_assembly_open(domain, "Assembly-CSharp\0".as_ptr());
  let save_manager = il2cpp::class_from_name(assembly.image, "\0".as_ptr(), "SaveManager\0".as_ptr());
  let get_purchase = il2cpp::class_get_method_from_name(save_manager, "GetPurchase\0".as_ptr(), 2).method_pointer;

  /* Our patch:
   *  mov al, 1
   *  retn
   */

  let mut old_protection: DWORD = 0;
  VirtualProtect(get_purchase as _, 3, PAGE_EXECUTE_READWRITE, &mut old_protection);
  std::ptr::copy(b"\xB0\x01\xC3", get_purchase as _, 3);
  VirtualProtect(get_purchase as _, 3, old_protection, &mut old_protection);

  return 0;
}

#[allow(non_snake_case)]
#[no_mangle]
pub unsafe extern "system" fn DllMain(instance: HINSTANCE, reason: DWORD, _reserved: LPVOID) -> BOOL {
  DisableThreadLibraryCalls(instance);

  if reason == DLL_PROCESS_ATTACH {
    CreateThread(std::ptr::null_mut(), 0, Some(init_thread), instance as LPVOID, 0, std::ptr::null_mut());
  }

  return TRUE;
}