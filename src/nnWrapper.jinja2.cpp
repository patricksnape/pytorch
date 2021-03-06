// {{header1}}
// {{header2}}

extern "C" {
    #include "lua.h"
    #include "lauxlib.h"
    #include "lualib.h"
}

#ifndef _WIN32
    #include <dlfcn.h>
#endif

#include <iostream>
#include <stdexcept>

#include "luaT.h"
#include "THTensor.h"
#include "THStorage.h"
#include "LuaHelper.h"
#include "nnWrapper.h"

using namespace std;

{% set types = [
    {'Real': 'Long','real': 'long'},
    {'Real': 'Float', 'real': 'float'},
    {'Real': 'Double', 'real': 'double'},
    {'Real': 'Byte', 'real': 'unsigned char'}
]
%}

lua_State *luaInit() {
    #ifndef _WIN32
//    cout << "luaInit" << endl;
    void *hdl = dlopen("libPyTorchLua.so", RTLD_NOW | RTLD_GLOBAL);
    if(hdl == 0) {
        cout << "Failed to load libPyTorchLua.so, trying dylib..." << endl;
        hdl = dlopen("libPyTorchLua.dylib", RTLD_NOW | RTLD_GLOBAL);
    }
    if(hdl == 0) {
        cout << "Failed to load libPyTorchLua.dylib, fatal" << endl;
        cout << dlerror() << endl;
        throw runtime_error(string("Couldnt load liblua5.1.so or liblua.dylib") + dlerror());
    } else {
////        cout << "loaded lua library" << endl;
    }
    void *err = dlopen("/home/ubuntu/torch/install/lib/lua/5.1/libpaths.so", RTLD_NOW | RTLD_GLOBAL);
//    cout << "err " << (long)err << endl;

    #endif

    lua_State *L = luaL_newstate();
    luaL_openlibs(L);

    lua_getglobal(L, "require");
    lua_pushstring(L, "torch");
    lua_call(L, 1, 0);
//    lua_setglobal(L, "torch");

    lua_getglobal(L, "require");
    lua_pushstring(L, "nn");
    lua_call(L, 1, 1);
    lua_setglobal(L, "nn");

    return L;
}
//void luaRequire(lua_State *L, const char *libName) {
//    lua_getglobal(L, "require");
//    lua_pushstring(L, libName);
//    lua_call(L, 1, 1);
//    lua_setglobal(L, libName);
//}
void luaClose(lua_State *L) {
    lua_close(L);
}
void collectGarbage(lua_State *L) {
    pushGlobal(L, "collectgarbage");
    lua_call(L, 0, 0);
}

{% for typedict in types %}
{% set Real = typedict['Real'] %}
{% set real = typedict['real'] %}
int TH{{Real}}Storage_getRefCount(TH{{Real}}Storage *self) {
    return self->refcount;
}
int TH{{Real}}Tensor_getRefCount(TH{{Real}}Tensor *self) {
    return self->refcount;
}
{% endfor %}

long pointerAsInt(void *ptr) {
    return (long)ptr;
}

