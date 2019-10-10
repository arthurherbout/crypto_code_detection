#include "torch/utils.h"
#include "THCl.h"
//#include "THFile.h"
#include "luaT.h"

extern "C" {
  void cltorch_ClStorage_init(lua_State* L);
}

#define EXCEPT_TO_THERROR(method) \
try { \
  method; \
} catch(exception &e) { \
  THError("Something went wrong: %s", e.what()); \
}

/* everything is as the generic Storage.c, except few things (see below) */

#define real float
#define Real Cl
#define TH_GENERIC_FILE "generic/Storage.c"

#define torch_Storage_(NAME) TH_CONCAT_4(torch_,Real,Storage_,NAME)

#define THFile_readRealRaw(file, data, size)                            \
  {                                                                     \
    float *fdata = (float*)THAlloc(sizeof(float)*size);                 \
    THError("Not implemented"); \
    THFile_readFloatRaw(file, fdata, size);                             \
/*    THClCheck(clMemcpy(data, fdata, size * sizeof(float), clMemcpyHostToDevice));*/ \
    THFree(fdata);                                                      \
  }

#define THFile_writeRealRaw(file, data, size)                           \
  {                                                                     \
    float *fdata = (float*)THAlloc(sizeof(float)*size);                 \
    THError("Not implemented"); \
/*    THClCheck(clMemcpy(fdata, data, size * sizeof(float), clMemcpyDeviceToHost));*/ \
    THFile_writeFloatRaw(file, fdata, size);                            \
    THFree(fdata);                                                      \
  }

#define torch_Storage TH_CONCAT_STRING_3(torch.,Real,Storage)

#include "generic/Storage.cpp"

#undef real
#undef Real
#undef TH_GENERIC_FILE

/* now we overwrite some methods specific to ClStorage */

static int cltorch_ClStorage_copy(lua_State *L)
{
  THClState *state = cltorch_getstate(L);
  THClStorage *storage = static_cast<THClStorage *>(luaT_checkudata(L, 1, "torch.ClStorage"));
  void *src;
  if( (src = luaT_toudata(L, 2, "torch.ClStorage")) ) {
    EXCEPT_TO_THERROR(THClStorage_copy(state, storage, static_cast<THClStorage *>(src)));
  } else if( (src = luaT_toudata(L, 2, "torch.ByteStorage")) ) {
    EXCEPT_TO_THERROR(THClStorage_copyByte(state, storage, static_cast<THByteStorage *>(src)));
  } else if( (src = luaT_toudata(L, 2, "torch.CharStorage")) ) {
    EXCEPT_TO_THERROR(THClStorage_copyChar(state, storage, static_cast<THCharStorage *>(src)));
  } else if( (src = luaT_toudata(L, 2, "torch.ShortStorage")) ) {
    EXCEPT_TO_THERROR(THClStorage_copyShort(state, storage, static_cast<THShortStorage *>(src)));
  } else if( (src = luaT_toudata(L, 2, "torch.IntStorage")) ) {
    EXCEPT_TO_THERROR(THClStorage_copyInt(state, storage, static_cast<THIntStorage *>(src)));
  } else if( (src = luaT_toudata(L, 2, "torch.LongStorage")) ) {
    EXCEPT_TO_THERROR(THClStorage_copyLong(state, storage, static_cast<THLongStorage *>(src)));
  } else if( (src = luaT_toudata(L, 2, "torch.FloatStorage")) ) {
    EXCEPT_TO_THERROR(THClStorage_copyFloat(state, storage, static_cast<THFloatStorage *>(src)));
  } else if( (src = luaT_toudata(L, 2, "torch.DoubleStorage")) ) {
    EXCEPT_TO_THERROR(THClStorage_copyDouble(state, storage, static_cast<THDoubleStorage *>(src)));
  } else if( (src = luaT_toudata(L, 2, "torch.ClStorage")) ) {
    EXCEPT_TO_THERROR(THClStorage_copyCl(state, storage, static_cast<THClStorage *>(src)));
  } else
    luaL_typerror(L, 2, "torch.*Storage");

  lua_settop(L, 1);
  return 1;
}

