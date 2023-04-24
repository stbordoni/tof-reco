// Do NOT change. Changes will be lost next time file is generated

#define R__DICTIONARY_FILENAME srcdITofObjectsDict
#define R__NO_DEPRECATION

/*******************************************************************/
#include <stddef.h>
#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <assert.h>
#define G__DICTIONARY
#include "ROOT/RConfig.hxx"
#include "TClass.h"
#include "TDictAttributeMap.h"
#include "TInterpreter.h"
#include "TROOT.h"
#include "TBuffer.h"
#include "TMemberInspector.h"
#include "TInterpreter.h"
#include "TVirtualMutex.h"
#include "TError.h"

#ifndef G__ROOT
#define G__ROOT
#endif

#include "RtypesImp.h"
#include "TIsAProxy.h"
#include "TFileMergeInfo.h"
#include <algorithm>
#include "TCollectionProxyInfo.h"
/*******************************************************************/

#include "TDataMember.h"

// Header files passed as explicit arguments
#include "../include/TofHit.h"
#include "../include/TofSignal.h"
#include "../include/TofEvent.h"
#include "../include/TofRun.h"

// Header files passed via #pragma extra_include

// The generated code does not explicitly qualify STL entities
namespace std {} using namespace std;

namespace ROOT {
   static TClass *TofHit_Dictionary();
   static void TofHit_TClassManip(TClass*);
   static void *new_TofHit(void *p = nullptr);
   static void *newArray_TofHit(Long_t size, void *p);
   static void delete_TofHit(void *p);
   static void deleteArray_TofHit(void *p);
   static void destruct_TofHit(void *p);

   // Function generating the singleton type initializer
   static TGenericClassInfo *GenerateInitInstanceLocal(const ::TofHit*)
   {
      ::TofHit *ptr = nullptr;
      static ::TVirtualIsAProxy* isa_proxy = new ::TIsAProxy(typeid(::TofHit));
      static ::ROOT::TGenericClassInfo 
         instance("TofHit", "TofHit.h", 31,
                  typeid(::TofHit), ::ROOT::Internal::DefineBehavior(ptr, ptr),
                  &TofHit_Dictionary, isa_proxy, 4,
                  sizeof(::TofHit) );
      instance.SetNew(&new_TofHit);
      instance.SetNewArray(&newArray_TofHit);
      instance.SetDelete(&delete_TofHit);
      instance.SetDeleteArray(&deleteArray_TofHit);
      instance.SetDestructor(&destruct_TofHit);
      return &instance;
   }
   TGenericClassInfo *GenerateInitInstance(const ::TofHit*)
   {
      return GenerateInitInstanceLocal(static_cast<::TofHit*>(nullptr));
   }
   // Static variable to force the class initialization
   static ::ROOT::TGenericClassInfo *_R__UNIQUE_DICT_(Init) = GenerateInitInstanceLocal(static_cast<const ::TofHit*>(nullptr)); R__UseDummy(_R__UNIQUE_DICT_(Init));

   // Dictionary for non-ClassDef classes
   static TClass *TofHit_Dictionary() {
      TClass* theClass =::ROOT::GenerateInitInstanceLocal(static_cast<const ::TofHit*>(nullptr))->GetClass();
      TofHit_TClassManip(theClass);
   return theClass;
   }

   static void TofHit_TClassManip(TClass* ){
   }

} // end of namespace ROOT

namespace ROOT {
   static TClass *TofSignal_Dictionary();
   static void TofSignal_TClassManip(TClass*);
   static void *new_TofSignal(void *p = nullptr);
   static void *newArray_TofSignal(Long_t size, void *p);
   static void delete_TofSignal(void *p);
   static void deleteArray_TofSignal(void *p);
   static void destruct_TofSignal(void *p);

