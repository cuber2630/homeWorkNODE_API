
#include "chnApi.h"
#include "GameBase.h"
#include "ChnMathApi.h"
#include <assert.h>
#include <string>

using namespace std;

//api 版號
#define CHN_API_VERSION "chnapi20200415v0.1"

static ChnMathApi	MathEnter;

napi_ref ChnApi::constructor;

ChnApi::ChnApi(double value)
    : value_(value), env_(nullptr), wrapper_(nullptr) {}

ChnApi::~ChnApi() {
  napi_delete_reference(env_, wrapper_);
}

void ChnApi::Destructor(napi_env env,
                          void* nativeObject,
                          void* /*finalize_hint*/) {
  reinterpret_cast<ChnApi*>(nativeObject)->~ChnApi();
}

#define DECLARE_NAPI_METHOD(name, func)                                        \
  { name, 0, func, 0, 0, 0, napi_default, 0 }

//=====以下還沒改================
napi_value ChnApi::Init(napi_env env, napi_value exports) {
  
  napi_status status;
  napi_property_descriptor properties[] = {
      {"value", 0, 0, GetValue, SetValue, 0, napi_default, 0},
      DECLARE_NAPI_METHOD("plusOne", PlusOne),
      DECLARE_NAPI_METHOD("multiply", Multiply),

      DECLARE_NAPI_METHOD("getApiVersion", GetApiVersion),
      DECLARE_NAPI_METHOD("initNewUser", InitNewUser),
      DECLARE_NAPI_METHOD("getMathVersion", GetMathVersion),

      DECLARE_NAPI_METHOD("setUserLogInData", SetUserLogInData),
      DECLARE_NAPI_METHOD("checkUserTotalBet", CheckUserTotalBet),
      DECLARE_NAPI_METHOD("getMathData", GetMathData),

      DECLARE_NAPI_METHOD("getGameBase", GetGameBaseValue),
      DECLARE_NAPI_METHOD("jsonExample", JsonExample),
  };

  napi_value cons;
  status = napi_define_class(
      env, "ChnApi", NAPI_AUTO_LENGTH, New, nullptr, 11, properties, &cons);
  assert(status == napi_ok);

  status = napi_create_reference(env, cons, 1, &constructor);
  assert(status == napi_ok);

  status = napi_set_named_property(env, exports, "ChnApi", cons);
  assert(status == napi_ok);
  
  return exports;
}

