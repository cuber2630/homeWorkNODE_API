
#include "chnApi.h"
#include "GameBase.h"
#include "ChnMathApi.h"
#include <assert.h>
#include <string>

using namespace std;

//api 版號
#define CHN_API_VERSION "chnapi20200415v0.1"

static ChnMathApi	MathEnter;

napi_ref MyObject::constructor;

MyObject::MyObject(double value)
    : value_(value), env_(nullptr), wrapper_(nullptr) {}

MyObject::~MyObject() {
  napi_delete_reference(env_, wrapper_);
}

void MyObject::Destructor(napi_env env,
                          void* nativeObject,
                          void* /*finalize_hint*/) {
  reinterpret_cast<MyObject*>(nativeObject)->~MyObject();
}

#define DECLARE_NAPI_METHOD(name, func)                                        \
  { name, 0, func, 0, 0, 0, napi_default, 0 }

//=====以下還沒改================
napi_value MyObject::Init(napi_env env, napi_value exports) {
  
  napi_status status;
  napi_property_descriptor properties[] = {
      {"value", 0, 0, GetValue, SetValue, 0, napi_default, 0},
      DECLARE_NAPI_METHOD("plusOne", PlusOne),
      DECLARE_NAPI_METHOD("multiply", Multiply),

      DECLARE_NAPI_METHOD("getApiVersion", GetApiVersion),
      DECLARE_NAPI_METHOD("getMathVersion", GetMathVersion),
      DECLARE_NAPI_METHOD("getGameBase", GetGameBaseValue),
      DECLARE_NAPI_METHOD("initNewUser", InitNewUser),
  };

  napi_value cons;
  status = napi_define_class(
      env, "MyObject", NAPI_AUTO_LENGTH, New, nullptr, 7, properties, &cons);
  assert(status == napi_ok);

  status = napi_create_reference(env, cons, 1, &constructor);
  assert(status == napi_ok);

  status = napi_set_named_property(env, exports, "MyObject", cons);
  assert(status == napi_ok);
  return exports;
}

napi_value MyObject::New(napi_env env, napi_callback_info info) {
  napi_status status;

  napi_value target;
  status = napi_get_new_target(env, info, &target);
  assert(status == napi_ok);
  bool is_constructor = target != nullptr;

  if (is_constructor) {
    // Invoked as constructor: `new MyObject(...)`
    size_t argc = 1;
    napi_value args[1];
    napi_value jsthis;
    status = napi_get_cb_info(env, info, &argc, args, &jsthis, nullptr);
    assert(status == napi_ok);

    double value = 0;

    napi_valuetype valuetype;
    status = napi_typeof(env, args[0], &valuetype);
    assert(status == napi_ok);

    if (valuetype != napi_undefined) {
      status = napi_get_value_double(env, args[0], &value);
      assert(status == napi_ok);
    }

    MyObject* obj = new MyObject(value);

    obj->env_ = env;
    status = napi_wrap(env,
                       jsthis,
                       reinterpret_cast<void*>(obj),
                       MyObject::Destructor,
                       nullptr,  // finalize_hint
                       &obj->wrapper_);
    assert(status == napi_ok);

    return jsthis;
  } else {
    // Invoked as plain function `MyObject(...)`, turn into construct call.
    size_t argc_ = 1;
    napi_value args[1];
    status = napi_get_cb_info(env, info, &argc_, args, nullptr, nullptr);
    assert(status == napi_ok);

    const size_t argc = 1;
    napi_value argv[argc] = {args[0]};

    napi_value cons;
    status = napi_get_reference_value(env, constructor, &cons);
    assert(status == napi_ok);

    napi_value instance;
    status = napi_new_instance(env, cons, argc, argv, &instance);
    assert(status == napi_ok);

    return instance;
  }
}


napi_value MyObject::GetValue(napi_env env, napi_callback_info info) {
  napi_status status;

  napi_value jsthis;
  status = napi_get_cb_info(env, info, nullptr, nullptr, &jsthis, nullptr);
  assert(status == napi_ok);

  MyObject* obj;
  status = napi_unwrap(env, jsthis, reinterpret_cast<void**>(&obj));
  assert(status == napi_ok);

  napi_value num;
  status = napi_create_double(env, obj->value_, &num);
  assert(status == napi_ok);

  return num;
}