   // Function generating the singleton type initializer
   static TGenericClassInfo *GenerateInitInstanceLocal(const ::TofSignal*)
   {
      ::TofSignal *ptr = nullptr;
      static ::TVirtualIsAProxy* isa_proxy = new ::TIsAProxy(typeid(::TofSignal));
      static ::ROOT::TGenericClassInfo 
         instance("TofSignal", "TofSignal.h", 7,
                  typeid(::TofSignal), ::ROOT::Internal::DefineBehavior(ptr, ptr),
                  &TofSignal_Dictionary, isa_proxy, 4,
                  sizeof(::TofSignal) );
      instance.SetNew(&new_TofSignal);
      instance.SetNewArray(&newArray_TofSignal);
      instance.SetDelete(&delete_TofSignal);
      instance.SetDeleteArray(&deleteArray_TofSignal);
      instance.SetDestructor(&destruct_TofSignal);
      return &instance;
   }
   TGenericClassInfo *GenerateInitInstance(const ::TofSignal*)
   {
      return GenerateInitInstanceLocal(static_cast<::TofSignal*>(nullptr));
   }
   // Static variable to force the class initialization
   static ::ROOT::TGenericClassInfo *_R__UNIQUE_DICT_(Init) = GenerateInitInstanceLocal(static_cast<const ::TofSignal*>(nullptr)); R__UseDummy(_R__UNIQUE_DICT_(Init));

   // Dictionary for non-ClassDef classes
   static TClass *TofSignal_Dictionary() {
      TClass* theClass =::ROOT::GenerateInitInstanceLocal(static_cast<const ::TofSignal*>(nullptr))->GetClass();
      TofSignal_TClassManip(theClass);
   return theClass;
   }

   static void TofSignal_TClassManip(TClass* ){
   }

} // end of namespace ROOT

namespace ROOT {
   static TClass *TofEvent_Dictionary();
   static void TofEvent_TClassManip(TClass*);
   static void *new_TofEvent(void *p = nullptr);
   static void *newArray_TofEvent(Long_t size, void *p);
   static void delete_TofEvent(void *p);
   static void deleteArray_TofEvent(void *p);
   static void destruct_TofEvent(void *p);

   // Function generating the singleton type initializer
   static TGenericClassInfo *GenerateInitInstanceLocal(const ::TofEvent*)
   {
      ::TofEvent *ptr = nullptr;
      static ::TVirtualIsAProxy* isa_proxy = new ::TIsAProxy(typeid(::TofEvent));
      static ::ROOT::TGenericClassInfo 
         instance("TofEvent", "TofEvent.h", 8,
                  typeid(::TofEvent), ::ROOT::Internal::DefineBehavior(ptr, ptr),
                  &TofEvent_Dictionary, isa_proxy, 4,
                  sizeof(::TofEvent) );
      instance.SetNew(&new_TofEvent);
      instance.SetNewArray(&newArray_TofEvent);
      instance.SetDelete(&delete_TofEvent);
      instance.SetDeleteArray(&deleteArray_TofEvent);
      instance.SetDestructor(&destruct_TofEvent);
      return &instance;
   }
   TGenericClassInfo *GenerateInitInstance(const ::TofEvent*)
   {
      return GenerateInitInstanceLocal(static_cast<::TofEvent*>(nullptr));
   }
   // Static variable to force the class initialization
   static ::ROOT::TGenericClassInfo *_R__UNIQUE_DICT_(Init) = GenerateInitInstanceLocal(static_cast<const ::TofEvent*>(nullptr)); R__UseDummy(_R__UNIQUE_DICT_(Init));

   // Dictionary for non-ClassDef classes
   static TClass *TofEvent_Dictionary() {
      TClass* theClass =::ROOT::GenerateInitInstanceLocal(static_cast<const ::TofEvent*>(nullptr))->GetClass();
      TofEvent_TClassManip(theClass);
   return theClass;
   }

   static void TofEvent_TClassManip(TClass* ){
   }

} // end of namespace ROOT

namespace ROOT {
   static TClass *TofRun_Dictionary();
   static void TofRun_TClassManip(TClass*);
   static void delete_TofRun(void *p);
   static void deleteArray_TofRun(void *p);
   static void destruct_TofRun(void *p);