#define CL_IMPLEMENT_STORAGE_COPY(TYPEC)                              \
  static int cltorch_##TYPEC##Storage_copy(lua_State *L)                \
  {                                                                     \
    TH##TYPEC##Storage *storage = static_cast<TH##TYPEC##Storage *>(luaT_checkudata(L, 1, "torch." #TYPEC "Storage")); \
    void *src;                                                          \
    if( (src = luaT_toudata(L, 2, "torch." #TYPEC "Storage")) )         \
      TH##TYPEC##Storage_copy(storage, static_cast<TH##TYPEC##Storage *>(src));                            \
    else if( (src = luaT_toudata(L, 2, "torch.ByteStorage")) )          \
      TH##TYPEC##Storage_copyByte(storage, static_cast<THByteStorage *>(src));                        \
    else if( (src = luaT_toudata(L, 2, "torch.CharStorage")) )          \
      TH##TYPEC##Storage_copyChar(storage, static_cast<THCharStorage *>(src));                        \
    else if( (src = luaT_toudata(L, 2, "torch.ShortStorage")) )         \
      TH##TYPEC##Storage_copyShort(storage, static_cast<THShortStorage *>(src));                       \
    else if( (src = luaT_toudata(L, 2, "torch.IntStorage")) )           \
      TH##TYPEC##Storage_copyInt(storage, static_cast<THIntStorage *>(src));                         \
    else if( (src = luaT_toudata(L, 2, "torch.LongStorage")) )          \
      TH##TYPEC##Storage_copyLong(storage, static_cast<THLongStorage *>(src));                        \
    else if( (src = luaT_toudata(L, 2, "torch.FloatStorage")) )         \
      TH##TYPEC##Storage_copyFloat(storage, static_cast<THFloatStorage *>(src));                       \
    else if( (src = luaT_toudata(L, 2, "torch.DoubleStorage")) )        \
      TH##TYPEC##Storage_copyDouble(storage, static_cast<THDoubleStorage *>(src));                      \
    else if( (src = luaT_toudata(L, 2, "torch.ClStorage")) )          \
      TH##TYPEC##Storage_copyCl(cltorch_getstate(L), storage, static_cast<THClStorage *>(src));   \
    else                                                                \
      luaL_typerror(L, 2, "torch.*Storage");                            \
                                                                        \
    lua_settop(L, 1);                                                   \
    return 1;                                                           \
}

CL_IMPLEMENT_STORAGE_COPY(Byte)
CL_IMPLEMENT_STORAGE_COPY(Char)
CL_IMPLEMENT_STORAGE_COPY(Short)
CL_IMPLEMENT_STORAGE_COPY(Int)
CL_IMPLEMENT_STORAGE_COPY(Long)
CL_IMPLEMENT_STORAGE_COPY(Float)
CL_IMPLEMENT_STORAGE_COPY(Double)

void cltorch_ClStorage_init(lua_State* L)
{
  /* the standard stuff */
  torch_ClStorage_init(L);

  /* the copy methods */
  {
    int i;

    const char* tnames[8] = {"torch.ByteStorage",
                             "torch.CharStorage",
                             "torch.ShortStorage",
                             "torch.IntStorage",
                             "torch.LongStorage",
                             "torch.FloatStorage",
                             "torch.DoubleStorage",
                             "torch.ClStorage"};

    static int (*funcs[8])(lua_State*) = {cltorch_ByteStorage_copy,
                                          cltorch_CharStorage_copy,
                                          cltorch_ShortStorage_copy,
                                          cltorch_IntStorage_copy,
                                          cltorch_LongStorage_copy,
                                          cltorch_FloatStorage_copy,
                                          cltorch_DoubleStorage_copy,
                                          cltorch_ClStorage_copy};

    for(i = 0; i < 8; i++)
    {
      luaT_pushmetatable(L, tnames[i]);
      lua_pushcfunction(L, funcs[i]);
      lua_setfield(L, -2, "copy");
      lua_pop(L, 1);
    }
  }
}