napi_value MyObject::SetValue(napi_env env, napi_callback_info info) {
  napi_status status;

  size_t argc = 1;
  napi_value value;
  napi_value jsthis;
  status = napi_get_cb_info(env, info, &argc, &value, &jsthis, nullptr);
  assert(status == napi_ok);

  MyObject* obj;
  status = napi_unwrap(env, jsthis, reinterpret_cast<void**>(&obj));
  assert(status == napi_ok);

  status = napi_get_value_double(env, value, &obj->value_);
  assert(status == napi_ok);

  return nullptr;
}

napi_value MyObject::PlusOne(napi_env env, napi_callback_info info) {
  napi_status status;

  napi_value jsthis;
  status = napi_get_cb_info(env, info, nullptr, nullptr, &jsthis, nullptr);
  assert(status == napi_ok);

  MyObject* obj;
  status = napi_unwrap(env, jsthis, reinterpret_cast<void**>(&obj));
  assert(status == napi_ok);

  obj->value_ += 1;

  napi_value num;
  status = napi_create_double(env, obj->value_, &num);
  assert(status == napi_ok);

  return num;
}


napi_value MyObject::Multiply(napi_env env, napi_callback_info info) {
  napi_status status;

  size_t argc = 1;
  napi_value args[1];
  napi_value jsthis;
  status = napi_get_cb_info(env, info, &argc, args, &jsthis, nullptr);
  assert(status == napi_ok);

  napi_valuetype valuetype;
  status = napi_typeof(env, args[0], &valuetype);
  assert(status == napi_ok);

  double multiple = 1;
  if (valuetype != napi_undefined) {
    status = napi_get_value_double(env, args[0], &multiple);
    assert(status == napi_ok);
  }

  MyObject* obj;
  status = napi_unwrap(env, jsthis, reinterpret_cast<void**>(&obj));
  assert(status == napi_ok);

  napi_value cons;
  status = napi_get_reference_value(env, constructor, &cons);
  assert(status == napi_ok);

  const int kArgCount = 1;
  napi_value argv[kArgCount];
  status = napi_create_double(env, obj->value_ * multiple, argv);
  assert(status == napi_ok);

  napi_value instance;
  status = napi_new_instance(env, cons, kArgCount, argv, &instance);
  assert(status == napi_ok);

  return instance;
}

//==================================================================


napi_value MyObject::GetApiVersion(napi_env env, napi_callback_info info) {
  
  napi_status status;
  napi_value str;
  
  string a;
  a = CHN_API_VERSION;
  int num = a.length();

  status = napi_create_string_utf8(env, CHN_API_VERSION, num, &str);
  assert(status == napi_ok);
  return str;
}

//新玩家建制記憶體與建立機率版本號碼
napi_value MyObject::InitNewUser(napi_env env, napi_callback_info info) {
    
  char recordData[MAXIMUM_USER_RECORD_DATA_SIZE];
  napi_status status;
  napi_value str;

  status = napi_create_string_utf8(env, "InitNewUser", 11, &str);
  assert(status == napi_ok);

  MathEnter.CreateNewChance(recordData);
  return str;
}

//拿到版本
napi_value MyObject::GetMathVersion(napi_env env, napi_callback_info info) {
  
  char version[16]={0};
  MathEnter.GetVersion(version);
  string a;
  a = version;
  int num = a.length();
  napi_status status;
  napi_value str;

  status = napi_create_string_utf8(env, version, num, &str);
  assert(status == napi_ok);

  return str;
}

napi_value MyObject::GetGameBaseValue(napi_env env, napi_callback_info info) {
  
  GameBase cGameBase(10);
  cGameBase.GameLoop(0);
  double showNumber = (double)cGameBase.showNumber;

  napi_status status;
  napi_value num;
  status = napi_create_double(env, showNumber, &num);
  assert(status == napi_ok);

  return num;
}
//==================================================================

napi_value InitAll(napi_env env, napi_value exports) {
  return MyObject::Init(env, exports);
}

NAPI_MODULE(NODE_GYP_MODULE_NAME, InitAll)