   // Function generating the singleton type initializer
   static TGenericClassInfo *GenerateInitInstanceLocal(const ::TofRun*)
   {
      ::TofRun *ptr = nullptr;
      static ::TVirtualIsAProxy* isa_proxy = new ::TIsAProxy(typeid(::TofRun));
      static ::ROOT::TGenericClassInfo 
         instance("TofRun", "TofRun.h", 9,
                  typeid(::TofRun), ::ROOT::Internal::DefineBehavior(ptr, ptr),
                  &TofRun_Dictionary, isa_proxy, 4,
                  sizeof(::TofRun) );
      instance.SetDelete(&delete_TofRun);
      instance.SetDeleteArray(&deleteArray_TofRun);
      instance.SetDestructor(&destruct_TofRun);
      return &instance;
   }
   TGenericClassInfo *GenerateInitInstance(const ::TofRun*)
   {
      return GenerateInitInstanceLocal(static_cast<::TofRun*>(nullptr));
   }
   // Static variable to force the class initialization
   static ::ROOT::TGenericClassInfo *_R__UNIQUE_DICT_(Init) = GenerateInitInstanceLocal(static_cast<const ::TofRun*>(nullptr)); R__UseDummy(_R__UNIQUE_DICT_(Init));

   // Dictionary for non-ClassDef classes
   static TClass *TofRun_Dictionary() {
      TClass* theClass =::ROOT::GenerateInitInstanceLocal(static_cast<const ::TofRun*>(nullptr))->GetClass();
      TofRun_TClassManip(theClass);
   return theClass;
   }

   static void TofRun_TClassManip(TClass* ){
   }

} // end of namespace ROOT

namespace ROOT {
   // Wrappers around operator new
   static void *new_TofHit(void *p) {
      return  p ? new(p) ::TofHit : new ::TofHit;
   }
   static void *newArray_TofHit(Long_t nElements, void *p) {
      return p ? new(p) ::TofHit[nElements] : new ::TofHit[nElements];
   }
   // Wrapper around operator delete
   static void delete_TofHit(void *p) {
      delete (static_cast<::TofHit*>(p));
   }
   static void deleteArray_TofHit(void *p) {
      delete [] (static_cast<::TofHit*>(p));
   }
   static void destruct_TofHit(void *p) {
      typedef ::TofHit current_t;
      (static_cast<current_t*>(p))->~current_t();
   }
} // end of namespace ROOT for class ::TofHit

namespace ROOT {
   // Wrappers around operator new
   static void *new_TofSignal(void *p) {
      return  p ? new(p) ::TofSignal : new ::TofSignal;
   }
   static void *newArray_TofSignal(Long_t nElements, void *p) {
      return p ? new(p) ::TofSignal[nElements] : new ::TofSignal[nElements];
   }
   // Wrapper around operator delete
   static void delete_TofSignal(void *p) {
      delete (static_cast<::TofSignal*>(p));
   }
   static void deleteArray_TofSignal(void *p) {
      delete [] (static_cast<::TofSignal*>(p));
   }
   static void destruct_TofSignal(void *p) {
      typedef ::TofSignal current_t;
      (static_cast<current_t*>(p))->~current_t();
   }
} // end of namespace ROOT for class ::TofSignal

namespace ROOT {
   // Wrappers around operator new
   static void *new_TofEvent(void *p) {
      return  p ? new(p) ::TofEvent : new ::TofEvent;
   }
   static void *newArray_TofEvent(Long_t nElements, void *p) {
      return p ? new(p) ::TofEvent[nElements] : new ::TofEvent[nElements];
   }
   // Wrapper around operator delete
   static void delete_TofEvent(void *p) {
      delete (static_cast<::TofEvent*>(p));
   }
   static void deleteArray_TofEvent(void *p) {
      delete [] (static_cast<::TofEvent*>(p));
   }
   static void destruct_TofEvent(void *p) {
      typedef ::TofEvent current_t;
      (static_cast<current_t*>(p))->~current_t();
   }
} // end of namespace ROOT for class ::TofEvent

namespace ROOT {
   // Wrapper around operator delete
   static void delete_TofRun(void *p) {
      delete (static_cast<::TofRun*>(p));
   }
   static void deleteArray_TofRun(void *p) {
      delete [] (static_cast<::TofRun*>(p));
   }
   static void destruct_TofRun(void *p) {
      typedef ::TofRun current_t;
      (static_cast<current_t*>(p))->~current_t();
   }
} // end of namespace ROOT for class ::TofRun

