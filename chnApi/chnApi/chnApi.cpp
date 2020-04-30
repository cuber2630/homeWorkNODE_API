﻿
#include "chnApi.h"
#include "GameBase.h"
#include "ChnMathApi.h"
#include <assert.h>

using namespace std;

//api 版號
#define CHN_API_VERSION "chnapi20200415v0.1"

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
Napi::Object MyObject::Init(Napi::Env env, Napi::Object exports) {
  
  napi_status status;
  napi_property_descriptor properties[] = {
      {"value", 0, 0, GetValue, SetValue, 0, napi_default, 0},
      DECLARE_NAPI_METHOD("plusOne", PlusOne),
      DECLARE_NAPI_METHOD("multiply", Multiply),
  };
  

  Napi::Function func =
      DefineClass(env,
                  "MyObject",
                  {
                   InstanceMethod("getApiVersion", &MyObject::GetApiVersion),
                   InstanceMethod("getMathVersion", &MyObject::GetMathVersion),
                   InstanceMethod("getGameBase", &MyObject::GetGameBaseValue),
                   InstanceMethod("initNewUser", &MyObject::InitNewUser),
                   InstanceMethod("plusOne", &MyObject::PlusOne),
                   InstanceMethod("value", &MyObject::GetValue),                   
                   InstanceMethod("multiply", &MyObject::Multiply)                  
                  });

  constructor = Napi::Persistent(func);
  constructor.SuppressDestruct();

  exports.Set("MyObject", func);
  return exports;
}

MyObject::MyObject(const Napi::CallbackInfo& info)
    : Napi::ObjectWrap<MyObject>(info) {
  Napi::Env env = info.Env();
  Napi::HandleScope scope(env);

  int length = info.Length();

  if (length <= 0 || !info[0].IsNumber()) {
    Napi::TypeError::New(env, "Number expected").ThrowAsJavaScriptException();
    return;
  }

  Napi::Number value = info[0].As<Napi::Number>();
  this->value_ = value.DoubleValue();
}

Napi::Value MyObject::GetValue(const Napi::CallbackInfo& info) {
  double num = this->value_;

  return Napi::Number::New(info.Env(), num);
}



Napi::Value MyObject::PlusOne(const Napi::CallbackInfo& info) {
  this->value_ = this->value_ + 1;

  return MyObject::GetValue(info);
}

Napi::Value MyObject::Multiply(const Napi::CallbackInfo& info) {
  Napi::Number multiple;
  if (info.Length() <= 0 || !info[0].IsNumber()) {
    multiple = Napi::Number::New(info.Env(), 1);
  } else {
    multiple = info[0].As<Napi::Number>();
  }

  Napi::Object obj = constructor.New(
      {Napi::Number::New(info.Env(), this->value_ * multiple.DoubleValue())});

  return obj;
}

//==================================================================


Napi::Value MyObject::GetApiVersion(const Napi::CallbackInfo& info) {
  
  return Napi::String::New(info.Env(), CHN_API_VERSION);
}

//新玩家建制記憶體與建立機率版本號碼
Napi::Value MyObject::InitNewUser(const Napi::CallbackInfo& info) {
    
  char recordData[MAXIMUM_USER_RECORD_DATA_SIZE];
  MathEnter.CreateNewChance(recordData);
  return Napi::String::New(info.Env(), "InitNewUser");
}

//拿到版本
Napi::Value MyObject::GetMathVersion(const Napi::CallbackInfo& info) {
  
  char version[16];
  MathEnter.GetVersion(version);
  return Napi::String::New(info.Env(), version);
}

Napi::Value MyObject::GetGameBaseValue(const Napi::CallbackInfo& info) {
  
  GameBase cGameBase(10);
  cGameBase.GameLoop(0);
  double num = (double)cGameBase.showNumber;
  return Napi::Number::New(info.Env(), num);
}
//==================================================================

Napi::Object InitAll(Napi::Env env, Napi::Object exports) {
  return MyObject::Init(env, exports);
}

NODE_API_MODULE(addon, InitAll)