#[macro_use]
extern crate detour;

use winapi::{
  ctypes::*,
  shared::minwindef::*,
  um::{
    libloaderapi::*,
    processthreadsapi::*,
    winnt::*,
  }
};

mod il2cpp;

static_detour! {
  static get_purchase_hook_def: unsafe extern "cdecl" fn(*const c_void , *const c_void, *const c_void) -> bool;
}

type GetPurchaseT = unsafe extern "cdecl" fn(*const c_void , *const c_void, *const c_void) -> bool;
fn get_purchase_hook(_item: *const c_void , _bundle: *const c_void, _info: *const c_void) -> bool {
  return true;
}

unsafe extern "system" fn init_thread(_instance: LPVOID) -> DWORD {
  let domain = il2cpp::domain_get();
  let assembly = il2cpp::domain_assembly_open(domain, "Assembly-CSharp\0".as_ptr());
  let save_manager = il2cpp::class_from_name((*assembly).image, "\0".as_ptr(), "SaveManager\0".as_ptr());
  let get_purchase = std::mem::transmute::<_, GetPurchaseT>((*il2cpp::class_get_method_from_name(save_manager, "GetPurchase\0".as_ptr(), 2)).method_pointer);

  get_purchase_hook_def.initialize(get_purchase, get_purchase_hook).unwrap().enable().unwrap();

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