namespace ROOT {
   static TClass *vectorlEstringgR_Dictionary();
   static void vectorlEstringgR_TClassManip(TClass*);
   static void *new_vectorlEstringgR(void *p = nullptr);
   static void *newArray_vectorlEstringgR(Long_t size, void *p);
   static void delete_vectorlEstringgR(void *p);
   static void deleteArray_vectorlEstringgR(void *p);
   static void destruct_vectorlEstringgR(void *p);

   // Function generating the singleton type initializer
   static TGenericClassInfo *GenerateInitInstanceLocal(const vector<string>*)
   {
      vector<string> *ptr = nullptr;
      static ::TVirtualIsAProxy* isa_proxy = new ::TIsAProxy(typeid(vector<string>));
      static ::ROOT::TGenericClassInfo 
         instance("vector<string>", -2, "vector", 389,
                  typeid(vector<string>), ::ROOT::Internal::DefineBehavior(ptr, ptr),
                  &vectorlEstringgR_Dictionary, isa_proxy, 0,
                  sizeof(vector<string>) );
      instance.SetNew(&new_vectorlEstringgR);
      instance.SetNewArray(&newArray_vectorlEstringgR);
      instance.SetDelete(&delete_vectorlEstringgR);
      instance.SetDeleteArray(&deleteArray_vectorlEstringgR);
      instance.SetDestructor(&destruct_vectorlEstringgR);
      instance.AdoptCollectionProxyInfo(TCollectionProxyInfo::Generate(TCollectionProxyInfo::Pushback< vector<string> >()));

      ::ROOT::AddClassAlternate("vector<string>","std::vector<std::__cxx11::basic_string<char, std::char_traits<char>, std::allocator<char> >, std::allocator<std::__cxx11::basic_string<char, std::char_traits<char>, std::allocator<char> > > >");
      return &instance;
   }
   // Static variable to force the class initialization
   static ::ROOT::TGenericClassInfo *_R__UNIQUE_DICT_(Init) = GenerateInitInstanceLocal(static_cast<const vector<string>*>(nullptr)); R__UseDummy(_R__UNIQUE_DICT_(Init));

   // Dictionary for non-ClassDef classes
   static TClass *vectorlEstringgR_Dictionary() {
      TClass* theClass =::ROOT::GenerateInitInstanceLocal(static_cast<const vector<string>*>(nullptr))->GetClass();
      vectorlEstringgR_TClassManip(theClass);
   return theClass;
   }

   static void vectorlEstringgR_TClassManip(TClass* ){
   }

} // end of namespace ROOT

namespace ROOT {
   // Wrappers around operator new
   static void *new_vectorlEstringgR(void *p) {
      return  p ? ::new((::ROOT::Internal::TOperatorNewHelper*)p) vector<string> : new vector<string>;
   }
   static void *newArray_vectorlEstringgR(Long_t nElements, void *p) {
      return p ? ::new((::ROOT::Internal::TOperatorNewHelper*)p) vector<string>[nElements] : new vector<string>[nElements];
   }
   // Wrapper around operator delete
   static void delete_vectorlEstringgR(void *p) {
      delete (static_cast<vector<string>*>(p));
   }
   static void deleteArray_vectorlEstringgR(void *p) {
      delete [] (static_cast<vector<string>*>(p));
   }
   static void destruct_vectorlEstringgR(void *p) {
      typedef vector<string> current_t;
      (static_cast<current_t*>(p))->~current_t();
   }
} // end of namespace ROOT for class vector<string>

namespace ROOT {
   static TClass *vectorlEdoublegR_Dictionary();
   static void vectorlEdoublegR_TClassManip(TClass*);
   static void *new_vectorlEdoublegR(void *p = nullptr);
   static void *newArray_vectorlEdoublegR(Long_t size, void *p);
   static void delete_vectorlEdoublegR(void *p);
   static void deleteArray_vectorlEdoublegR(void *p);
   static void destruct_vectorlEdoublegR(void *p);

