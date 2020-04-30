#ifndef MYOBJECT_H
#define MYOBJECT_H

#include <node_api.h>

class MyObject {
 public:
  static napi_value Init(napi_env env, napi_value exports);
  static void Destructor(napi_env env, void* nativeObject, void* finalize_hint);

 private:
  explicit MyObject(double value_ = 0);
  ~MyObject();
 
  static napi_value New(napi_env env, napi_callback_info info);
  static napi_value GetValue(napi_env env, napi_callback_info info);
  static napi_value SetValue(napi_env env, napi_callback_info info);
  static napi_value PlusOne(napi_env env, napi_callback_info info);
  static napi_value Multiply(napi_env env, napi_callback_info info);

  static napi_value GetApiVersion(napi_env env, napi_callback_info info);
  static napi_value InitNewUser(napi_env env, napi_callback_info info);   
  static napi_value GetMathVersion(napi_env env, napi_callback_info info);
  static napi_value GetGameBaseValue(napi_env env, napi_callback_info info); 

  // Napi::Value GetValue(const Napi::CallbackInfo& info);
  // Napi::Value PlusOne(const Napi::CallbackInfo& info);
  // Napi::Value Multiply(const Napi::CallbackInfo& info);  
  
  // Napi::Value GetApiVersion(const Napi::CallbackInfo& info);  
  // Napi::Value InitNewUser(const Napi::CallbackInfo& info);   
  // Napi::Value GetMathVersion(const Napi::CallbackInfo& info);
  // Napi::Value GetGameBaseValue(const Napi::CallbackInfo& info);  

  static napi_ref constructor;
  double value_;
  napi_env env_;
  napi_ref wrapper_;
};

#endif
