//
// File generated by /mnt/data/T2K/frozen/ROOT/v5r34p09n04/Linux-x86_64/bin/rootcint at Mon Oct  7 17:21:58 2019

// Do NOT change. Changes will be lost next time file is generated
//

#define R__DICTIONARY_FILENAME dOdOdIdictdIDocString_Dict
#include "RConfig.h" //rootcint 4834
#if !defined(R__ACCESS_IN_SYMBOL)
//Break the privacy of classes -- Disabled for the moment
#define private public
#define protected public
#endif

// Since CINT ignores the std namespace, we need to do so in this file.
namespace std {} using namespace std;
#include "DocString_Dict.h"

#include "TClass.h"
#include "TBuffer.h"
#include "TMemberInspector.h"
#include "TError.h"

#ifndef G__ROOT
#define G__ROOT
#endif

#include "RtypesImp.h"
#include "TIsAProxy.h"
#include "TFileMergeInfo.h"

// START OF SHADOWS

namespace ROOT {
   namespace Shadow {
   } // of namespace Shadow
} // of namespace ROOT
// END OF SHADOWS

namespace ROOT {
   void DocString_ShowMembers(void *obj, TMemberInspector &R__insp);
   static void *new_DocString(void *p = 0);
   static void *newArray_DocString(Long_t size, void *p);
   static void delete_DocString(void *p);
   static void deleteArray_DocString(void *p);
   static void destruct_DocString(void *p);

   // Function generating the singleton type initializer
   static TGenericClassInfo *GenerateInitInstanceLocal(const ::DocString*)
   {
      ::DocString *ptr = 0;
      static ::TVirtualIsAProxy* isa_proxy = new ::TInstrumentedIsAProxy< ::DocString >(0);
      static ::ROOT::TGenericClassInfo 
         instance("DocString", ::DocString::Class_Version(), "./../src/DocString.hxx", 16,
                  typeid(::DocString), DefineBehavior(ptr, ptr),
                  &::DocString::Dictionary, isa_proxy, 4,
                  sizeof(::DocString) );
      instance.SetNew(&new_DocString);
      instance.SetNewArray(&newArray_DocString);
      instance.SetDelete(&delete_DocString);
      instance.SetDeleteArray(&deleteArray_DocString);
      instance.SetDestructor(&destruct_DocString);
      return &instance;
   }
   TGenericClassInfo *GenerateInitInstance(const ::DocString*)
   {
      return GenerateInitInstanceLocal((::DocString*)0);
   }
   // Static variable to force the class initialization
   static ::ROOT::TGenericClassInfo *_R__UNIQUE_(Init) = GenerateInitInstanceLocal((const ::DocString*)0x0); R__UseDummy(_R__UNIQUE_(Init));
} // end of namespace ROOT

//______________________________________________________________________________
TClass *DocString::fgIsA = 0;  // static to hold class pointer

//______________________________________________________________________________
const char *DocString::Class_Name()
{
   return "DocString";
}

//______________________________________________________________________________
const char *DocString::ImplFileName()
{
   return ::ROOT::GenerateInitInstanceLocal((const ::DocString*)0x0)->GetImplFileName();
}

//______________________________________________________________________________
int DocString::ImplFileLine()
{
   return ::ROOT::GenerateInitInstanceLocal((const ::DocString*)0x0)->GetImplFileLine();
}

//______________________________________________________________________________
void DocString::Dictionary()
{
   fgIsA = ::ROOT::GenerateInitInstanceLocal((const ::DocString*)0x0)->GetClass();
}

//______________________________________________________________________________
TClass *DocString::Class()
{
   if (!fgIsA) fgIsA = ::ROOT::GenerateInitInstanceLocal((const ::DocString*)0x0)->GetClass();
   return fgIsA;
}

//______________________________________________________________________________
void DocString::Streamer(TBuffer &R__b)
{
   // Stream an object of class DocString.

   if (R__b.IsReading()) {
      R__b.ReadClassBuffer(DocString::Class(),this);
   } else {
      R__b.WriteClassBuffer(DocString::Class(),this);
   }
}