   // Function generating the singleton type initializer
   static TGenericClassInfo *GenerateInitInstanceLocal(const vector<double>*)
   {
      vector<double> *ptr = nullptr;
      static ::TVirtualIsAProxy* isa_proxy = new ::TIsAProxy(typeid(vector<double>));
      static ::ROOT::TGenericClassInfo 
         instance("vector<double>", -2, "vector", 389,
                  typeid(vector<double>), ::ROOT::Internal::DefineBehavior(ptr, ptr),
                  &vectorlEdoublegR_Dictionary, isa_proxy, 0,
                  sizeof(vector<double>) );
      instance.SetNew(&new_vectorlEdoublegR);
      instance.SetNewArray(&newArray_vectorlEdoublegR);
      instance.SetDelete(&delete_vectorlEdoublegR);
      instance.SetDeleteArray(&deleteArray_vectorlEdoublegR);
      instance.SetDestructor(&destruct_vectorlEdoublegR);
      instance.AdoptCollectionProxyInfo(TCollectionProxyInfo::Generate(TCollectionProxyInfo::Pushback< vector<double> >()));

      ::ROOT::AddClassAlternate("vector<double>","std::vector<double, std::allocator<double> >");
      return &instance;
   }
   // Static variable to force the class initialization
   static ::ROOT::TGenericClassInfo *_R__UNIQUE_DICT_(Init) = GenerateInitInstanceLocal(static_cast<const vector<double>*>(nullptr)); R__UseDummy(_R__UNIQUE_DICT_(Init));

   // Dictionary for non-ClassDef classes
   static TClass *vectorlEdoublegR_Dictionary() {
      TClass* theClass =::ROOT::GenerateInitInstanceLocal(static_cast<const vector<double>*>(nullptr))->GetClass();
      vectorlEdoublegR_TClassManip(theClass);
   return theClass;
   }

   static void vectorlEdoublegR_TClassManip(TClass* ){
   }

} // end of namespace ROOT

namespace ROOT {
   // Wrappers around operator new
   static void *new_vectorlEdoublegR(void *p) {
      return  p ? ::new((::ROOT::Internal::TOperatorNewHelper*)p) vector<double> : new vector<double>;
   }
   static void *newArray_vectorlEdoublegR(Long_t nElements, void *p) {
      return p ? ::new((::ROOT::Internal::TOperatorNewHelper*)p) vector<double>[nElements] : new vector<double>[nElements];
   }
   // Wrapper around operator delete
   static void delete_vectorlEdoublegR(void *p) {
      delete (static_cast<vector<double>*>(p));
   }
   static void deleteArray_vectorlEdoublegR(void *p) {
      delete [] (static_cast<vector<double>*>(p));
   }
   static void destruct_vectorlEdoublegR(void *p) {
      typedef vector<double> current_t;
      (static_cast<current_t*>(p))->~current_t();
   }
} // end of namespace ROOT for class vector<double>

namespace ROOT {
   static TClass *vectorlETofSignalgR_Dictionary();
   static void vectorlETofSignalgR_TClassManip(TClass*);
   static void *new_vectorlETofSignalgR(void *p = nullptr);
   static void *newArray_vectorlETofSignalgR(Long_t size, void *p);
   static void delete_vectorlETofSignalgR(void *p);
   static void deleteArray_vectorlETofSignalgR(void *p);
   static void destruct_vectorlETofSignalgR(void *p);