napi_value ChnApi::New(napi_env env, napi_callback_info info) {
  napi_status status;

  napi_value target;
  status = napi_get_new_target(env, info, &target);
  assert(status == napi_ok);
  bool is_constructor = target != nullptr;

  if (is_constructor) {
    // Invoked as constructor: `new ChnApi(...)`
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

    ChnApi* obj = new ChnApi(value);

    obj->env_ = env;
    status = napi_wrap(env,
                       jsthis,
                       reinterpret_cast<void*>(obj),
                       ChnApi::Destructor,
                       nullptr,  // finalize_hint
                       &obj->wrapper_);
    assert(status == napi_ok);

    return jsthis;
  } else {
    // Invoked as plain function `ChnApi(...)`, turn into construct call.
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


//==================================================================


napi_value ChnApi::GetApiVersion(napi_env env, napi_callback_info info) {
  
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
napi_value ChnApi::InitNewUser(napi_env env, napi_callback_info info) {
    
  char recordData[MAXIMUM_USER_RECORD_DATA_SIZE];
  MathEnter.CreateNewChance(recordData);

  napi_status status = napi_generic_failure;
  napi_value str;
  status = napi_create_string_utf8(env, "InitNewUser", 11, &str);
  assert(status == napi_ok);
  return str;
}

//拿到版本
napi_value ChnApi::GetMathVersion(napi_env env, napi_callback_info info) {
  
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


//拿到玩家登入資料, 與傳出開局資料
napi_value ChnApi::SetUserLogInData(napi_env env, napi_callback_info info) {
  
  size_t argc = 1;
  napi_value args[1];
  napi_status status;
  status = napi_get_cb_info(env, info, &argc, args, nullptr, nullptr);
  assert(status == napi_ok);

  napi_valuetype valuetype;
  status = napi_typeof(env, args[0], &valuetype);
  assert(status == napi_ok);

  napi_value objData;
  status = napi_create_object(env, &objData);
  assert(status == napi_ok);

  if (valuetype != napi_undefined) {
    status = napi_set_named_property(env, objData, "userLoginData", args[0]);
    assert(status == napi_ok);
  }

  GameConfig_t config_data={0};
  napi_value addObj;  
  status = napi_get_named_property(env, objData, "userLoginData", &addObj);
  assert(status == napi_ok);

  napi_value userId;  
  status = napi_get_named_property(env, addObj, "userId", &userId);
  assert(status == napi_ok);

  status = napi_get_value_int32(env, userId, &config_data.userId);
  assert(status == napi_ok);
  napi_value over;  
  status = napi_create_int32(env, config_data.userId, &over);
  assert(status == napi_ok);
  //MathEnter.UserLogIn();
  
  return over;
}


//檢查玩家押分，遊玩資料
napi_value ChnApi::CheckUserTotalBet(napi_env env, napi_callback_info info) {
  
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


//機率計算，及丟出資料
napi_value ChnApi::GetMathData(napi_env env, napi_callback_info info) {
  
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
//=====測試==============
napi_value ChnApi::GetGameBaseValue(napi_env env, napi_callback_info info) {
  
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

//創一個新的OBJ物件至OBJ內
napi_value setNewObjProperty(napi_env env, napi_status status, napi_value object, const char *name ){
  napi_value newObject;  
  status = napi_create_object(env, &newObject);
  status = napi_set_named_property(env, object, name, newObject);
  assert(status == napi_ok);
  return object;
}

//創一個新的數值物件至OBJ內
napi_value setNewValueProperty(napi_env env, napi_status status, napi_value object, const char *name, double source_value ){
  napi_value n_value;  
  status = napi_create_double(env, source_value, &n_value);
  status = napi_set_named_property(env, object, name, n_value);
  assert(status == napi_ok);
  return object;
}

//創一個新的字串物件至OBJ內
napi_value setNewStrProperty(napi_env env, napi_status status, napi_value object, const char *name, const char *strData ){
  napi_value n_str;  
  status = napi_create_string_utf8(env, strData, NAPI_AUTO_LENGTH, &n_str);
  status = napi_set_named_property(env, object, name, n_str);
  assert(status == napi_ok);
  return object;
}
//新玩家建制記憶體與建立機率版本號碼
napi_value ChnApi::JsonExample(napi_env env, napi_callback_info info) {
    
  napi_status status = napi_generic_failure;

  //主要obj
  napi_value main_object;  
  status = napi_create_object(env, &main_object);
  
  napi_value object1;  
  status = napi_create_object(env, &object1);
  object1 = setNewObjProperty(env, status, object1, "dd");

  

  status = napi_set_named_property(env, main_object, "o1", object1);
 
  main_object = setNewObjProperty(env, status, main_object, "kind");

  napi_value addObj;  
  status = napi_get_named_property(env, object1, "dd", &addObj);
  addObj = setNewValueProperty(env, status, addObj, "d1", 9987);
  addObj = setNewStrProperty(env, status, addObj, "d2", "9987");

  napi_value id;
  status = napi_create_string_utf8(env, "wang", NAPI_AUTO_LENGTH, &id);
  status = napi_set_named_property(env, main_object, "id", id);

  napi_value data;
  status = napi_create_string_utf8(env, "submitResult->date", NAPI_AUTO_LENGTH, &data);
  status = napi_set_named_property(env, main_object, "data", data);

  
  char symbol[15] = {1,2,3,4,5,6,7,8,9,10,1,2,3,4,5};
  napi_value sym;
  status = napi_create_array(env, &sym);
  for(int i = 0 ; i < sizeof(symbol); i++){

    napi_value e;
    status = napi_create_int32(env, symbol[i], &e);
    status = napi_set_element(env, sym, i, e);
  }
  status = napi_set_named_property(env, main_object, "sym", sym);

  assert(status == napi_ok);
  return main_object;
}

//

napi_value ChnApi::GetValue(napi_env env, napi_callback_info info) {
  napi_status status;

  napi_value jsthis;
  status = napi_get_cb_info(env, info, nullptr, nullptr, &jsthis, nullptr);
  assert(status == napi_ok);

  ChnApi* obj;
  status = napi_unwrap(env, jsthis, reinterpret_cast<void**>(&obj));
  assert(status == napi_ok);

  napi_value num;
  status = napi_create_double(env, obj->value_, &num);
  assert(status == napi_ok);

  return num;
}

napi_value ChnApi::SetValue(napi_env env, napi_callback_info info) {
  napi_status status;

  size_t argc = 1;
  napi_value value;
  napi_value jsthis;
  status = napi_get_cb_info(env, info, &argc, &value, &jsthis, nullptr);
  assert(status == napi_ok);

  ChnApi* obj;
  status = napi_unwrap(env, jsthis, reinterpret_cast<void**>(&obj));
  assert(status == napi_ok);

  status = napi_get_value_double(env, value, &obj->value_);
  assert(status == napi_ok);

  return nullptr;
}

napi_value ChnApi::PlusOne(napi_env env, napi_callback_info info) {
  napi_status status;

  napi_value jsthis;
  status = napi_get_cb_info(env, info, nullptr, nullptr, &jsthis, nullptr);
  assert(status == napi_ok);

  ChnApi* obj;
  status = napi_unwrap(env, jsthis, reinterpret_cast<void**>(&obj));
  assert(status == napi_ok);

  obj->value_ += 1;

  napi_value num;
  status = napi_create_double(env, obj->value_, &num);
  assert(status == napi_ok);

  return num;
}


napi_value ChnApi::Multiply(napi_env env, napi_callback_info info) {
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

  ChnApi* obj;
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


napi_value InitAll(napi_env env, napi_value exports) {
  return ChnApi::Init(env, exports);
}

NAPI_MODULE(NODE_GYP_MODULE_NAME, InitAll)