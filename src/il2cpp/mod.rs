use winapi::{
  ctypes::*,
  shared::minwindef::*,
  um::libloaderapi::*
};

#[repr(C)]
pub struct il2cpp_assembly {
  pub image: *const c_void
}

#[repr(C)]
pub struct method_info {
  pub method_pointer: *const c_void
}

fn get_module_handle(name: *const u8) -> HMODULE {
  unsafe { return GetModuleHandleA(name as _) }
}

fn get_proc_address(module: HMODULE, name: *const u8) -> FARPROC {
  unsafe { return GetProcAddress(module, name as _) }
}

fn get_il2cpp_method(name: *const u8) -> FARPROC {
  return get_proc_address(get_module_handle("GameAssembly.dll\0".as_ptr()), name);
}

type Il2cppDomainGetT = unsafe extern "cdecl" fn() -> *const c_void;
pub fn domain_get() -> *const c_void {
  unsafe { return std::mem::transmute::<_, Il2cppDomainGetT>(get_il2cpp_method("il2cpp_domain_get\0".as_ptr()))(); }
}

type Il2cppDomainAssemblyOpenT = unsafe extern "cdecl" fn(*const c_void, *const u8) -> *const il2cpp_assembly;
pub fn domain_assembly_open<'a>(domain: *const c_void, name: *const u8) -> &'a il2cpp_assembly {
  unsafe { return &*std::mem::transmute::<_, Il2cppDomainAssemblyOpenT>(get_il2cpp_method("il2cpp_domain_assembly_open\0".as_ptr()))(domain, name); }
}

type Il2cppClassFromNameT = unsafe extern "cdecl" fn(*const c_void, *const u8, *const u8) -> *const c_void;
pub fn class_from_name(image: *const c_void, namespace: *const u8, name: *const u8) -> *const c_void {
  unsafe { return std::mem::transmute::<_, Il2cppClassFromNameT>(get_il2cpp_method("il2cpp_class_from_name\0".as_ptr()))(image, namespace, name); }
}

type Il2cppClassGetMethodFromNameT = unsafe extern "cdecl" fn(*const c_void, *const u8, c_int) -> *const method_info;
pub fn class_get_method_from_name<'a>(class: *const c_void, name: *const u8, arg_count: c_int) -> &'a method_info {
  unsafe { return &*std::mem::transmute::<_, Il2cppClassGetMethodFromNameT>(get_il2cpp_method("il2cpp_class_get_method_from_name\0".as_ptr()))(class, name, arg_count); }
}