//______________________________________________________________________________
void DocString::ShowMembers(TMemberInspector &R__insp)
{
      // Inspect the data members of an object of class DocString.
      TClass *R__cl = ::DocString::IsA();
      if (R__cl || R__insp.IsA()) { }
      R__insp.Inspect(R__cl, R__insp.GetParent(), "_tree", (void*)&_tree);
      R__insp.InspectMember("string", (void*)&_tree, "_tree.", false);
      R__insp.Inspect(R__cl, R__insp.GetParent(), "_name", (void*)&_name);
      R__insp.InspectMember("string", (void*)&_name, "_name.", false);
      R__insp.Inspect(R__cl, R__insp.GetParent(), "_doc", (void*)&_doc);
      R__insp.InspectMember("string", (void*)&_doc, "_doc.", false);
      R__insp.Inspect(R__cl, R__insp.GetParent(), "_type", (void*)&_type);
      R__insp.InspectMember("string", (void*)&_type, "_type.", false);
      R__insp.Inspect(R__cl, R__insp.GetParent(), "_counter1", (void*)&_counter1);
      R__insp.InspectMember("string", (void*)&_counter1, "_counter1.", false);
      R__insp.Inspect(R__cl, R__insp.GetParent(), "_counter2", (void*)&_counter2);
      R__insp.InspectMember("string", (void*)&_counter2, "_counter2.", false);
      R__insp.Inspect(R__cl, R__insp.GetParent(), "_counter3", (void*)&_counter3);
      R__insp.InspectMember("string", (void*)&_counter3, "_counter3.", false);
      R__insp.Inspect(R__cl, R__insp.GetParent(), "_size1", &_size1);
      R__insp.Inspect(R__cl, R__insp.GetParent(), "_size2", &_size2);
      R__insp.Inspect(R__cl, R__insp.GetParent(), "_size3", &_size3);
      TObject::ShowMembers(R__insp);
}

namespace ROOT {
   // Wrappers around operator new
   static void *new_DocString(void *p) {
      return  p ? new(p) ::DocString : new ::DocString;
   }
   static void *newArray_DocString(Long_t nElements, void *p) {
      return p ? new(p) ::DocString[nElements] : new ::DocString[nElements];
   }
   // Wrapper around operator delete
   static void delete_DocString(void *p) {
      delete ((::DocString*)p);
   }
   static void deleteArray_DocString(void *p) {
      delete [] ((::DocString*)p);
   }
   static void destruct_DocString(void *p) {
      typedef ::DocString current_t;
      ((current_t*)p)->~current_t();
   }
} // end of namespace ROOT for class ::DocString

/********************************************************
* ../dict/DocString_Dict.cxx
* CAUTION: DON'T CHANGE THIS FILE. THIS FILE IS AUTOMATICALLY GENERATED
*          FROM HEADER FILES LISTED IN G__setup_cpp_environmentXXX().
*          CHANGE THOSE HEADER FILES AND REGENERATE THIS FILE.
********************************************************/

#ifdef G__MEMTEST
#undef malloc
#undef free
#endif

#if defined(__GNUC__) && __GNUC__ >= 4 && ((__GNUC_MINOR__ == 2 && __GNUC_PATCHLEVEL__ >= 1) || (__GNUC_MINOR__ >= 3))
#pragma GCC diagnostic ignored "-Wstrict-aliasing"
#endif

extern "C" void G__cpp_reset_tagtableDocString_Dict();

extern "C" void G__set_cpp_environmentDocString_Dict() {
  G__cpp_reset_tagtableDocString_Dict();
}
#include <new>
extern "C" int G__cpp_dllrevDocString_Dict() { return(30051515); }

/*********************************************************
* Member function Interface Method
*********************************************************/

/* DocString */
static int G__DocString_Dict_168_0_1(G__value* result7, G__CONST char* funcname, struct G__param* libp, int hash)
{
   DocString* p = NULL;
   char* gvp = (char*) G__getgvp();
   int n = G__getaryconstruct();
   if (n) {
     if ((gvp == (char*)G__PVOID) || (gvp == 0)) {
       p = new DocString[n];
     } else {
       p = new((void*) gvp) DocString[n];
     }
   } else {
     if ((gvp == (char*)G__PVOID) || (gvp == 0)) {
       p = new DocString;
     } else {
       p = new((void*) gvp) DocString;
     }
   }
   result7->obj.i = (long) p;
   result7->ref = (long) p;
   G__set_tagnum(result7,G__get_linked_tagnum(&G__DocString_DictLN_DocString));
   return(1 || funcname || hash || result7 || libp) ;
}