   // Function generating the singleton type initializer
   static TGenericClassInfo *GenerateInitInstanceLocal(const vector<TofSignal>*)
   {
      vector<TofSignal> *ptr = nullptr;
      static ::TVirtualIsAProxy* isa_proxy = new ::TIsAProxy(typeid(vector<TofSignal>));
      static ::ROOT::TGenericClassInfo 
         instance("vector<TofSignal>", -2, "vector", 389,
                  typeid(vector<TofSignal>), ::ROOT::Internal::DefineBehavior(ptr, ptr),
                  &vectorlETofSignalgR_Dictionary, isa_proxy, 4,
                  sizeof(vector<TofSignal>) );
      instance.SetNew(&new_vectorlETofSignalgR);
      instance.SetNewArray(&newArray_vectorlETofSignalgR);
      instance.SetDelete(&delete_vectorlETofSignalgR);
      instance.SetDeleteArray(&deleteArray_vectorlETofSignalgR);
      instance.SetDestructor(&destruct_vectorlETofSignalgR);
      instance.AdoptCollectionProxyInfo(TCollectionProxyInfo::Generate(TCollectionProxyInfo::Pushback< vector<TofSignal> >()));

      ::ROOT::AddClassAlternate("vector<TofSignal>","std::vector<TofSignal, std::allocator<TofSignal> >");
      return &instance;
   }
   // Static variable to force the class initialization
   static ::ROOT::TGenericClassInfo *_R__UNIQUE_DICT_(Init) = GenerateInitInstanceLocal(static_cast<const vector<TofSignal>*>(nullptr)); R__UseDummy(_R__UNIQUE_DICT_(Init));

   // Dictionary for non-ClassDef classes
   static TClass *vectorlETofSignalgR_Dictionary() {
      TClass* theClass =::ROOT::GenerateInitInstanceLocal(static_cast<const vector<TofSignal>*>(nullptr))->GetClass();
      vectorlETofSignalgR_TClassManip(theClass);
   return theClass;
   }

   static void vectorlETofSignalgR_TClassManip(TClass* ){
   }

} // end of namespace ROOT

namespace ROOT {
   // Wrappers around operator new
   static void *new_vectorlETofSignalgR(void *p) {
      return  p ? ::new((::ROOT::Internal::TOperatorNewHelper*)p) vector<TofSignal> : new vector<TofSignal>;
   }
   static void *newArray_vectorlETofSignalgR(Long_t nElements, void *p) {
      return p ? ::new((::ROOT::Internal::TOperatorNewHelper*)p) vector<TofSignal>[nElements] : new vector<TofSignal>[nElements];
   }
   // Wrapper around operator delete
   static void delete_vectorlETofSignalgR(void *p) {
      delete (static_cast<vector<TofSignal>*>(p));
   }
   static void deleteArray_vectorlETofSignalgR(void *p) {
      delete [] (static_cast<vector<TofSignal>*>(p));
   }
   static void destruct_vectorlETofSignalgR(void *p) {
      typedef vector<TofSignal> current_t;
      (static_cast<current_t*>(p))->~current_t();
   }
} // end of namespace ROOT for class vector<TofSignal>

namespace ROOT {
   static TClass *vectorlETofHitgR_Dictionary();
   static void vectorlETofHitgR_TClassManip(TClass*);
   static void *new_vectorlETofHitgR(void *p = nullptr);
   static void *newArray_vectorlETofHitgR(Long_t size, void *p);
   static void delete_vectorlETofHitgR(void *p);
   static void deleteArray_vectorlETofHitgR(void *p);
   static void destruct_vectorlETofHitgR(void *p);

   // Function generating the singleton type initializer
   static TGenericClassInfo *GenerateInitInstanceLocal(const vector<TofHit>*)
   {
      vector<TofHit> *ptr = nullptr;
      static ::TVirtualIsAProxy* isa_proxy = new ::TIsAProxy(typeid(vector<TofHit>));
      static ::ROOT::TGenericClassInfo 
         instance("vector<TofHit>", -2, "vector", 389,
                  typeid(vector<TofHit>), ::ROOT::Internal::DefineBehavior(ptr, ptr),
                  &vectorlETofHitgR_Dictionary, isa_proxy, 4,
                  sizeof(vector<TofHit>) );
      instance.SetNew(&new_vectorlETofHitgR);
      instance.SetNewArray(&newArray_vectorlETofHitgR);
      instance.SetDelete(&delete_vectorlETofHitgR);
      instance.SetDeleteArray(&deleteArray_vectorlETofHitgR);
      instance.SetDestructor(&destruct_vectorlETofHitgR);
      instance.AdoptCollectionProxyInfo(TCollectionProxyInfo::Generate(TCollectionProxyInfo::Pushback< vector<TofHit> >()));

      ::ROOT::AddClassAlternate("vector<TofHit>","std::vector<TofHit, std::allocator<TofHit> >");
      return &instance;
   }
   // Static variable to force the class initialization
   static ::ROOT::TGenericClassInfo *_R__UNIQUE_DICT_(Init) = GenerateInitInstanceLocal(static_cast<const vector<TofHit>*>(nullptr)); R__UseDummy(_R__UNIQUE_DICT_(Init));

