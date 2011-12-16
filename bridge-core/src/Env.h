#ifndef BRIDGE_ENV_H
#define BRIDGE_ENV_H

#include "defines.h"
#include <node.h>
#ifdef __APPLE__
# include <JavaVM/jni.h>
#else
# include <jni.h>
#endif

#include "Interface.h"
#include "Utils.h"

namespace bridge {

class Conv;
class VM;

class LIB_EXPORT Env {
public:
#ifdef ANDROID
	static LIB_EXPORT void setupEnv(VM *vm);
#endif
	static LIB_EXPORT Env *getEnv();
	static LIB_EXPORT Env *getEnv_nocheck();
	LIB_EXPORT v8::Local<v8::Value> load(v8::Handle<v8::String> moduleName, v8::Handle<v8::Object> moduleExports);
  inline Conv *getConv() {return conv;}
  inline Interface *getInterface(classId class_) {return interfaces->get(Interface::classId2Idx(class_));}
  inline VM *getVM() {return vm;}
  void setAsync();

private:
	Env(VM *vm);
	~Env();
  int initJava(node::Isolate *nodeIsolate);
  static void atExit();
  static void asyncCb(uv_async_t *async, int status);

	static Env         *initOnce(VM *vm);
	node::Isolate      *nodeIsolate;
	v8::Isolate        *v8Isolate;
	VM                 *vm;
  Conv               *conv;
  TArray<Interface*> *interfaces;
  
  uv_async_t         async;

  /* JNI */
	jclass             jEnvClass;
  jobject            jEnv;
	jmethodID          createMethodId;
	jmethodID          releaseMethodId;
  jmethodID          loadMethodId;
  jmethodID          onEntryMethodId;
};

} //namespace bridge
#endif