static int G__DocString_Dict_168_0_2(G__value* result7, G__CONST char* funcname, struct G__param* libp, int hash)
{
   DocString* p = NULL;
   char* gvp = (char*) G__getgvp();
   //m: 10
   if ((gvp == (char*)G__PVOID) || (gvp == 0)) {
     p = new DocString(
*(string*) libp->para[0].ref, *(string*) libp->para[1].ref
, *(string*) libp->para[2].ref, *(string*) libp->para[3].ref
, (int) G__int(libp->para[4]), *(string*) libp->para[5].ref
, (int) G__int(libp->para[6]), *(string*) libp->para[7].ref
, (int) G__int(libp->para[8]), *(string*) libp->para[9].ref);
   } else {
     p = new((void*) gvp) DocString(
*(string*) libp->para[0].ref, *(string*) libp->para[1].ref
, *(string*) libp->para[2].ref, *(string*) libp->para[3].ref
, (int) G__int(libp->para[4]), *(string*) libp->para[5].ref
, (int) G__int(libp->para[6]), *(string*) libp->para[7].ref
, (int) G__int(libp->para[8]), *(string*) libp->para[9].ref);
   }
   result7->obj.i = (long) p;
   result7->ref = (long) p;
   G__set_tagnum(result7,G__get_linked_tagnum(&G__DocString_DictLN_DocString));
   return(1 || funcname || hash || result7 || libp) ;
}

static int G__DocString_Dict_168_0_3(G__value* result7, G__CONST char* funcname, struct G__param* libp, int hash)
{
      ((DocString*) G__getstructoffset())->Explain();
      G__setnull(result7);
   return(1 || funcname || hash || result7 || libp) ;
}

static int G__DocString_Dict_168_0_4(G__value* result7, G__CONST char* funcname, struct G__param* libp, int hash)
{
      {
         string* pobj;
         string xobj = ((DocString*) G__getstructoffset())->Name();
         pobj = new string(xobj);
         result7->obj.i = (long) ((void*) pobj);
         result7->ref = result7->obj.i;
         G__store_tempobject(*result7);
      }
   return(1 || funcname || hash || result7 || libp) ;
}

static int G__DocString_Dict_168_0_5(G__value* result7, G__CONST char* funcname, struct G__param* libp, int hash)
{
      {
         string* pobj;
         string xobj = ((DocString*) G__getstructoffset())->Tree();
         pobj = new string(xobj);
         result7->obj.i = (long) ((void*) pobj);
         result7->ref = result7->obj.i;
         G__store_tempobject(*result7);
      }
   return(1 || funcname || hash || result7 || libp) ;
}

static int G__DocString_Dict_168_0_6(G__value* result7, G__CONST char* funcname, struct G__param* libp, int hash)
{
      G__letint(result7, 85, (long) DocString::Class());
   return(1 || funcname || hash || result7 || libp) ;
}

static int G__DocString_Dict_168_0_7(G__value* result7, G__CONST char* funcname, struct G__param* libp, int hash)
{
      G__letint(result7, 67, (long) DocString::Class_Name());
   return(1 || funcname || hash || result7 || libp) ;
}

static int G__DocString_Dict_168_0_8(G__value* result7, G__CONST char* funcname, struct G__param* libp, int hash)
{
      G__letint(result7, 115, (long) DocString::Class_Version());
   return(1 || funcname || hash || result7 || libp) ;
}

static int G__DocString_Dict_168_0_9(G__value* result7, G__CONST char* funcname, struct G__param* libp, int hash)
{
      DocString::Dictionary();
      G__setnull(result7);
   return(1 || funcname || hash || result7 || libp) ;
}

static int G__DocString_Dict_168_0_13(G__value* result7, G__CONST char* funcname, struct G__param* libp, int hash)
{
      ((DocString*) G__getstructoffset())->StreamerNVirtual(*(TBuffer*) libp->para[0].ref);
      G__setnull(result7);
   return(1 || funcname || hash || result7 || libp) ;
}

static int G__DocString_Dict_168_0_14(G__value* result7, G__CONST char* funcname, struct G__param* libp, int hash)
{
      G__letint(result7, 67, (long) DocString::DeclFileName());
   return(1 || funcname || hash || result7 || libp) ;
}

static int G__DocString_Dict_168_0_15(G__value* result7, G__CONST char* funcname, struct G__param* libp, int hash)
{
      G__letint(result7, 105, (long) DocString::ImplFileLine());
   return(1 || funcname || hash || result7 || libp) ;
}

static int G__DocString_Dict_168_0_16(G__value* result7, G__CONST char* funcname, struct G__param* libp, int hash)
{
      G__letint(result7, 67, (long) DocString::ImplFileName());
   return(1 || funcname || hash || result7 || libp) ;
}

static int G__DocString_Dict_168_0_17(G__value* result7, G__CONST char* funcname, struct G__param* libp, int hash)
{
      G__letint(result7, 105, (long) DocString::DeclFileLine());
   return(1 || funcname || hash || result7 || libp) ;
}

// automatic copy constructor
static int G__DocString_Dict_168_0_18(G__value* result7, G__CONST char* funcname, struct G__param* libp, int hash)