   // Dictionary for non-ClassDef classes
   static TClass *vectorlETofHitgR_Dictionary() {
      TClass* theClass =::ROOT::GenerateInitInstanceLocal(static_cast<const vector<TofHit>*>(nullptr))->GetClass();
      vectorlETofHitgR_TClassManip(theClass);
   return theClass;
   }

   static void vectorlETofHitgR_TClassManip(TClass* ){
   }

} // end of namespace ROOT

namespace ROOT {
   // Wrappers around operator new
   static void *new_vectorlETofHitgR(void *p) {
      return  p ? ::new((::ROOT::Internal::TOperatorNewHelper*)p) vector<TofHit> : new vector<TofHit>;
   }
   static void *newArray_vectorlETofHitgR(Long_t nElements, void *p) {
      return p ? ::new((::ROOT::Internal::TOperatorNewHelper*)p) vector<TofHit>[nElements] : new vector<TofHit>[nElements];
   }
   // Wrapper around operator delete
   static void delete_vectorlETofHitgR(void *p) {
      delete (static_cast<vector<TofHit>*>(p));
   }
   static void deleteArray_vectorlETofHitgR(void *p) {
      delete [] (static_cast<vector<TofHit>*>(p));
   }
   static void destruct_vectorlETofHitgR(void *p) {
      typedef vector<TofHit> current_t;
      (static_cast<current_t*>(p))->~current_t();
   }
} // end of namespace ROOT for class vector<TofHit>

namespace ROOT {
   static TClass *vectorlETofEventgR_Dictionary();
   static void vectorlETofEventgR_TClassManip(TClass*);
   static void *new_vectorlETofEventgR(void *p = nullptr);
   static void *newArray_vectorlETofEventgR(Long_t size, void *p);
   static void delete_vectorlETofEventgR(void *p);
   static void deleteArray_vectorlETofEventgR(void *p);
   static void destruct_vectorlETofEventgR(void *p);

   // Function generating the singleton type initializer
   static TGenericClassInfo *GenerateInitInstanceLocal(const vector<TofEvent>*)
   {
      vector<TofEvent> *ptr = nullptr;
      static ::TVirtualIsAProxy* isa_proxy = new ::TIsAProxy(typeid(vector<TofEvent>));
      static ::ROOT::TGenericClassInfo 
         instance("vector<TofEvent>", -2, "vector", 389,
                  typeid(vector<TofEvent>), ::ROOT::Internal::DefineBehavior(ptr, ptr),
                  &vectorlETofEventgR_Dictionary, isa_proxy, 4,
                  sizeof(vector<TofEvent>) );
      instance.SetNew(&new_vectorlETofEventgR);
      instance.SetNewArray(&newArray_vectorlETofEventgR);
      instance.SetDelete(&delete_vectorlETofEventgR);
      instance.SetDeleteArray(&deleteArray_vectorlETofEventgR);
      instance.SetDestructor(&destruct_vectorlETofEventgR);
      instance.AdoptCollectionProxyInfo(TCollectionProxyInfo::Generate(TCollectionProxyInfo::Pushback< vector<TofEvent> >()));

      ::ROOT::AddClassAlternate("vector<TofEvent>","std::vector<TofEvent, std::allocator<TofEvent> >");
      return &instance;
   }
   // Static variable to force the class initialization
   static ::ROOT::TGenericClassInfo *_R__UNIQUE_DICT_(Init) = GenerateInitInstanceLocal(static_cast<const vector<TofEvent>*>(nullptr)); R__UseDummy(_R__UNIQUE_DICT_(Init));