{
   DocString* p;
   void* tmp = (void*) G__int(libp->para[0]);
   p = new DocString(*(DocString*) tmp);
   result7->obj.i = (long) p;
   result7->ref = (long) p;
   G__set_tagnum(result7,G__get_linked_tagnum(&G__DocString_DictLN_DocString));
   return(1 || funcname || hash || result7 || libp) ;
}

// automatic destructor
typedef DocString G__TDocString;
static int G__DocString_Dict_168_0_19(G__value* result7, G__CONST char* funcname, struct G__param* libp, int hash)
{
   char* gvp = (char*) G__getgvp();
   long soff = G__getstructoffset();
   int n = G__getaryconstruct();
   //
   //has_a_delete: 1
   //has_own_delete1arg: 0
   //has_own_delete2arg: 0
   //
   if (!soff) {
     return(1);
   }
   if (n) {
     if (gvp == (char*)G__PVOID) {
       delete[] (DocString*) soff;
     } else {
       G__setgvp((long) G__PVOID);
       for (int i = n - 1; i >= 0; --i) {
         ((DocString*) (soff+(sizeof(DocString)*i)))->~G__TDocString();
       }
       G__setgvp((long)gvp);
     }
   } else {
     if (gvp == (char*)G__PVOID) {
       delete (DocString*) soff;
     } else {
       G__setgvp((long) G__PVOID);
       ((DocString*) (soff))->~G__TDocString();
       G__setgvp((long)gvp);
     }
   }
   G__setnull(result7);
   return(1 || funcname || hash || result7 || libp) ;
}

// automatic assignment operator
static int G__DocString_Dict_168_0_20(G__value* result7, G__CONST char* funcname, struct G__param* libp, int hash)
{
   DocString* dest = (DocString*) G__getstructoffset();
   *dest = *(DocString*) libp->para[0].ref;
   const DocString& obj = *dest;
   result7->ref = (long) (&obj);
   result7->obj.i = (long) (&obj);
   return(1 || funcname || hash || result7 || libp) ;
}


/* Setting up global function */

/*********************************************************
* Member function Stub
*********************************************************/

/* DocString */

/*********************************************************
* Global function Stub
*********************************************************/

/*********************************************************
* Get size of pointer to member function
*********************************************************/
class G__Sizep2memfuncDocString_Dict {
 public:
  G__Sizep2memfuncDocString_Dict(): p(&G__Sizep2memfuncDocString_Dict::sizep2memfunc) {}
    size_t sizep2memfunc() { return(sizeof(p)); }
  private:
    size_t (G__Sizep2memfuncDocString_Dict::*p)();
};

size_t G__get_sizep2memfuncDocString_Dict()
{
  G__Sizep2memfuncDocString_Dict a;
  G__setsizep2memfunc((int)a.sizep2memfunc());
  return((size_t)a.sizep2memfunc());
}


/*********************************************************
* virtual base class offset calculation interface
*********************************************************/

   /* Setting up class inheritance */

/*********************************************************
* Inheritance information setup/
*********************************************************/
extern "C" void G__cpp_setup_inheritanceDocString_Dict() {

   /* Setting up class inheritance */
   if(0==G__getnumbaseclass(G__get_linked_tagnum(&G__DocString_DictLN_DocString))) {
     DocString *G__Lderived;
     G__Lderived=(DocString*)0x1000;
     {
       TObject *G__Lpbase=(TObject*)G__Lderived;
       G__inheritance_setup(G__get_linked_tagnum(&G__DocString_DictLN_DocString),G__get_linked_tagnum(&G__DocString_DictLN_TObject),(long)G__Lpbase-(long)G__Lderived,1,1);
     }
   }
}

/*********************************************************
* typedef information setup/
*********************************************************/
extern "C" void G__cpp_setup_typetableDocString_Dict() {

   /* Setting up typedef entry */
   G__search_typename2("Version_t",115,-1,0,-1);
   G__setnewtype(-1,"Class version identifier (short)",0);
   G__search_typename2("vector<ROOT::TSchemaHelper>",117,G__get_linked_tagnum(&G__DocString_DictLN_vectorlEROOTcLcLTSchemaHelpercOallocatorlEROOTcLcLTSchemaHelpergRsPgR),0,-1);
   G__setnewtype(-1,NULL,0);
   G__search_typename2("reverse_iterator<const_iterator>",117,G__get_linked_tagnum(&G__DocString_DictLN_reverse_iteratorlEvectorlEROOTcLcLTSchemaHelpercOallocatorlEROOTcLcLTSchemaHelpergRsPgRcLcLiteratorgR),0,G__get_linked_tagnum(&G__DocString_DictLN_vectorlEROOTcLcLTSchemaHelpercOallocatorlEROOTcLcLTSchemaHelpergRsPgR));
   G__setnewtype(-1,NULL,0);
   G__search_typename2("reverse_iterator<iterator>",117,G__get_linked_tagnum(&G__DocString_DictLN_reverse_iteratorlEvectorlEROOTcLcLTSchemaHelpercOallocatorlEROOTcLcLTSchemaHelpergRsPgRcLcLiteratorgR),0,G__get_linked_tagnum(&G__DocString_DictLN_vectorlEROOTcLcLTSchemaHelpercOallocatorlEROOTcLcLTSchemaHelpergRsPgR));
   G__setnewtype(-1,NULL,0);
   G__search_typename2("vector<TVirtualArray*>",117,G__get_linked_tagnum(&G__DocString_DictLN_vectorlETVirtualArraymUcOallocatorlETVirtualArraymUgRsPgR),0,-1);
   G__setnewtype(-1,NULL,0);
   G__search_typename2("reverse_iterator<const_iterator>",117,G__get_linked_tagnum(&G__DocString_DictLN_reverse_iteratorlEvectorlETVirtualArraymUcOallocatorlETVirtualArraymUgRsPgRcLcLiteratorgR),0,G__get_linked_tagnum(&G__DocString_DictLN_vectorlETVirtualArraymUcOallocatorlETVirtualArraymUgRsPgR));
   G__setnewtype(-1,NULL,0);
   G__search_typename2("reverse_iterator<iterator>",117,G__get_linked_tagnum(&G__DocString_DictLN_reverse_iteratorlEvectorlETVirtualArraymUcOallocatorlETVirtualArraymUgRsPgRcLcLiteratorgR),0,G__get_linked_tagnum(&G__DocString_DictLN_vectorlETVirtualArraymUcOallocatorlETVirtualArraymUgRsPgR));
   G__setnewtype(-1,NULL,0);
}

/*********************************************************
* Data Member information setup/
*********************************************************/

   /* Setting up class,struct,union tag member variable */

   /* DocString */
static void G__setup_memvarDocString(void) {
   G__tag_memvar_setup(G__get_linked_tagnum(&G__DocString_DictLN_DocString));
   { DocString *p; p=(DocString*)0x1000; if (p) { }
   G__memvar_setup((void*)0,85,0,0,G__get_linked_tagnum(&G__DocString_DictLN_TClass),-1,-2,4,"fgIsA=",0,(char*)NULL);
   G__memvar_setup((void*)0,117,0,0,G__get_linked_tagnum(&G__DocString_DictLN_string),-1,-1,2,"_tree=",0,(char*)NULL);
   G__memvar_setup((void*)0,117,0,0,G__get_linked_tagnum(&G__DocString_DictLN_string),-1,-1,2,"_name=",0,(char*)NULL);
   G__memvar_setup((void*)0,117,0,0,G__get_linked_tagnum(&G__DocString_DictLN_string),-1,-1,2,"_doc=",0,(char*)NULL);
   G__memvar_setup((void*)0,117,0,0,G__get_linked_tagnum(&G__DocString_DictLN_string),-1,-1,2,"_type=",0,(char*)NULL);
   G__memvar_setup((void*)0,117,0,0,G__get_linked_tagnum(&G__DocString_DictLN_string),-1,-1,2,"_counter1=",0,(char*)NULL);
   G__memvar_setup((void*)0,117,0,0,G__get_linked_tagnum(&G__DocString_DictLN_string),-1,-1,2,"_counter2=",0,(char*)NULL);
   G__memvar_setup((void*)0,117,0,0,G__get_linked_tagnum(&G__DocString_DictLN_string),-1,-1,2,"_counter3=",0,(char*)NULL);
   G__memvar_setup((void*)0,105,0,0,-1,-1,-1,2,"_size1=",0,(char*)NULL);
   G__memvar_setup((void*)0,105,0,0,-1,-1,-1,2,"_size2=",0,(char*)NULL);
   G__memvar_setup((void*)0,105,0,0,-1,-1,-1,2,"_size3=",0,(char*)NULL);
   }
   G__tag_memvar_reset();
}

extern "C" void G__cpp_setup_memvarDocString_Dict() {
}
/***********************************************************
************************************************************
************************************************************
************************************************************
************************************************************
************************************************************
************************************************************
***********************************************************/