   // Dictionary for non-ClassDef classes
   static TClass *vectorlETofEventgR_Dictionary() {
      TClass* theClass =::ROOT::GenerateInitInstanceLocal(static_cast<const vector<TofEvent>*>(nullptr))->GetClass();
      vectorlETofEventgR_TClassManip(theClass);
   return theClass;
   }

   static void vectorlETofEventgR_TClassManip(TClass* ){
   }

} // end of namespace ROOT

namespace ROOT {
   // Wrappers around operator new
   static void *new_vectorlETofEventgR(void *p) {
      return  p ? ::new((::ROOT::Internal::TOperatorNewHelper*)p) vector<TofEvent> : new vector<TofEvent>;
   }
   static void *newArray_vectorlETofEventgR(Long_t nElements, void *p) {
      return p ? ::new((::ROOT::Internal::TOperatorNewHelper*)p) vector<TofEvent>[nElements] : new vector<TofEvent>[nElements];
   }
   // Wrapper around operator delete
   static void delete_vectorlETofEventgR(void *p) {
      delete (static_cast<vector<TofEvent>*>(p));
   }
   static void deleteArray_vectorlETofEventgR(void *p) {
      delete [] (static_cast<vector<TofEvent>*>(p));
   }
   static void destruct_vectorlETofEventgR(void *p) {
      typedef vector<TofEvent> current_t;
      (static_cast<current_t*>(p))->~current_t();
   }
} // end of namespace ROOT for class vector<TofEvent>

namespace {
  void TriggerDictionaryInitialization_TofObjectsDict_Impl() {
    static const char* headers[] = {
"../include/TofHit.h",
"../include/TofSignal.h",
"../include/TofEvent.h",
"../include/TofRun.h",
nullptr
    };
    static const char* includePaths[] = {
"/snap/root-framework/923/usr/local/include/",
"/home/emanuele/Documents/ToF/reconstruction/",
nullptr
    };
    static const char* fwdDeclCode = R"DICTFWDDCLS(
#line 1 "TofObjectsDict dictionary forward declarations' payload"
#pragma clang diagnostic ignored "-Wkeyword-compat"
#pragma clang diagnostic ignored "-Wignored-attributes"
#pragma clang diagnostic ignored "-Wreturn-type-c-linkage"
extern int __Cling_AutoLoading_Map;
class __attribute__((annotate("$clingAutoload$../include/TofHit.h")))  TofHit;
namespace std{template <typename _Tp> class __attribute__((annotate("$clingAutoload$bits/allocator.h")))  __attribute__((annotate("$clingAutoload$string")))  allocator;
}
class __attribute__((annotate("$clingAutoload$../include/TofSignal.h")))  TofSignal;
class __attribute__((annotate("$clingAutoload$../include/TofEvent.h")))  TofEvent;
class __attribute__((annotate("$clingAutoload$../include/TofRun.h")))  TofRun;
)DICTFWDDCLS";
    static const char* payloadCode = R"DICTPAYLOAD(
#line 1 "TofObjectsDict dictionary payload"


#define _BACKWARD_BACKWARD_WARNING_H
// Inline headers
#include "../include/TofHit.h"
#include "../include/TofSignal.h"
#include "../include/TofEvent.h"
#include "../include/TofRun.h"

#undef  _BACKWARD_BACKWARD_WARNING_H
)DICTPAYLOAD";
    static const char* classesHeaders[] = {
"TofEvent", payloadCode, "@",
"TofHit", payloadCode, "@",
"TofRun", payloadCode, "@",
"TofSignal", payloadCode, "@",
nullptr
};
    static bool isInitialized = false;
    if (!isInitialized) {
      TROOT::RegisterModule("TofObjectsDict",
        headers, includePaths, payloadCode, fwdDeclCode,
        TriggerDictionaryInitialization_TofObjectsDict_Impl, {}, classesHeaders, /*hasCxxModule*/false);
      isInitialized = true;
    }
  }
  static struct DictInit {
    DictInit() {
      TriggerDictionaryInitialization_TofObjectsDict_Impl();
    }
  } __TheDictionaryInitializer;
}
void TriggerDictionaryInitialization_TofObjectsDict() {
  TriggerDictionaryInitialization_TofObjectsDict_Impl();
}