/*********************************************************
* Member function information setup for each class
*********************************************************/
static void G__setup_memfuncDocString(void) {
   /* DocString */
   G__tag_memfunc_setup(G__get_linked_tagnum(&G__DocString_DictLN_DocString));
   G__memfunc_setup("DocString",909,G__DocString_Dict_168_0_1, 105, G__get_linked_tagnum(&G__DocString_DictLN_DocString), -1, 0, 0, 1, 1, 0, "", (char*)NULL, (void*) NULL, 0);
   G__memfunc_setup("DocString",909,G__DocString_Dict_168_0_2, 105, G__get_linked_tagnum(&G__DocString_DictLN_DocString), -1, 0, 10, 1, 1, 0, 
"u 'string' - 11 - tree u 'string' - 11 - name "
"u 'string' - 11 - doc u 'string' - 11 - type "
"i - - 0 - size1 u 'string' - 11 - counter1 "
"i - - 0 - size2 u 'string' - 11 - counter2 "
"i - - 0 - size3 u 'string' - 11 - counter3", (char*)NULL, (void*) NULL, 0);
   G__memfunc_setup("Explain",721,G__DocString_Dict_168_0_3, 121, -1, -1, 0, 0, 1, 1, 0, "", (char*)NULL, (void*) NULL, 0);
   G__memfunc_setup("Name",385,G__DocString_Dict_168_0_4, 117, G__get_linked_tagnum(&G__DocString_DictLN_string), -1, 0, 0, 1, 1, 0, "", (char*)NULL, (void*) NULL, 0);
   G__memfunc_setup("Tree",400,G__DocString_Dict_168_0_5, 117, G__get_linked_tagnum(&G__DocString_DictLN_string), -1, 0, 0, 1, 1, 0, "", (char*)NULL, (void*) NULL, 0);
   G__memfunc_setup("Class",502,G__DocString_Dict_168_0_6, 85, G__get_linked_tagnum(&G__DocString_DictLN_TClass), -1, 0, 0, 3, 1, 0, "", (char*)NULL, (void*) G__func2void( (TClass* (*)())(&DocString::Class) ), 0);
   G__memfunc_setup("Class_Name",982,G__DocString_Dict_168_0_7, 67, -1, -1, 0, 0, 3, 1, 1, "", (char*)NULL, (void*) G__func2void( (const char* (*)())(&DocString::Class_Name) ), 0);
   G__memfunc_setup("Class_Version",1339,G__DocString_Dict_168_0_8, 115, -1, G__defined_typename("Version_t"), 0, 0, 3, 1, 0, "", (char*)NULL, (void*) G__func2void( (Version_t (*)())(&DocString::Class_Version) ), 0);
   G__memfunc_setup("Dictionary",1046,G__DocString_Dict_168_0_9, 121, -1, -1, 0, 0, 3, 1, 0, "", (char*)NULL, (void*) G__func2void( (void (*)())(&DocString::Dictionary) ), 0);
   G__memfunc_setup("IsA",253,(G__InterfaceMethod) NULL,85, G__get_linked_tagnum(&G__DocString_DictLN_TClass), -1, 0, 0, 1, 1, 8, "", (char*)NULL, (void*) NULL, 1);
   G__memfunc_setup("ShowMembers",1132,(G__InterfaceMethod) NULL,121, -1, -1, 0, 1, 1, 1, 0, "u 'TMemberInspector' - 1 - -", (char*)NULL, (void*) NULL, 1);
   G__memfunc_setup("Streamer",835,(G__InterfaceMethod) NULL,121, -1, -1, 0, 1, 1, 1, 0, "u 'TBuffer' - 1 - -", (char*)NULL, (void*) NULL, 1);
   G__memfunc_setup("StreamerNVirtual",1656,G__DocString_Dict_168_0_13, 121, -1, -1, 0, 1, 1, 1, 0, "u 'TBuffer' - 1 - ClassDef_StreamerNVirtual_b", (char*)NULL, (void*) NULL, 0);
   G__memfunc_setup("DeclFileName",1145,G__DocString_Dict_168_0_14, 67, -1, -1, 0, 0, 3, 1, 1, "", (char*)NULL, (void*) G__func2void( (const char* (*)())(&DocString::DeclFileName) ), 0);
   G__memfunc_setup("ImplFileLine",1178,G__DocString_Dict_168_0_15, 105, -1, -1, 0, 0, 3, 1, 0, "", (char*)NULL, (void*) G__func2void( (int (*)())(&DocString::ImplFileLine) ), 0);
   G__memfunc_setup("ImplFileName",1171,G__DocString_Dict_168_0_16, 67, -1, -1, 0, 0, 3, 1, 1, "", (char*)NULL, (void*) G__func2void( (const char* (*)())(&DocString::ImplFileName) ), 0);
   G__memfunc_setup("DeclFileLine",1152,G__DocString_Dict_168_0_17, 105, -1, -1, 0, 0, 3, 1, 0, "", (char*)NULL, (void*) G__func2void( (int (*)())(&DocString::DeclFileLine) ), 0);
   // automatic copy constructor
   G__memfunc_setup("DocString", 909, G__DocString_Dict_168_0_18, (int) ('i'), G__get_linked_tagnum(&G__DocString_DictLN_DocString), -1, 0, 1, 1, 1, 0, "u 'DocString' - 11 - -", (char*) NULL, (void*) NULL, 0);
   // automatic destructor
   G__memfunc_setup("~DocString", 1035, G__DocString_Dict_168_0_19, (int) ('y'), -1, -1, 0, 0, 1, 1, 0, "", (char*) NULL, (void*) NULL, 1);
   // automatic assignment operator
   G__memfunc_setup("operator=", 937, G__DocString_Dict_168_0_20, (int) ('u'), G__get_linked_tagnum(&G__DocString_DictLN_DocString), -1, 1, 1, 1, 1, 0, "u 'DocString' - 11 - -", (char*) NULL, (void*) NULL, 0);
   G__tag_memfunc_reset();
}


/*********************************************************
* Member function information setup
*********************************************************/
extern "C" void G__cpp_setup_memfuncDocString_Dict() {
}

/*********************************************************
* Global variable information setup for each class
*********************************************************/
static void G__cpp_setup_global0() {

   /* Setting up global variables */
   G__resetplocal();

}

static void G__cpp_setup_global1() {

   G__resetglobalenv();
}
extern "C" void G__cpp_setup_globalDocString_Dict() {
  G__cpp_setup_global0();
  G__cpp_setup_global1();
}

/*********************************************************
* Global function information setup for each class
*********************************************************/
static void G__cpp_setup_func0() {
   G__lastifuncposition();

}

static void G__cpp_setup_func1() {
}

static void G__cpp_setup_func2() {
}

static void G__cpp_setup_func3() {
}

static void G__cpp_setup_func4() {
}

static void G__cpp_setup_func5() {
}

static void G__cpp_setup_func6() {
}

static void G__cpp_setup_func7() {
}

static void G__cpp_setup_func8() {
}

static void G__cpp_setup_func9() {
}

static void G__cpp_setup_func10() {
}

static void G__cpp_setup_func11() {
}

static void G__cpp_setup_func12() {

   G__resetifuncposition();
}

extern "C" void G__cpp_setup_funcDocString_Dict() {
  G__cpp_setup_func0();
  G__cpp_setup_func1();
  G__cpp_setup_func2();
  G__cpp_setup_func3();
  G__cpp_setup_func4();
  G__cpp_setup_func5();
  G__cpp_setup_func6();
  G__cpp_setup_func7();
  G__cpp_setup_func8();
  G__cpp_setup_func9();
  G__cpp_setup_func10();
  G__cpp_setup_func11();
  G__cpp_setup_func12();
}

/*********************************************************
* Class,struct,union,enum tag information setup
*********************************************************/
/* Setup class/struct taginfo */
G__linked_taginfo G__DocString_DictLN_TClass = { "TClass" , 99 , -1 };
G__linked_taginfo G__DocString_DictLN_TBuffer = { "TBuffer" , 99 , -1 };
G__linked_taginfo G__DocString_DictLN_TMemberInspector = { "TMemberInspector" , 99 , -1 };
G__linked_taginfo G__DocString_DictLN_TObject = { "TObject" , 99 , -1 };
G__linked_taginfo G__DocString_DictLN_string = { "string" , 99 , -1 };
G__linked_taginfo G__DocString_DictLN_vectorlEROOTcLcLTSchemaHelpercOallocatorlEROOTcLcLTSchemaHelpergRsPgR = { "vector<ROOT::TSchemaHelper,allocator<ROOT::TSchemaHelper> >" , 99 , -1 };
G__linked_taginfo G__DocString_DictLN_reverse_iteratorlEvectorlEROOTcLcLTSchemaHelpercOallocatorlEROOTcLcLTSchemaHelpergRsPgRcLcLiteratorgR = { "reverse_iterator<vector<ROOT::TSchemaHelper,allocator<ROOT::TSchemaHelper> >::iterator>" , 99 , -1 };
G__linked_taginfo G__DocString_DictLN_vectorlETVirtualArraymUcOallocatorlETVirtualArraymUgRsPgR = { "vector<TVirtualArray*,allocator<TVirtualArray*> >" , 99 , -1 };
G__linked_taginfo G__DocString_DictLN_reverse_iteratorlEvectorlETVirtualArraymUcOallocatorlETVirtualArraymUgRsPgRcLcLiteratorgR = { "reverse_iterator<vector<TVirtualArray*,allocator<TVirtualArray*> >::iterator>" , 99 , -1 };
G__linked_taginfo G__DocString_DictLN_DocString = { "DocString" , 99 , -1 };

/* Reset class/struct taginfo */
extern "C" void G__cpp_reset_tagtableDocString_Dict() {
  G__DocString_DictLN_TClass.tagnum = -1 ;
  G__DocString_DictLN_TBuffer.tagnum = -1 ;
  G__DocString_DictLN_TMemberInspector.tagnum = -1 ;
  G__DocString_DictLN_TObject.tagnum = -1 ;
  G__DocString_DictLN_string.tagnum = -1 ;
  G__DocString_DictLN_vectorlEROOTcLcLTSchemaHelpercOallocatorlEROOTcLcLTSchemaHelpergRsPgR.tagnum = -1 ;
  G__DocString_DictLN_reverse_iteratorlEvectorlEROOTcLcLTSchemaHelpercOallocatorlEROOTcLcLTSchemaHelpergRsPgRcLcLiteratorgR.tagnum = -1 ;
  G__DocString_DictLN_vectorlETVirtualArraymUcOallocatorlETVirtualArraymUgRsPgR.tagnum = -1 ;
  G__DocString_DictLN_reverse_iteratorlEvectorlETVirtualArraymUcOallocatorlETVirtualArraymUgRsPgRcLcLiteratorgR.tagnum = -1 ;
  G__DocString_DictLN_DocString.tagnum = -1 ;
}


extern "C" void G__cpp_setup_tagtableDocString_Dict() {

   /* Setting up class,struct,union tag entry */
   G__get_linked_tagnum_fwd(&G__DocString_DictLN_TClass);
   G__get_linked_tagnum_fwd(&G__DocString_DictLN_TBuffer);
   G__get_linked_tagnum_fwd(&G__DocString_DictLN_TMemberInspector);
   G__get_linked_tagnum_fwd(&G__DocString_DictLN_TObject);
   G__get_linked_tagnum_fwd(&G__DocString_DictLN_string);
   G__get_linked_tagnum_fwd(&G__DocString_DictLN_vectorlEROOTcLcLTSchemaHelpercOallocatorlEROOTcLcLTSchemaHelpergRsPgR);
   G__get_linked_tagnum_fwd(&G__DocString_DictLN_reverse_iteratorlEvectorlEROOTcLcLTSchemaHelpercOallocatorlEROOTcLcLTSchemaHelpergRsPgRcLcLiteratorgR);
   G__get_linked_tagnum_fwd(&G__DocString_DictLN_vectorlETVirtualArraymUcOallocatorlETVirtualArraymUgRsPgR);
   G__get_linked_tagnum_fwd(&G__DocString_DictLN_reverse_iteratorlEvectorlETVirtualArraymUcOallocatorlETVirtualArraymUgRsPgRcLcLiteratorgR);
   G__tagtable_setup(G__get_linked_tagnum_fwd(&G__DocString_DictLN_DocString),sizeof(DocString),-1,324864,(char*)NULL,G__setup_memvarDocString,G__setup_memfuncDocString);
}
extern "C" void G__cpp_setupDocString_Dict(void) {
  G__check_setup_version(30051515,"G__cpp_setupDocString_Dict()");
  G__set_cpp_environmentDocString_Dict();
  G__cpp_setup_tagtableDocString_Dict();

  G__cpp_setup_inheritanceDocString_Dict();

  G__cpp_setup_typetableDocString_Dict();

  G__cpp_setup_memvarDocString_Dict();

  G__cpp_setup_memfuncDocString_Dict();
  G__cpp_setup_globalDocString_Dict();
  G__cpp_setup_funcDocString_Dict();

   if(0==G__getsizep2memfunc()) G__get_sizep2memfuncDocString_Dict();
  return;
}
class G__cpp_setup_initDocString_Dict {
  public:
    G__cpp_setup_initDocString_Dict() { G__add_setup_func("DocString_Dict",(G__incsetup)(&G__cpp_setupDocString_Dict)); G__call_setup_funcs(); }
   ~G__cpp_setup_initDocString_Dict() { G__remove_setup_func("DocString_Dict"); }
};
G__cpp_setup_initDocString_Dict G__cpp_setup_initializerDocString_Dict;

