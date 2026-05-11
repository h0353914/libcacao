// AUTO-GENERATED: 由 Ghidra MCP 批量反編譯 libcacao_client.so (ARM 32-bit)
// 原始庫: tools_Libcacao/refs/so_32/libcacao_client.so
// 反編譯工具: ghidra-mcp v4.3.0


// ====== 0000cd00: _GLOBAL__sub_I_ICacaoService.cpp ======
void _GLOBAL__sub_I_ICacaoService_cpp(void)

{
  String16 *this;
  
  this = *(String16 **)(DAT_0000cd28 + 0xcd0a);
  android::String16::String16(this,(char *)(DAT_0000cd2c + 0xcd0c));
  __cxa_atexit(*(undefined4 *)(DAT_0000cd30 + 0xcd1a),this,*(undefined4 *)(DAT_0000cd34 + 0xcd1c));
  return;
}

// ====== 0000cd38: _GLOBAL__sub_I_ICacaoProcessCallback.cpp ======
void _GLOBAL__sub_I_ICacaoProcessCallback_cpp(void)

{
  String16 *this;
  
  this = *(String16 **)(DAT_0000cd60 + 0xcd42);
  android::String16::String16(this,(char *)(DAT_0000cd64 + 0xcd44));
  __cxa_atexit(*(undefined4 *)(DAT_0000cd68 + 0xcd52),this,*(undefined4 *)(DAT_0000cd6c + 0xcd54));
  return;
}

// ====== 0000cd70: _GLOBAL__sub_I_ICacaoClient.cpp ======
void _GLOBAL__sub_I_ICacaoClient_cpp(void)

{
  String16 *this;
  
  this = *(String16 **)(DAT_0000cd98 + 0xcd7a);
  android::String16::String16(this,(char *)(DAT_0000cd9c + 0xcd7c));
  __cxa_atexit(*(undefined4 *)(DAT_0000cda0 + 0xcd8a),this,*(undefined4 *)(DAT_0000cda4 + 0xcd8c));
  return;
}

// ====== 0000cda8: _GLOBAL__sub_I_ICacao.cpp ======
void _GLOBAL__sub_I_ICacao_cpp(void)

{
  String16 *this;
  
  this = *(String16 **)(DAT_0000cdd0 + 0xcdb2);
  android::String16::String16(this,(char *)(DAT_0000cdd4 + 0xcdb4));
  __cxa_atexit(*(undefined4 *)(DAT_0000cdd8 + 0xcdc2),this,*(undefined4 *)(DAT_0000cddc + 0xcdc4));
  return;
}

// ====== 0000cde0: _GLOBAL__sub_I_Cacao.cpp ======
void _GLOBAL__sub_I_Cacao_cpp(void)

{
  int iVar1;
  undefined4 *puVar2;
  undefined4 *puVar3;
  undefined4 uVar4;
  pthread_mutex_t *__mutex;
  
  iVar1 = DAT_0000ce34;
  puVar3 = (undefined4 *)(DAT_0000ce38 + 0xcdee);
  puVar2 = *(undefined4 **)(DAT_0000ce30 + 0xcdec);
  *puVar2 = 0;
  uVar4 = *puVar3;
  __cxa_atexit(*(undefined4 *)(iVar1 + 0xcdf6),puVar2,uVar4);
  iVar1 = DAT_0000ce40;
  puVar2 = *(undefined4 **)(DAT_0000ce3c + 0xce06);
  *puVar2 = 0;
  __cxa_atexit(*(undefined4 *)(iVar1 + 0xce0e),puVar2,uVar4);
  __mutex = *(pthread_mutex_t **)(DAT_0000ce44 + 0xce1a);
  pthread_mutex_init(__mutex,(pthread_mutexattr_t *)0x0);
  __cxa_atexit(DAT_0000ce48 + 0xce2a,__mutex,uVar4);
  return;
}

// ====== 0000ce4c: _GLOBAL__sub_I_cacao_imagebuf.cpp ======
void _GLOBAL__sub_I_cacao_imagebuf_cpp(void)

{
  int iVar1;
  undefined4 *puVar2;
  undefined4 *puVar3;
  undefined4 uVar4;
  
  iVar1 = DAT_0000ce88;
  puVar3 = (undefined4 *)(DAT_0000ce8c + 0xce5a);
  puVar2 = *(undefined4 **)(DAT_0000ce84 + 0xce58);
  *puVar2 = 0;
  uVar4 = *puVar3;
  __cxa_atexit(*(undefined4 *)(iVar1 + 0xce62),puVar2,uVar4);
  iVar1 = DAT_0000ce94;
  puVar2 = *(undefined4 **)(DAT_0000ce90 + 0xce72);
  *puVar2 = 0;
  __cxa_atexit(*(undefined4 *)(iVar1 + 0xce7a),puVar2,uVar4);
  return;
}

// ====== 0000ce98: __on_dlclose ======
void __on_dlclose(void)

{
                    /* WARNING: Could not recover jumptable at 0x00013634. Too many branches */
                    /* WARNING: Treating indirect jump as call */
  (*(__cxa_atexit + DAT_00013638))(DAT_0000cea0 + 0xce9e);
  return;
}

// ====== 0000cea4: __atexit_handler_wrapper ======
void __atexit_handler_wrapper(code *UNRECOVERED_JUMPTABLE)

{
  if (UNRECOVERED_JUMPTABLE != (code *)0x0) {
                    /* WARNING: Could not recover jumptable at 0x0000cea6. Too many branches */
                    /* WARNING: Treating indirect jump as call */
    (*UNRECOVERED_JUMPTABLE)();
    return;
  }
  return;
}

// ====== 0000ceac: atexit ======
int atexit(__func *__func)

{
  int iVar1;
  
  iVar1 = __cxa_atexit(DAT_0000cebc + 0xceb6,__func,DAT_0000cec0 + 0xceb8);
  return iVar1;
}

// ====== 0000cec4: pthread_atfork ======
int pthread_atfork(__prepare *__prepare,__parent *__parent,__child *__child)

{
  int iVar1;
  
  iVar1 = (*(android::sp<android::ICacaoProcessCallback>::operator= + DAT_00013658))
                    (__prepare,__parent,__child,DAT_0000cecc + 0xceca);
  return iVar1;
}

// ====== 0000ced0: getInterfaceDescriptor ======
/* non-virtual thunk to android::BnInterface<android::ICacaoService>::getInterfaceDescriptor() const
    */

undefined4 __thiscall
android::BnInterface<android::ICacaoService>::getInterfaceDescriptor
          (BnInterface<android::ICacaoService> *this)

{
  return *(undefined4 *)(DAT_0000ced8 + 0xced6);
}

// ====== 0000cedc: asInterface ======
/* android::ICacaoService::asInterface(android::sp<android::IBinder> const&) */

void __thiscall android::ICacaoService::asInterface(ICacaoService *this,sp *param_1)

{
  BpCacaoService *this_00;
  undefined4 *puVar1;
  undefined4 *puVar2;
  ICacaoProcessCallback *local_1c;
  int local_18;
  
  puVar2 = (undefined4 *)(DAT_0000cf40 + 0xceea);
  local_18 = *(int *)*puVar2;
  *(undefined4 *)this = 0;
  puVar1 = *(undefined4 **)param_1;
  if (puVar1 != (undefined4 *)0x0) {
    (**(code **)*puVar1)
              ((sp<android::Cacao::DeathNotifier> *)&local_1c,puVar1,
               *(undefined4 *)(DAT_0000cf44 + 0xcf00));
    sp<android::ICacaoProcessCallback>::operator=
              ((sp<android::ICacaoProcessCallback> *)this,local_1c);
    sp<android::Cacao::DeathNotifier>::~sp((sp<android::Cacao::DeathNotifier> *)&local_1c);
    if (*(int *)this == 0) {
      this_00 = operator_new(0x1c);
      BpCacaoService::BpCacaoService(this_00,param_1);
      sp<android::ICacaoProcessCallback>::operator=
                ((sp<android::ICacaoProcessCallback> *)this,(ICacaoProcessCallback *)this_00);
    }
  }
  if (*(int *)*puVar2 == local_18) {
    return;
  }
                    /* WARNING: Subroutine does not return */
  __stack_chk_fail();
}

// ====== 0000cf48: operator= ======
/* android::sp<android::ICacaoProcessCallback>::TEMPNAMEPLACEHOLDERVALUE(android::ICacaoProcessCallback*)
    */

sp<android::ICacaoProcessCallback> * __thiscall
android::sp<android::ICacaoProcessCallback>::operator=
          (sp<android::ICacaoProcessCallback> *this,ICacaoProcessCallback *param_1)

{
  int *piVar1;
  
  piVar1 = *(int **)this;
  if (param_1 != (ICacaoProcessCallback *)0x0) {
    android::RefBase::incStrong(param_1 + *(int *)(*(int *)param_1 + -0xc));
  }
  if (piVar1 != (int *)0x0) {
    android::RefBase::decStrong((void *)(*(int *)(*piVar1 + -0xc) + (int)piVar1));
  }
  if (piVar1 != *(int **)this) {
    android::sp_report_race();
  }
  *(ICacaoProcessCallback **)this = param_1;
  return this;
}

// ====== 0000cf80: ~sp ======
/* android::sp<android::Cacao::DeathNotifier>::~sp() */

sp<android::Cacao::DeathNotifier> * __thiscall
android::sp<android::Cacao::DeathNotifier>::~sp(sp<android::Cacao::DeathNotifier> *this)

{
  int *piVar1;
  
  piVar1 = *(int **)this;
  if (piVar1 != (int *)0x0) {
    android::RefBase::decStrong((void *)((int)piVar1 + *(int *)(*piVar1 + -0xc)));
  }
  return this;
}

// ====== 0000cf9c: BpCacaoService ======
/* android::BpCacaoService::BpCacaoService(android::sp<android::IBinder> const&) */

BpCacaoService * __thiscall
android::BpCacaoService::BpCacaoService(BpCacaoService *this,sp *param_1)

{
  int iVar1;
  int *piVar2;
  int iVar3;
  
  android::RefBase::RefBase((RefBase *)(this + 0x14));
  iVar3 = *(int *)(DAT_0000cff4 + 0xcfb0);
  android::IInterface::IInterface((IInterface *)this);
  iVar1 = *(int *)(DAT_0000cff8 + 0xcfc2);
  *(int *)(this + 0x14) = iVar1 + 0x48;
  *(int *)this = iVar1 + 0xc;
  piVar2 = (int *)android::BpRefBase::BpRefBase((BpRefBase *)(this + 4),(sp *)(iVar3 + 0x18));
  iVar1 = *(int *)(DAT_0000cffc + 0xcfe0);
  *(int *)(this + 0x14) = iVar1 + 0x68;
  *(int *)this = iVar1 + 0xc;
  *piVar2 = iVar1 + 0x38;
  return this;
}

// ====== 0000d000: ICacaoClient ======
/* android::ICacaoClient::ICacaoClient() */

void __thiscall android::ICacaoClient::ICacaoClient(ICacaoClient *this)

{
  int *piVar1;
  int *in_r1;
  int iVar2;
  
  piVar1 = (int *)android::IInterface::IInterface((IInterface *)this);
  iVar2 = *in_r1;
  *piVar1 = iVar2;
  *(int *)((int)piVar1 + *(int *)(iVar2 + -0xc)) = in_r1[3];
  return;
}

// ====== 0000d018: ~ICacaoProcessCallback ======
/* android::ICacaoProcessCallback::~ICacaoProcessCallback() */

void __thiscall android::ICacaoProcessCallback::~ICacaoProcessCallback(ICacaoProcessCallback *this)

{
  android::IInterface::~IInterface((IInterface *)this);
  return;
}

// ====== 0000d01e: ~DeathRecipient ======
/* virtual thunk to android::IBinder::DeathRecipient::~DeathRecipient() */

void __thiscall android::IBinder::DeathRecipient::~DeathRecipient(DeathRecipient *this)

{
  code *pcVar1;
  
                    /* WARNING: Does not return */
  pcVar1 = (code *)software_udf(0xfe,0xd01e);
  (*pcVar1)();
}

// ====== 0000d020: onTransact ======
/* android::BnCacaoService::onTransact(unsigned int, android::Parcel const&, android::Parcel*,
   unsigned int) */

void android::BnCacaoService::onTransact(uint param_1,Parcel *param_2,Parcel *param_3,uint param_4)

{
  int iVar1;
  undefined4 uVar2;
  undefined4 uVar3;
  undefined4 *puVar4;
  int iVar5;
  code *pcVar6;
  undefined4 *puVar7;
  undefined4 *puVar8;
  undefined4 auStack_358 [100];
  sp<android::IInterface> asStack_1c8 [4];
  int local_1c4;
  undefined4 local_1c0;
  int local_1bc;
  undefined4 auStack_1b8 [96];
  undefined4 local_38;
  undefined4 local_34;
  int local_28;
  
  puVar7 = (undefined4 *)(DAT_0000d1ec + 0xd036);
  local_28 = *(int *)*puVar7;
  switch(param_2) {
  case (Parcel *)0x1:
    iVar1 = android::Parcel::checkInterface((IBinder *)param_3);
    if (iVar1 == 0) break;
    android::Parcel::readStrongBinder();
    ICacaoClient::asInterface((ICacaoClient *)&local_1bc,(sp *)&local_1c0);
    sp<android::Cacao::DeathNotifier>::~sp((sp<android::Cacao::DeathNotifier> *)&local_1c0);
    (**(code **)(*(int *)param_1 + 0x10))
              ((sp<android::Cacao::DeathNotifier> *)&local_1c0,param_1,(ICacaoClient *)&local_1bc);
    sp<android::IInterface>::sp<android::ICacaoService>(asStack_1c8,(sp *)&local_1c0);
    android::IInterface::asBinder((sp *)&local_1c4);
    android::Parcel::writeStrongBinder((sp *)param_4);
    sp<android::Cacao::DeathNotifier>::~sp((sp<android::Cacao::DeathNotifier> *)&local_1c4);
    sp<android::Cacao::DeathNotifier>::~sp((sp<android::Cacao::DeathNotifier> *)asStack_1c8);
    sp<android::Cacao::DeathNotifier>::~sp((sp<android::Cacao::DeathNotifier> *)&local_1c0);
    sp<android::Cacao::DeathNotifier>::~sp((sp<android::Cacao::DeathNotifier> *)&local_1bc);
LAB_0000d1ca:
    iVar1 = 0;
    goto LAB_0000d1d2;
  case (Parcel *)0x2:
    iVar1 = android::Parcel::checkInterface((IBinder *)param_3);
    if (iVar1 != 0) {
      (**(code **)(*(int *)param_1 + 0x14))(param_1);
LAB_0000d1c2:
      android::Parcel::writeInt32(param_4);
      goto LAB_0000d1ca;
    }
    break;
  case (Parcel *)0x3:
    iVar1 = android::Parcel::checkInterface((IBinder *)param_3);
    if (iVar1 != 0) {
      local_1c0 = android::Parcel::readInt32();
      android::Parcel::readStrongBinder();
      android::IMemory::asInterface((sp *)&local_1c4);
      sp<android::Cacao::DeathNotifier>::~sp((sp<android::Cacao::DeathNotifier> *)&local_1bc);
      __aeabi_memclr4(auStack_1b8,400);
      iVar1 = android::Parcel::readInt32();
      local_1bc = iVar1;
      if (iVar1 != 0) {
        iVar5 = 0;
        puVar4 = auStack_1b8;
        do {
          uVar2 = android::Parcel::readFileDescriptor();
          iVar5 = iVar5 + 1;
          *puVar4 = uVar2;
          puVar4 = puVar4 + 1;
        } while (iVar5 != iVar1);
      }
      if (local_1c4 != 0) {
        local_38 = android::IMemory::size();
        local_34 = android::IMemory::pointer();
      }
      pcVar6 = *(code **)(*(int *)param_1 + 0x18);
      iVar1 = 400;
      puVar4 = auStack_358;
      puVar8 = auStack_1b8;
      do {
        iVar1 = iVar1 + -4;
        *puVar4 = *puVar8;
        puVar4 = puVar4 + 1;
        puVar8 = puVar8 + 1;
      } while (iVar1 != 0);
      (*pcVar6)(param_1,&local_1c0,(sp<android::Cacao::DeathNotifier> *)&local_1c4,local_1bc);
      android::Parcel::writeInt32(param_4);
      sp<android::Cacao::DeathNotifier>::~sp((sp<android::Cacao::DeathNotifier> *)&local_1c4);
      goto LAB_0000d1ca;
    }
    break;
  case (Parcel *)0x4:
    iVar1 = android::Parcel::checkInterface((IBinder *)param_3);
    if (iVar1 != 0) {
      uVar2 = android::Parcel::readInt32();
      uVar3 = android::Parcel::readInt32();
      (**(code **)(*(int *)param_1 + 0x1c))(param_1,uVar2,uVar3);
      goto LAB_0000d1c2;
    }
    break;
  default:
    iVar1 = *(int *)*puVar7 - local_28;
    if (iVar1 == 0) {
      (*(android::Parcel::readInt32 + DAT_000137b8))(param_1 + 4,param_2,param_3,param_4);
      return;
    }
    goto LAB_0000d1e6;
  }
  iVar1 = -1;
LAB_0000d1d2:
  if (*(int *)*puVar7 == local_28) {
    return;
  }
LAB_0000d1e6:
                    /* WARNING: Subroutine does not return */
  __stack_chk_fail(iVar1);
}

// ====== 0000d1f0: sp<android::ICacaoService> ======
/* android::sp<android::IInterface>::sp<android::ICacaoService>(android::sp<android::ICacaoService>
   const&) */

sp<android::IInterface> * __thiscall
android::sp<android::IInterface>::sp<android::ICacaoService>
          (sp<android::IInterface> *this,sp *param_1)

{
  int *piVar1;
  
  piVar1 = *(int **)param_1;
  *(int **)this = piVar1;
  if (piVar1 != (int *)0x0) {
    android::RefBase::incStrong((void *)((int)piVar1 + *(int *)(*piVar1 + -0xc)));
  }
  return this;
}

// ====== 0000d20e: onTransact ======
/* non-virtual thunk to android::BnCacaoService::onTransact(unsigned int, android::Parcel const&,
   android::Parcel*, unsigned int) */

void __thiscall
android::BnCacaoService::onTransact
          (BnCacaoService *this,uint param_1,Parcel *param_2,Parcel *param_3,uint param_4)

{
  (*(android::String16::size + DAT_00013848))(this + -4);
  return;
}

// ====== 0000d214: onAsBinder ======
/* android::BnInterface<android::ICacaoProcessCallback>::onAsBinder() */

BnInterface<android::ICacaoProcessCallback> * __thiscall
android::BnInterface<android::ICacaoProcessCallback>::onAsBinder
          (BnInterface<android::ICacaoProcessCallback> *this)

{
  return this + 4;
}

// ====== 0000d218: queryLocalInterface ======
/* android::BnInterface<android::ICacaoService>::queryLocalInterface(android::String16 const&) */

void android::BnInterface<android::ICacaoService>::queryLocalInterface(String16 *param_1)

{
  undefined4 uVar1;
  undefined4 uVar2;
  int iVar3;
  MemoryBase *in_r1;
  undefined4 *in_r2;
  undefined4 uVar4;
  undefined4 uVar5;
  
  uVar5 = *in_r2;
  uVar1 = android::String16::size();
  uVar4 = **(undefined4 **)(DAT_0000d258 + 0xd230);
  uVar2 = android::String16::size();
  iVar3 = strzcmp16(uVar5,uVar1,uVar4,uVar2);
  if (iVar3 != 0) {
    *(undefined4 *)param_1 = 0;
    return;
  }
  sp<android::IMemory>::sp<android::MemoryBase>((sp<android::IMemory> *)param_1,in_r1);
  return;
}

// ====== 0000d25c: queryLocalInterface ======
/* non-virtual thunk to
   android::BnInterface<android::ICacaoService>::queryLocalInterface(android::String16 const&) */

void __thiscall
android::BnInterface<android::ICacaoService>::queryLocalInterface
          (BnInterface<android::ICacaoService> *this,String16 *param_1)

{
  undefined4 uVar1;
  undefined4 uVar2;
  int iVar3;
  undefined4 *in_r2;
  undefined4 uVar4;
  undefined4 uVar5;
  
  uVar5 = *in_r2;
  uVar1 = android::String16::size();
  uVar4 = **(undefined4 **)(DAT_0000d2a0 + 0xd274);
  uVar2 = android::String16::size();
  iVar3 = strzcmp16(uVar5,uVar1,uVar4,uVar2);
  if (iVar3 != 0) {
    *(undefined4 *)this = 0;
    return;
  }
  sp<android::IMemory>::sp<android::MemoryBase>
            ((sp<android::IMemory> *)this,(MemoryBase *)(param_1 + -4));
  return;
}

// ====== 0000d2a4: onAsBinder ======
/* android::BpInterface<android::ICacao>::onAsBinder() */

undefined4 __thiscall
android::BpInterface<android::ICacao>::onAsBinder(BpInterface<android::ICacao> *this)

{
  return *(undefined4 *)(this + 8);
}

// ====== 0000d2a8: ~BpCacaoService ======
/* android::BpCacaoService::~BpCacaoService() */

BpCacaoService * __thiscall android::BpCacaoService::~BpCacaoService(BpCacaoService *this)

{
  android::BpRefBase::~BpRefBase((BpRefBase *)(this + 4));
  android::IInterface::~IInterface((IInterface *)this);
  android::RefBase::~RefBase((RefBase *)(this + 0x14));
  return this;
}

// ====== 0000d2d8: ~BpCacaoService ======
/* android::BpCacaoService::~BpCacaoService() */

void __thiscall android::BpCacaoService::~BpCacaoService(BpCacaoService *this)

{
  android::BpRefBase::~BpRefBase((BpRefBase *)(this + 4));
  android::IInterface::~IInterface((IInterface *)this);
  android::RefBase::~RefBase((RefBase *)(this + 0x14));
  operator_delete(this);
  return;
}

// ====== 0000d30c: connect ======
/* android::BpCacaoService::connect(android::sp<android::ICacaoClient> const&) */

void android::BpCacaoService::connect(sp *param_1)

{
  int in_r1;
  sp *in_r2;
  undefined4 *puVar1;
  sp<android::IInterface> asStack_94 [4];
  sp asStack_90 [4];
  Parcel aPStack_8c [52];
  Parcel aPStack_58 [52];
  int local_24;
  
  puVar1 = (undefined4 *)(DAT_0000d3b8 + 0xd320);
  local_24 = *(int *)*puVar1;
  android::Parcel::Parcel(aPStack_58);
  android::Parcel::Parcel(aPStack_8c);
  android::Parcel::writeInterfaceToken((String16 *)aPStack_58);
  sp<android::IInterface>::sp<android::ICacaoService>(asStack_94,in_r2);
  android::IInterface::asBinder(asStack_90);
  android::Parcel::writeStrongBinder((sp *)aPStack_58);
  sp<android::Cacao::DeathNotifier>::~sp((sp<android::Cacao::DeathNotifier> *)asStack_90);
  sp<android::Cacao::DeathNotifier>::~sp((sp<android::Cacao::DeathNotifier> *)asStack_94);
  (**(code **)(**(int **)(in_r1 + 8) + 0x14))(*(int **)(in_r1 + 8),1,aPStack_58,aPStack_8c,0);
  android::Parcel::readStrongBinder();
  ICacao::asInterface((ICacao *)param_1,asStack_90);
  sp<android::Cacao::DeathNotifier>::~sp((sp<android::Cacao::DeathNotifier> *)asStack_90);
  android::Parcel::~Parcel(aPStack_8c);
  android::Parcel::~Parcel(aPStack_58);
  if (*(int *)*puVar1 == local_24) {
    return;
  }
                    /* WARNING: Subroutine does not return */
  __stack_chk_fail();
}

// ====== 0000d3c0: getServicePid ======
/* android::BpCacaoService::getServicePid() */

undefined4 __thiscall android::BpCacaoService::getServicePid(BpCacaoService *this)

{
  undefined4 uVar1;
  undefined4 *puVar2;
  Parcel aPStack_80 [52];
  Parcel aPStack_4c [52];
  int local_18;
  
  puVar2 = (undefined4 *)(DAT_0000d428 + 0xd3cc);
  local_18 = *(int *)*puVar2;
  android::Parcel::Parcel(aPStack_4c);
  android::Parcel::Parcel(aPStack_80);
  android::Parcel::writeInterfaceToken((String16 *)aPStack_4c);
  (**(code **)(**(int **)(this + 8) + 0x14))(*(int **)(this + 8),2,aPStack_4c,aPStack_80,0);
  uVar1 = android::Parcel::readInt32();
  android::Parcel::~Parcel(aPStack_80);
  android::Parcel::~Parcel(aPStack_4c);
  if (*(int *)*puVar2 == local_18) {
    return uVar1;
  }
                    /* WARNING: Subroutine does not return */
  __stack_chk_fail();
}

// ====== 0000d430: getCaps ======
/* android::BpCacaoService::getCaps(cacao::ProcessCtrlCaps::CameraIndex const&,
   android::sp<android::IMemory> const&, cacao::ISerialize::SerializedData) */

undefined4 __thiscall
android::BpCacaoService::getCaps(BpCacaoService *this,undefined4 param_2_00,sp *param_2,int param_4)

{
  int iVar1;
  undefined4 uVar2;
  int iVar3;
  int *piVar4;
  undefined4 *puVar5;
  sp<android::IInterface> asStack_94 [4];
  sp asStack_90 [4];
  Parcel aPStack_8c [52];
  Parcel aPStack_58 [52];
  int local_24;
  int local_4;
  
  puVar5 = (undefined4 *)(DAT_0000d508 + 0xd448);
  local_24 = *(int *)*puVar5;
  local_4 = param_4;
  android::Parcel::Parcel(aPStack_58);
  android::Parcel::Parcel(aPStack_8c);
  android::Parcel::writeInterfaceToken((String16 *)aPStack_58);
  android::Parcel::writeInt32((int)aPStack_58);
  sp<android::IInterface>::sp<android::ICacaoService>(asStack_94,param_2);
  android::IInterface::asBinder(asStack_90);
  android::Parcel::writeStrongBinder((sp *)aPStack_58);
  sp<android::Cacao::DeathNotifier>::~sp((sp<android::Cacao::DeathNotifier> *)asStack_90);
  sp<android::Cacao::DeathNotifier>::~sp((sp<android::Cacao::DeathNotifier> *)asStack_94);
  iVar1 = local_4;
  android::Parcel::writeInt32((int)aPStack_58);
  if (iVar1 != 0) {
    piVar4 = &local_4;
    iVar3 = 0;
    do {
      piVar4 = piVar4 + 1;
      android::Parcel::writeFileDescriptor((int)aPStack_58,SUB41(*piVar4,0));
      iVar3 = iVar3 + 1;
    } while (iVar3 != iVar1);
  }
  (**(code **)(**(int **)(this + 8) + 0x14))(*(int **)(this + 8),3,aPStack_58,aPStack_8c,0);
  uVar2 = android::Parcel::readInt32();
  android::Parcel::~Parcel(aPStack_8c);
  android::Parcel::~Parcel(aPStack_58);
  if (*(int *)*puVar5 == local_24) {
    return uVar2;
  }
                    /* WARNING: Subroutine does not return */
  __stack_chk_fail();
}

// ====== 0000d510: getJpegBufferSize ======
/* android::BpCacaoService::getJpegBufferSize(cacao::ImageSize) */

undefined4 android::BpCacaoService::getJpegBufferSize(int param_1)

{
  undefined4 uVar1;
  undefined4 *puVar2;
  Parcel aPStack_88 [52];
  Parcel aPStack_54 [52];
  int local_20;
  
  puVar2 = (undefined4 *)(DAT_0000d598 + 0xd524);
  local_20 = *(int *)*puVar2;
  android::Parcel::Parcel(aPStack_54);
  android::Parcel::Parcel(aPStack_88);
  android::Parcel::writeInterfaceToken((String16 *)aPStack_54);
  android::Parcel::writeInt32((int)aPStack_54);
  android::Parcel::writeInt32((int)aPStack_54);
  (**(code **)(**(int **)(param_1 + 8) + 0x14))(*(int **)(param_1 + 8),4,aPStack_54,aPStack_88,0);
  uVar1 = android::Parcel::readInt32();
  android::Parcel::~Parcel(aPStack_88);
  android::Parcel::~Parcel(aPStack_54);
  if (*(int *)*puVar2 == local_20) {
    return uVar1;
  }
                    /* WARNING: Subroutine does not return */
  __stack_chk_fail();
}

// ====== 0000d5a0: ~BpCacaoService ======
/* non-virtual thunk to android::BpCacaoService::~BpCacaoService() */

void __thiscall android::BpCacaoService::~BpCacaoService(BpCacaoService *this)

{
  android::BpRefBase::~BpRefBase((BpRefBase *)this);
  android::IInterface::~IInterface((IInterface *)(this + -4));
  android::RefBase::~RefBase((RefBase *)(this + 0x10));
  return;
}

// ====== 0000d5d0: ~BpCacaoService ======
/* non-virtual thunk to android::BpCacaoService::~BpCacaoService() */

void __thiscall android::BpCacaoService::~BpCacaoService(BpCacaoService *this)

{
  android::BpRefBase::~BpRefBase((BpRefBase *)this);
  android::IInterface::~IInterface((IInterface *)(this + -4));
  android::RefBase::~RefBase((RefBase *)(this + 0x10));
  operator_delete((IInterface *)(this + -4));
  return;
}

// ====== 0000d608: ~BpCacaoService ======
/* virtual thunk to android::BpCacaoService::~BpCacaoService() */

void __thiscall android::BpCacaoService::~BpCacaoService(BpCacaoService *this)

{
  IInterface *this_00;
  
  this_00 = (IInterface *)(this + *(int *)(*(int *)this + -0xc));
  android::BpRefBase::~BpRefBase((BpRefBase *)(this_00 + 4));
  android::IInterface::~IInterface(this_00);
  android::RefBase::~RefBase((RefBase *)(this_00 + 0x14));
  return;
}

// ====== 0000d63c: ~BpCacaoService ======
/* virtual thunk to android::BpCacaoService::~BpCacaoService() */

void __thiscall android::BpCacaoService::~BpCacaoService(BpCacaoService *this)

{
  IInterface *this_00;
  
  this_00 = (IInterface *)(this + *(int *)(*(int *)this + -0xc));
  android::BpRefBase::~BpRefBase((BpRefBase *)(this_00 + 4));
  android::IInterface::~IInterface(this_00);
  android::RefBase::~RefBase((RefBase *)(this_00 + 0x14));
  operator_delete(this_00);
  return;
}

// ====== 0000d678: sp<android::MemoryBase> ======
/* android::sp<android::IMemory>::sp<android::MemoryBase>(android::MemoryBase*) */

sp<android::IMemory> * __thiscall
android::sp<android::IMemory>::sp<android::MemoryBase>
          (sp<android::IMemory> *this,MemoryBase *param_1)

{
  *(MemoryBase **)this = param_1;
  if (param_1 != (MemoryBase *)0x0) {
    android::RefBase::incStrong(param_1 + *(int *)(*(int *)param_1 + -0xc));
  }
  return this;
}

// ====== 0000d694: getInterfaceDescriptor ======
/* non-virtual thunk to
   android::BnInterface<android::ICacaoProcessCallback>::getInterfaceDescriptor() const */

undefined4 __thiscall
android::BnInterface<android::ICacaoProcessCallback>::getInterfaceDescriptor
          (BnInterface<android::ICacaoProcessCallback> *this)

{
  return *(undefined4 *)(DAT_0000d69c + 0xd69a);
}

// ====== 0000d6a0: asInterface ======
/* android::ICacaoProcessCallback::asInterface(android::sp<android::IBinder> const&) */

void __thiscall android::ICacaoProcessCallback::asInterface(ICacaoProcessCallback *this,sp *param_1)

{
  BpCacaoProcessCallback *this_00;
  undefined4 *puVar1;
  undefined4 *puVar2;
  ICacaoProcessCallback *local_1c;
  int local_18;
  
  puVar2 = (undefined4 *)(DAT_0000d704 + 0xd6ae);
  local_18 = *(int *)*puVar2;
  *(undefined4 *)this = 0;
  puVar1 = *(undefined4 **)param_1;
  if (puVar1 != (undefined4 *)0x0) {
    (**(code **)*puVar1)
              ((sp<android::Cacao::DeathNotifier> *)&local_1c,puVar1,
               *(undefined4 *)(DAT_0000d708 + 0xd6c4));
    sp<android::ICacaoProcessCallback>::operator=
              ((sp<android::ICacaoProcessCallback> *)this,local_1c);
    sp<android::Cacao::DeathNotifier>::~sp((sp<android::Cacao::DeathNotifier> *)&local_1c);
    if (*(int *)this == 0) {
      this_00 = operator_new(0x1c);
      BpCacaoProcessCallback::BpCacaoProcessCallback(this_00,param_1);
      sp<android::ICacaoProcessCallback>::operator=
                ((sp<android::ICacaoProcessCallback> *)this,(ICacaoProcessCallback *)this_00);
    }
  }
  if (*(int *)*puVar2 == local_18) {
    return;
  }
                    /* WARNING: Subroutine does not return */
  __stack_chk_fail();
}

// ====== 0000d70c: BpCacaoProcessCallback ======
/* android::BpCacaoProcessCallback::BpCacaoProcessCallback(android::sp<android::IBinder> const&) */

BpCacaoProcessCallback * __thiscall
android::BpCacaoProcessCallback::BpCacaoProcessCallback(BpCacaoProcessCallback *this,sp *param_1)

{
  int iVar1;
  int *piVar2;
  int iVar3;
  
  android::RefBase::RefBase((RefBase *)(this + 0x14));
  iVar3 = *(int *)(DAT_0000d764 + 0xd720);
  android::IInterface::IInterface((IInterface *)this);
  iVar1 = *(int *)(DAT_0000d768 + 0xd732);
  *(int *)(this + 0x14) = iVar1 + 0x40;
  *(int *)this = iVar1 + 0xc;
  piVar2 = (int *)android::BpRefBase::BpRefBase((BpRefBase *)(this + 4),(sp *)(iVar3 + 0x18));
  iVar1 = *(int *)(DAT_0000d76c + 0xd750);
  *(int *)(this + 0x14) = iVar1 + 0x60;
  *(int *)this = iVar1 + 0xc;
  *piVar2 = iVar1 + 0x30;
  return this;
}

// ====== 0000d770: onTransact ======
/* android::BnCacaoProcessCallback::onTransact(unsigned int, android::Parcel const&,
   android::Parcel*, unsigned int) */

undefined4
android::BnCacaoProcessCallback::onTransact
          (uint param_1,Parcel *param_2,Parcel *param_3,uint param_4)

{
  int iVar1;
  undefined4 uVar2;
  undefined4 *puVar3;
  int iVar4;
  undefined4 *puVar5;
  code *pcVar6;
  undefined4 *puVar7;
  undefined4 auStack_350 [99];
  undefined4 local_1c4;
  int local_1bc;
  int local_1b8;
  undefined4 auStack_1b4 [96];
  undefined4 local_34;
  undefined4 local_30;
  int local_24;
  
  puVar7 = (undefined4 *)(DAT_0000d908 + 0xd786);
  local_24 = *(int *)*puVar7;
  if (param_2 == (Parcel *)0x2) {
    iVar1 = android::Parcel::checkInterface((IBinder *)param_3);
    if (iVar1 == 0) {
LAB_0000d8ea:
      uVar2 = 0xffffffff;
    }
    else {
      android::Parcel::readStrongBinder();
      android::IMemory::asInterface((sp *)&local_1bc);
      sp<android::Cacao::DeathNotifier>::~sp((sp<android::Cacao::DeathNotifier> *)&local_1b8);
      __aeabi_memclr4(auStack_1b4,400);
      iVar1 = android::Parcel::readInt32();
      local_1b8 = iVar1;
      if (iVar1 != 0) {
        iVar4 = 0;
        puVar5 = auStack_1b4;
        do {
          uVar2 = android::Parcel::readFileDescriptor();
          iVar4 = iVar4 + 1;
          *puVar5 = uVar2;
          puVar5 = puVar5 + 1;
        } while (iVar4 != iVar1);
      }
      if (local_1bc != 0) {
        local_34 = android::IMemory::size();
        local_30 = android::IMemory::pointer();
      }
      uVar2 = 0;
      iVar1 = 0x18c;
      pcVar6 = *(code **)(*(int *)param_1 + 0x14);
      puVar5 = auStack_1b4;
      local_1c4 = 0;
      puVar3 = auStack_350;
      do {
        puVar5 = puVar5 + 1;
        iVar1 = iVar1 + -4;
        *puVar3 = *puVar5;
        puVar3 = puVar3 + 1;
      } while (iVar1 != 0);
      (*pcVar6)(param_1,(sp<android::Cacao::DeathNotifier> *)&local_1bc,local_1b8,auStack_1b4[0]);
      sp<android::Cacao::DeathNotifier>::~sp((sp<android::Cacao::DeathNotifier> *)&local_1bc);
    }
  }
  else {
    if (param_2 != (Parcel *)0x1) {
      if (*(int *)*puVar7 == local_24) {
        uVar2 = (*(android::Parcel::readInt32 + DAT_000137b8))(param_1 + 4,param_2,param_3);
        return uVar2;
      }
      goto LAB_0000d904;
    }
    iVar1 = android::Parcel::checkInterface((IBinder *)param_3);
    if (iVar1 == 0) goto LAB_0000d8ea;
    android::Parcel::readStrongBinder();
    android::IMemory::asInterface((sp *)&local_1bc);
    sp<android::Cacao::DeathNotifier>::~sp((sp<android::Cacao::DeathNotifier> *)&local_1b8);
    __aeabi_memclr4(auStack_1b4,400);
    iVar1 = android::Parcel::readInt32();
    local_1b8 = iVar1;
    if (iVar1 != 0) {
      iVar4 = 0;
      puVar5 = auStack_1b4;
      do {
        uVar2 = android::Parcel::readFileDescriptor();
        iVar4 = iVar4 + 1;
        *puVar5 = uVar2;
        puVar5 = puVar5 + 1;
      } while (iVar4 != iVar1);
    }
    if (local_1bc != 0) {
      local_34 = android::IMemory::size();
      local_30 = android::IMemory::pointer();
    }
    iVar1 = 0x18c;
    pcVar6 = *(code **)(*(int *)param_1 + 0x10);
    puVar5 = auStack_1b4;
    puVar3 = auStack_350;
    do {
      puVar5 = puVar5 + 1;
      iVar1 = iVar1 + -4;
      *puVar3 = *puVar5;
      puVar3 = puVar3 + 1;
    } while (iVar1 != 0);
    (*pcVar6)(param_1,(sp<android::Cacao::DeathNotifier> *)&local_1bc,local_1b8,auStack_1b4[0]);
    sp<android::Cacao::DeathNotifier>::~sp((sp<android::Cacao::DeathNotifier> *)&local_1bc);
    uVar2 = 0;
  }
  if (*(int *)*puVar7 == local_24) {
    return uVar2;
  }
LAB_0000d904:
                    /* WARNING: Subroutine does not return */
  __stack_chk_fail();
}

// ====== 0000d90c: onTransact ======
/* non-virtual thunk to android::BnCacaoProcessCallback::onTransact(unsigned int, android::Parcel
   const&, android::Parcel*, unsigned int) */

void __thiscall
android::BnCacaoProcessCallback::onTransact
          (BnCacaoProcessCallback *this,uint param_1,Parcel *param_2,Parcel *param_3,uint param_4)

{
  (*(sp<android::IMemory>::sp<android::MemoryBase> + DAT_00013958))(this + -4);
  return;
}

// ====== 0000d914: queryLocalInterface ======
/* android::BnInterface<android::ICacaoProcessCallback>::queryLocalInterface(android::String16
   const&) */

void android::BnInterface<android::ICacaoProcessCallback>::queryLocalInterface(String16 *param_1)

{
  undefined4 uVar1;
  undefined4 uVar2;
  int iVar3;
  MemoryBase *in_r1;
  undefined4 *in_r2;
  undefined4 uVar4;
  undefined4 uVar5;
  
  uVar5 = *in_r2;
  uVar1 = android::String16::size();
  uVar4 = **(undefined4 **)(DAT_0000d954 + 0xd92c);
  uVar2 = android::String16::size();
  iVar3 = strzcmp16(uVar5,uVar1,uVar4,uVar2);
  if (iVar3 != 0) {
    *(undefined4 *)param_1 = 0;
    return;
  }
  sp<android::IMemory>::sp<android::MemoryBase>((sp<android::IMemory> *)param_1,in_r1);
  return;
}

// ====== 0000d958: queryLocalInterface ======
/* non-virtual thunk to
   android::BnInterface<android::ICacaoProcessCallback>::queryLocalInterface(android::String16
   const&) */

void __thiscall
android::BnInterface<android::ICacaoProcessCallback>::queryLocalInterface
          (BnInterface<android::ICacaoProcessCallback> *this,String16 *param_1)

{
  undefined4 uVar1;
  undefined4 uVar2;
  int iVar3;
  undefined4 *in_r2;
  undefined4 uVar4;
  undefined4 uVar5;
  
  uVar5 = *in_r2;
  uVar1 = android::String16::size();
  uVar4 = **(undefined4 **)(DAT_0000d99c + 0xd970);
  uVar2 = android::String16::size();
  iVar3 = strzcmp16(uVar5,uVar1,uVar4,uVar2);
  if (iVar3 != 0) {
    *(undefined4 *)this = 0;
    return;
  }
  sp<android::IMemory>::sp<android::MemoryBase>
            ((sp<android::IMemory> *)this,(MemoryBase *)(param_1 + -4));
  return;
}

// ====== 0000d9a0: ~BpCacaoProcessCallback ======
/* android::BpCacaoProcessCallback::~BpCacaoProcessCallback() */

BpCacaoProcessCallback * __thiscall
android::BpCacaoProcessCallback::~BpCacaoProcessCallback(BpCacaoProcessCallback *this)

{
  android::BpRefBase::~BpRefBase((BpRefBase *)(this + 4));
  android::IInterface::~IInterface((IInterface *)this);
  android::RefBase::~RefBase((RefBase *)(this + 0x14));
  return this;
}

// ====== 0000d9d0: ~BpCacaoProcessCallback ======
/* android::BpCacaoProcessCallback::~BpCacaoProcessCallback() */

void __thiscall
android::BpCacaoProcessCallback::~BpCacaoProcessCallback(BpCacaoProcessCallback *this)

{
  android::BpRefBase::~BpRefBase((BpRefBase *)(this + 4));
  android::IInterface::~IInterface((IInterface *)this);
  android::RefBase::~RefBase((RefBase *)(this + 0x14));
  operator_delete(this);
  return;
}

// ====== 0000da04: notifyProgress ======
/* android::BpCacaoProcessCallback::notifyProgress(android::sp<android::IMemory> const&,
   cacao::ISerialize::SerializedData) */

void android::BpCacaoProcessCallback::notifyProgress
               (int param_1,sp *param_2,int param_3,undefined4 param_4)

{
  int iVar1;
  int iVar2;
  int *piVar3;
  undefined4 *puVar4;
  sp<android::IInterface> asStack_98 [4];
  sp asStack_94 [4];
  Parcel aPStack_90 [52];
  Parcel aPStack_5c [52];
  int local_28;
  int local_8;
  int iStack_4;
  
  puVar4 = (undefined4 *)(DAT_0000dac8 + 0xda1c);
  local_28 = *(int *)*puVar4;
  local_8 = param_3;
  iStack_4 = param_4;
  android::Parcel::Parcel(aPStack_5c);
  android::Parcel::Parcel(aPStack_90);
  android::Parcel::writeInterfaceToken((String16 *)aPStack_5c);
  sp<android::IInterface>::sp<android::ICacaoService>(asStack_98,param_2);
  android::IInterface::asBinder(asStack_94);
  android::Parcel::writeStrongBinder((sp *)aPStack_5c);
  sp<android::Cacao::DeathNotifier>::~sp((sp<android::Cacao::DeathNotifier> *)asStack_94);
  sp<android::Cacao::DeathNotifier>::~sp((sp<android::Cacao::DeathNotifier> *)asStack_98);
  iVar1 = local_8;
  android::Parcel::writeInt32((int)aPStack_5c);
  if (iVar1 != 0) {
    piVar3 = &local_8;
    iVar2 = 0;
    do {
      piVar3 = piVar3 + 1;
      android::Parcel::writeFileDescriptor((int)aPStack_5c,SUB41(*piVar3,0));
      iVar2 = iVar2 + 1;
    } while (iVar2 != iVar1);
  }
  (**(code **)(**(int **)(param_1 + 8) + 0x14))(*(int **)(param_1 + 8),1,aPStack_5c,aPStack_90,0);
  android::Parcel::~Parcel(aPStack_90);
  android::Parcel::~Parcel(aPStack_5c);
  if (*(int *)*puVar4 == local_28) {
    return;
  }
                    /* WARNING: Subroutine does not return */
  __stack_chk_fail();
}

// ====== 0000dad0: notifyResult ======
/* android::BpCacaoProcessCallback::notifyResult(android::sp<android::IMemory> const&,
   cacao::ISerialize::SerializedData, android::Vector<cacao::ImageBuf*>*) */

void __thiscall
android::BpCacaoProcessCallback::notifyResult
          (BpCacaoProcessCallback *this,sp *param_1,int param_3,undefined4 param_4)

{
  int iVar1;
  undefined4 uVar2;
  int *piVar3;
  uint uVar4;
  int iVar5;
  undefined4 *puVar6;
  int in_stack_0000018c;
  sp<android::IInterface> asStack_9c [4];
  sp asStack_98 [4];
  Parcel aPStack_94 [52];
  Parcel aPStack_60 [52];
  int local_2c;
  int local_8;
  int iStack_4;
  
  puVar6 = (undefined4 *)(DAT_0000dbd8 + 0xdae8);
  local_2c = *(int *)*puVar6;
  local_8 = param_3;
  iStack_4 = param_4;
  android::Parcel::Parcel(aPStack_60);
  android::Parcel::Parcel(aPStack_94);
  android::Parcel::writeInterfaceToken((String16 *)aPStack_60);
  sp<android::IInterface>::sp<android::ICacaoService>(asStack_9c,param_1);
  android::IInterface::asBinder(asStack_98);
  android::Parcel::writeStrongBinder((sp *)aPStack_60);
  sp<android::Cacao::DeathNotifier>::~sp((sp<android::Cacao::DeathNotifier> *)asStack_98);
  sp<android::Cacao::DeathNotifier>::~sp((sp<android::Cacao::DeathNotifier> *)asStack_9c);
  iVar1 = local_8;
  android::Parcel::writeInt32((int)aPStack_60);
  if (iVar1 != 0) {
    piVar3 = &local_8;
    iVar5 = 0;
    do {
      piVar3 = piVar3 + 1;
      android::Parcel::writeFileDescriptor((int)aPStack_60,SUB41(*piVar3,0));
      iVar5 = iVar5 + 1;
    } while (iVar5 != iVar1);
  }
  (**(code **)(**(int **)(this + 8) + 0x14))(*(int **)(this + 8),2,aPStack_60,aPStack_94,0);
  if ((in_stack_0000018c != 0) && (*(int *)(in_stack_0000018c + 8) != 0)) {
    uVar4 = 0;
    do {
      uVar2 = (**(code **)(**(int **)(*(int *)(in_stack_0000018c + 4) + uVar4 * 4) + 0x10))();
      native_handle_close();
      native_handle_delete(uVar2);
      piVar3 = *(int **)(*(int *)(in_stack_0000018c + 4) + uVar4 * 4);
      if (piVar3 != (int *)0x0) {
        (**(code **)(*piVar3 + 4))();
      }
      uVar4 = uVar4 + 1;
    } while (uVar4 < *(uint *)(in_stack_0000018c + 8));
  }
  android::Parcel::~Parcel(aPStack_94);
  android::Parcel::~Parcel(aPStack_60);
  if (*(int *)*puVar6 == local_2c) {
    return;
  }
                    /* WARNING: Subroutine does not return */
  __stack_chk_fail();
}

// ====== 0000dbe0: ~BpCacaoProcessCallback ======
/* non-virtual thunk to android::BpCacaoProcessCallback::~BpCacaoProcessCallback() */

void __thiscall
android::BpCacaoProcessCallback::~BpCacaoProcessCallback(BpCacaoProcessCallback *this)

{
  android::BpRefBase::~BpRefBase((BpRefBase *)this);
  android::IInterface::~IInterface((IInterface *)(this + -4));
  android::RefBase::~RefBase((RefBase *)(this + 0x10));
  return;
}

// ====== 0000dc10: ~BpCacaoProcessCallback ======
/* non-virtual thunk to android::BpCacaoProcessCallback::~BpCacaoProcessCallback() */

void __thiscall
android::BpCacaoProcessCallback::~BpCacaoProcessCallback(BpCacaoProcessCallback *this)

{
  android::BpRefBase::~BpRefBase((BpRefBase *)this);
  android::IInterface::~IInterface((IInterface *)(this + -4));
  android::RefBase::~RefBase((RefBase *)(this + 0x10));
  operator_delete((IInterface *)(this + -4));
  return;
}

// ====== 0000dc48: ~BpCacaoProcessCallback ======
/* virtual thunk to android::BpCacaoProcessCallback::~BpCacaoProcessCallback() */

void __thiscall
android::BpCacaoProcessCallback::~BpCacaoProcessCallback(BpCacaoProcessCallback *this)

{
  IInterface *this_00;
  
  this_00 = (IInterface *)(this + *(int *)(*(int *)this + -0xc));
  android::BpRefBase::~BpRefBase((BpRefBase *)(this_00 + 4));
  android::IInterface::~IInterface(this_00);
  android::RefBase::~RefBase((RefBase *)(this_00 + 0x14));
  return;
}

// ====== 0000dc7c: ~BpCacaoProcessCallback ======
/* virtual thunk to android::BpCacaoProcessCallback::~BpCacaoProcessCallback() */

void __thiscall
android::BpCacaoProcessCallback::~BpCacaoProcessCallback(BpCacaoProcessCallback *this)

{
  IInterface *this_00;
  
  this_00 = (IInterface *)(this + *(int *)(*(int *)this + -0xc));
  android::BpRefBase::~BpRefBase((BpRefBase *)(this_00 + 4));
  android::IInterface::~IInterface(this_00);
  android::RefBase::~RefBase((RefBase *)(this_00 + 0x14));
  operator_delete(this_00);
  return;
}

// ====== 0000dcb8: getInterfaceDescriptor ======
/* non-virtual thunk to android::BnInterface<android::ICacaoClient>::getInterfaceDescriptor() const
    */

undefined4 __thiscall
android::BnInterface<android::ICacaoClient>::getInterfaceDescriptor
          (BnInterface<android::ICacaoClient> *this)

{
  return *(undefined4 *)(DAT_0000dcc0 + 0xdcbe);
}

// ====== 0000dcc4: asInterface ======
/* android::ICacaoClient::asInterface(android::sp<android::IBinder> const&) */

void __thiscall android::ICacaoClient::asInterface(ICacaoClient *this,sp *param_1)

{
  BpCacaoClient *this_00;
  undefined4 *puVar1;
  undefined4 *puVar2;
  ICacaoProcessCallback *local_1c;
  int local_18;
  
  puVar2 = (undefined4 *)(DAT_0000dd28 + 0xdcd2);
  local_18 = *(int *)*puVar2;
  *(undefined4 *)this = 0;
  puVar1 = *(undefined4 **)param_1;
  if (puVar1 != (undefined4 *)0x0) {
    (**(code **)*puVar1)
              ((sp<android::Cacao::DeathNotifier> *)&local_1c,puVar1,
               *(undefined4 *)(DAT_0000dd2c + 0xdce8));
    sp<android::ICacaoProcessCallback>::operator=
              ((sp<android::ICacaoProcessCallback> *)this,local_1c);
    sp<android::Cacao::DeathNotifier>::~sp((sp<android::Cacao::DeathNotifier> *)&local_1c);
    if (*(int *)this == 0) {
      this_00 = operator_new(0x1c);
      BpCacaoClient::BpCacaoClient(this_00,param_1);
      sp<android::ICacaoProcessCallback>::operator=
                ((sp<android::ICacaoProcessCallback> *)this,(ICacaoProcessCallback *)this_00);
    }
  }
  if (*(int *)*puVar2 == local_18) {
    return;
  }
                    /* WARNING: Subroutine does not return */
  __stack_chk_fail();
}

// ====== 0000dd30: BpCacaoClient ======
/* android::BpCacaoClient::BpCacaoClient(android::sp<android::IBinder> const&) */

BpCacaoClient * __thiscall android::BpCacaoClient::BpCacaoClient(BpCacaoClient *this,sp *param_1)

{
  int iVar1;
  int *piVar2;
  int iVar3;
  
  android::RefBase::RefBase((RefBase *)(this + 0x14));
  iVar3 = *(int *)(DAT_0000dd88 + 0xdd44);
  android::IInterface::IInterface((IInterface *)this);
  iVar1 = *(int *)(DAT_0000dd8c + 0xdd56);
  *(int *)(this + 0x14) = iVar1 + 0x3c;
  *(int *)this = iVar1 + 0xc;
  piVar2 = (int *)android::BpRefBase::BpRefBase((BpRefBase *)(this + 4),(sp *)(iVar3 + 0x18));
  iVar1 = *(int *)(DAT_0000dd90 + 0xdd74);
  *(int *)(this + 0x14) = iVar1 + 0x5c;
  *(int *)this = iVar1 + 0xc;
  *piVar2 = iVar1 + 0x2c;
  return this;
}

// ====== 0000dd94: onTransact ======
/* android::BnCacaoClient::onTransact(unsigned int, android::Parcel const&, android::Parcel*,
   unsigned int) */

void android::BnCacaoClient::onTransact(uint param_1,Parcel *param_2,Parcel *param_3,uint param_4)

{
  int iVar1;
  undefined4 uVar2;
  undefined4 uVar3;
  undefined4 *puVar4;
  int iVar5;
  code *pcVar6;
  undefined4 *puVar7;
  undefined4 *puVar8;
  undefined4 auStack_350 [100];
  int local_1c0;
  int local_1bc;
  undefined4 auStack_1b8 [96];
  undefined4 local_38;
  undefined4 local_34;
  int local_28;
  
  puVar7 = (undefined4 *)(DAT_0000de8c + 0xddae);
  local_28 = *(int *)*puVar7;
  if (param_2 == (Parcel *)0x1) {
    iVar1 = android::Parcel::checkInterface((IBinder *)param_3);
    if (iVar1 == 0) {
      iVar1 = -1;
    }
    else {
      uVar2 = android::Parcel::readInt32();
      android::Parcel::readStrongBinder();
      android::IMemory::asInterface((sp *)&local_1c0);
      sp<android::Cacao::DeathNotifier>::~sp((sp<android::Cacao::DeathNotifier> *)&local_1bc);
      __aeabi_memclr4(auStack_1b8,400);
      iVar1 = android::Parcel::readInt32();
      local_1bc = iVar1;
      if (iVar1 != 0) {
        iVar5 = 0;
        puVar4 = auStack_1b8;
        do {
          uVar3 = android::Parcel::readFileDescriptor();
          iVar5 = iVar5 + 1;
          *puVar4 = uVar3;
          puVar4 = puVar4 + 1;
        } while (iVar5 != iVar1);
      }
      if (local_1c0 != 0) {
        local_38 = android::IMemory::size();
        local_34 = android::IMemory::pointer();
      }
      pcVar6 = *(code **)(*(int *)param_1 + 0x10);
      iVar1 = 400;
      puVar4 = auStack_350;
      puVar8 = auStack_1b8;
      do {
        iVar1 = iVar1 + -4;
        *puVar4 = *puVar8;
        puVar4 = puVar4 + 1;
        puVar8 = puVar8 + 1;
      } while (iVar1 != 0);
      (*pcVar6)(param_1,uVar2,(sp<android::Cacao::DeathNotifier> *)&local_1c0,local_1bc);
      sp<android::Cacao::DeathNotifier>::~sp((sp<android::Cacao::DeathNotifier> *)&local_1c0);
      iVar1 = 0;
    }
    if (*(int *)*puVar7 == local_28) {
      return;
    }
  }
  else {
    iVar1 = *(int *)*puVar7 - local_28;
    if (iVar1 == 0) {
      (*(android::Parcel::readInt32 + DAT_000137b8))(param_1 + 4,param_2,param_3);
      return;
    }
  }
                    /* WARNING: Subroutine does not return */
  __stack_chk_fail(iVar1);
}

// ====== 0000de90: onTransact ======
/* non-virtual thunk to android::BnCacaoClient::onTransact(unsigned int, android::Parcel const&,
   android::Parcel*, unsigned int) */

void __thiscall
android::BnCacaoClient::onTransact
          (BnCacaoClient *this,uint param_1,Parcel *param_2,Parcel *param_3,uint param_4)

{
  (*(sp<android::IMemory>::sp<android::MemoryBase> + DAT_000139b8))(this + -4);
  return;
}

// ====== 0000de98: queryLocalInterface ======
/* android::BnInterface<android::ICacaoClient>::queryLocalInterface(android::String16 const&) */

void android::BnInterface<android::ICacaoClient>::queryLocalInterface(String16 *param_1)

{
  undefined4 uVar1;
  undefined4 uVar2;
  int iVar3;
  MemoryBase *in_r1;
  undefined4 *in_r2;
  undefined4 uVar4;
  undefined4 uVar5;
  
  uVar5 = *in_r2;
  uVar1 = android::String16::size();
  uVar4 = **(undefined4 **)(DAT_0000ded8 + 0xdeb0);
  uVar2 = android::String16::size();
  iVar3 = strzcmp16(uVar5,uVar1,uVar4,uVar2);
  if (iVar3 != 0) {
    *(undefined4 *)param_1 = 0;
    return;
  }
  sp<android::IMemory>::sp<android::MemoryBase>((sp<android::IMemory> *)param_1,in_r1);
  return;
}

// ====== 0000dedc: queryLocalInterface ======
/* non-virtual thunk to
   android::BnInterface<android::ICacaoClient>::queryLocalInterface(android::String16 const&) */

void __thiscall
android::BnInterface<android::ICacaoClient>::queryLocalInterface
          (BnInterface<android::ICacaoClient> *this,String16 *param_1)

{
  undefined4 uVar1;
  undefined4 uVar2;
  int iVar3;
  undefined4 *in_r2;
  undefined4 uVar4;
  undefined4 uVar5;
  
  uVar5 = *in_r2;
  uVar1 = android::String16::size();
  uVar4 = **(undefined4 **)(DAT_0000df20 + 0xdef4);
  uVar2 = android::String16::size();
  iVar3 = strzcmp16(uVar5,uVar1,uVar4,uVar2);
  if (iVar3 != 0) {
    *(undefined4 *)this = 0;
    return;
  }
  sp<android::IMemory>::sp<android::MemoryBase>
            ((sp<android::IMemory> *)this,(MemoryBase *)(param_1 + -4));
  return;
}

// ====== 0000df24: ~BpCacaoClient ======
/* android::BpCacaoClient::~BpCacaoClient() */

BpCacaoClient * __thiscall android::BpCacaoClient::~BpCacaoClient(BpCacaoClient *this)

{
  android::BpRefBase::~BpRefBase((BpRefBase *)(this + 4));
  android::IInterface::~IInterface((IInterface *)this);
  android::RefBase::~RefBase((RefBase *)(this + 0x14));
  return this;
}

// ====== 0000df54: ~BpCacaoClient ======
/* android::BpCacaoClient::~BpCacaoClient() */

void __thiscall android::BpCacaoClient::~BpCacaoClient(BpCacaoClient *this)

{
  android::BpRefBase::~BpRefBase((BpRefBase *)(this + 4));
  android::IInterface::~IInterface((IInterface *)this);
  android::RefBase::~RefBase((RefBase *)(this + 0x14));
  operator_delete(this);
  return;
}

// ====== 0000df88: notifyEvent ======
/* android::BpCacaoClient::notifyEvent(int, android::sp<android::IMemory> const&,
   cacao::ISerialize::SerializedData) */

void __thiscall
android::BpCacaoClient::notifyEvent
          (BpCacaoClient *this,undefined4 param_2_00,sp *param_2,int param_4)

{
  int iVar1;
  int iVar2;
  int *piVar3;
  undefined4 *puVar4;
  sp<android::IInterface> asStack_94 [4];
  sp asStack_90 [4];
  Parcel aPStack_8c [52];
  Parcel aPStack_58 [52];
  int local_24;
  int local_4;
  
  puVar4 = (undefined4 *)(DAT_0000e054 + 0xdfa0);
  local_24 = *(int *)*puVar4;
  local_4 = param_4;
  android::Parcel::Parcel(aPStack_58);
  android::Parcel::Parcel(aPStack_8c);
  android::Parcel::writeInterfaceToken((String16 *)aPStack_58);
  android::Parcel::writeInt32((int)aPStack_58);
  sp<android::IInterface>::sp<android::ICacaoService>(asStack_94,param_2);
  android::IInterface::asBinder(asStack_90);
  android::Parcel::writeStrongBinder((sp *)aPStack_58);
  sp<android::Cacao::DeathNotifier>::~sp((sp<android::Cacao::DeathNotifier> *)asStack_90);
  sp<android::Cacao::DeathNotifier>::~sp((sp<android::Cacao::DeathNotifier> *)asStack_94);
  iVar1 = local_4;
  android::Parcel::writeInt32((int)aPStack_58);
  if (iVar1 != 0) {
    piVar3 = &local_4;
    iVar2 = 0;
    do {
      piVar3 = piVar3 + 1;
      android::Parcel::writeFileDescriptor((int)aPStack_58,SUB41(*piVar3,0));
      iVar2 = iVar2 + 1;
    } while (iVar2 != iVar1);
  }
  (**(code **)(**(int **)(this + 8) + 0x14))(*(int **)(this + 8),1,aPStack_58,aPStack_8c,1);
  android::Parcel::~Parcel(aPStack_8c);
  android::Parcel::~Parcel(aPStack_58);
  if (*(int *)*puVar4 == local_24) {
    return;
  }
                    /* WARNING: Subroutine does not return */
  __stack_chk_fail();
}

// ====== 0000e05c: ~BpCacaoClient ======
/* non-virtual thunk to android::BpCacaoClient::~BpCacaoClient() */

void __thiscall android::BpCacaoClient::~BpCacaoClient(BpCacaoClient *this)

{
  android::BpRefBase::~BpRefBase((BpRefBase *)this);
  android::IInterface::~IInterface((IInterface *)(this + -4));
  android::RefBase::~RefBase((RefBase *)(this + 0x10));
  return;
}

// ====== 0000e08c: ~BpCacaoClient ======
/* non-virtual thunk to android::BpCacaoClient::~BpCacaoClient() */

void __thiscall android::BpCacaoClient::~BpCacaoClient(BpCacaoClient *this)

{
  android::BpRefBase::~BpRefBase((BpRefBase *)this);
  android::IInterface::~IInterface((IInterface *)(this + -4));
  android::RefBase::~RefBase((RefBase *)(this + 0x10));
  operator_delete((IInterface *)(this + -4));
  return;
}

// ====== 0000e0c4: ~BpCacaoClient ======
/* virtual thunk to android::BpCacaoClient::~BpCacaoClient() */

void __thiscall android::BpCacaoClient::~BpCacaoClient(BpCacaoClient *this)

{
  IInterface *this_00;
  
  this_00 = (IInterface *)(this + *(int *)(*(int *)this + -0xc));
  android::BpRefBase::~BpRefBase((BpRefBase *)(this_00 + 4));
  android::IInterface::~IInterface(this_00);
  android::RefBase::~RefBase((RefBase *)(this_00 + 0x14));
  return;
}

// ====== 0000e0f8: ~BpCacaoClient ======
/* virtual thunk to android::BpCacaoClient::~BpCacaoClient() */

void __thiscall android::BpCacaoClient::~BpCacaoClient(BpCacaoClient *this)

{
  IInterface *this_00;
  
  this_00 = (IInterface *)(this + *(int *)(*(int *)this + -0xc));
  android::BpRefBase::~BpRefBase((BpRefBase *)(this_00 + 4));
  android::IInterface::~IInterface(this_00);
  android::RefBase::~RefBase((RefBase *)(this_00 + 0x14));
  operator_delete(this_00);
  return;
}

// ====== 0000e134: getInterfaceDescriptor ======
/* non-virtual thunk to android::BnInterface<android::ICacao>::getInterfaceDescriptor() const */

undefined4 __thiscall
android::BnInterface<android::ICacao>::getInterfaceDescriptor(BnInterface<android::ICacao> *this)

{
  return *(undefined4 *)(DAT_0000e13c + 0xe13a);
}

// ====== 0000e140: asInterface ======
/* android::ICacao::asInterface(android::sp<android::IBinder> const&) */

void __thiscall android::ICacao::asInterface(ICacao *this,sp *param_1)

{
  BpCacao *this_00;
  undefined4 *puVar1;
  undefined4 *puVar2;
  ICacaoProcessCallback *local_1c;
  int local_18;
  
  puVar2 = (undefined4 *)(DAT_0000e1a4 + 0xe14e);
  local_18 = *(int *)*puVar2;
  *(undefined4 *)this = 0;
  puVar1 = *(undefined4 **)param_1;
  if (puVar1 != (undefined4 *)0x0) {
    (**(code **)*puVar1)
              ((sp<android::Cacao::DeathNotifier> *)&local_1c,puVar1,
               *(undefined4 *)(DAT_0000e1a8 + 0xe164));
    sp<android::ICacaoProcessCallback>::operator=
              ((sp<android::ICacaoProcessCallback> *)this,local_1c);
    sp<android::Cacao::DeathNotifier>::~sp((sp<android::Cacao::DeathNotifier> *)&local_1c);
    if (*(int *)this == 0) {
      this_00 = operator_new(0x1c);
      BpCacao::BpCacao(this_00,param_1);
      sp<android::ICacaoProcessCallback>::operator=
                ((sp<android::ICacaoProcessCallback> *)this,(ICacaoProcessCallback *)this_00);
    }
  }
  if (*(int *)*puVar2 == local_18) {
    return;
  }
                    /* WARNING: Subroutine does not return */
  __stack_chk_fail();
}

// ====== 0000e1ac: BpCacao ======
/* android::BpCacao::BpCacao(android::sp<android::IBinder> const&) */

BpCacao * __thiscall android::BpCacao::BpCacao(BpCacao *this,sp *param_1)

{
  int iVar1;
  int *piVar2;
  int iVar3;
  
  android::RefBase::RefBase((RefBase *)(this + 0x14));
  iVar3 = *(int *)(DAT_0000e204 + 0xe1c0);
  android::IInterface::IInterface((IInterface *)this);
  iVar1 = *(int *)(DAT_0000e208 + 0xe1d2);
  *(int *)(this + 0x14) = iVar1 + 0x58;
  *(int *)this = iVar1 + 0xc;
  piVar2 = (int *)android::BpRefBase::BpRefBase((BpRefBase *)(this + 4),(sp *)(iVar3 + 0x18));
  iVar1 = *(int *)(DAT_0000e20c + 0xe1f0);
  *(int *)(this + 0x14) = iVar1 + 0x78;
  *(int *)this = iVar1 + 0xc;
  *piVar2 = iVar1 + 0x48;
  return this;
}

// ====== 0000e210: onTransact ======
/* android::BnCacao::onTransact(unsigned int, android::Parcel const&, android::Parcel*, unsigned
   int) */

undefined4 android::BnCacao::onTransact(uint param_1,Parcel *param_2,Parcel *param_3,uint param_4)

{
  uint uVar1;
  undefined4 uVar2;
  undefined4 uVar3;
  ImageBuf *pIVar4;
  undefined4 *puVar5;
  int *piVar6;
  int *piVar7;
  int iVar8;
  int iVar9;
  undefined4 uVar10;
  int **ppiVar11;
  int iVar12;
  sp<android::Cacao::DeathNotifier> *psVar13;
  code *pcVar14;
  int *piVar15;
  uint uVar16;
  undefined4 *puVar17;
  undefined8 uVar18;
  undefined4 local_6c0 [99];
  VectorImpl *local_534;
  int *local_530;
  undefined4 auStack_52c [101];
  undefined4 local_398;
  ICacaoProcessCallback *pIStack_394;
  int *local_390;
  int *local_38c;
  int local_388 [5];
  undefined4 local_374;
  ICacaoProcessCallback aIStack_370 [4];
  int local_36c;
  int local_368;
  int local_364;
  uint local_360;
  int local_354;
  int local_350 [97];
  undefined4 local_1cc;
  undefined4 local_1c8;
  int *local_1bc;
  int *local_1b8 [96];
  undefined4 local_38;
  undefined4 local_34;
  int local_28;
  
  puVar17 = (undefined4 *)(DAT_0000e890 + 0xe228);
  local_28 = *(int *)*puVar17;
  switch(param_2) {
  case (Parcel *)0x1:
    iVar9 = android::Parcel::checkInterface((IBinder *)param_3);
    if (iVar9 == 0) break;
    (**(code **)(*(int *)param_1 + 0x10))(param_1);
LAB_0000e822:
    uVar10 = 0;
    goto LAB_0000e82a;
  case (Parcel *)0x2:
    iVar9 = android::Parcel::checkInterface((IBinder *)param_3);
    if (iVar9 != 0) {
      android::Parcel::readStrongBinder();
      android::IMemory::asInterface((sp *)local_350);
      sp<android::Cacao::DeathNotifier>::~sp((sp<android::Cacao::DeathNotifier> *)&local_1bc);
      __aeabi_memclr4(local_1b8,400);
      piVar6 = (int *)android::Parcel::readInt32();
      local_1bc = piVar6;
      if (piVar6 != (int *)0x0) {
        piVar15 = (int *)0x0;
        ppiVar11 = local_1b8;
        do {
          piVar7 = (int *)android::Parcel::readFileDescriptor();
          piVar15 = (int *)((int)piVar15 + 1);
          *ppiVar11 = piVar7;
          ppiVar11 = ppiVar11 + 1;
        } while (piVar15 != piVar6);
      }
      if (local_350[0] != 0) {
        local_38 = android::IMemory::size();
        local_34 = android::IMemory::pointer();
      }
      ppiVar11 = local_1b8;
      pcVar14 = *(code **)(*(int *)param_1 + 0x14);
      iVar9 = 0x18c;
      puVar5 = local_6c0;
      do {
        ppiVar11 = ppiVar11 + 1;
        iVar9 = iVar9 + -4;
        *puVar5 = *ppiVar11;
        puVar5 = puVar5 + 1;
      } while (iVar9 != 0);
      piVar6 = local_350;
      piVar15 = local_1bc;
LAB_0000e478:
      psVar13 = (sp<android::Cacao::DeathNotifier> *)local_350;
      (*pcVar14)(param_1,piVar6,piVar15,local_1b8[0]);
      android::Parcel::writeInt32(param_4);
LAB_0000e7f4:
      sp<android::Cacao::DeathNotifier>::~sp(psVar13);
      goto LAB_0000e822;
    }
    break;
  case (Parcel *)0x3:
    iVar9 = android::Parcel::checkInterface((IBinder *)param_3);
    if (iVar9 != 0) {
      (**(code **)(*(int *)param_1 + 0x18))(param_1);
LAB_0000e81a:
      android::Parcel::writeInt32(param_4);
      goto LAB_0000e822;
    }
    break;
  case (Parcel *)0x4:
    iVar9 = android::Parcel::checkInterface((IBinder *)param_3);
    if (iVar9 != 0) {
      local_38c = (int *)android::Parcel::readInt32();
      android::Parcel::readStrongBinder();
      android::IMemory::asInterface((sp *)local_350);
      sp<android::Cacao::DeathNotifier>::~sp((sp<android::Cacao::DeathNotifier> *)&local_1bc);
      __aeabi_memclr4(local_1b8,400);
      piVar6 = (int *)android::Parcel::readInt32();
      local_1bc = piVar6;
      if (piVar6 != (int *)0x0) {
        piVar15 = (int *)0x0;
        ppiVar11 = local_1b8;
        do {
          piVar7 = (int *)android::Parcel::readFileDescriptor();
          piVar15 = (int *)((int)piVar15 + 1);
          *ppiVar11 = piVar7;
          ppiVar11 = ppiVar11 + 1;
        } while (piVar15 != piVar6);
      }
      if (local_350[0] != 0) {
        local_38 = android::IMemory::size();
        local_34 = android::IMemory::pointer();
      }
      iVar9 = 400;
      pcVar14 = *(code **)(*(int *)param_1 + 0x1c);
      puVar5 = local_6c0;
      ppiVar11 = local_1b8;
      do {
        iVar9 = iVar9 + -4;
        *puVar5 = *ppiVar11;
        puVar5 = puVar5 + 1;
        ppiVar11 = ppiVar11 + 1;
      } while (iVar9 != 0);
LAB_0000e46c:
      piVar6 = local_38c;
      piVar15 = local_350;
      local_1b8[0] = local_1bc;
      goto LAB_0000e478;
    }
    break;
  case (Parcel *)0x5:
    iVar9 = android::Parcel::checkInterface((IBinder *)param_3);
    if (iVar9 != 0) {
      local_38c = (int *)android::Parcel::readInt32();
      android::Parcel::readStrongBinder();
      android::IMemory::asInterface((sp *)local_350);
      sp<android::Cacao::DeathNotifier>::~sp((sp<android::Cacao::DeathNotifier> *)&local_1bc);
      __aeabi_memclr4(local_1b8,400);
      piVar6 = (int *)android::Parcel::readInt32();
      local_1bc = piVar6;
      if (piVar6 != (int *)0x0) {
        piVar15 = (int *)0x0;
        ppiVar11 = local_1b8;
        do {
          piVar7 = (int *)android::Parcel::readFileDescriptor();
          piVar15 = (int *)((int)piVar15 + 1);
          *ppiVar11 = piVar7;
          ppiVar11 = ppiVar11 + 1;
        } while (piVar15 != piVar6);
      }
      if (local_350[0] != 0) {
        local_38 = android::IMemory::size();
        local_34 = android::IMemory::pointer();
      }
      iVar9 = 400;
      pcVar14 = *(code **)(*(int *)param_1 + 0x20);
      puVar5 = local_6c0;
      ppiVar11 = local_1b8;
      do {
        iVar9 = iVar9 + -4;
        *puVar5 = *ppiVar11;
        puVar5 = puVar5 + 1;
        ppiVar11 = ppiVar11 + 1;
      } while (iVar9 != 0);
      goto LAB_0000e46c;
    }
    break;
  case (Parcel *)0x6:
    iVar9 = android::Parcel::checkInterface((IBinder *)param_3);
    if (iVar9 != 0) {
      local_390 = (int *)param_1;
      android::Parcel::readStrongBinder();
      android::IMemory::asInterface((sp *)&local_354);
      sp<android::Cacao::DeathNotifier>::~sp((sp<android::Cacao::DeathNotifier> *)&local_1bc);
      __aeabi_memclr4(local_1b8,400);
      piVar6 = (int *)android::Parcel::readInt32();
      local_1bc = piVar6;
      if (piVar6 != (int *)0x0) {
        piVar15 = (int *)0x0;
        ppiVar11 = local_1b8;
        do {
          piVar7 = (int *)android::Parcel::readFileDescriptor();
          piVar15 = (int *)((int)piVar15 + 1);
          *ppiVar11 = piVar7;
          ppiVar11 = ppiVar11 + 1;
        } while (piVar15 != piVar6);
      }
      local_38c = (int *)param_4;
      if (local_354 != 0) {
        local_38 = android::IMemory::size();
        local_34 = android::IMemory::pointer();
      }
      uVar1 = android::Parcel::readInt32();
      android::VectorImpl::VectorImpl((VectorImpl *)&local_368,4,7);
      local_368 = *(int *)(DAT_0000e894 + 0xe512) + 8;
      if (uVar1 != 0) {
        uVar16 = 0;
LAB_0000e520:
        uVar10 = android::Parcel::readUint32();
        uVar2 = android::Parcel::readUint32();
        uVar3 = android::Parcel::readInt32();
        uVar18 = android::Parcel::readNativeHandle();
        pIVar4 = cacao::ObjectBase::operator_new
                           ((ObjectBase *)0x28,(uint)((ulonglong)uVar18 >> 0x20));
        local_6c0[1] = 0;
        local_6c0[0] = uVar3;
        local_350[0] = cacao::ImageBuf::ImageBuf(pIVar4,(int)uVar18,uVar10,uVar2);
        if (local_350[0] != 0) goto code_r0x0000e562;
        puVar17 = (undefined4 *)(DAT_0000e898 + 0xe84e);
        local_350[0] = 0;
        if (local_360 != 0) {
          uVar16 = 0;
          uVar1 = local_360;
          do {
            piVar6 = *(int **)(local_364 + uVar16 * 4);
            if (piVar6 != (int *)0x0) {
              (**(code **)(*piVar6 + 4))();
              uVar1 = local_360;
            }
            uVar16 = uVar16 + 1;
          } while (uVar16 < uVar1);
        }
        uVar10 = 0xfffffff4;
        goto LAB_0000e86a;
      }
LAB_0000e574:
      android::Parcel::readStrongBinder();
      android::IMemory::asInterface((sp *)&local_36c);
      sp<android::Cacao::DeathNotifier>::~sp((sp<android::Cacao::DeathNotifier> *)local_350);
      __aeabi_memclr4(local_350 + 1,400);
      iVar9 = android::Parcel::readInt32();
      local_350[0] = iVar9;
      if (iVar9 != 0) {
        iVar12 = 0;
        piVar6 = local_350 + 1;
        do {
          iVar8 = android::Parcel::readFileDescriptor();
          iVar12 = iVar12 + 1;
          *piVar6 = iVar8;
          piVar6 = piVar6 + 1;
        } while (iVar12 != iVar9);
      }
      puVar17 = (undefined4 *)(DAT_0000e89c + 0xe5c8);
      if (local_36c != 0) {
        local_1cc = android::IMemory::size();
        local_1c8 = android::IMemory::pointer();
      }
      uVar10 = android::Parcel::readInt32();
      android::Parcel::readStrongBinder();
      ICacaoProcessCallback::asInterface(aIStack_370,(sp *)&local_374);
      sp<android::Cacao::DeathNotifier>::~sp((sp<android::Cacao::DeathNotifier> *)&local_374);
      piVar6 = local_390;
      pcVar14 = *(code **)(*local_390 + 0x24);
      android::VectorImpl::VectorImpl((VectorImpl *)local_388,(VectorImpl *)&local_368);
      iVar9 = 0x18c;
      iVar12 = *(int *)(DAT_0000e8a4 + 0xe61c) + 8;
      local_530 = &local_36c;
      ppiVar11 = local_1b8;
      puVar5 = local_6c0;
      local_534 = (VectorImpl *)local_388;
      local_398 = uVar10;
      pIStack_394 = aIStack_370;
      local_388[0] = iVar12;
      do {
        ppiVar11 = ppiVar11 + 1;
        iVar9 = iVar9 + -4;
        *puVar5 = *ppiVar11;
        puVar5 = puVar5 + 1;
      } while (iVar9 != 0);
      iVar9 = 0x194;
      puVar5 = auStack_52c;
      psVar13 = (sp<android::Cacao::DeathNotifier> *)local_350;
      do {
        iVar9 = iVar9 + -4;
        *puVar5 = *(undefined4 *)psVar13;
        puVar5 = puVar5 + 1;
        psVar13 = psVar13 + 4;
      } while (iVar9 != 0);
      uVar10 = (*pcVar14)(piVar6,&local_354,local_1bc,local_1b8[0]);
      local_388[0] = iVar12;
      android::VectorImpl::finish_vector();
      android::VectorImpl::~VectorImpl((VectorImpl *)local_388);
      local_374 = uVar10;
      android::Parcel::write(local_38c,(uint)&local_374);
      sp<android::Cacao::DeathNotifier>::~sp((sp<android::Cacao::DeathNotifier> *)aIStack_370);
      sp<android::Cacao::DeathNotifier>::~sp((sp<android::Cacao::DeathNotifier> *)&local_36c);
      uVar10 = 0;
LAB_0000e86a:
      local_368 = *(int *)(DAT_0000e8a0 + 0xe870) + 8;
      android::VectorImpl::finish_vector();
      android::VectorImpl::~VectorImpl((VectorImpl *)&local_368);
      sp<android::Cacao::DeathNotifier>::~sp((sp<android::Cacao::DeathNotifier> *)&local_354);
      goto LAB_0000e82a;
    }
    break;
  case (Parcel *)0x7:
    iVar9 = android::Parcel::checkInterface((IBinder *)param_3);
    if (iVar9 != 0) {
      local_38c = (int *)param_4;
      android::Parcel::readStrongBinder();
      android::IMemory::asInterface((sp *)&local_368);
      sp<android::Cacao::DeathNotifier>::~sp((sp<android::Cacao::DeathNotifier> *)&local_1bc);
      __aeabi_memclr4(local_1b8,400);
      piVar6 = (int *)android::Parcel::readInt32();
      local_1bc = piVar6;
      if (piVar6 != (int *)0x0) {
        piVar15 = (int *)0x0;
        ppiVar11 = local_1b8;
        do {
          piVar7 = (int *)android::Parcel::readFileDescriptor();
          piVar15 = (int *)((int)piVar15 + 1);
          *ppiVar11 = piVar7;
          ppiVar11 = ppiVar11 + 1;
        } while (piVar15 != piVar6);
      }
      if (local_368 != 0) {
        local_38 = android::IMemory::size();
        local_34 = android::IMemory::pointer();
      }
      android::Parcel::readStrongBinder();
      android::Parcel::readStrongBinder();
      android::IMemory::asInterface((sp *)&local_36c);
      sp<android::Cacao::DeathNotifier>::~sp((sp<android::Cacao::DeathNotifier> *)local_350);
      __aeabi_memclr4(local_350 + 1,400);
      iVar9 = android::Parcel::readInt32();
      local_350[0] = iVar9;
      if (iVar9 != 0) {
        iVar12 = 0;
        piVar6 = local_350 + 1;
        do {
          iVar8 = android::Parcel::readFileDescriptor();
          iVar12 = iVar12 + 1;
          *piVar6 = iVar8;
          piVar6 = piVar6 + 1;
        } while (iVar12 != iVar9);
      }
      if (local_36c != 0) {
        local_1cc = android::IMemory::size();
        local_1c8 = android::IMemory::pointer();
      }
      uVar10 = android::Parcel::readInt32();
      android::Parcel::readStrongBinder();
      ICacaoProcessCallback::asInterface(aIStack_370,(sp *)&local_374);
      sp<android::Cacao::DeathNotifier>::~sp((sp<android::Cacao::DeathNotifier> *)&local_374);
      iVar9 = 0x18c;
      pcVar14 = *(code **)(*(int *)param_1 + 0x28);
      local_534 = (VectorImpl *)&local_354;
      local_530 = &local_36c;
      ppiVar11 = local_1b8;
      puVar5 = local_6c0;
      local_398 = uVar10;
      pIStack_394 = aIStack_370;
      do {
        ppiVar11 = ppiVar11 + 1;
        iVar9 = iVar9 + -4;
        *puVar5 = *ppiVar11;
        piVar6 = local_38c;
        puVar5 = puVar5 + 1;
      } while (iVar9 != 0);
      iVar9 = 0x194;
      puVar5 = auStack_52c;
      psVar13 = (sp<android::Cacao::DeathNotifier> *)local_350;
      do {
        iVar9 = iVar9 + -4;
        *puVar5 = *(undefined4 *)psVar13;
        puVar5 = puVar5 + 1;
        psVar13 = psVar13 + 4;
      } while (iVar9 != 0);
      psVar13 = (sp<android::Cacao::DeathNotifier> *)&local_368;
      local_374 = (*pcVar14)(param_1,psVar13,local_1bc,local_1b8[0]);
      android::Parcel::write(piVar6,(uint)&local_374);
      sp<android::Cacao::DeathNotifier>::~sp((sp<android::Cacao::DeathNotifier> *)aIStack_370);
      sp<android::Cacao::DeathNotifier>::~sp((sp<android::Cacao::DeathNotifier> *)&local_36c);
      sp<android::Cacao::DeathNotifier>::~sp((sp<android::Cacao::DeathNotifier> *)&local_354);
      goto LAB_0000e7f4;
    }
    break;
  case (Parcel *)0x8:
    iVar9 = android::Parcel::checkInterface((IBinder *)param_3);
    if (iVar9 != 0) {
      uVar10 = android::Parcel::readAligned<intptr_t>();
      (**(code **)(*(int *)param_1 + 0x2c))(param_1,uVar10);
      goto LAB_0000e81a;
    }
    break;
  default:
    if (*(int *)*puVar17 == local_28) {
      uVar10 = (*(android::Parcel::readInt32 + DAT_000137b8))(param_1 + 4,param_2,param_3,param_4);
      return uVar10;
    }
    goto LAB_0000e88a;
  }
  uVar10 = 0xffffffff;
LAB_0000e82a:
  if (*(int *)*puVar17 == local_28) {
    return uVar10;
  }
LAB_0000e88a:
                    /* WARNING: Subroutine does not return */
  __stack_chk_fail();
code_r0x0000e562:
  android::VectorImpl::insertAt(&local_368,(uint)local_350,local_360);
  uVar16 = uVar16 + 1;
  if (uVar1 <= uVar16) goto LAB_0000e574;
  goto LAB_0000e520;
}

// ====== 0000e8a8: ~Vector ======
/* android::Vector<cacao::ImageBuf*>::~Vector() */

void __thiscall android::Vector<cacao::ImageBuf*>::~Vector(Vector<cacao::ImageBuf*> *this)

{
  *(int *)this = *(int *)(DAT_0000e8c8 + 0xe8b2) + 8;
  android::VectorImpl::finish_vector();
  android::VectorImpl::~VectorImpl((VectorImpl *)this);
  return;
}

// ====== 0000e8cc: onTransact ======
/* non-virtual thunk to android::BnCacao::onTransact(unsigned int, android::Parcel const&,
   android::Parcel*, unsigned int) */

void __thiscall
android::BnCacao::onTransact
          (BnCacao *this,uint param_1,Parcel *param_2,Parcel *param_3,uint param_4)

{
  (*(sp<android::IMemory>::sp<android::MemoryBase> + DAT_00013ac8))(this + -4);
  return;
}

// ====== 0000e8d4: queryLocalInterface ======
/* android::BnInterface<android::ICacao>::queryLocalInterface(android::String16 const&) */

void android::BnInterface<android::ICacao>::queryLocalInterface(String16 *param_1)

{
  undefined4 uVar1;
  undefined4 uVar2;
  int iVar3;
  MemoryBase *in_r1;
  undefined4 *in_r2;
  undefined4 uVar4;
  undefined4 uVar5;
  
  uVar5 = *in_r2;
  uVar1 = android::String16::size();
  uVar4 = **(undefined4 **)(DAT_0000e914 + 0xe8ec);
  uVar2 = android::String16::size();
  iVar3 = strzcmp16(uVar5,uVar1,uVar4,uVar2);
  if (iVar3 != 0) {
    *(undefined4 *)param_1 = 0;
    return;
  }
  sp<android::IMemory>::sp<android::MemoryBase>((sp<android::IMemory> *)param_1,in_r1);
  return;
}

// ====== 0000e918: queryLocalInterface ======
/* non-virtual thunk to android::BnInterface<android::ICacao>::queryLocalInterface(android::String16
   const&) */

void __thiscall
android::BnInterface<android::ICacao>::queryLocalInterface
          (BnInterface<android::ICacao> *this,String16 *param_1)

{
  undefined4 uVar1;
  undefined4 uVar2;
  int iVar3;
  undefined4 *in_r2;
  undefined4 uVar4;
  undefined4 uVar5;
  
  uVar5 = *in_r2;
  uVar1 = android::String16::size();
  uVar4 = **(undefined4 **)(DAT_0000e95c + 0xe930);
  uVar2 = android::String16::size();
  iVar3 = strzcmp16(uVar5,uVar1,uVar4,uVar2);
  if (iVar3 != 0) {
    *(undefined4 *)this = 0;
    return;
  }
  sp<android::IMemory>::sp<android::MemoryBase>
            ((sp<android::IMemory> *)this,(MemoryBase *)(param_1 + -4));
  return;
}

// ====== 0000e960: ~BpCacao ======
/* android::BpCacao::~BpCacao() */

BpCacao * __thiscall android::BpCacao::~BpCacao(BpCacao *this)

{
  android::BpRefBase::~BpRefBase((BpRefBase *)(this + 4));
  android::IInterface::~IInterface((IInterface *)this);
  android::RefBase::~RefBase((RefBase *)(this + 0x14));
  return this;
}

// ====== 0000e990: ~BpCacao ======
/* android::BpCacao::~BpCacao() */

void __thiscall android::BpCacao::~BpCacao(BpCacao *this)

{
  android::BpRefBase::~BpRefBase((BpRefBase *)(this + 4));
  android::IInterface::~IInterface((IInterface *)this);
  android::RefBase::~RefBase((RefBase *)(this + 0x14));
  operator_delete(this);
  return;
}

// ====== 0000e9c4: disconnect ======
/* android::BpCacao::disconnect() */

void __thiscall android::BpCacao::disconnect(BpCacao *this)

{
  undefined4 *puVar1;
  Parcel aPStack_80 [52];
  Parcel aPStack_4c [52];
  int local_18;
  
  puVar1 = (undefined4 *)(DAT_0000ea20 + 0xe9d0);
  local_18 = *(int *)*puVar1;
  android::Parcel::Parcel(aPStack_4c);
  android::Parcel::Parcel(aPStack_80);
  android::Parcel::writeInterfaceToken((String16 *)aPStack_4c);
  (**(code **)(**(int **)(this + 8) + 0x14))(*(int **)(this + 8),1,aPStack_4c,aPStack_80,0);
  android::Parcel::~Parcel(aPStack_80);
  android::Parcel::~Parcel(aPStack_4c);
  if (*(int *)*puVar1 == local_18) {
    return;
  }
                    /* WARNING: Subroutine does not return */
  __stack_chk_fail();
}

// ====== 0000ea28: start ======
/* android::BpCacao::start(android::sp<android::IMemory> const&, cacao::ISerialize::SerializedData)
    */

undefined4 android::BpCacao::start(int param_1,sp *param_2,int param_3,undefined4 param_4)

{
  int iVar1;
  undefined4 uVar2;
  int iVar3;
  int *piVar4;
  undefined4 *puVar5;
  sp<android::IInterface> asStack_98 [4];
  sp asStack_94 [4];
  Parcel aPStack_90 [52];
  Parcel aPStack_5c [52];
  int local_28;
  int local_8;
  int iStack_4;
  
  puVar5 = (undefined4 *)(DAT_0000eaf8 + 0xea40);
  local_28 = *(int *)*puVar5;
  local_8 = param_3;
  iStack_4 = param_4;
  android::Parcel::Parcel(aPStack_5c);
  android::Parcel::Parcel(aPStack_90);
  android::Parcel::writeInterfaceToken((String16 *)aPStack_5c);
  sp<android::IInterface>::sp<android::ICacaoService>(asStack_98,param_2);
  android::IInterface::asBinder(asStack_94);
  android::Parcel::writeStrongBinder((sp *)aPStack_5c);
  sp<android::Cacao::DeathNotifier>::~sp((sp<android::Cacao::DeathNotifier> *)asStack_94);
  sp<android::Cacao::DeathNotifier>::~sp((sp<android::Cacao::DeathNotifier> *)asStack_98);
  iVar1 = local_8;
  android::Parcel::writeInt32((int)aPStack_5c);
  if (iVar1 != 0) {
    piVar4 = &local_8;
    iVar3 = 0;
    do {
      piVar4 = piVar4 + 1;
      android::Parcel::writeFileDescriptor((int)aPStack_5c,SUB41(*piVar4,0));
      iVar3 = iVar3 + 1;
    } while (iVar3 != iVar1);
  }
  (**(code **)(**(int **)(param_1 + 8) + 0x14))(*(int **)(param_1 + 8),2,aPStack_5c,aPStack_90,0);
  uVar2 = android::Parcel::readInt32();
  android::Parcel::~Parcel(aPStack_90);
  android::Parcel::~Parcel(aPStack_5c);
  if (*(int *)*puVar5 == local_28) {
    return uVar2;
  }
                    /* WARNING: Subroutine does not return */
  __stack_chk_fail();
}

// ====== 0000eb00: stop ======
/* android::BpCacao::stop() */

undefined4 __thiscall android::BpCacao::stop(BpCacao *this)

{
  undefined4 uVar1;
  undefined4 *puVar2;
  Parcel aPStack_80 [52];
  Parcel aPStack_4c [52];
  int local_18;
  
  puVar2 = (undefined4 *)(DAT_0000eb68 + 0xeb0c);
  local_18 = *(int *)*puVar2;
  android::Parcel::Parcel(aPStack_4c);
  android::Parcel::Parcel(aPStack_80);
  android::Parcel::writeInterfaceToken((String16 *)aPStack_4c);
  (**(code **)(**(int **)(this + 8) + 0x14))(*(int **)(this + 8),3,aPStack_4c,aPStack_80,0);
  uVar1 = android::Parcel::readInt32();
  android::Parcel::~Parcel(aPStack_80);
  android::Parcel::~Parcel(aPStack_4c);
  if (*(int *)*puVar2 == local_18) {
    return uVar1;
  }
                    /* WARNING: Subroutine does not return */
  __stack_chk_fail();
}

// ====== 0000eb70: setConfig ======
/* android::BpCacao::setConfig(int, android::sp<android::IMemory> const&,
   cacao::ISerialize::SerializedData) */

undefined4 __thiscall
android::BpCacao::setConfig(BpCacao *this,undefined4 param_2_00,sp *param_2,int param_4)

{
  int iVar1;
  undefined4 uVar2;
  int iVar3;
  int *piVar4;
  undefined4 *puVar5;
  sp<android::IInterface> asStack_94 [4];
  sp asStack_90 [4];
  Parcel aPStack_8c [52];
  Parcel aPStack_58 [52];
  int local_24;
  int local_4;
  
  puVar5 = (undefined4 *)(DAT_0000ec48 + 0xeb88);
  local_24 = *(int *)*puVar5;
  local_4 = param_4;
  android::Parcel::Parcel(aPStack_58);
  android::Parcel::Parcel(aPStack_8c);
  android::Parcel::writeInterfaceToken((String16 *)aPStack_58);
  android::Parcel::writeInt32((int)aPStack_58);
  sp<android::IInterface>::sp<android::ICacaoService>(asStack_94,param_2);
  android::IInterface::asBinder(asStack_90);
  android::Parcel::writeStrongBinder((sp *)aPStack_58);
  sp<android::Cacao::DeathNotifier>::~sp((sp<android::Cacao::DeathNotifier> *)asStack_90);
  sp<android::Cacao::DeathNotifier>::~sp((sp<android::Cacao::DeathNotifier> *)asStack_94);
  iVar1 = local_4;
  android::Parcel::writeInt32((int)aPStack_58);
  if (iVar1 != 0) {
    piVar4 = &local_4;
    iVar3 = 0;
    do {
      piVar4 = piVar4 + 1;
      android::Parcel::writeFileDescriptor((int)aPStack_58,SUB41(*piVar4,0));
      iVar3 = iVar3 + 1;
    } while (iVar3 != iVar1);
  }
  (**(code **)(**(int **)(this + 8) + 0x14))(*(int **)(this + 8),4,aPStack_58,aPStack_8c,0);
  uVar2 = android::Parcel::readInt32();
  android::Parcel::~Parcel(aPStack_8c);
  android::Parcel::~Parcel(aPStack_58);
  if (*(int *)*puVar5 == local_24) {
    return uVar2;
  }
                    /* WARNING: Subroutine does not return */
  __stack_chk_fail();
}

// ====== 0000ec50: getConfig ======
/* android::BpCacao::getConfig(int, android::sp<android::IMemory> const&,
   cacao::ISerialize::SerializedData) */

undefined4 __thiscall
android::BpCacao::getConfig(BpCacao *this,undefined4 param_2_00,sp *param_2,int param_4)

{
  int iVar1;
  undefined4 uVar2;
  int iVar3;
  int *piVar4;
  undefined4 *puVar5;
  sp<android::IInterface> asStack_94 [4];
  sp asStack_90 [4];
  Parcel aPStack_8c [52];
  Parcel aPStack_58 [52];
  int local_24;
  int local_4;
  
  puVar5 = (undefined4 *)(DAT_0000ed28 + 0xec68);
  local_24 = *(int *)*puVar5;
  local_4 = param_4;
  android::Parcel::Parcel(aPStack_58);
  android::Parcel::Parcel(aPStack_8c);
  android::Parcel::writeInterfaceToken((String16 *)aPStack_58);
  android::Parcel::writeInt32((int)aPStack_58);
  sp<android::IInterface>::sp<android::ICacaoService>(asStack_94,param_2);
  android::IInterface::asBinder(asStack_90);
  android::Parcel::writeStrongBinder((sp *)aPStack_58);
  sp<android::Cacao::DeathNotifier>::~sp((sp<android::Cacao::DeathNotifier> *)asStack_90);
  sp<android::Cacao::DeathNotifier>::~sp((sp<android::Cacao::DeathNotifier> *)asStack_94);
  iVar1 = local_4;
  android::Parcel::writeInt32((int)aPStack_58);
  if (iVar1 != 0) {
    piVar4 = &local_4;
    iVar3 = 0;
    do {
      piVar4 = piVar4 + 1;
      android::Parcel::writeFileDescriptor((int)aPStack_58,SUB41(*piVar4,0));
      iVar3 = iVar3 + 1;
    } while (iVar3 != iVar1);
  }
  (**(code **)(**(int **)(this + 8) + 0x14))(*(int **)(this + 8),5,aPStack_58,aPStack_8c,0);
  uVar2 = android::Parcel::readInt32();
  android::Parcel::~Parcel(aPStack_8c);
  android::Parcel::~Parcel(aPStack_58);
  if (*(int *)*puVar5 == local_24) {
    return uVar2;
  }
                    /* WARNING: Subroutine does not return */
  __stack_chk_fail();
}

// ====== 0000ed30: process ======
/* android::BpCacao::process(android::sp<android::IMemory> const&,
   cacao::ISerialize::SerializedData, android::Vector<cacao::ImageBuf*>,
   android::sp<android::IMemory> const&, cacao::ISerialize::SerializedData, int,
   android::sp<android::ICacaoProcessCallback> const&) */

undefined4 android::BpCacao::process(int param_1,sp *param_2,int param_3,undefined4 param_4)

{
  int iVar1;
  undefined4 uVar2;
  int iVar3;
  uint uVar4;
  int *piVar5;
  undefined4 *puVar6;
  int in_stack_0000018c;
  sp *in_stack_00000190;
  int in_stack_00000194;
  sp *in_stack_0000032c;
  sp local_a4 [8];
  sp<android::IInterface> asStack_9c [4];
  Parcel aPStack_98 [52];
  Parcel aPStack_64 [52];
  int local_30;
  int local_8;
  int iStack_4;
  
  local_30 = **(int **)(DAT_0000eefc + 0xed46);
  local_8 = param_3;
  iStack_4 = param_4;
  android::Parcel::Parcel(aPStack_64);
  android::Parcel::Parcel(aPStack_98);
  android::Parcel::writeInterfaceToken((String16 *)aPStack_64);
  sp<android::IInterface>::sp<android::ICacaoService>(asStack_9c,param_2);
  android::IInterface::asBinder(local_a4);
  android::Parcel::writeStrongBinder((sp *)aPStack_64);
  sp<android::Cacao::DeathNotifier>::~sp((sp<android::Cacao::DeathNotifier> *)local_a4);
  sp<android::Cacao::DeathNotifier>::~sp((sp<android::Cacao::DeathNotifier> *)asStack_9c);
  iVar1 = local_8;
  android::Parcel::writeInt32((int)aPStack_64);
  puVar6 = &stack0x00000194;
  if (iVar1 != 0) {
    piVar5 = &local_8;
    iVar3 = 0;
    do {
      piVar5 = piVar5 + 1;
      android::Parcel::writeFileDescriptor((int)aPStack_64,SUB41(*piVar5,0));
      iVar3 = iVar3 + 1;
    } while (iVar3 != iVar1);
  }
  android::Parcel::writeInt32((int)aPStack_64);
  if (*(int *)(in_stack_0000018c + 8) != 0) {
    uVar4 = 0;
    do {
      (**(code **)(**(int **)(*(int *)(in_stack_0000018c + 4) + uVar4 * 4) + 0x1c))(local_a4);
      android::Parcel::writeUint32((uint)aPStack_64);
      (**(code **)(**(int **)(*(int *)(in_stack_0000018c + 4) + uVar4 * 4) + 0x1c))(local_a4);
      android::Parcel::writeUint32((uint)aPStack_64);
      (**(code **)(**(int **)(*(int *)(in_stack_0000018c + 4) + uVar4 * 4) + 0x3c))();
      android::Parcel::writeInt32((int)aPStack_64);
      (**(code **)(**(int **)(*(int *)(in_stack_0000018c + 4) + uVar4 * 4) + 0x10))();
      android::Parcel::writeNativeHandle((native_handle *)aPStack_64);
      uVar4 = uVar4 + 1;
    } while (uVar4 < *(uint *)(in_stack_0000018c + 8));
  }
  sp<android::IInterface>::sp<android::ICacaoService>(asStack_9c,in_stack_00000190);
  android::IInterface::asBinder(local_a4);
  android::Parcel::writeStrongBinder((sp *)aPStack_64);
  sp<android::Cacao::DeathNotifier>::~sp((sp<android::Cacao::DeathNotifier> *)local_a4);
  sp<android::Cacao::DeathNotifier>::~sp((sp<android::Cacao::DeathNotifier> *)asStack_9c);
  iVar1 = in_stack_00000194;
  android::Parcel::writeInt32((int)aPStack_64);
  if (iVar1 != 0) {
    iVar3 = 0;
    do {
      puVar6 = puVar6 + 1;
      android::Parcel::writeFileDescriptor((int)aPStack_64,SUB41(*puVar6,0));
      iVar3 = iVar3 + 1;
    } while (iVar3 != iVar1);
  }
  android::Parcel::writeInt32((int)aPStack_64);
  sp<android::IInterface>::sp<android::ICacaoService>(asStack_9c,in_stack_0000032c);
  android::IInterface::asBinder(local_a4);
  android::Parcel::writeStrongBinder((sp *)aPStack_64);
  sp<android::Cacao::DeathNotifier>::~sp((sp<android::Cacao::DeathNotifier> *)local_a4);
  sp<android::Cacao::DeathNotifier>::~sp((sp<android::Cacao::DeathNotifier> *)asStack_9c);
  (**(code **)(**(int **)(param_1 + 8) + 0x14))
            (*(int **)(param_1 + 8),6,(sp *)aPStack_64,aPStack_98,0);
  uVar2 = android::Parcel::readAligned<intptr_t>();
  android::Parcel::~Parcel(aPStack_98);
  android::Parcel::~Parcel(aPStack_64);
  if (**(int **)(DAT_0000ef04 + 0xeee4) == local_30) {
    return uVar2;
  }
                    /* WARNING: Subroutine does not return */
  __stack_chk_fail();
}

// ====== 0000ef08: process ======
/* android::BpCacao::process(android::sp<android::IMemory> const&,
   cacao::ISerialize::SerializedData, android::sp<android::IBinder> const&,
   android::sp<android::IMemory> const&, cacao::ISerialize::SerializedData, int,
   android::sp<android::ICacaoProcessCallback> const&) */

undefined4 android::BpCacao::process(int param_1,sp *param_2,int param_3,undefined4 param_4)

{
  int iVar1;
  undefined4 uVar2;
  int iVar3;
  int *piVar4;
  undefined4 *puVar5;
  sp *in_stack_00000190;
  int in_stack_00000194;
  sp *in_stack_0000032c;
  sp<android::IInterface> asStack_a0 [4];
  sp asStack_9c [4];
  Parcel aPStack_98 [52];
  Parcel aPStack_64 [52];
  int local_30;
  int local_8;
  int iStack_4;
  
  local_30 = **(int **)(DAT_0000f06c + 0xef1e);
  local_8 = param_3;
  iStack_4 = param_4;
  android::Parcel::Parcel(aPStack_64);
  android::Parcel::Parcel(aPStack_98);
  android::Parcel::writeInterfaceToken((String16 *)aPStack_64);
  sp<android::IInterface>::sp<android::ICacaoService>(asStack_a0,param_2);
  android::IInterface::asBinder(asStack_9c);
  android::Parcel::writeStrongBinder((sp *)aPStack_64);
  sp<android::Cacao::DeathNotifier>::~sp((sp<android::Cacao::DeathNotifier> *)asStack_9c);
  sp<android::Cacao::DeathNotifier>::~sp((sp<android::Cacao::DeathNotifier> *)asStack_a0);
  iVar1 = local_8;
  android::Parcel::writeInt32((int)aPStack_64);
  puVar5 = &stack0x00000194;
  if (iVar1 != 0) {
    piVar4 = &local_8;
    iVar3 = 0;
    do {
      piVar4 = piVar4 + 1;
      android::Parcel::writeFileDescriptor((int)aPStack_64,SUB41(*piVar4,0));
      iVar3 = iVar3 + 1;
    } while (iVar3 != iVar1);
  }
  android::Parcel::writeStrongBinder((sp *)aPStack_64);
  sp<android::IInterface>::sp<android::ICacaoService>(asStack_a0,in_stack_00000190);
  android::IInterface::asBinder(asStack_9c);
  android::Parcel::writeStrongBinder((sp *)aPStack_64);
  sp<android::Cacao::DeathNotifier>::~sp((sp<android::Cacao::DeathNotifier> *)asStack_9c);
  sp<android::Cacao::DeathNotifier>::~sp((sp<android::Cacao::DeathNotifier> *)asStack_a0);
  iVar1 = in_stack_00000194;
  android::Parcel::writeInt32((int)aPStack_64);
  if (iVar1 != 0) {
    iVar3 = 0;
    do {
      puVar5 = puVar5 + 1;
      android::Parcel::writeFileDescriptor((int)aPStack_64,SUB41(*puVar5,0));
      iVar3 = iVar3 + 1;
    } while (iVar3 != iVar1);
  }
  android::Parcel::writeInt32((int)aPStack_64);
  sp<android::IInterface>::sp<android::ICacaoService>(asStack_a0,in_stack_0000032c);
  android::IInterface::asBinder(asStack_9c);
  android::Parcel::writeStrongBinder((sp *)aPStack_64);
  sp<android::Cacao::DeathNotifier>::~sp((sp<android::Cacao::DeathNotifier> *)asStack_9c);
  sp<android::Cacao::DeathNotifier>::~sp((sp<android::Cacao::DeathNotifier> *)asStack_a0);
  (**(code **)(**(int **)(param_1 + 8) + 0x14))
            (*(int **)(param_1 + 8),7,(sp *)aPStack_64,aPStack_98,0);
  uVar2 = android::Parcel::readAligned<intptr_t>();
  android::Parcel::~Parcel(aPStack_98);
  android::Parcel::~Parcel(aPStack_64);
  if (**(int **)(DAT_0000f074 + 0xf052) == local_30) {
    return uVar2;
  }
                    /* WARNING: Subroutine does not return */
  __stack_chk_fail();
}

// ====== 0000f078: cancel ======
/* android::BpCacao::cancel(int) */

undefined4 __thiscall android::BpCacao::cancel(BpCacao *this,int param_1)

{
  undefined4 uVar1;
  undefined4 *puVar2;
  Parcel aPStack_84 [52];
  Parcel aPStack_50 [52];
  int local_1c;
  int local_18;
  
  puVar2 = (undefined4 *)(DAT_0000f0ec + 0xf086);
  local_18 = *(int *)*puVar2;
  local_1c = param_1;
  android::Parcel::Parcel(aPStack_50);
  android::Parcel::Parcel(aPStack_84);
  android::Parcel::writeInterfaceToken((String16 *)aPStack_50);
  android::Parcel::write(aPStack_50,(uint)&local_1c);
  (**(code **)(**(int **)(this + 8) + 0x14))(*(int **)(this + 8),8,aPStack_50,aPStack_84,0);
  uVar1 = android::Parcel::readInt32();
  android::Parcel::~Parcel(aPStack_84);
  android::Parcel::~Parcel(aPStack_50);
  if (*(int *)*puVar2 == local_18) {
    return uVar1;
  }
                    /* WARNING: Subroutine does not return */
  __stack_chk_fail();
}

// ====== 0000f0f4: ~BpCacao ======
/* non-virtual thunk to android::BpCacao::~BpCacao() */

void __thiscall android::BpCacao::~BpCacao(BpCacao *this)

{
  android::BpRefBase::~BpRefBase((BpRefBase *)this);
  android::IInterface::~IInterface((IInterface *)(this + -4));
  android::RefBase::~RefBase((RefBase *)(this + 0x10));
  return;
}

// ====== 0000f124: ~BpCacao ======
/* non-virtual thunk to android::BpCacao::~BpCacao() */

void __thiscall android::BpCacao::~BpCacao(BpCacao *this)

{
  android::BpRefBase::~BpRefBase((BpRefBase *)this);
  android::IInterface::~IInterface((IInterface *)(this + -4));
  android::RefBase::~RefBase((RefBase *)(this + 0x10));
  operator_delete((IInterface *)(this + -4));
  return;
}

// ====== 0000f15c: ~BpCacao ======
/* virtual thunk to android::BpCacao::~BpCacao() */

void __thiscall android::BpCacao::~BpCacao(BpCacao *this)

{
  IInterface *this_00;
  
  this_00 = (IInterface *)(this + *(int *)(*(int *)this + -0xc));
  android::BpRefBase::~BpRefBase((BpRefBase *)(this_00 + 4));
  android::IInterface::~IInterface(this_00);
  android::RefBase::~RefBase((RefBase *)(this_00 + 0x14));
  return;
}

// ====== 0000f190: ~BpCacao ======
/* virtual thunk to android::BpCacao::~BpCacao() */

void __thiscall android::BpCacao::~BpCacao(BpCacao *this)

{
  IInterface *this_00;
  
  this_00 = (IInterface *)(this + *(int *)(*(int *)this + -0xc));
  android::BpRefBase::~BpRefBase((BpRefBase *)(this_00 + 4));
  android::IInterface::~IInterface(this_00);
  android::RefBase::~RefBase((RefBase *)(this_00 + 0x14));
  operator_delete(this_00);
  return;
}

// ====== 0000f1cc: ~Vector ======
/* android::Vector<cacao::ImageBuf*>::~Vector() */

void __thiscall android::Vector<cacao::ImageBuf*>::~Vector(Vector<cacao::ImageBuf*> *this)

{
  void *pvVar1;
  
  *(int *)this = *(int *)(DAT_0000f1f0 + 0xf1d6) + 8;
  android::VectorImpl::finish_vector();
  pvVar1 = (void *)android::VectorImpl::~VectorImpl((VectorImpl *)this);
  operator_delete(pvVar1);
  return;
}

// ====== 0000f1f4: ~AbstractUnwindCursor ======
/* libunwind::AbstractUnwindCursor::~AbstractUnwindCursor() */

AbstractUnwindCursor * __thiscall
libunwind::AbstractUnwindCursor::~AbstractUnwindCursor(AbstractUnwindCursor *this)

{
  return this;
}

// ====== 0000f1f6: do_copy ======
/* android::Vector<cacao::ImageBuf*>::do_copy(void*, void const*, unsigned int) const */

void __thiscall
android::Vector<cacao::ImageBuf*>::do_copy
          (Vector<cacao::ImageBuf*> *this,void *param_1,void *param_2,uint param_3)

{
  __aeabi_memcpy(param_1,param_2,param_3 << 2);
  return;
}

// ====== 0000f206: do_splat ======
/* android::Vector<cacao::ImageBuf*>::do_splat(void*, void const*, unsigned int) const */

Vector<cacao::ImageBuf*> * __thiscall
android::Vector<cacao::ImageBuf*>::do_splat
          (Vector<cacao::ImageBuf*> *this,void *param_1,void *param_2,uint param_3)

{
  for (; param_3 != 0; param_3 = param_3 - 1) {
    this = *(Vector<cacao::ImageBuf*> **)param_2;
    *(Vector<cacao::ImageBuf*> **)param_1 = this;
    param_1 = (undefined4 *)((int)param_1 + 4);
  }
  return this;
}

// ====== 0000f214: do_move_backward ======
/* android::Vector<cacao::ImageBuf*>::do_move_backward(void*, void const*, unsigned int) const */

void __thiscall
android::Vector<cacao::ImageBuf*>::do_move_backward
          (Vector<cacao::ImageBuf*> *this,void *param_1,void *param_2,uint param_3)

{
  __aeabi_memmove(param_1,param_2,param_3 << 2);
  return;
}

// ====== 0000f224: ~Mutex ======
/* android::Mutex::~Mutex() */

Mutex * __thiscall android::Mutex::~Mutex(Mutex *this)

{
  pthread_mutex_destroy((pthread_mutex_t *)this);
  return this;
}

// ====== 0000f230: CacaoProcessCallback ======
/* android::Cacao::CacaoProcessCallback::CacaoProcessCallback(cacao::ICBProcessResultListener*,
   cacao::ProcessResultBase*) */

CacaoProcessCallback * __thiscall
android::Cacao::CacaoProcessCallback::CacaoProcessCallback
          (CacaoProcessCallback *this,ICBProcessResultListener *param_1,ProcessResultBase *param_2)

{
  int iVar1;
  undefined4 uVar2;
  undefined4 in_r3;
  
  ICacaoClient::ICacaoClient((ICacaoClient *)this);
  android::BBinder::BBinder((BBinder *)(this + 4));
  iVar1 = *(int *)(param_1 + 8);
  *(int *)this = iVar1;
  *(undefined4 *)(this + *(int *)(iVar1 + -0xc)) = *(undefined4 *)(param_1 + 0x2c);
  *(undefined4 *)(this + 4) = *(undefined4 *)(param_1 + 0x30);
  iVar1 = *(int *)(param_1 + 4);
  *(int *)this = iVar1;
  *(undefined4 *)(this + *(int *)(iVar1 + -0xc)) = *(undefined4 *)(param_1 + 0x34);
  *(undefined4 *)(this + 4) = *(undefined4 *)(param_1 + 0x38);
  iVar1 = *(int *)param_1;
  *(int *)this = iVar1;
  *(undefined4 *)(this + *(int *)(iVar1 + -0xc)) = *(undefined4 *)(param_1 + 0x3c);
  uVar2 = *(undefined4 *)(param_1 + 0x40);
  *(ProcessResultBase **)(this + 0x10) = param_2;
  *(undefined4 *)(this + 0x14) = in_r3;
  *(undefined4 *)(this + 4) = uVar2;
  return this;
}

// ====== 0000f288: CacaoProcessCallback ======
/* android::Cacao::CacaoProcessCallback::CacaoProcessCallback(cacao::ICBProcessResultListener*,
   cacao::ProcessResultBase*) */

CacaoProcessCallback * __thiscall
android::Cacao::CacaoProcessCallback::CacaoProcessCallback
          (CacaoProcessCallback *this,ICBProcessResultListener *param_1,ProcessResultBase *param_2)

{
  int iVar1;
  
  android::RefBase::RefBase((RefBase *)(this + 0x18));
  ICacaoClient::ICacaoClient((ICacaoClient *)this);
  android::BBinder::BBinder((BBinder *)(this + 4));
  iVar1 = *(int *)(DAT_0000f2d8 + 0xf2ba);
  *(int *)this = iVar1 + 0xc;
  *(int *)(this + 4) = iVar1 + 0x38;
  *(ICBProcessResultListener **)(this + 0x10) = param_1;
  *(ProcessResultBase **)(this + 0x14) = param_2;
  *(int *)(this + 0x18) = iVar1 + 0x98;
  return this;
}

// ====== 0000f2dc: ~CacaoProcessCallback ======
/* android::Cacao::CacaoProcessCallback::~CacaoProcessCallback() */

void __thiscall
android::Cacao::CacaoProcessCallback::~CacaoProcessCallback(CacaoProcessCallback *this)

{
  int iVar1;
  int *in_r1;
  
  iVar1 = *in_r1;
  *(int *)this = iVar1;
  *(int *)(this + *(int *)(iVar1 + -0xc)) = in_r1[0xf];
  *(int *)(this + 4) = in_r1[0x10];
  android::BBinder::~BBinder((BBinder *)(this + 4));
  (*(~CacaoProcessCallback + DAT_00013b78))(this,in_r1 + 3);
  return;
}

// ====== 0000f30c: ~CacaoProcessCallback ======
/* android::Cacao::CacaoProcessCallback::~CacaoProcessCallback() */

CacaoProcessCallback * __thiscall
android::Cacao::CacaoProcessCallback::~CacaoProcessCallback(CacaoProcessCallback *this)

{
  ~CacaoProcessCallback(this);
  android::RefBase::~RefBase((RefBase *)(this + 0x18));
  return this;
}

// ====== 0000f32c: ~CacaoProcessCallback ======
/* non-virtual thunk to android::Cacao::CacaoProcessCallback::~CacaoProcessCallback() */

void __thiscall
android::Cacao::CacaoProcessCallback::~CacaoProcessCallback(CacaoProcessCallback *this)

{
  ~CacaoProcessCallback(this + -4);
  android::RefBase::~RefBase((RefBase *)(this + 0x14));
  return;
}

// ====== 0000f34c: ~CacaoProcessCallback ======
/* virtual thunk to android::Cacao::CacaoProcessCallback::~CacaoProcessCallback() */

void __thiscall
android::Cacao::CacaoProcessCallback::~CacaoProcessCallback(CacaoProcessCallback *this)

{
  int iVar1;
  
  iVar1 = ~CacaoProcessCallback(this + *(int *)(*(int *)this + -0xc));
  android::RefBase::~RefBase((RefBase *)(iVar1 + 0x18));
  return;
}

// ====== 0000f370: ~CacaoProcessCallback ======
/* android::Cacao::CacaoProcessCallback::~CacaoProcessCallback() */

void __thiscall
android::Cacao::CacaoProcessCallback::~CacaoProcessCallback(CacaoProcessCallback *this)

{
  ~CacaoProcessCallback(this);
  android::RefBase::~RefBase((RefBase *)(this + 0x18));
  operator_delete(this);
  return;
}

// ====== 0000f398: ~CacaoProcessCallback ======
/* non-virtual thunk to android::Cacao::CacaoProcessCallback::~CacaoProcessCallback() */

void __thiscall
android::Cacao::CacaoProcessCallback::~CacaoProcessCallback(CacaoProcessCallback *this)

{
  (*(ICacaoClient::ICacaoClient + DAT_00013b98))(this + -4);
  return;
}

// ====== 0000f39e: ~CacaoProcessCallback ======
/* virtual thunk to android::Cacao::CacaoProcessCallback::~CacaoProcessCallback() */

void __thiscall
android::Cacao::CacaoProcessCallback::~CacaoProcessCallback(CacaoProcessCallback *this)

{
  (*(ICacaoClient::ICacaoClient + DAT_00013b98))(this + *(int *)(*(int *)this + -0xc));
  return;
}

// ====== 0000f3aa: notifyProgress ======
/* android::Cacao::CacaoProcessCallback::notifyProgress(android::sp<android::IMemory> const&,
   cacao::ISerialize::SerializedData) */

void android::Cacao::CacaoProcessCallback::notifyProgress
               (int param_1,undefined4 param_2,undefined4 param_3,undefined4 param_4)

{
  int *piVar1;
  int *piVar2;
  undefined4 local_8;
  undefined4 uStack_4;
  
  piVar2 = *(int **)(param_1 + 0x10);
  piVar1 = *(int **)(param_1 + 0x14);
  local_8 = param_3;
  uStack_4 = param_4;
  if (piVar1 != (int *)0x0) {
    (**(code **)(*piVar1 + 0x2c))(piVar1,&local_8);
  }
  if (piVar2 != (int *)0x0) {
    (**(code **)(*piVar2 + 8))(piVar2,*(undefined4 *)(param_1 + 0x14));
  }
  return;
}

// ====== 0000f3d6: notifyResult ======
/* android::Cacao::CacaoProcessCallback::notifyResult(android::sp<android::IMemory> const&,
   cacao::ISerialize::SerializedData, android::Vector<cacao::ImageBuf*>*) */

void __thiscall
android::Cacao::CacaoProcessCallback::notifyResult
          (CacaoProcessCallback *this,undefined4 param_2,undefined4 param_3,undefined4 param_4)

{
  int *piVar1;
  int *piVar2;
  undefined4 local_8;
  undefined4 uStack_4;
  
  piVar2 = *(int **)(this + 0x10);
  piVar1 = *(int **)(this + 0x14);
  local_8 = param_3;
  uStack_4 = param_4;
  if (piVar1 != (int *)0x0) {
    (**(code **)(*piVar1 + 0x2c))(piVar1,&local_8);
  }
  if (piVar2 != (int *)0x0) {
    (**(code **)(*piVar2 + 0xc))(piVar2,*(undefined4 *)(this + 0x14));
  }
  return;
}

// ====== 0000f404: CacaoClient ======
/* android::Cacao::CacaoClient::CacaoClient() */

CacaoClient * __thiscall android::Cacao::CacaoClient::CacaoClient(CacaoClient *this)

{
  int iVar1;
  int *in_r1;
  int *piVar2;
  
  ICacaoClient::ICacaoClient((ICacaoClient *)this);
  android::BBinder::BBinder((BBinder *)(this + 4));
  iVar1 = in_r1[2];
  *(int *)this = iVar1;
  *(int *)(this + *(int *)(iVar1 + -0xc)) = in_r1[0xb];
  *(int *)(this + 4) = in_r1[0xc];
  iVar1 = in_r1[1];
  *(int *)this = iVar1;
  *(int *)(this + *(int *)(iVar1 + -0xc)) = in_r1[0xd];
  *(int *)(this + 4) = in_r1[0xe];
  iVar1 = in_r1[0xf];
  *(int *)(this + 0x10) = iVar1;
  *(int *)(this + *(int *)(iVar1 + -0xc) + 0x10) = in_r1[0x10];
  *(int *)(this + 0x14) = *(int *)(DAT_0000f48c + 0xf456) + 8;
  iVar1 = *in_r1;
  *(int *)this = iVar1;
  *(int *)(this + *(int *)(iVar1 + -0xc)) = in_r1[0x11];
  piVar2 = (int *)(DAT_0000f490 + 0xf46e);
  *(int *)(this + 4) = in_r1[0x12];
  iVar1 = *piVar2;
  *(int *)(this + 0x10) = in_r1[0x13];
  *(int *)(this + 0x14) = iVar1 + 0xc0;
  *(undefined4 *)(this + 0x18) = 0;
  pthread_mutex_init((pthread_mutex_t *)(this + 0x1c),(pthread_mutexattr_t *)0x0);
  *(undefined4 *)(this + 0x20) = 0;
  *(undefined4 *)(this + 0x24) = 0;
  return this;
}

// ====== 0000f494: CacaoClient ======
/* android::Cacao::CacaoClient::CacaoClient() */

CacaoClient * __thiscall android::Cacao::CacaoClient::CacaoClient(CacaoClient *this)

{
  int iVar1;
  
  android::RefBase::RefBase((RefBase *)(this + 0x28));
  ICacaoClient::ICacaoClient((ICacaoClient *)this);
  android::BBinder::BBinder((BBinder *)(this + 4));
  iVar1 = *(int *)(DAT_0000f4f8 + 0xf4c2);
  *(int *)(this + 0x28) = iVar1 + 0x108;
  *(int *)this = iVar1 + 0xc;
  *(int *)(this + 4) = iVar1 + 0x5c;
  *(int *)(this + 0x10) = iVar1 + 0xac;
  *(int *)(this + 0x14) = iVar1 + 0xc0;
  *(undefined4 *)(this + 0x18) = 0;
  pthread_mutex_init((pthread_mutex_t *)(this + 0x1c),(pthread_mutexattr_t *)0x0);
  *(undefined4 *)(this + 0x20) = 0;
  *(undefined4 *)(this + 0x24) = 0;
  return this;
}

// ====== 0000f4fc: ~CacaoClient ======
/* android::Cacao::CacaoClient::~CacaoClient() */

void __thiscall android::Cacao::CacaoClient::~CacaoClient(CacaoClient *this)

{
  int iVar1;
  int *in_r1;
  int *piVar2;
  
  iVar1 = *in_r1;
  *(int *)this = iVar1;
  *(int *)(this + *(int *)(iVar1 + -0xc)) = in_r1[0x11];
  piVar2 = (int *)(DAT_0000f548 + 0xf516);
  *(int *)(this + 4) = in_r1[0x12];
  iVar1 = *piVar2;
  *(int *)(this + 0x10) = in_r1[0x13];
  *(int *)(this + 0x14) = iVar1 + 0xc0;
  pthread_mutex_destroy((pthread_mutex_t *)(this + 0x1c));
  sp<android::Cacao::DeathNotifier>::~sp((sp<android::Cacao::DeathNotifier> *)(this + 0x18));
  android::BBinder::~BBinder((BBinder *)(this + 4));
  (*(~CacaoClient + DAT_00013bc8))(this,in_r1 + 3);
  return;
}

// ====== 0000f54c: ~CacaoClient ======
/* android::Cacao::CacaoClient::~CacaoClient() */

CacaoClient * __thiscall android::Cacao::CacaoClient::~CacaoClient(CacaoClient *this)

{
  ~CacaoClient(this);
  android::RefBase::~RefBase((RefBase *)(this + 0x28));
  return this;
}

// ====== 0000f56c: ~CacaoClient ======
/* non-virtual thunk to android::Cacao::CacaoClient::~CacaoClient() */

void __thiscall android::Cacao::CacaoClient::~CacaoClient(CacaoClient *this)

{
  ~CacaoClient(this + -4);
  android::RefBase::~RefBase((RefBase *)(this + 0x24));
  return;
}

// ====== 0000f58c: ~CacaoClient ======
/* non-virtual thunk to android::Cacao::CacaoClient::~CacaoClient() */

void __thiscall android::Cacao::CacaoClient::~CacaoClient(CacaoClient *this)

{
  ~CacaoClient(this + -0x10);
  android::RefBase::~RefBase((RefBase *)(this + 0x18));
  return;
}

// ====== 0000f5b0: ~CacaoClient ======
/* non-virtual thunk to android::Cacao::CacaoClient::~CacaoClient() */

void __thiscall android::Cacao::CacaoClient::~CacaoClient(CacaoClient *this)

{
  ~CacaoClient(this + -0x14);
  android::RefBase::~RefBase((RefBase *)(this + 0x14));
  return;
}

// ====== 0000f5d4: ~CacaoClient ======
/* virtual thunk to android::Cacao::CacaoClient::~CacaoClient() */

void __thiscall android::Cacao::CacaoClient::~CacaoClient(CacaoClient *this)

{
  int iVar1;
  
  iVar1 = ~CacaoClient(this + *(int *)(*(int *)this + -0xc));
  android::RefBase::~RefBase((RefBase *)(iVar1 + 0x28));
  return;
}

// ====== 0000f5f8: ~CacaoClient ======
/* android::Cacao::CacaoClient::~CacaoClient() */

void __thiscall android::Cacao::CacaoClient::~CacaoClient(CacaoClient *this)

{
  ~CacaoClient(this);
  android::RefBase::~RefBase((RefBase *)(this + 0x28));
  operator_delete(this);
  return;
}

// ====== 0000f620: ~CacaoClient ======
/* non-virtual thunk to android::Cacao::CacaoClient::~CacaoClient() */

void __thiscall android::Cacao::CacaoClient::~CacaoClient(CacaoClient *this)

{
  (*(pthread_mutex_lock + DAT_00013be8))(this + -4);
  return;
}

// ====== 0000f626: ~CacaoClient ======
/* non-virtual thunk to android::Cacao::CacaoClient::~CacaoClient() */

void __thiscall android::Cacao::CacaoClient::~CacaoClient(CacaoClient *this)

{
  (*(pthread_mutex_lock + DAT_00013be8))(this + -0x10);
  return;
}

// ====== 0000f62c: ~CacaoClient ======
/* non-virtual thunk to android::Cacao::CacaoClient::~CacaoClient() */

void __thiscall android::Cacao::CacaoClient::~CacaoClient(CacaoClient *this)

{
  (*(pthread_mutex_lock + DAT_00013be8))(this + -0x14);
  return;
}

// ====== 0000f632: ~CacaoClient ======
/* virtual thunk to android::Cacao::CacaoClient::~CacaoClient() */

void __thiscall android::Cacao::CacaoClient::~CacaoClient(CacaoClient *this)

{
  (*(pthread_mutex_lock + DAT_00013be8))(this + *(int *)(*(int *)this + -0xc));
  return;
}

// ====== 0000f640: connect ======
/* android::Cacao::CacaoClient::connect(android::sp<android::ICacaoService> const&) */

undefined4 __thiscall android::Cacao::CacaoClient::connect(CacaoClient *this,sp *param_1)

{
  sp<android::IMemory> *this_00;
  int *piVar1;
  undefined4 *puVar2;
  undefined4 uVar3;
  code *pcVar4;
  undefined4 uVar5;
  int *local_34;
  sp<android::IMemory> asStack_30 [4];
  sp<android::Cacao::DeathNotifier> asStack_2c [4];
  int local_28;
  
  puVar2 = (undefined4 *)(DAT_0000f728 + 0xf654);
  local_28 = *(int *)*puVar2;
  pthread_mutex_lock((pthread_mutex_t *)(this + 0x1c));
  piVar1 = *(int **)param_1;
  uVar5 = 0;
  if (piVar1 == (int *)0x0) {
    uVar5 = 0xffffff99;
  }
  this_00 = (sp<android::IMemory> *)(this + 0x18);
  uVar3 = uVar5;
  if (*(int *)this_00 != 0) {
    uVar3 = 0xffffff95;
  }
  if ((piVar1 != (int *)0x0) && (*(int *)this_00 == 0)) {
    pcVar4 = *(code **)(*piVar1 + 0x10);
    sp<android::IMemory>::sp<android::MemoryBase>(asStack_30,(MemoryBase *)this);
    (*pcVar4)(asStack_2c,piVar1,asStack_30);
    sp<android::IMemory>::operator=(this_00,(sp *)asStack_2c);
    sp<android::Cacao::DeathNotifier>::~sp(asStack_2c);
    sp<android::Cacao::DeathNotifier>::~sp((sp<android::Cacao::DeathNotifier> *)asStack_30);
    if (*(int *)(this + 0x18) == 0) {
      uVar3 = 0xffffff91;
      puVar2 = (undefined4 *)(DAT_0000f72c + 0xf70c);
    }
    else {
      sp<android::IInterface>::sp<android::ICacaoService>
                ((sp<android::IInterface> *)asStack_2c,(sp *)this_00);
      android::IInterface::asBinder((sp *)&local_34);
      pcVar4 = *(code **)(*local_34 + 0x18);
      sp<android::IBinder::DeathRecipient>::sp<android::Cacao::CacaoClient>
                ((sp<android::IBinder::DeathRecipient> *)asStack_30,this);
      (*pcVar4)(local_34,(sp<android::IBinder::DeathRecipient> *)asStack_30,0,0);
      sp<android::Cacao::DeathNotifier>::~sp((sp<android::Cacao::DeathNotifier> *)asStack_30);
      sp<android::Cacao::DeathNotifier>::~sp((sp<android::Cacao::DeathNotifier> *)&local_34);
      sp<android::Cacao::DeathNotifier>::~sp(asStack_2c);
      puVar2 = (undefined4 *)(DAT_0000f730 + 0xf702);
      uVar3 = uVar5;
    }
  }
  pthread_mutex_unlock((pthread_mutex_t *)(this + 0x1c));
  if (*(int *)*puVar2 == local_28) {
    return uVar3;
  }
                    /* WARNING: Subroutine does not return */
  __stack_chk_fail();
}

// ====== 0000f734: operator= ======
/* android::sp<android::IMemory>::TEMPNAMEPLACEHOLDERVALUE(android::sp<android::IMemory>&&) */

sp<android::IMemory> * __thiscall
android::sp<android::IMemory>::operator=(sp<android::IMemory> *this,sp *param_1)

{
  int *piVar1;
  
  piVar1 = *(int **)this;
  if (piVar1 != (int *)0x0) {
    android::RefBase::decStrong((void *)(*(int *)(*piVar1 + -0xc) + (int)piVar1));
  }
  if (piVar1 != *(int **)this) {
    android::sp_report_race();
  }
  *(undefined4 *)this = *(undefined4 *)param_1;
  *(undefined4 *)param_1 = 0;
  return this;
}

// ====== 0000f762: sp<android::Cacao::CacaoClient> ======
/* android::sp<android::IBinder::DeathRecipient>::sp<android::Cacao::CacaoClient>(android::Cacao::CacaoClient*)
    */

sp<android::IBinder::DeathRecipient> * __thiscall
android::sp<android::IBinder::DeathRecipient>::sp<android::Cacao::CacaoClient>
          (sp<android::IBinder::DeathRecipient> *this,CacaoClient *param_1)

{
  CacaoClient *pCVar1;
  CacaoClient *pCVar2;
  
  pCVar1 = param_1 + 0x10;
  pCVar2 = param_1;
  if (param_1 != (CacaoClient *)0x0) {
    pCVar2 = pCVar1;
  }
  *(CacaoClient **)this = pCVar2;
  if (param_1 != (CacaoClient *)0x0) {
    android::RefBase::incStrong(pCVar1 + *(int *)(*(int *)pCVar1 + -0xc));
  }
  return this;
}

// ====== 0000f788: disconnect ======
/* android::Cacao::CacaoClient::disconnect() */

void __thiscall android::Cacao::CacaoClient::disconnect(CacaoClient *this)

{
  sp<android::ICacaoService> *this_00;
  undefined4 *puVar1;
  code *pcVar2;
  wp<android::IBinder::DeathRecipient> awStack_38 [8];
  sp<android::IInterface> asStack_30 [4];
  int *local_2c;
  int local_28;
  
  puVar1 = (undefined4 *)(DAT_0000f824 + 0xf79a);
  local_28 = *(int *)*puVar1;
  pthread_mutex_lock((pthread_mutex_t *)(this + 0x1c));
  this_00 = (sp<android::ICacaoService> *)(this + 0x18);
  if (*(int *)this_00 != 0) {
    sp<android::IInterface>::sp<android::ICacaoService>(asStack_30,(sp *)this_00);
    android::IInterface::asBinder((sp *)&local_2c);
    pcVar2 = *(code **)(*local_2c + 0x1c);
    wp<android::IBinder::DeathRecipient>::wp<android::Cacao::CacaoClient>(awStack_38,this);
    puVar1 = (undefined4 *)(DAT_0000f828 + 0xf7ea);
    (*pcVar2)(local_2c,awStack_38,0,0,0);
    wp<android::IBinder::DeathRecipient>::~wp(awStack_38);
    sp<android::Cacao::DeathNotifier>::~sp((sp<android::Cacao::DeathNotifier> *)&local_2c);
    sp<android::Cacao::DeathNotifier>::~sp((sp<android::Cacao::DeathNotifier> *)asStack_30);
    (**(code **)(**(int **)(this + 0x18) + 0x10))();
    sp<android::ICacaoService>::clear(this_00);
  }
  pthread_mutex_unlock((pthread_mutex_t *)(this + 0x1c));
  if (*(int *)*puVar1 == local_28) {
    return;
  }
                    /* WARNING: Subroutine does not return */
  __stack_chk_fail();
}

// ====== 0000f82c: wp<android::Cacao::CacaoClient> ======
/* android::wp<android::IBinder::DeathRecipient>::wp<android::Cacao::CacaoClient>(android::Cacao::CacaoClient*)
    */

wp<android::IBinder::DeathRecipient> * __thiscall
android::wp<android::IBinder::DeathRecipient>::wp<android::Cacao::CacaoClient>
          (wp<android::IBinder::DeathRecipient> *this,CacaoClient *param_1)

{
  CacaoClient *pCVar1;
  undefined4 uVar2;
  
  pCVar1 = param_1 + 0x10;
  if (param_1 == (CacaoClient *)0x0) {
    pCVar1 = (CacaoClient *)0x0;
  }
  *(CacaoClient **)this = pCVar1;
  if (param_1 != (CacaoClient *)0x0) {
    uVar2 = android::RefBase::createWeak(param_1 + *(int *)(*(int *)param_1 + -0xc));
    *(undefined4 *)(this + 4) = uVar2;
  }
  return this;
}

// ====== 0000f852: ~wp ======
/* android::wp<android::IBinder::DeathRecipient>::~wp() */

wp<android::IBinder::DeathRecipient> * __thiscall
android::wp<android::IBinder::DeathRecipient>::~wp(wp<android::IBinder::DeathRecipient> *this)

{
  if (*(int *)this != 0) {
    android::RefBase::weakref_type::decWeak(*(void **)(this + 4));
  }
  return this;
}

// ====== 0000f866: clear ======
/* android::sp<android::ICacaoService>::clear() */

void __thiscall android::sp<android::ICacaoService>::clear(sp<android::ICacaoService> *this)

{
  int *piVar1;
  
  piVar1 = *(int **)this;
  if (piVar1 != (int *)0x0) {
    android::RefBase::decStrong((void *)((int)piVar1 + *(int *)(*piVar1 + -0xc)));
    *(undefined4 *)this = 0;
  }
  return;
}

// ====== 0000f884: notifyEvent ======
/* android::Cacao::CacaoClient::notifyEvent(int, android::sp<android::IMemory> const&,
   cacao::ISerialize::SerializedData) */

void __thiscall
android::Cacao::CacaoClient::notifyEvent
          (CacaoClient *this,uint param_1,undefined4 param_3,undefined4 param_4)

{
  int *piVar1;
  int *piVar2;
  undefined4 local_4;
  
  piVar2 = *(int **)(this + 0x20);
  if (piVar2 != (int *)0x0) {
    local_4 = param_4;
    piVar1 = (int *)cacao::ProcessFactory::createEvent(param_1,(SerializedData *)&local_4);
    if (piVar1 == (int *)0x0) {
      __android_log_print(6,DAT_0000f8d8 + 0xf8c8,DAT_0000f8dc + 0xf8ca);
    }
    else {
      (**(code **)(*piVar1 + 0x18))(piVar1,*(undefined4 *)(this + 0x24));
      (**(code **)(*piVar2 + 8))(piVar2,piVar1);
      (**(code **)(*piVar1 + 4))(piVar1);
    }
  }
  return;
}

// ====== 0000f8e0: binderDied ======
/* non-virtual thunk to android::Cacao::CacaoClient::binderDied(android::wp<android::IBinder>
   const&) */

void __thiscall android::Cacao::CacaoClient::binderDied(CacaoClient *this,wp *param_1)

{
  __android_log_print(5,DAT_0000f8f0 + 0xf8ea,DAT_0000f8f4 + 0xf8ec);
  return;
}

// ====== 0000f8f8: subscribeEvents ======
/* android::Cacao::CacaoClient::subscribeEvents(cacao::ICBProcessEventListener*, void*) */

undefined4 __thiscall
android::Cacao::CacaoClient::subscribeEvents
          (CacaoClient *this,ICBProcessEventListener *param_1,void *param_2)

{
  pthread_mutex_lock((pthread_mutex_t *)(this + 0x1c));
  *(ICBProcessEventListener **)(this + 0x20) = param_1;
  *(void **)(this + 0x24) = param_2;
  pthread_mutex_unlock((pthread_mutex_t *)(this + 0x1c));
  return 0;
}

// ====== 0000f91c: subscribeEvents ======
/* non-virtual thunk to
   android::Cacao::CacaoClient::subscribeEvents(cacao::ICBProcessEventListener*, void*) */

undefined4 __thiscall
android::Cacao::CacaoClient::subscribeEvents
          (CacaoClient *this,ICBProcessEventListener *param_1,void *param_2)

{
  pthread_mutex_lock((pthread_mutex_t *)(this + 8));
  *(ICBProcessEventListener **)(this + 0xc) = param_1;
  *(void **)(this + 0x10) = param_2;
  pthread_mutex_unlock((pthread_mutex_t *)(this + 8));
  return 0;
}

// ====== 0000f940: start ======
/* android::Cacao::CacaoClient::start(cacao::ProcessModeBase const*) */

int __thiscall android::Cacao::CacaoClient::start(CacaoClient *this,ProcessModeBase *param_1)

{
  uint uVar1;
  int iVar2;
  int *piVar3;
  int iVar4;
  undefined4 *puVar5;
  undefined4 *puVar6;
  code *pcVar7;
  undefined4 *puVar8;
  undefined4 auStack_348 [100];
  CacaoClient aCStack_1b8 [4];
  int local_1b4;
  undefined4 local_1b0;
  undefined4 uStack_1ac;
  undefined4 auStack_1a8 [99];
  int local_1c;
  
  puVar8 = (undefined4 *)(DAT_0000fa30 + 0xf958);
  local_1c = *(int *)*puVar8;
  pthread_mutex_lock((pthread_mutex_t *)(this + 0x1c));
  local_1b4 = 0;
  __aeabi_memclr8(&local_1b0,0x194);
  if (*(int *)(this + 0x18) == 0) {
    iVar2 = -0x65;
  }
  else if (param_1 == (ProcessModeBase *)0x0) {
    iVar2 = -0x67;
  }
  else {
    uVar1 = (**(code **)(*(int *)param_1 + 0x14))(param_1);
    allocMemory(aCStack_1b8,uVar1);
    sp<android::IMemory>::operator=((sp<android::IMemory> *)&local_1b4,(sp *)aCStack_1b8);
    sp<android::Cacao::DeathNotifier>::~sp((sp<android::Cacao::DeathNotifier> *)aCStack_1b8);
    if (local_1b4 == 0) {
      iVar2 = -0x66;
      goto LAB_0000fa08;
    }
    makeSerializedData((sp *)&local_1b4,(SerializedData *)&local_1b0);
    iVar2 = (**(code **)(*(int *)param_1 + 0x18))(param_1,&local_1b0);
    if (-1 < iVar2) {
      piVar3 = *(int **)(this + 0x18);
      iVar2 = 0x18c;
      pcVar7 = *(code **)(*piVar3 + 0x14);
      puVar5 = auStack_1a8;
      puVar6 = auStack_348;
      do {
        iVar2 = iVar2 + -4;
        *puVar6 = *puVar5;
        puVar5 = puVar5 + 1;
        puVar6 = puVar6 + 1;
      } while (iVar2 != 0);
      iVar4 = (*pcVar7)(piVar3,&local_1b4,local_1b0,uStack_1ac);
      iVar2 = iVar4;
      if (iVar4 != -0x6e) {
        iVar2 = -0x6f;
      }
      if (iVar4 == 0) {
        iVar2 = iVar4;
      }
    }
  }
  if (local_1b4 != 0) {
    sp<android::ICacaoService>::clear((sp<android::ICacaoService> *)&local_1b4);
  }
LAB_0000fa08:
  sp<android::Cacao::DeathNotifier>::~sp((sp<android::Cacao::DeathNotifier> *)&local_1b4);
  pthread_mutex_unlock((pthread_mutex_t *)(this + 0x1c));
  if (*(int *)*puVar8 != local_1c) {
                    /* WARNING: Subroutine does not return */
    __stack_chk_fail();
  }
  return iVar2;
}

// ====== 0000fa34: allocMemory ======
/* android::Cacao::CacaoClient::allocMemory(unsigned int) */

void __thiscall android::Cacao::CacaoClient::allocMemory(CacaoClient *this,uint param_1)

{
  MemoryHeapBase *this_00;
  MemoryBase *this_01;
  undefined4 *puVar1;
  sp<android::IMemoryHeap> asStack_20 [4];
  int local_1c;
  
  puVar1 = (undefined4 *)(DAT_0000faa0 + 0xfa46);
  local_1c = *(int *)*puVar1;
  this_00 = operator_new(0x38);
  android::MemoryHeapBase::MemoryHeapBase(this_00,param_1,0,(char *)0x0);
  sp<android::IMemoryHeap>::sp<android::MemoryHeapBase>(asStack_20,this_00);
  this_01 = operator_new(0x24);
  android::MemoryBase::MemoryBase(this_01,(sp *)asStack_20,0,param_1);
  sp<android::IMemory>::sp<android::MemoryBase>((sp<android::IMemory> *)this,this_01);
  sp<android::Cacao::DeathNotifier>::~sp((sp<android::Cacao::DeathNotifier> *)asStack_20);
  if (*(int *)*puVar1 == local_1c) {
    return;
  }
                    /* WARNING: Subroutine does not return */
  __stack_chk_fail();
}

// ====== 0000faa4: makeSerializedData ======
/* android::Cacao::CacaoClient::makeSerializedData(android::sp<android::IMemory> const&,
   cacao::ISerialize::SerializedData*) */

void android::Cacao::CacaoClient::makeSerializedData(sp *param_1,SerializedData *param_2)

{
  undefined4 uVar1;
  
  if ((param_2 != (SerializedData *)0x0) && (*(int *)param_1 != 0)) {
    uVar1 = android::IMemory::size();
    *(undefined4 *)(param_2 + 0x184) = uVar1;
    uVar1 = android::IMemory::pointer();
    *(undefined4 *)(param_2 + 0x188) = uVar1;
  }
  return;
}

// ====== 0000fac4: freeMemory ======
/* android::Cacao::CacaoClient::freeMemory(android::sp<android::IMemory>&) */

void android::Cacao::CacaoClient::freeMemory(sp *param_1)

{
  if (*(int *)param_1 != 0) {
    sp<android::ICacaoService>::clear((sp<android::ICacaoService> *)param_1);
    return;
  }
  return;
}

// ====== 0000face: start ======
/* non-virtual thunk to android::Cacao::CacaoClient::start(cacao::ProcessModeBase const*) */

void __thiscall android::Cacao::CacaoClient::start(CacaoClient *this,ProcessModeBase *param_1)

{
  (*(code *)(DAT_00013d68 + 0x13d6c))(this + -0x14);
  return;
}

// ====== 0000fad4: stop ======
/* android::Cacao::CacaoClient::stop() */

int __thiscall android::Cacao::CacaoClient::stop(CacaoClient *this)

{
  int iVar1;
  int iVar2;
  
  pthread_mutex_lock((pthread_mutex_t *)(this + 0x1c));
  if (*(int **)(this + 0x18) == (int *)0x0) {
    iVar2 = -0x65;
  }
  else {
    iVar1 = (**(code **)(**(int **)(this + 0x18) + 0x18))();
    iVar2 = iVar1;
    if (iVar1 != -0x6e) {
      iVar2 = -0x6f;
    }
    if (iVar1 == 0) {
      iVar2 = iVar1;
    }
  }
  pthread_mutex_unlock((pthread_mutex_t *)(this + 0x1c));
  return iVar2;
}

// ====== 0000fb0e: stop ======
/* non-virtual thunk to android::Cacao::CacaoClient::stop() */

void __thiscall android::Cacao::CacaoClient::stop(CacaoClient *this)

{
  (*(code *)(DAT_00013d78 + 0x13d7c))(this + -0x14);
  return;
}

// ====== 0000fb14: setConfig ======
/* android::Cacao::CacaoClient::setConfig(cacao::ProcessConfigBase const*) */

int __thiscall android::Cacao::CacaoClient::setConfig(CacaoClient *this,ProcessConfigBase *param_1)

{
  uint uVar1;
  int iVar2;
  undefined4 uVar3;
  undefined4 *puVar4;
  int iVar5;
  undefined4 *puVar6;
  int *piVar7;
  code *pcVar8;
  undefined4 *puVar9;
  undefined4 auStack_350 [100];
  CacaoClient aCStack_1c0 [4];
  int local_1bc;
  undefined4 local_1b8 [102];
  int local_20;
  
  puVar9 = (undefined4 *)(DAT_0000fc14 + 0xfb2c);
  local_20 = *(int *)*puVar9;
  pthread_mutex_lock((pthread_mutex_t *)(this + 0x1c));
  local_1bc = 0;
  __aeabi_memclr8(local_1b8,0x194);
  if (*(int *)(this + 0x18) == 0) {
    iVar2 = -0x65;
  }
  else if (param_1 == (ProcessConfigBase *)0x0) {
    iVar2 = -0x67;
  }
  else {
    uVar1 = (**(code **)(*(int *)param_1 + 0x1c))(param_1);
    allocMemory(aCStack_1c0,uVar1);
    sp<android::IMemory>::operator=((sp<android::IMemory> *)&local_1bc,(sp *)aCStack_1c0);
    sp<android::Cacao::DeathNotifier>::~sp((sp<android::Cacao::DeathNotifier> *)aCStack_1c0);
    if (local_1bc == 0) {
      iVar2 = -0x66;
      goto LAB_0000fbec;
    }
    makeSerializedData((sp *)&local_1bc,(SerializedData *)local_1b8);
    iVar2 = (**(code **)(*(int *)param_1 + 0x20))(param_1,local_1b8);
    if (-1 < iVar2) {
      piVar7 = *(int **)(this + 0x18);
      pcVar8 = *(code **)(*piVar7 + 0x1c);
      uVar3 = (**(code **)(*(int *)param_1 + 0x10))(param_1);
      iVar2 = 400;
      puVar4 = (undefined4 *)((uint)local_1b8 | 4);
      puVar6 = auStack_350;
      do {
        iVar2 = iVar2 + -4;
        *puVar6 = *puVar4;
        puVar4 = puVar4 + 1;
        puVar6 = puVar6 + 1;
      } while (iVar2 != 0);
      iVar5 = (*pcVar8)(piVar7,uVar3,&local_1bc,local_1b8[0]);
      iVar2 = iVar5;
      if (iVar5 != -0x6e) {
        iVar2 = -0x6f;
      }
      if (iVar5 == 0) {
        iVar2 = iVar5;
      }
    }
  }
  if (local_1bc != 0) {
    sp<android::ICacaoService>::clear((sp<android::ICacaoService> *)&local_1bc);
  }
LAB_0000fbec:
  sp<android::Cacao::DeathNotifier>::~sp((sp<android::Cacao::DeathNotifier> *)&local_1bc);
  pthread_mutex_unlock((pthread_mutex_t *)(this + 0x1c));
  if (*(int *)*puVar9 != local_20) {
                    /* WARNING: Subroutine does not return */
    __stack_chk_fail();
  }
  return iVar2;
}

// ====== 0000fc18: setConfig ======
/* non-virtual thunk to android::Cacao::CacaoClient::setConfig(cacao::ProcessConfigBase const*) */

void __thiscall android::Cacao::CacaoClient::setConfig(CacaoClient *this,ProcessConfigBase *param_1)

{
                    /* WARNING: Could not recover jumptable at 0x00013d84. Too many branches */
                    /* WARNING: Treating indirect jump as call */
  (*(code *)(DAT_00013d88 + 0x13d8c))(this + -0x14);
  return;
}

// ====== 0000fc20: getConfig ======
/* android::Cacao::CacaoClient::getConfig(cacao::ProcessConfigBase*) */

int __thiscall android::Cacao::CacaoClient::getConfig(CacaoClient *this,ProcessConfigBase *param_1)

{
  uint uVar1;
  int iVar2;
  undefined4 uVar3;
  undefined4 *puVar4;
  int iVar5;
  undefined4 *puVar6;
  int *piVar7;
  undefined4 *puVar8;
  code *pcVar9;
  undefined4 auStack_350 [100];
  CacaoClient aCStack_1c0 [4];
  int local_1bc;
  undefined4 local_1b8 [99];
  undefined4 local_2c;
  undefined4 uStack_28;
  int local_24;
  
  puVar8 = (undefined4 *)(DAT_0000fd30 + 0xfc38);
  local_24 = *(int *)*puVar8;
  pthread_mutex_lock((pthread_mutex_t *)(this + 0x1c));
  local_1bc = 0;
  __aeabi_memclr8(local_1b8,0x194);
  if (*(int *)(this + 0x18) == 0) {
    iVar2 = -0x65;
  }
  else if (param_1 == (ProcessConfigBase *)0x0) {
    iVar2 = -0x67;
  }
  else {
    uVar1 = (**(code **)(*(int *)param_1 + 0x1c))(param_1);
    allocMemory(aCStack_1c0,uVar1);
    sp<android::IMemory>::operator=((sp<android::IMemory> *)&local_1bc,(sp *)aCStack_1c0);
    sp<android::Cacao::DeathNotifier>::~sp((sp<android::Cacao::DeathNotifier> *)aCStack_1c0);
    if (local_1bc == 0) {
      iVar2 = -0x66;
      goto LAB_0000fd0a;
    }
    makeSerializedData((sp *)&local_1bc,(SerializedData *)local_1b8);
    iVar2 = (**(code **)(*(int *)param_1 + 0x20))(param_1,local_1b8);
    if (-1 < iVar2) {
      piVar7 = *(int **)(this + 0x18);
      pcVar9 = *(code **)(*piVar7 + 0x20);
      uVar3 = (**(code **)(*(int *)param_1 + 0x10))(param_1);
      iVar2 = 400;
      puVar4 = (undefined4 *)((uint)local_1b8 | 4);
      puVar6 = auStack_350;
      do {
        iVar2 = iVar2 + -4;
        *puVar6 = *puVar4;
        puVar4 = puVar4 + 1;
        puVar6 = puVar6 + 1;
      } while (iVar2 != 0);
      iVar5 = (*pcVar9)(piVar7,uVar3,&local_1bc,local_1b8[0]);
      iVar2 = iVar5;
      if (iVar5 != -0x6e) {
        iVar2 = -0x6f;
      }
      if (iVar5 == 0) {
        local_2c = 0;
        uStack_28 = 0;
        iVar2 = (**(code **)(*(int *)param_1 + 0x24))(param_1,local_1b8);
      }
    }
  }
  if (local_1bc != 0) {
    sp<android::ICacaoService>::clear((sp<android::ICacaoService> *)&local_1bc);
  }
LAB_0000fd0a:
  sp<android::Cacao::DeathNotifier>::~sp((sp<android::Cacao::DeathNotifier> *)&local_1bc);
  pthread_mutex_unlock((pthread_mutex_t *)(this + 0x1c));
  if (*(int *)*puVar8 == local_24) {
    return iVar2;
  }
                    /* WARNING: Subroutine does not return */
  __stack_chk_fail();
}

// ====== 0000fd34: getConfig ======
/* non-virtual thunk to android::Cacao::CacaoClient::getConfig(cacao::ProcessConfigBase*) */

void __thiscall android::Cacao::CacaoClient::getConfig(CacaoClient *this,ProcessConfigBase *param_1)

{
                    /* WARNING: Could not recover jumptable at 0x00013d94. Too many branches */
                    /* WARNING: Treating indirect jump as call */
  (*(CacaoProcessCallback::CacaoProcessCallback + DAT_00013d98))(this + -0x14);
  return;
}

// ====== 0000fd3c: process ======
/* android::Cacao::CacaoClient::process(cacao::ProcessParamBase const*,
   cacao::ICBProcessResultListener*, cacao::ProcessResultBase*) */

int __thiscall
android::Cacao::CacaoClient::process
          (CacaoClient *this,ProcessParamBase *param_1,ICBProcessResultListener *param_2,
          ProcessResultBase *param_3)

{
  uint uVar1;
  CacaoProcessCallback *this_00;
  undefined4 *puVar2;
  int iVar3;
  undefined4 *puVar4;
  int *piVar5;
  int iVar6;
  int *piVar7;
  undefined4 *puVar8;
  undefined4 auStack_860 [99];
  VectorImpl *local_6d4;
  int *local_6d0;
  undefined4 auStack_6cc [101];
  undefined4 local_538;
  sp<android::ICacaoProcessCallback> *psStack_534;
  code *local_52c;
  int local_528 [5];
  CacaoClient aCStack_514 [4];
  int local_510;
  int local_50c;
  int local_508 [5];
  int local_4f4;
  undefined4 local_4f0;
  undefined4 uStack_4ec;
  undefined4 auStack_4e8 [100];
  undefined4 auStack_358 [102];
  undefined1 auStack_1c0 [408];
  int local_28;
  
  puVar8 = (undefined4 *)(DAT_0000ff60 + 0xfd5a);
  local_28 = *(int *)*puVar8;
  pthread_mutex_lock((pthread_mutex_t *)(this + 0x1c));
  iVar6 = 0;
  local_4f4 = 0;
  android::VectorImpl::VectorImpl((VectorImpl *)local_508,4,7);
  piVar7 = (int *)(DAT_0000ff64 + 0xfd82);
  local_508[0] = *piVar7 + 8;
  local_510 = 0;
  local_50c = 0;
  __aeabi_memclr8(auStack_1c0,0x194);
  __aeabi_memclr8(auStack_358,0x194);
  if (*(int *)(this + 0x18) == 0) {
    iVar6 = -0x65;
  }
  else {
    if (param_3 == (ProcessResultBase *)0x0) {
      iVar6 = -0x67;
    }
    if ((param_1 != (ProcessParamBase *)0x0) && (param_3 != (ProcessResultBase *)0x0)) {
      uVar1 = (**(code **)(*(int *)param_1 + 0x18))(param_1);
      allocMemory(aCStack_514,uVar1);
      sp<android::IMemory>::operator=((sp<android::IMemory> *)&local_4f4,(sp *)aCStack_514);
      sp<android::Cacao::DeathNotifier>::~sp((sp<android::Cacao::DeathNotifier> *)aCStack_514);
      if (local_4f4 == 0) {
        iVar6 = -0x66;
        goto LAB_0000ff04;
      }
      makeSerializedData((sp *)&local_4f4,auStack_1c0);
      iVar6 = (**(code **)(*(int *)param_1 + 0x1c))(param_1,auStack_1c0);
    }
    if (-1 < iVar6) {
      uVar1 = (**(code **)(*(int *)param_3 + 0x24))(param_3);
      allocMemory(aCStack_514,uVar1);
      sp<android::IMemory>::operator=((sp<android::IMemory> *)&local_50c,(sp *)aCStack_514);
      sp<android::Cacao::DeathNotifier>::~sp((sp<android::Cacao::DeathNotifier> *)aCStack_514);
      if (local_50c == 0) {
LAB_0000fef6:
        iVar6 = -0x66;
      }
      else {
        makeSerializedData((sp *)&local_50c,(SerializedData *)auStack_358);
        iVar6 = (**(code **)(*(int *)param_3 + 0x28))(param_3,auStack_358);
        if (-1 < iVar6) {
          this_00 = operator_new(0x20);
          CacaoProcessCallback::CacaoProcessCallback(this_00,param_2,param_3);
          sp<android::ICacaoProcessCallback>::operator=
                    ((sp<android::ICacaoProcessCallback> *)&local_510,
                     (ICacaoProcessCallback *)this_00);
          if (local_510 == 0) goto LAB_0000fef6;
          piVar5 = *(int **)(this + 0x18);
          local_52c = *(code **)(*piVar5 + 0x24);
          __aeabi_memcpy8(&local_4f0,auStack_1c0,0x194);
          android::VectorImpl::VectorImpl((VectorImpl *)local_528,(VectorImpl *)local_508);
          iVar6 = *piVar7;
          iVar3 = 0x18c;
          local_538 = 1;
          local_6d0 = &local_50c;
          puVar2 = auStack_4e8;
          puVar4 = auStack_860;
          local_6d4 = (VectorImpl *)local_528;
          psStack_534 = (sp<android::ICacaoProcessCallback> *)&local_510;
          local_528[0] = iVar6 + 8;
          do {
            iVar3 = iVar3 + -4;
            *puVar4 = *puVar2;
            puVar2 = puVar2 + 1;
            puVar4 = puVar4 + 1;
          } while (iVar3 != 0);
          iVar3 = 0x194;
          puVar2 = auStack_6cc;
          puVar4 = auStack_358;
          do {
            iVar3 = iVar3 + -4;
            *puVar2 = *puVar4;
            puVar2 = puVar2 + 1;
            puVar4 = puVar4 + 1;
          } while (iVar3 != 0);
          iVar3 = (*local_52c)(piVar5,&local_4f4,local_4f0,uStack_4ec);
          local_528[0] = iVar6 + 8;
          android::VectorImpl::finish_vector();
          android::VectorImpl::~VectorImpl((VectorImpl *)local_528);
          iVar6 = iVar3;
          if (iVar3 != -0x6e) {
            iVar6 = -0x6f;
          }
          if (iVar3 == 0) {
            iVar6 = iVar3;
          }
        }
      }
    }
  }
  if (local_4f4 != 0) {
    sp<android::ICacaoService>::clear((sp<android::ICacaoService> *)&local_4f4);
  }
LAB_0000ff04:
  if (local_50c != 0) {
    sp<android::ICacaoService>::clear((sp<android::ICacaoService> *)&local_50c);
  }
  sp<android::Cacao::DeathNotifier>::~sp((sp<android::Cacao::DeathNotifier> *)&local_510);
  sp<android::Cacao::DeathNotifier>::~sp((sp<android::Cacao::DeathNotifier> *)&local_50c);
  local_508[0] = *piVar7 + 8;
  android::VectorImpl::finish_vector();
  android::VectorImpl::~VectorImpl((VectorImpl *)local_508);
  sp<android::Cacao::DeathNotifier>::~sp((sp<android::Cacao::DeathNotifier> *)&local_4f4);
  pthread_mutex_unlock((pthread_mutex_t *)(this + 0x1c));
  if (*(int *)*puVar8 != local_28) {
                    /* WARNING: Subroutine does not return */
    __stack_chk_fail();
  }
  return iVar6;
}

// ====== 0000ff68: process ======
/* non-virtual thunk to android::Cacao::CacaoClient::process(cacao::ProcessParamBase const*,
   cacao::ICBProcessResultListener*, cacao::ProcessResultBase*) */

void __thiscall
android::Cacao::CacaoClient::process
          (CacaoClient *this,ProcessParamBase *param_1,ICBProcessResultListener *param_2,
          ProcessResultBase *param_3)

{
  (*(code *)(DAT_00013dd8 + 0x13ddc))(this + -0x14);
  return;
}

// ====== 0000ff70: processAsync ======
/* android::Cacao::CacaoClient::processAsync(cacao::ProcessParamBase const*,
   android::Vector<cacao::ImageBuf*>*, cacao::ICBProcessResultListener*, cacao::ProcessResultBase*)
    */

int __thiscall
android::Cacao::CacaoClient::processAsync
          (CacaoClient *this,ProcessParamBase *param_1,Vector *param_2,
          ICBProcessResultListener *param_3,ProcessResultBase *param_4)

{
  uint uVar1;
  CacaoProcessCallback *this_00;
  VectorImpl *this_01;
  undefined4 *puVar2;
  undefined4 *puVar3;
  int iVar4;
  int *piVar5;
  undefined4 *puVar6;
  int *piVar7;
  int iVar8;
  undefined4 auStack_860 [99];
  VectorImpl *local_6d4;
  sp *psStack_6d0;
  undefined4 auStack_6cc [101];
  undefined4 local_538;
  sp<android::ICacaoProcessCallback> *psStack_534;
  ICBProcessResultListener *local_52c;
  int local_528 [5];
  CacaoClient aCStack_514 [4];
  int local_510;
  int local_50c;
  int local_508 [2];
  uint local_500;
  int local_4f4;
  undefined4 local_4f0;
  undefined4 uStack_4ec;
  undefined4 auStack_4e8 [100];
  undefined4 auStack_358 [102];
  undefined1 auStack_1c0 [408];
  int local_28;
  
  puVar6 = (undefined4 *)(DAT_000101f0 + 0xff8e);
  local_28 = *(int *)*puVar6;
  pthread_mutex_lock((pthread_mutex_t *)(this + 0x1c));
  iVar4 = 0;
  local_4f4 = 0;
  android::VectorImpl::VectorImpl((VectorImpl *)local_508,4,7);
  piVar5 = (int *)(DAT_000101f4 + 0xffb4);
  local_508[0] = *piVar5 + 8;
  local_50c = 0;
  __aeabi_memclr8(auStack_1c0,0x194);
  __aeabi_memclr8(auStack_358,0x194);
  local_510 = 0;
  if (*(int *)(this + 0x18) != 0) {
    if (param_4 == (ProcessResultBase *)0x0) {
      iVar4 = -0x67;
    }
    if (param_3 == (ICBProcessResultListener *)0x0) {
      iVar4 = -0x67;
    }
    if (((param_1 != (ProcessParamBase *)0x0) && (param_3 != (ICBProcessResultListener *)0x0)) &&
       (param_4 != (ProcessResultBase *)0x0)) {
      uVar1 = (**(code **)(*(int *)param_1 + 0x18))(param_1);
      allocMemory(aCStack_514,uVar1);
      sp<android::IMemory>::operator=((sp<android::IMemory> *)&local_4f4,(sp *)aCStack_514);
      sp<android::Cacao::DeathNotifier>::~sp((sp<android::Cacao::DeathNotifier> *)aCStack_514);
      if (local_4f4 == 0) {
        iVar4 = 0;
        goto LAB_0001019c;
      }
      makeSerializedData((sp *)&local_4f4,auStack_1c0);
      iVar4 = (**(code **)(*(int *)param_1 + 0x1c))(param_1,auStack_1c0);
    }
    local_52c = param_3;
    if (((param_2 != (Vector *)0x0) && (-1 < iVar4)) && (*(int *)(param_2 + 8) != 0)) {
      iVar8 = 0;
      uVar1 = 0;
      do {
        android::VectorImpl::insertAt(local_508,*(int *)(param_2 + 4) + iVar8,local_500);
        uVar1 = uVar1 + 1;
        iVar8 = iVar8 + 4;
      } while (uVar1 < *(uint *)(param_2 + 8));
    }
    if (iVar4 < 0) {
      iVar4 = 0;
      puVar6 = (undefined4 *)(DAT_000101f8 + 0x10182);
      piVar5 = (int *)(DAT_00010204 + 0x10184);
    }
    else {
      uVar1 = (**(code **)(*(int *)param_4 + 0x24))(param_4);
      allocMemory(aCStack_514,uVar1);
      sp<android::IMemory>::operator=((sp<android::IMemory> *)&local_50c,(sp *)aCStack_514);
      sp<android::Cacao::DeathNotifier>::~sp((sp<android::Cacao::DeathNotifier> *)aCStack_514);
      puVar6 = (undefined4 *)(DAT_000101fc + 0x100ac);
      if (local_50c == 0) {
        iVar4 = 0;
        piVar5 = (int *)(DAT_00010208 + 0x1018c);
      }
      else {
        makeSerializedData((sp *)&local_50c,(SerializedData *)auStack_358);
        iVar4 = (**(code **)(*(int *)param_4 + 0x28))(param_4,auStack_358);
        piVar5 = (int *)(DAT_00010200 + 0x100ce);
        if (iVar4 < 0) {
          iVar4 = 0;
        }
        else {
          this_00 = operator_new(0x20);
          CacaoProcessCallback::CacaoProcessCallback(this_00,local_52c,param_4);
          sp<android::ICacaoProcessCallback>::operator=
                    ((sp<android::ICacaoProcessCallback> *)&local_510,
                     (ICacaoProcessCallback *)this_00);
          iVar4 = 0;
          if (local_510 != 0) {
            piVar7 = *(int **)(this + 0x18);
            local_52c = *(ICBProcessResultListener **)(*piVar7 + 0x24);
            __aeabi_memcpy8(&local_4f0,auStack_1c0,0x194);
            this_01 = (VectorImpl *)
                      android::VectorImpl::VectorImpl
                                ((VectorImpl *)local_528,(VectorImpl *)local_508);
            iVar8 = *piVar5;
            iVar4 = 0x18c;
            local_538 = 0;
            puVar2 = auStack_4e8;
            puVar3 = auStack_860;
            local_6d4 = this_01;
            psStack_6d0 = (sp *)&local_50c;
            psStack_534 = (sp<android::ICacaoProcessCallback> *)&local_510;
            local_528[0] = iVar8 + 8;
            do {
              iVar4 = iVar4 + -4;
              *puVar3 = *puVar2;
              puVar2 = puVar2 + 1;
              puVar3 = puVar3 + 1;
            } while (iVar4 != 0);
            iVar4 = 0x194;
            puVar2 = auStack_6cc;
            puVar3 = auStack_358;
            do {
              iVar4 = iVar4 + -4;
              *puVar2 = *puVar3;
              puVar2 = puVar2 + 1;
              puVar3 = puVar3 + 1;
            } while (iVar4 != 0);
            iVar4 = (*(code *)local_52c)(piVar7,&local_4f4,local_4f0,uStack_4ec);
            local_528[0] = iVar8 + 8;
            android::VectorImpl::finish_vector();
            android::VectorImpl::~VectorImpl(this_01);
            piVar5 = (int *)(DAT_0001020c + 0x10174);
          }
        }
      }
    }
  }
  if (local_4f4 != 0) {
    sp<android::ICacaoService>::clear((sp<android::ICacaoService> *)&local_4f4);
  }
LAB_0001019c:
  if (local_50c != 0) {
    sp<android::ICacaoService>::clear((sp<android::ICacaoService> *)&local_50c);
  }
  sp<android::Cacao::DeathNotifier>::~sp((sp<android::Cacao::DeathNotifier> *)&local_510);
  sp<android::Cacao::DeathNotifier>::~sp((sp<android::Cacao::DeathNotifier> *)&local_50c);
  local_508[0] = *piVar5 + 8;
  android::VectorImpl::finish_vector();
  android::VectorImpl::~VectorImpl((VectorImpl *)local_508);
  sp<android::Cacao::DeathNotifier>::~sp((sp<android::Cacao::DeathNotifier> *)&local_4f4);
  pthread_mutex_unlock((pthread_mutex_t *)(this + 0x1c));
  if (*(int *)*puVar6 == local_28) {
    return iVar4;
  }
                    /* WARNING: Subroutine does not return */
  __stack_chk_fail();
}

// ====== 00010210: processAsync ======
/* non-virtual thunk to android::Cacao::CacaoClient::processAsync(cacao::ProcessParamBase const*,
   android::Vector<cacao::ImageBuf*>*, cacao::ICBProcessResultListener*, cacao::ProcessResultBase*)
    */

void __thiscall
android::Cacao::CacaoClient::processAsync
          (CacaoClient *this,ProcessParamBase *param_1,Vector *param_2,
          ICBProcessResultListener *param_3,ProcessResultBase *param_4)

{
  (*(code *)(DAT_00013de8 + 0x13dec))(this + -0x14);
  return;
}

// ====== 00010218: processAsyncWithBinder ======
/* android::Cacao::CacaoClient::processAsyncWithBinder(cacao::ProcessParamBase const*,
   android::sp<android::IBinder> const&, cacao::ICBProcessResultListener*,
   cacao::ProcessResultBase*) */

int __thiscall
android::Cacao::CacaoClient::processAsyncWithBinder
          (CacaoClient *this,ProcessParamBase *param_1,sp *param_2,ICBProcessResultListener *param_3
          ,ProcessResultBase *param_4)

{
  uint uVar1;
  CacaoProcessCallback *this_00;
  int *piVar2;
  undefined4 *puVar3;
  undefined4 *puVar4;
  undefined4 *puVar5;
  int iVar6;
  code *pcVar7;
  undefined4 auStack_698 [99];
  sp *local_50c;
  sp *psStack_508;
  undefined4 auStack_504 [101];
  undefined4 local_370;
  sp<android::ICacaoProcessCallback> *psStack_36c;
  CacaoClient aCStack_368 [4];
  int local_364;
  int local_360;
  int local_35c;
  undefined4 auStack_358 [102];
  undefined4 local_1c0;
  undefined4 local_1bc;
  undefined4 auStack_1b8 [100];
  int local_28;
  
  puVar5 = (undefined4 *)(DAT_000103f8 + 0x10234);
  local_28 = *(int *)*puVar5;
  pthread_mutex_lock((pthread_mutex_t *)(this + 0x1c));
  iVar6 = 0;
  local_360 = 0;
  local_35c = 0;
  __aeabi_memclr8(&local_1c0,0x194);
  __aeabi_memclr8(auStack_358,0x194);
  local_364 = 0;
  if (*(int *)(this + 0x18) != 0) {
    if (param_4 == (ProcessResultBase *)0x0) {
      iVar6 = -0x67;
    }
    if (param_3 == (ICBProcessResultListener *)0x0) {
      iVar6 = -0x67;
    }
    if ((param_1 != (ProcessParamBase *)0x0) &&
       (param_3 != (ICBProcessResultListener *)0x0 && param_4 != (ProcessResultBase *)0x0)) {
      uVar1 = (**(code **)(*(int *)param_1 + 0x18))(param_1);
      allocMemory(aCStack_368,uVar1);
      sp<android::IMemory>::operator=((sp<android::IMemory> *)&local_35c,(sp *)aCStack_368);
      sp<android::Cacao::DeathNotifier>::~sp((sp<android::Cacao::DeathNotifier> *)aCStack_368);
      if (local_35c == 0) {
        iVar6 = 0;
        puVar5 = (undefined4 *)(DAT_000103fc + 0x103aa);
        goto LAB_000103b8;
      }
      makeSerializedData((sp *)&local_35c,(SerializedData *)&local_1c0);
      iVar6 = (**(code **)(*(int *)param_1 + 0x1c))(param_1,&local_1c0);
    }
    if (iVar6 < 0) {
      iVar6 = 0;
      puVar5 = (undefined4 *)(DAT_00010400 + 0x10350);
    }
    else {
      uVar1 = (**(code **)(*(int *)param_4 + 0x24))(param_4);
      allocMemory(aCStack_368,uVar1);
      sp<android::IMemory>::operator=((sp<android::IMemory> *)&local_360,(sp *)aCStack_368);
      sp<android::Cacao::DeathNotifier>::~sp((sp<android::Cacao::DeathNotifier> *)aCStack_368);
      if (local_360 == 0) {
        iVar6 = 0;
        puVar5 = (undefined4 *)(DAT_00010404 + 0x1039a);
      }
      else {
        makeSerializedData((sp *)&local_360,(SerializedData *)auStack_358);
        iVar6 = (**(code **)(*(int *)param_4 + 0x28))(param_4,auStack_358);
        if (iVar6 < 0) {
          iVar6 = 0;
          puVar5 = (undefined4 *)(DAT_00010408 + 0x103a2);
        }
        else {
          this_00 = operator_new(0x20);
          CacaoProcessCallback::CacaoProcessCallback(this_00,param_3,param_4);
          sp<android::ICacaoProcessCallback>::operator=
                    ((sp<android::ICacaoProcessCallback> *)&local_364,
                     (ICacaoProcessCallback *)this_00);
          iVar6 = 0;
          if (local_364 == 0) {
            puVar5 = (undefined4 *)(DAT_0001040c + 0x103b0);
          }
          else {
            piVar2 = *(int **)(this + 0x18);
            iVar6 = 0x18c;
            pcVar7 = *(code **)(*piVar2 + 0x28);
            local_370 = 0;
            puVar5 = (undefined4 *)(DAT_00010410 + 0x1031c);
            puVar3 = auStack_1b8;
            puVar4 = auStack_698;
            local_50c = param_2;
            psStack_508 = (sp *)&local_360;
            psStack_36c = (sp<android::ICacaoProcessCallback> *)&local_364;
            do {
              iVar6 = iVar6 + -4;
              *puVar4 = *puVar3;
              puVar3 = puVar3 + 1;
              puVar4 = puVar4 + 1;
            } while (iVar6 != 0);
            iVar6 = 0x194;
            puVar3 = auStack_504;
            puVar4 = auStack_358;
            do {
              iVar6 = iVar6 + -4;
              *puVar3 = *puVar4;
              puVar3 = puVar3 + 1;
              puVar4 = puVar4 + 1;
            } while (iVar6 != 0);
            iVar6 = (*pcVar7)(piVar2,&local_35c,local_1c0,local_1bc);
          }
        }
      }
    }
  }
  if (local_35c != 0) {
    sp<android::ICacaoService>::clear((sp<android::ICacaoService> *)&local_35c);
  }
LAB_000103b8:
  if (local_360 != 0) {
    sp<android::ICacaoService>::clear((sp<android::ICacaoService> *)&local_360);
  }
  sp<android::Cacao::DeathNotifier>::~sp((sp<android::Cacao::DeathNotifier> *)&local_364);
  sp<android::Cacao::DeathNotifier>::~sp((sp<android::Cacao::DeathNotifier> *)&local_360);
  sp<android::Cacao::DeathNotifier>::~sp((sp<android::Cacao::DeathNotifier> *)&local_35c);
  pthread_mutex_unlock((pthread_mutex_t *)(this + 0x1c));
  if (*(int *)*puVar5 == local_28) {
    return iVar6;
  }
                    /* WARNING: Subroutine does not return */
  __stack_chk_fail();
}

// ====== 00010414: processAsyncWithBinder ======
/* non-virtual thunk to android::Cacao::CacaoClient::processAsyncWithBinder(cacao::ProcessParamBase
   const*, android::sp<android::IBinder> const&, cacao::ICBProcessResultListener*,
   cacao::ProcessResultBase*) */

void __thiscall
android::Cacao::CacaoClient::processAsyncWithBinder
          (CacaoClient *this,ProcessParamBase *param_1,sp *param_2,ICBProcessResultListener *param_3
          ,ProcessResultBase *param_4)

{
                    /* WARNING: Could not recover jumptable at 0x00013df4. Too many branches */
                    /* WARNING: Treating indirect jump as call */
  (*(code *)(DAT_00013df8 + 0x13dfc))(this + -0x14);
  return;
}

// ====== 0001041a: cancel ======
/* android::Cacao::CacaoClient::cancel(void*) */

int __thiscall android::Cacao::CacaoClient::cancel(CacaoClient *this,void *param_1)

{
  int *piVar1;
  int iVar2;
  int iVar3;
  
  pthread_mutex_lock((pthread_mutex_t *)(this + 0x1c));
  piVar1 = *(int **)(this + 0x18);
  if (piVar1 == (int *)0x0) {
    iVar3 = -0x65;
  }
  else {
    iVar2 = (**(code **)(*piVar1 + 0x2c))(piVar1,param_1);
    iVar3 = iVar2;
    if (iVar2 != -0x6e) {
      iVar3 = -0x6f;
    }
    if (iVar2 == 0) {
      iVar3 = iVar2;
    }
  }
  pthread_mutex_unlock((pthread_mutex_t *)(this + 0x1c));
  return iVar3;
}

// ====== 00010458: cancel ======
/* non-virtual thunk to android::Cacao::CacaoClient::cancel(void*) */

void __thiscall android::Cacao::CacaoClient::cancel(CacaoClient *this,void *param_1)

{
                    /* WARNING: Could not recover jumptable at 0x00013e04. Too many branches */
                    /* WARNING: Treating indirect jump as call */
  (*(sp<android::IInterface>::sp<android::ICacaoService> + DAT_00013e08))(this + -0x14);
  return;
}

// ====== 0001045e: sp<android::MemoryHeapBase> ======
/* android::sp<android::IMemoryHeap>::sp<android::MemoryHeapBase>(android::MemoryHeapBase*) */

sp<android::IMemoryHeap> * __thiscall
android::sp<android::IMemoryHeap>::sp<android::MemoryHeapBase>
          (sp<android::IMemoryHeap> *this,MemoryHeapBase *param_1)

{
  if (param_1 == (MemoryHeapBase *)0x0) {
    *(undefined4 *)this = 0;
  }
  else {
    *(MemoryHeapBase **)this = param_1 + *(int *)(*(int *)param_1 + -0xc);
    android::RefBase::incStrong
              (param_1 + *(int *)(*(int *)(param_1 + *(int *)(*(int *)param_1 + -0xc)) + -0xc) +
                         *(int *)(*(int *)param_1 + -0xc));
  }
  return this;
}

// ====== 0001048e: DeathNotifier ======
/* android::Cacao::DeathNotifier::DeathNotifier() */

void __thiscall android::Cacao::DeathNotifier::DeathNotifier(DeathNotifier *this)

{
  int *in_r1;
  int iVar1;
  
  iVar1 = in_r1[1];
  *(int *)this = iVar1;
  *(int *)(this + *(int *)(iVar1 + -0xc)) = in_r1[2];
  iVar1 = *in_r1;
  *(int *)this = iVar1;
  *(int *)(this + *(int *)(iVar1 + -0xc)) = in_r1[3];
  return;
}

// ====== 000104a8: DeathNotifier ======
/* android::Cacao::DeathNotifier::DeathNotifier() */

DeathNotifier * __thiscall android::Cacao::DeathNotifier::DeathNotifier(DeathNotifier *this)

{
  int iVar1;
  
  android::RefBase::RefBase((RefBase *)(this + 4));
  iVar1 = *(int *)(DAT_000104c8 + 0x104b8);
  *(int *)this = iVar1 + 0xc;
  *(int *)(this + 4) = iVar1 + 0x34;
  return this;
}

// ====== 000104cc: ~DeathNotifier ======
/* android::Cacao::DeathNotifier::~DeathNotifier() */

void __thiscall android::Cacao::DeathNotifier::~DeathNotifier(DeathNotifier *this)

{
  int *in_r1;
  int iVar1;
  
  iVar1 = *in_r1;
  *(int *)this = iVar1;
  *(int *)(this + *(int *)(iVar1 + -0xc)) = in_r1[3];
  return;
}

// ====== 000104dc: ~DeathNotifier ======
/* android::Cacao::DeathNotifier::~DeathNotifier() */

DeathNotifier * __thiscall android::Cacao::DeathNotifier::~DeathNotifier(DeathNotifier *this)

{
  int iVar1;
  
  iVar1 = *(int *)(DAT_00010500 + 0x104e6);
  *(int *)this = iVar1 + 0xc;
  *(int *)(this + 4) = iVar1 + 0x34;
  android::RefBase::~RefBase((RefBase *)(this + 4));
  return this;
}

// ====== 00010504: ~DeathNotifier ======
/* virtual thunk to android::Cacao::DeathNotifier::~DeathNotifier() */

void __thiscall android::Cacao::DeathNotifier::~DeathNotifier(DeathNotifier *this)

{
  int iVar1;
  int iVar2;
  
  iVar1 = *(int *)(*(int *)this + -0xc);
  iVar2 = *(int *)(DAT_00010524 + 0x1050c);
  *(int *)(this + iVar1) = iVar2 + 0xc;
  *(int *)(this + iVar1 + 4) = iVar2 + 0x34;
  android::RefBase::~RefBase((RefBase *)(this + iVar1 + 4));
  return;
}

// ====== 00010528: ~DeathNotifier ======
/* android::Cacao::DeathNotifier::~DeathNotifier() */

void __thiscall android::Cacao::DeathNotifier::~DeathNotifier(DeathNotifier *this)

{
  int iVar1;
  
  iVar1 = *(int *)(DAT_00010550 + 0x10532);
  *(int *)this = iVar1 + 0xc;
  *(int *)(this + 4) = iVar1 + 0x34;
  android::RefBase::~RefBase((RefBase *)(this + 4));
  operator_delete(this);
  return;
}

// ====== 00010554: ~DeathNotifier ======
/* virtual thunk to android::Cacao::DeathNotifier::~DeathNotifier() */

void __thiscall android::Cacao::DeathNotifier::~DeathNotifier(DeathNotifier *this)

{
  int iVar1;
  DeathNotifier *pDVar2;
  
  iVar1 = *(int *)(DAT_00010584 + 0x1055e);
  pDVar2 = this + *(int *)(*(int *)this + -0xc);
  *(int *)(this + *(int *)(*(int *)this + -0xc)) = iVar1 + 0xc;
  *(int *)(pDVar2 + 4) = iVar1 + 0x34;
  android::RefBase::~RefBase((RefBase *)(pDVar2 + 4));
  operator_delete(pDVar2);
  return;
}

// ====== 00010588: binderDied ======
/* android::Cacao::DeathNotifier::binderDied(android::wp<android::IBinder> const&) */

void android::Cacao::DeathNotifier::binderDied(wp *param_1)

{
  int iVar1;
  undefined4 *puVar2;
  undefined4 *puVar3;
  sp<android::ICacaoService> *this;
  code *pcVar4;
  undefined4 *puVar5;
  wp<android::IBinder::DeathRecipient> awStack_38 [8];
  sp<android::IInterface> asStack_30 [4];
  int *local_2c;
  int local_28;
  
  puVar3 = (undefined4 *)(DAT_00010644 + 0x10598);
  puVar5 = (undefined4 *)(DAT_00010648 + 0x1059a);
  local_28 = *(int *)*puVar3;
  pthread_mutex_lock((pthread_mutex_t *)*puVar5);
  puVar2 = (undefined4 *)(DAT_0001064c + 0x105ac);
  if (*(int *)*puVar2 != 0) {
    sp<android::IInterface>::sp<android::ICacaoService>(asStack_30,(sp *)*puVar2);
    android::IInterface::asBinder((sp *)&local_2c);
    pcVar4 = *(code **)(*local_2c + 0x1c);
    this = *(sp<android::ICacaoService> **)(DAT_00010650 + 0x105d6);
    wp<android::IBinder::DeathRecipient>::wp<android::Cacao::DeathNotifier>(awStack_38,(sp *)this);
    (*pcVar4)(local_2c,awStack_38,0,0,0);
    puVar3 = (undefined4 *)(DAT_00010660 + 0x105f6);
    wp<android::IBinder::DeathRecipient>::~wp(awStack_38);
    sp<android::Cacao::DeathNotifier>::~sp((sp<android::Cacao::DeathNotifier> *)&local_2c);
    sp<android::Cacao::DeathNotifier>::~sp((sp<android::Cacao::DeathNotifier> *)asStack_30);
    sp<android::ICacaoService>::clear(this);
  }
  sp<android::ICacaoService>::clear((sp<android::ICacaoService> *)*puVar2);
  iVar1 = DAT_0001065c + 0x1061c;
  **(undefined4 **)(DAT_00010654 + 0x1061a) = 0;
  __android_log_print(5,DAT_00010658 + 0x10626,iVar1);
  pthread_mutex_unlock((pthread_mutex_t *)*puVar5);
  if (*(int *)*puVar3 == local_28) {
    return;
  }
                    /* WARNING: Subroutine does not return */
  __stack_chk_fail();
}

// ====== 00010664: wp<android::Cacao::DeathNotifier> ======
/* android::wp<android::IBinder::DeathRecipient>::wp<android::Cacao::DeathNotifier>(android::sp<android::Cacao::DeathNotifier>
   const&) */

wp<android::IBinder::DeathRecipient> * __thiscall
android::wp<android::IBinder::DeathRecipient>::wp<android::Cacao::DeathNotifier>
          (wp<android::IBinder::DeathRecipient> *this,sp *param_1)

{
  int *piVar1;
  undefined4 uVar2;
  
  piVar1 = *(int **)param_1;
  *(int **)this = piVar1;
  if (piVar1 != (int *)0x0) {
    uVar2 = android::RefBase::createWeak((void *)((int)piVar1 + *(int *)(*piVar1 + -0xc)));
    *(undefined4 *)(this + 4) = uVar2;
  }
  return this;
}

// ====== 00010684: create ======
/* android::Cacao::create() */

Cacao * android::Cacao::create(void)

{
  __pid_t _Var1;
  uint uVar2;
  Cacao *this;
  int iVar3;
  undefined4 *puVar4;
  sp<android::Cacao::DeathNotifier> local_18 [4];
  int local_14;
  
  puVar4 = (undefined4 *)(DAT_00010708 + 0x1068e);
  local_14 = *(int *)*puVar4;
  getService();
  android::ProcessState::self();
  android::ProcessState::startThreadPool();
  sp<android::Cacao::DeathNotifier>::~sp(local_18);
  if (**(int **)(DAT_0001070c + 0x106b0) != 0) {
    iVar3 = **(int **)(DAT_00010710 + 0x106ba);
    _Var1 = getpid();
    this = (Cacao *)0x0;
    if (iVar3 == _Var1) goto LAB_000106f2;
    this = operator_new(8);
    iVar3 = DAT_00010714;
    *(undefined4 *)(this + 4) = 0;
    *(int *)this = *(int *)(iVar3 + 0x106d6) + 8;
    uVar2 = connect(this);
    if (uVar2 < 0x80000000) goto LAB_000106f2;
    (**(code **)(*(int *)this + 4))(this);
  }
  this = (Cacao *)0x0;
LAB_000106f2:
  if (*(int *)*puVar4 == local_14) {
    return this;
  }
                    /* WARNING: Subroutine does not return */
  __stack_chk_fail();
}

// ====== 00010718: getService ======
/* android::Cacao::getService() */

void android::Cacao::getService(void)

{
  int *piVar1;
  undefined4 uVar2;
  ICacaoProcessCallback *pIVar3;
  undefined4 *puVar4;
  undefined4 *puVar5;
  uint uVar6;
  sp<android::IMemory> *this;
  sp<android::ICacaoProcessCallback> *this_00;
  undefined4 *puVar7;
  int iVar8;
  code *pcVar9;
  int *local_3c;
  String16 aSStack_38 [4];
  sp<android::Cacao::DeathNotifier> asStack_34 [4];
  int local_30;
  int *local_2c;
  int local_28;
  
  puVar4 = (undefined4 *)(DAT_000108c4 + 0x10726);
  puVar5 = (undefined4 *)(DAT_000108c8 + 0x10728);
  local_28 = *(int *)*puVar4;
  pthread_mutex_lock((pthread_mutex_t *)*puVar5);
  puVar7 = (undefined4 *)(DAT_000108cc + 0x10738);
  if (*(int *)*puVar7 == 0) {
    android::defaultServiceManager();
    if (local_2c == (int *)0x0) {
      __android_log_print(6,DAT_000108e4 + 0x107ca,DAT_000108e8 + 0x107cc);
    }
    else {
      uVar6 = 1;
      local_30 = 0;
      iVar8 = DAT_000108d0 + 0x1076c;
      while( true ) {
        piVar1 = local_2c;
        pcVar9 = *(code **)(*local_2c + 0x10);
        android::String16::String16(aSStack_38,(char *)(DAT_00010904 + 0x1079a));
        (*pcVar9)(asStack_34,piVar1,aSStack_38);
        sp<android::IMemory>::operator=((sp<android::IMemory> *)&local_30,(sp *)asStack_34);
        sp<android::Cacao::DeathNotifier>::~sp(asStack_34);
        android::String16::~String16(aSStack_38);
        if (local_30 != 0) break;
        usleep(500000);
        __android_log_print(5,DAT_000108fc + 0x1077e,iVar8,uVar6);
        if (9 < uVar6) {
          if (local_30 == 0) {
            __android_log_print(6,DAT_00010900 + 0x1088c,DAT_000108e0 + 0x1088a);
            puVar7 = (undefined4 *)(DAT_000108f4 + 0x10894);
            goto LAB_00010892;
          }
          break;
        }
        uVar6 = uVar6 + 1;
      }
      ICacaoService::asInterface((ICacaoService *)asStack_34,(sp *)&local_30);
      puVar7 = (undefined4 *)(DAT_000108f0 + 0x107e8);
      this = (sp<android::IMemory> *)*puVar7;
      sp<android::IMemory>::operator=(this,(sp *)asStack_34);
      sp<android::Cacao::DeathNotifier>::~sp(asStack_34);
      uVar2 = (**(code **)(**(int **)this + 0x14))();
      puVar4 = (undefined4 *)(DAT_000108d8 + 0x10806);
      **(undefined4 **)(DAT_000108d4 + 0x10804) = uVar2;
      if (*(int *)*puVar4 == 0) {
        pIVar3 = operator_new(0xc);
        android::RefBase::RefBase((RefBase *)(pIVar3 + 4));
        iVar8 = *(int *)(DAT_000108dc + 0x10824);
        *(int *)pIVar3 = iVar8 + 0xc;
        *(int *)(pIVar3 + 4) = iVar8 + 0x34;
        this_00 = (sp<android::ICacaoProcessCallback> *)*puVar4;
        sp<android::ICacaoProcessCallback>::operator=(this_00,pIVar3);
        sp<android::IInterface>::sp<android::ICacaoService>
                  ((sp<android::IInterface> *)asStack_34,(sp *)*puVar7);
        android::IInterface::asBinder((sp *)&local_3c);
        pcVar9 = *(code **)(*local_3c + 0x18);
        sp<android::IInterface>::sp<android::ICacaoService>
                  ((sp<android::IInterface> *)aSStack_38,(sp *)this_00);
        (*pcVar9)(local_3c,(sp<android::IInterface> *)aSStack_38,0,0);
        sp<android::Cacao::DeathNotifier>::~sp((sp<android::Cacao::DeathNotifier> *)aSStack_38);
        sp<android::Cacao::DeathNotifier>::~sp((sp<android::Cacao::DeathNotifier> *)&local_3c);
        sp<android::Cacao::DeathNotifier>::~sp(asStack_34);
      }
LAB_00010892:
      sp<android::Cacao::DeathNotifier>::~sp((sp<android::Cacao::DeathNotifier> *)&local_30);
      puVar4 = (undefined4 *)(DAT_000108ec + 0x108a0);
      puVar5 = (undefined4 *)(DAT_000108f8 + 0x108a2);
    }
    sp<android::Cacao::DeathNotifier>::~sp((sp<android::Cacao::DeathNotifier> *)&local_2c);
  }
  pthread_mutex_unlock((pthread_mutex_t *)*puVar5);
  if (*(int *)*puVar4 != local_28) {
                    /* WARNING: Subroutine does not return */
    __stack_chk_fail(*puVar7);
  }
  return;
}

// ====== 00010908: connect ======
/* android::Cacao::connect() */

uint __thiscall android::Cacao::connect(Cacao *this)

{
  CacaoClient *this_00;
  uint uVar1;
  undefined4 *puVar2;
  
  getService();
  puVar2 = (undefined4 *)(DAT_00010954 + 0x10916);
  if (*(int *)*puVar2 == 0) {
    uVar1 = 0;
  }
  else {
    this_00 = operator_new(0x30);
    CacaoClient::CacaoClient(this_00);
    sp<android::ICacaoProcessCallback>::operator=
              ((sp<android::ICacaoProcessCallback> *)(this + 4),(ICacaoProcessCallback *)this_00);
    if (*(CacaoClient **)(this + 4) == (CacaoClient *)0x0) {
      uVar1 = 0xffffff9a;
    }
    else {
      uVar1 = CacaoClient::connect(*(CacaoClient **)(this + 4),(sp *)*puVar2);
      if (uVar1 < 0x80000000) {
        return uVar1;
      }
    }
    disconnect(this);
  }
  return uVar1;
}

// ====== 00010958: getCaps ======
/* android::Cacao::getCaps(cacao::ProcessCtrlCaps::CameraIndex const&, cacao::Caps*) */

int android::Cacao::getCaps(CameraIndex *param_1,Caps *param_2)

{
  __pid_t _Var1;
  uint uVar2;
  MemoryHeapBase *this;
  MemoryBase *this_00;
  int *piVar3;
  undefined4 *puVar4;
  int iVar5;
  int iVar6;
  code *pcVar7;
  undefined4 *puVar8;
  undefined4 *puVar9;
  undefined4 auStack_350 [100];
  sp<android::IMemoryHeap> asStack_1c0 [4];
  int local_1bc;
  undefined4 local_1b8 [97];
  undefined4 local_34;
  undefined4 local_30;
  undefined4 local_2c;
  undefined4 uStack_28;
  int local_24;
  
  puVar8 = (undefined4 *)(DAT_00010a98 + 0x1096c);
  local_24 = *(int *)*puVar8;
  getService();
  puVar9 = (undefined4 *)(DAT_00010a9c + 0x1097e);
  if (*(int *)*puVar9 == 0) {
    iVar6 = 0;
  }
  else {
    iVar5 = **(int **)(DAT_00010aa0 + 0x1098c);
    _Var1 = getpid();
    iVar6 = 0;
    if (iVar5 != _Var1) {
      local_1bc = 0;
      __aeabi_memclr8(local_1b8,0x194);
      if (param_2 == (Caps *)0x0) {
        iVar6 = -0x67;
      }
      else {
        uVar2 = (**(code **)(*(int *)param_2 + 0x10))(param_2);
        this = operator_new(0x38);
        android::MemoryHeapBase::MemoryHeapBase(this,uVar2,0,(char *)0x0);
        sp<android::IMemoryHeap>::sp<android::MemoryHeapBase>(asStack_1c0,this);
        this_00 = operator_new(0x24);
        android::MemoryBase::MemoryBase(this_00,(sp *)asStack_1c0,0,uVar2);
        sp<android::ICacaoProcessCallback>::operator=
                  ((sp<android::ICacaoProcessCallback> *)&local_1bc,(ICacaoProcessCallback *)this_00
                  );
        iVar6 = local_1bc;
        sp<android::Cacao::DeathNotifier>::~sp((sp<android::Cacao::DeathNotifier> *)asStack_1c0);
        if (iVar6 == 0) {
          iVar6 = -0x66;
        }
        else {
          local_34 = android::IMemory::size();
          local_30 = android::IMemory::pointer();
          iVar6 = (**(code **)(*(int *)param_2 + 0x14))(param_2,local_1b8);
          if (-1 < iVar6) {
            iVar6 = 400;
            piVar3 = *(int **)*puVar9;
            pcVar7 = *(code **)(*piVar3 + 0x18);
            puVar9 = (undefined4 *)((uint)local_1b8 | 4);
            puVar4 = auStack_350;
            do {
              iVar6 = iVar6 + -4;
              *puVar4 = *puVar9;
              puVar9 = puVar9 + 1;
              puVar4 = puVar4 + 1;
            } while (iVar6 != 0);
            iVar5 = (*pcVar7)(piVar3,param_1,&local_1bc,local_1b8[0]);
            iVar6 = iVar5;
            if (iVar5 != -0x6e) {
              iVar6 = -0x6f;
            }
            if (iVar5 == 0) {
              local_2c = 0;
              uStack_28 = 0;
              iVar6 = (**(code **)(*(int *)param_2 + 0x18))(param_2,local_1b8);
            }
          }
        }
        if (local_1bc != 0) {
          sp<android::ICacaoService>::clear((sp<android::ICacaoService> *)&local_1bc);
        }
      }
      sp<android::Cacao::DeathNotifier>::~sp((sp<android::Cacao::DeathNotifier> *)&local_1bc);
    }
  }
  if (*(int *)*puVar8 == local_24) {
    return iVar6;
  }
                    /* WARNING: Subroutine does not return */
  __stack_chk_fail();
}

// ====== 00010aa4: getJpegBufferSize ======
/* android::Cacao::getJpegBufferSize(cacao::ImageSize) */

undefined4 __thiscall android::Cacao::getJpegBufferSize(Cacao *this,undefined4 param_2)

{
  __pid_t _Var1;
  int *piVar2;
  undefined4 uVar3;
  undefined4 *puVar4;
  int iVar5;
  
  getService();
  puVar4 = (undefined4 *)(DAT_00010ae4 + 0x10ab6);
  if ((*(int *)*puVar4 != 0) &&
     (iVar5 = **(int **)(DAT_00010ae8 + 0x10ac0), _Var1 = getpid(), iVar5 != _Var1)) {
    piVar2 = *(int **)*puVar4;
                    /* WARNING: Could not recover jumptable at 0x00010ae2. Too many branches */
                    /* WARNING: Treating indirect jump as call */
    uVar3 = (**(code **)(*piVar2 + 0x1c))(piVar2,this,param_2);
    return uVar3;
  }
  return 0;
}

// ====== 00010aec: isSameProcess ======
/* android::Cacao::isSameProcess() */

undefined4 android::Cacao::isSameProcess(void)

{
  __pid_t _Var1;
  undefined4 uVar2;
  int iVar3;
  
  getService();
  uVar2 = 0;
  if ((**(int **)(DAT_00010b14 + 0x10afa) != 0) &&
     (iVar3 = **(int **)(DAT_00010b18 + 0x10b04), _Var1 = getpid(), iVar3 == _Var1)) {
    uVar2 = 1;
  }
  return uVar2;
}

// ====== 00010b1c: ~Cacao ======
/* android::Cacao::~Cacao() */

Cacao * __thiscall android::Cacao::~Cacao(Cacao *this)

{
  *(int *)this = *(int *)(DAT_00010b40 + 0x10b28) + 8;
  disconnect(this);
  sp<android::Cacao::DeathNotifier>::~sp((sp<android::Cacao::DeathNotifier> *)(this + 4));
  return this;
}

// ====== 00010b44: disconnect ======
/* android::Cacao::disconnect() */

void __thiscall android::Cacao::disconnect(Cacao *this)

{
  CacaoClient *this_00;
  
  getService();
  if (**(int **)(DAT_00010b6c + 0x10b52) != 0) {
    this_00 = *(CacaoClient **)(this + 4);
    if (this_00 != (CacaoClient *)0x0) {
      CacaoClient::disconnect(this_00);
      (*(PAL_MemAllocEx + DAT_00013fc8))(this + 4);
      return;
    }
  }
  return;
}

// ====== 00010b70: ~Cacao ======
/* android::Cacao::~Cacao() */

void __thiscall android::Cacao::~Cacao(Cacao *this)

{
  *(int *)this = *(int *)(DAT_00010b98 + 0x10b7c) + 8;
  disconnect(this);
  sp<android::Cacao::DeathNotifier>::~sp((sp<android::Cacao::DeathNotifier> *)(this + 4));
  operator_delete(this);
  return;
}

// ====== 00010b9c: subscribeEvents ======
/* android::Cacao::subscribeEvents(cacao::ICBProcessEventListener*, void*) */

void android::Cacao::subscribeEvents(ICBProcessEventListener *param_1,void *param_2)

{
                    /* WARNING: Could not recover jumptable at 0x00010ba2. Too many branches */
                    /* WARNING: Treating indirect jump as call */
  (**(code **)(**(int **)(param_1 + 4) + 0x20))();
  return;
}

// ====== 00010ba4: start ======
/* android::Cacao::start(cacao::ProcessModeBase const*) */

void android::Cacao::start(ProcessModeBase *param_1)

{
                    /* WARNING: Could not recover jumptable at 0x00010baa. Too many branches */
                    /* WARNING: Treating indirect jump as call */
  (**(code **)(**(int **)(param_1 + 4) + 0x24))();
  return;
}

// ====== 00010bac: stop ======
/* android::Cacao::stop() */

void __thiscall android::Cacao::stop(Cacao *this)

{
                    /* WARNING: Could not recover jumptable at 0x00010bb2. Too many branches */
                    /* WARNING: Treating indirect jump as call */
  (**(code **)(**(int **)(this + 4) + 0x28))();
  return;
}

// ====== 00010bb4: setConfig ======
/* android::Cacao::setConfig(cacao::ProcessConfigBase const*) */

void android::Cacao::setConfig(ProcessConfigBase *param_1)

{
                    /* WARNING: Could not recover jumptable at 0x00010bba. Too many branches */
                    /* WARNING: Treating indirect jump as call */
  (**(code **)(**(int **)(param_1 + 4) + 0x2c))();
  return;
}

// ====== 00010bbc: getConfig ======
/* android::Cacao::getConfig(cacao::ProcessConfigBase*) */

void android::Cacao::getConfig(ProcessConfigBase *param_1)

{
                    /* WARNING: Could not recover jumptable at 0x00010bc2. Too many branches */
                    /* WARNING: Treating indirect jump as call */
  (**(code **)(**(int **)(param_1 + 4) + 0x30))();
  return;
}

// ====== 00010bc4: process ======
/* android::Cacao::process(cacao::ProcessParamBase const*, cacao::ICBProcessResultListener*,
   cacao::ProcessResultBase*) */

void android::Cacao::process
               (ProcessParamBase *param_1,ICBProcessResultListener *param_2,
               ProcessResultBase *param_3)

{
                    /* WARNING: Could not recover jumptable at 0x00010bce. Too many branches */
                    /* WARNING: Treating indirect jump as call */
  (**(code **)(**(int **)(param_1 + 4) + 0x34))();
  return;
}

// ====== 00010bd0: processAsync ======
/* android::Cacao::processAsync(cacao::ProcessParamBase const*, android::Vector<cacao::ImageBuf*>*,
   cacao::ICBProcessResultListener*, cacao::ProcessResultBase*) */

void android::Cacao::processAsync
               (ProcessParamBase *param_1,Vector *param_2,ICBProcessResultListener *param_3,
               ProcessResultBase *param_4)

{
                    /* WARNING: Could not recover jumptable at 0x00010bda. Too many branches */
                    /* WARNING: Treating indirect jump as call */
  (**(code **)(**(int **)(param_1 + 4) + 0x38))();
  return;
}

// ====== 00010bdc: processAsyncWithBinder ======
/* android::Cacao::processAsyncWithBinder(cacao::ProcessParamBase const*,
   android::sp<android::IBinder> const&, cacao::ICBProcessResultListener*,
   cacao::ProcessResultBase*) */

void android::Cacao::processAsyncWithBinder
               (ProcessParamBase *param_1,sp *param_2,ICBProcessResultListener *param_3,
               ProcessResultBase *param_4)

{
                    /* WARNING: Could not recover jumptable at 0x00010be6. Too many branches */
                    /* WARNING: Treating indirect jump as call */
  (**(code **)(**(int **)(param_1 + 4) + 0x3c))();
  return;
}

// ====== 00010be8: cancel ======
/* android::Cacao::cancel(void*) */

void android::Cacao::cancel(void *param_1)

{
                    /* WARNING: Could not recover jumptable at 0x00010bee. Too many branches */
                    /* WARNING: Treating indirect jump as call */
  (**(code **)(**(int **)((int)param_1 + 4) + 0x40))();
  return;
}

// ====== 00010bf0: Cacao ======
/* android::Cacao::Cacao() */

void __thiscall android::Cacao::Cacao(Cacao *this)

{
  *(undefined4 *)(this + 4) = 0;
  *(int *)this = *(int *)(DAT_00010c00 + 0x10bfa) + 8;
  return;
}

// ====== 00010c04: ObjectBase ======
/* cacao::ObjectBase::ObjectBase() */

void __thiscall cacao::ObjectBase::ObjectBase(ObjectBase *this)

{
  *(int *)this = *(int *)(DAT_00010c10 + 0x10c0a) + 8;
  return;
}

// ====== 00010c14: operator.new ======
/* cacao::ObjectBase::operator new(unsigned int) */

void * __thiscall cacao::ObjectBase::operator_new(ObjectBase *this,uint param_1)

{
  void *pvVar1;
  
  pvVar1 = (void *)PAL_MemAllocEx(0,this,DAT_00010c30 + 0x10c28,0x39,0);
  return pvVar1;
}

// ====== 00010c34: operator.delete ======
/* cacao::ObjectBase::operator delete(void*) */

void __thiscall cacao::ObjectBase::operator_delete(ObjectBase *this,void *param_1)

{
  (*(SharedBuffer::incRefCnt + DAT_00013fe8))(this,0);
  return;
}

// ====== 00010c3a: create ======
/* cacao::SharedBuffer::create() */

SharedBuffer * cacao::SharedBuffer::create(void)

{
  SharedBuffer *this;
  
  this = operator_new(8);
  *(undefined4 *)this = 0;
  pthread_mutex_init((pthread_mutex_t *)(this + 4),(pthread_mutexattr_t *)0x0);
  incRefCnt(this);
  return this;
}

// ====== 00010c5a: incRefCnt ======
/* cacao::SharedBuffer::incRefCnt() */

void __thiscall cacao::SharedBuffer::incRefCnt(SharedBuffer *this)

{
  pthread_mutex_lock((pthread_mutex_t *)(this + 4));
  *(int *)this = *(int *)this + 1;
  pthread_mutex_unlock((pthread_mutex_t *)(this + 4));
  return;
}

// ====== 00010c76: share ======
/* cacao::SharedBuffer::share(cacao::SharedBuffer*) */

SharedBuffer * cacao::SharedBuffer::share(SharedBuffer *param_1)

{
  if (param_1 != (SharedBuffer *)0x0) {
    incRefCnt(param_1);
  }
  return param_1;
}

// ====== 00010c86: release ======
/* cacao::SharedBuffer::release(cacao::SharedBuffer*) */

undefined4 cacao::SharedBuffer::release(SharedBuffer *param_1)

{
  int iVar1;
  
  if ((param_1 != (SharedBuffer *)0x0) && (iVar1 = decRefCnt(param_1), iVar1 == 0)) {
    pthread_mutex_destroy((pthread_mutex_t *)(param_1 + 4));
    operator_delete(param_1);
    return 1;
  }
  return 0;
}

// ====== 00010ca8: decRefCnt ======
/* cacao::SharedBuffer::decRefCnt() */

int __thiscall cacao::SharedBuffer::decRefCnt(SharedBuffer *this)

{
  int iVar1;
  
  pthread_mutex_lock((pthread_mutex_t *)(this + 4));
  iVar1 = *(int *)this;
  if (0 < iVar1) {
    iVar1 = iVar1 + -1;
    *(int *)this = iVar1;
  }
  pthread_mutex_unlock((pthread_mutex_t *)(this + 4));
  return iVar1;
}

// ====== 00010cc8: SharedBuffer ======
/* cacao::SharedBuffer::SharedBuffer() */

SharedBuffer * __thiscall cacao::SharedBuffer::SharedBuffer(SharedBuffer *this)

{
  *(undefined4 *)this = 0;
  pthread_mutex_init((pthread_mutex_t *)(this + 4),(pthread_mutexattr_t *)0x0);
  return this;
}

// ====== 00010cdc: ~SharedBuffer ======
/* cacao::SharedBuffer::~SharedBuffer() */

SharedBuffer * __thiscall cacao::SharedBuffer::~SharedBuffer(SharedBuffer *this)

{
  pthread_mutex_destroy((pthread_mutex_t *)(this + 4));
  return this;
}

// ====== 00010cec: ImageBuf ======
/* cacao::ImageBuf::ImageBuf() */

ImageBuf * __thiscall cacao::ImageBuf::ImageBuf(ImageBuf *this)

{
  undefined4 uVar1;
  int iVar2;
  
  ObjectBase::ObjectBase((ObjectBase *)this);
  *(undefined4 *)(this + 0x24) = 0;
  *(int *)this = *(int *)(DAT_00010d34 + 0x10d08) + 8;
  *(undefined8 *)(this + 0x14) = 0;
  *(undefined8 *)(this + 4) = 0;
  *(undefined8 *)(this + 0xc) = 0;
  *(undefined8 *)(this + 4) = 0;
  *(undefined8 *)(this + 0x1a) = 0;
  uVar1 = SharedBuffer::create();
  *(undefined4 *)(this + 0x24) = uVar1;
  iVar2 = getMapper();
  if (-1 < iVar2) {
    getAllocator();
  }
  return this;
}

// ====== 00010d38: getMapper ======
/* cacao::ImageBuf::getMapper() */

undefined4 cacao::ImageBuf::getMapper(void)

{
  int iVar1;
  undefined4 uVar2;
  undefined4 *puVar3;
  undefined4 *puVar4;
  ulonglong local_30;
  void *local_28;
  string asStack_20 [4];
  int local_1c;
  
  uVar2 = 0;
  puVar4 = (undefined4 *)(DAT_00010de8 + 0x10d4a);
  puVar3 = (undefined4 *)(DAT_00010dec + 0x10d4c);
  local_1c = *(int *)*puVar4;
  if (*(int *)*puVar3 == 0) {
    local_30 = 0xe;
    local_28 = (void *)0x0;
    __aeabi_memcpy((uint)&local_30 | 1,DAT_00010df0 + 0x10d78,7);
    local_28 = (void *)((uint)local_28 & 0xffffff00);
    android::hardware::graphics::mapper::V2_0::IMapper::getService(asStack_20,SUB41(&local_30,0));
    android::sp<android::IMemory>::operator=((sp<android::IMemory> *)*puVar3,(sp *)asStack_20);
    android::sp<android::Cacao::DeathNotifier>::~sp((sp<android::Cacao::DeathNotifier> *)asStack_20)
    ;
    if ((local_30 & 1) != 0) {
      operator_delete(local_28);
    }
    if ((*(undefined4 **)*puVar3 == (undefined4 *)0x0) ||
       (iVar1 = (**(code **)**(undefined4 **)*puVar3)(), iVar1 != 0)) {
      __android_log_print(6,DAT_00010df4 + 0x10dbc,DAT_00010df8 + 0x10dbe);
      android::sp<android::ICacaoProcessCallback>::operator=
                ((sp<android::ICacaoProcessCallback> *)*puVar3,(ICacaoProcessCallback *)0x0);
      uVar2 = 0xffffff91;
    }
    else {
      uVar2 = 0;
    }
  }
  if (*(int *)*puVar4 == local_1c) {
    return uVar2;
  }
                    /* WARNING: Subroutine does not return */
  __stack_chk_fail();
}

// ====== 00010dfc: getAllocator ======
/* cacao::ImageBuf::getAllocator() */

undefined4 cacao::ImageBuf::getAllocator(void)

{
  undefined4 uVar1;
  undefined4 *puVar2;
  undefined4 *puVar3;
  ulonglong local_30;
  void *local_28;
  string asStack_20 [4];
  int local_1c;
  
  uVar1 = 0;
  puVar3 = (undefined4 *)(DAT_00010e9c + 0x10e0e);
  puVar2 = (undefined4 *)(DAT_00010ea0 + 0x10e10);
  local_1c = *(int *)*puVar3;
  if (*(int *)*puVar2 == 0) {
    local_30 = 0xe;
    local_28 = (void *)0x0;
    __aeabi_memcpy((uint)&local_30 | 1,DAT_00010ea4 + 0x10e3c,7);
    local_28 = (void *)((uint)local_28 & 0xffffff00);
    android::hardware::graphics::allocator::V2_0::IAllocator::getService
              (asStack_20,SUB41(&local_30,0));
    android::sp<android::IMemory>::operator=((sp<android::IMemory> *)*puVar2,(sp *)asStack_20);
    android::sp<android::Cacao::DeathNotifier>::~sp((sp<android::Cacao::DeathNotifier> *)asStack_20)
    ;
    if ((local_30 & 1) != 0) {
      operator_delete(local_28);
    }
    if (*(int *)*puVar2 == 0) {
      __android_log_print(6,DAT_00010ea8 + 0x10e7c,DAT_00010eac + 0x10e7e);
      uVar1 = 0xffffff91;
    }
    else {
      uVar1 = 0;
    }
  }
  if (*(int *)*puVar3 == local_1c) {
    return uVar1;
  }
                    /* WARNING: Subroutine does not return */
  __stack_chk_fail();
}

// ====== 00010eb0: ImageBuf ======
/* cacao::ImageBuf::ImageBuf(cacao::ImageSize, unsigned int, cacao::ImageFormat, cacao::DataSpace)
    */

ImageBuf * __thiscall
cacao::ImageBuf::ImageBuf
          (ImageBuf *this,int param_2,int param_3,uint param_4,int param_5,undefined4 param_6)

{
  int iVar1;
  undefined4 uVar2;
  undefined4 *puVar3;
  undefined1 auStack_a0 [4];
  int local_9c;
  int local_98 [5];
  undefined1 local_84;
  uint local_80;
  uint uStack_7c;
  undefined4 local_78;
  undefined4 uStack_74;
  undefined4 local_70;
  undefined4 local_6c;
  undefined8 local_68;
  undefined8 local_60;
  int local_58;
  ImageBuf *local_54;
  int *piStack_50;
  function<void(android::hardware::graphics::mapper::V2_0::Error,void*)> *local_48;
  int local_40;
  undefined1 *local_3c;
  undefined8 *puStack_38;
  int *local_34;
  function<void(android::hardware::graphics::mapper::V2_0::Error,void*)> *local_30;
  int local_24;
  
  puVar3 = (undefined4 *)(DAT_00011074 + 0x10ec6);
  local_24 = *(int *)*puVar3;
  ObjectBase::ObjectBase((ObjectBase *)this);
  iVar1 = DAT_00011078;
  *(int *)(this + 4) = param_2;
  *(int *)this = *(int *)(iVar1 + 0x10ee4) + 8;
  *(int *)(this + 8) = param_3;
  *(undefined4 *)(this + 0xc) = 0;
  *(undefined4 *)(this + 0x10) = 0;
  *(uint *)(this + 0x14) = param_4;
  *(undefined4 *)(this + 0x18) = param_6;
  *(undefined4 *)(this + 0x1c) = 0;
  *(undefined2 *)(this + 0x20) = 0x101;
  *(undefined4 *)(this + 0x24) = 0;
  local_68 = 0;
  local_60 = 0;
  local_9c = getMapper();
  if (local_9c < 0) goto LAB_0001104c;
  local_9c = getAllocator();
  if (local_9c < 0) goto LAB_0001104c;
  if (param_4 == 0) {
    if (param_5 == 0x1000001) {
      param_4 = param_3 * param_2 * 2 + 0x10000;
      goto LAB_00010f28;
    }
    if (param_5 == 0x20001) {
      uStack_7c = param_3 + 1U & 0xfffffffe;
      param_4 = param_2 + 1U & 0xfffffffe;
      uStack_74 = 0x11;
    }
    else {
      if (param_5 != 0) {
        __android_log_print(6,DAT_0001109c + 0x11042,DAT_000110a0 + 0x11044);
        local_9c = -0x67;
        goto LAB_0001104c;
      }
      uStack_7c = param_3 + 1U & 0xfffffffe;
      param_4 = param_2 + 1U & 0xfffffffe;
      uStack_74 = 0x22;
    }
  }
  else {
LAB_00010f28:
    uStack_74 = 0x21;
    uStack_7c = 1;
  }
  local_6c = 0;
  local_70 = 0x33;
  local_78 = 1;
  local_40 = DAT_00011080 + 0x10f56;
  local_3c = auStack_a0;
  local_80 = param_4;
  puStack_38 = &local_68;
  local_34 = &local_9c;
  local_30 = (function<void(android::hardware::graphics::mapper::V2_0::Error,void*)> *)&local_40;
  (**(code **)(*(int *)**(undefined4 **)(DAT_0001107c + 0x10f42) + 0x34))
            (local_98,(int *)**(undefined4 **)(DAT_0001107c + 0x10f42),&local_80,
             (function<void(android::hardware::graphics::mapper::V2_0::Error,void*)> *)&local_40);
  std::function<void(android::hardware::graphics::mapper::V2_0::Error,void*)>::~function
            ((function<void(android::hardware::graphics::mapper::V2_0::Error,void*)> *)&local_40);
  local_84 = 1;
  if (local_98[0] != 0) {
    __android_log_print(6,DAT_00011084 + 0x10f7e,DAT_00011088 + 0x10f80);
    local_9c = -0x6f;
  }
  android::hardware::details::return_status::~return_status((return_status *)local_98);
  if (-1 < local_9c) {
    local_58 = DAT_00011090 + 0x10fb6;
    local_54 = this;
    piStack_50 = &local_9c;
    local_48 = (function<void(android::hardware::graphics::mapper::V2_0::Error,void*)> *)&local_58;
    (**(code **)(*(int *)**(undefined4 **)(DAT_0001108c + 0x10f9c) + 0x38))
              (&local_80,(int *)**(undefined4 **)(DAT_0001108c + 0x10f9c),&local_68,1,
               (function<void(android::hardware::graphics::mapper::V2_0::Error,void*)> *)&local_58);
    std::function<void(android::hardware::graphics::mapper::V2_0::Error,void*)>::~function
              ((function<void(android::hardware::graphics::mapper::V2_0::Error,void*)> *)&local_58);
    local_6c = CONCAT31(local_6c._1_3_,1);
    if (local_80 != 0) {
      __android_log_print(6,DAT_00011094 + 0x10fd4,DAT_00011098 + 0x10fd6);
      local_9c = -0x6f;
    }
    android::hardware::details::return_status::~return_status((return_status *)&local_80);
    if (-1 < local_9c) {
      uVar2 = SharedBuffer::create();
      *(undefined4 *)(this + 0x24) = uVar2;
    }
  }
LAB_0001104c:
  if ((local_60._4_1_ != '\0') && ((void *)local_68 != (void *)0x0)) {
    operator_delete__((void *)local_68);
  }
  if (*(int *)*puVar3 == local_24) {
    return this;
  }
                    /* WARNING: Subroutine does not return */
  __stack_chk_fail();
}

// ====== 000110a4: ~function ======
/* std::function<void (android::hardware::graphics::mapper::V2_0::Error, void*)>::~function() */

function<void(android::hardware::graphics::mapper::V2_0::Error,void*)> * __thiscall
std::function<void(android::hardware::graphics::mapper::V2_0::Error,void*)>::~function
          (function<void(android::hardware::graphics::mapper::V2_0::Error,void*)> *this)

{
  function<void(android::hardware::graphics::mapper::V2_0::Error,void*)> *pfVar1;
  code *pcVar2;
  
  pfVar1 = *(function<void(android::hardware::graphics::mapper::V2_0::Error,void*)> **)(this + 0x10)
  ;
  if (this == pfVar1) {
    pcVar2 = *(code **)(*(int *)pfVar1 + 0x10);
  }
  else {
    if (pfVar1 == (function<void(android::hardware::graphics::mapper::V2_0::Error,void*)> *)0x0) {
      return this;
    }
    pcVar2 = *(code **)(*(int *)pfVar1 + 0x14);
  }
  (*pcVar2)();
  return this;
}

// ====== 000110c0: ImageBuf ======
/* cacao::ImageBuf::ImageBuf(void*, cacao::ImageSize, cacao::DataSpace, cacao::ImageFormat) */

ImageBuf * __thiscall
cacao::ImageBuf::ImageBuf
          (ImageBuf *this,undefined4 param_1,int param_3,uint param_4,undefined4 param_5,int param_6
          )

{
  int iVar1;
  undefined4 uVar2;
  
  ObjectBase::ObjectBase((ObjectBase *)this);
  *(int *)(this + 4) = param_3;
  *(uint *)(this + 8) = param_4;
  *(undefined4 *)(this + 0xc) = param_1;
  *(undefined4 *)(this + 0x10) = 0;
  *(undefined4 *)(this + 0x14) = 0;
  *(undefined4 *)(this + 0x1c) = 0;
  *(undefined2 *)(this + 0x20) = 0x100;
  *(undefined4 *)(this + 0x24) = 0;
  *(undefined4 *)(this + 0x18) = param_5;
  *(int *)this = *(int *)(DAT_0001114c + 0x110ee) + 8;
  iVar1 = getMapper();
  if (iVar1 < 0) {
    return this;
  }
  if (param_6 == 0) {
LAB_00011112:
    iVar1 = (param_3 + 0x7fU & 0xffffff80) *
            (((param_4 >> 1) + 0x1f & 0xffffffe0) + (param_4 + 0x1f & 0xffffffe0));
  }
  else {
    if (param_6 != 0x1000001) {
      if (param_6 != 0x20001) goto LAB_00011140;
      goto LAB_00011112;
    }
    iVar1 = param_4 * param_3 * 2 + 0x10000;
  }
  *(int *)(this + 0x14) = iVar1;
LAB_00011140:
  uVar2 = SharedBuffer::create();
  *(undefined4 *)(this + 0x24) = uVar2;
  return this;
}

// ====== 00011150: ImageBuf ======
/* cacao::ImageBuf::ImageBuf(cacao::ImageBuf const&) */

ImageBuf * __thiscall cacao::ImageBuf::ImageBuf(ImageBuf *this,ImageBuf *param_1)

{
  undefined4 uVar1;
  SharedBuffer *this_00;
  
  ObjectBase::ObjectBase((ObjectBase *)this);
  *(undefined8 *)(this + 0x1a) = 0;
  *(undefined8 *)(this + 0x14) = 0;
  *(undefined8 *)(this + 4) = 0;
  *(undefined8 *)(this + 0xc) = 0;
  *(undefined4 *)(this + 0x24) = 0;
  *(int *)this = *(int *)(DAT_000111b8 + 0x1117e) + 8;
  uVar1 = *(undefined4 *)(param_1 + 8);
  *(undefined4 *)(this + 4) = *(undefined4 *)(param_1 + 4);
  *(undefined4 *)(this + 8) = uVar1;
  *(undefined4 *)(this + 0xc) = *(undefined4 *)(param_1 + 0xc);
  *(undefined4 *)(this + 0x10) = *(undefined4 *)(param_1 + 0x10);
  *(undefined4 *)(this + 0x14) = *(undefined4 *)(param_1 + 0x14);
  *(undefined4 *)(this + 0x18) = *(undefined4 *)(param_1 + 0x18);
  *(undefined4 *)(this + 0x1c) = *(undefined4 *)(param_1 + 0x1c);
  this[0x21] = param_1[0x21];
  this_00 = *(SharedBuffer **)(param_1 + 0x24);
  if (this_00 != (SharedBuffer *)0x0) {
    SharedBuffer::incRefCnt(this_00);
  }
  *(SharedBuffer **)(this + 0x24) = this_00;
  return this;
}

// ====== 000111bc: ~ImageBuf ======
/* cacao::ImageBuf::~ImageBuf() */

ImageBuf * __thiscall cacao::ImageBuf::~ImageBuf(ImageBuf *this)

{
  int iVar1;
  int *piVar2;
  undefined4 *puVar3;
  undefined4 *puVar4;
  return_status arStack_30 [28];
  int local_14;
  
  puVar4 = (undefined4 *)(DAT_00011230 + 0x111c8);
  local_14 = *(int *)*puVar4;
  *(int *)this = *(int *)(DAT_00011234 + 0x111d2) + 8;
  iVar1 = SharedBuffer::release(*(SharedBuffer **)(this + 0x24));
  if ((iVar1 != 0) && (puVar3 = (undefined4 *)(DAT_00011238 + 0x111e4), *(int *)*puVar3 != 0)) {
    if (*(int *)(this + 0x10) != 0) {
      (**(code **)(*(int *)this + 0x2c))(this);
    }
    if (this[0x20] != (ImageBuf)0x0) {
      piVar2 = *(int **)*puVar3;
      (**(code **)(*piVar2 + 0x3c))(arStack_30,piVar2,*(undefined4 *)(this + 0xc));
      *(undefined4 *)(this + 0xc) = 0;
      *(undefined4 *)(this + 0x10) = 0;
      android::hardware::details::return_status::~return_status(arStack_30);
    }
  }
  libunwind::AbstractUnwindCursor::~AbstractUnwindCursor((AbstractUnwindCursor *)this);
  if (*(int *)*puVar4 == local_14) {
    return this;
  }
                    /* WARNING: Subroutine does not return */
  __stack_chk_fail();
}

// ====== 0001123c: ~ImageBuf ======
/* cacao::ImageBuf::~ImageBuf() */

void __thiscall cacao::ImageBuf::~ImageBuf(ImageBuf *this)

{
  ~ImageBuf(this);
  (*(isValid + DAT_00014138))();
  return;
}

// ====== 0001124c: getBufSize ======
/* cacao::ImageBuf::getBufSize() const */

undefined4 __thiscall cacao::ImageBuf::getBufSize(ImageBuf *this)

{
  int iVar1;
  
  iVar1 = isValid(this);
  if (iVar1 != 0) {
    return *(undefined4 *)(this + 0x14);
  }
  __android_log_print(6,DAT_00011270 + 0x11266,DAT_00011274 + 0x11268,DAT_00011278 + 0x1126a);
  return 0;
}

// ====== 0001127c: isValid ======
/* cacao::ImageBuf::isValid() const */

undefined4 __thiscall cacao::ImageBuf::isValid(ImageBuf *this)

{
  if (*(int *)(this + 0xc) != 0) {
    return 1;
  }
  __android_log_print(6,DAT_00011298 + 0x11290,DAT_0001129c + 0x11292);
  return 0;
}

// ====== 000112a0: getBufAddress ======
/* cacao::ImageBuf::getBufAddress() */

int __thiscall cacao::ImageBuf::getBufAddress(ImageBuf *this)

{
  int iVar1;
  
  iVar1 = isValid(this);
  if (iVar1 == 0) {
    __android_log_print(6,DAT_000112d4 + 0x112bc,DAT_000112d8 + 0x112be,DAT_000112dc + 0x112c0);
    return 0;
  }
  if (*(int *)(this + 0x10) != 0) {
    return *(int *)(this + 0x10);
  }
  (**(code **)(*(int *)this + 0x28))(this);
  return *(int *)(this + 0x10);
}

// ====== 000112e0: getImageSize ======
/* cacao::ImageBuf::getImageSize() const */

void cacao::ImageBuf::getImageSize(void)

{
  undefined4 *in_r0;
  int in_r1;
  undefined4 uVar1;
  
  uVar1 = *(undefined4 *)(in_r1 + 8);
  *in_r0 = *(undefined4 *)(in_r1 + 4);
  in_r0[1] = uVar1;
  return;
}

// ====== 000112ea: getDirtyFlag ======
/* cacao::ImageBuf::getDirtyFlag() const */

bool __thiscall cacao::ImageBuf::getDirtyFlag(ImageBuf *this)

{
  int iVar1;
  
  iVar1 = isValid(this);
  if (iVar1 != 0) {
    return this[0x21] != (ImageBuf)0x0;
  }
  return false;
}

// ====== 00011304: setDirtyFlag ======
/* cacao::ImageBuf::setDirtyFlag(bool) */

void __thiscall cacao::ImageBuf::setDirtyFlag(ImageBuf *this,bool param_1)

{
  int iVar1;
  
  iVar1 = isValid(this);
  if (iVar1 != 0) {
    this[0x21] = (ImageBuf)param_1;
    return;
  }
  __android_log_print(6,DAT_0001132c + 0x11322,DAT_00011330 + 0x11324,DAT_00011334 + 0x11326);
  return;
}

// ====== 00011338: clearCache ======
/* cacao::ImageBuf::clearCache() */

void __thiscall cacao::ImageBuf::clearCache(ImageBuf *this)

{
  int iVar1;
  undefined4 uVar2;
  int *piVar3;
  code *pcVar4;
  undefined4 *puVar5;
  undefined4 *puVar6;
  undefined4 uVar7;
  undefined4 local_80;
  undefined4 uStack_7c;
  undefined4 local_78;
  undefined4 uStack_74;
  undefined4 local_70;
  int local_6c;
  int local_68 [5];
  undefined1 local_54;
  hidl_handle ahStack_50 [16];
  int local_40;
  undefined4 *local_3c;
  int *local_38;
  function<void(android::hardware::graphics::mapper::V2_0::Error,void*)> *local_30;
  int local_24;
  
  puVar6 = (undefined4 *)(DAT_00011428 + 0x11348);
  local_24 = *(int *)*puVar6;
  local_6c = 0;
  iVar1 = isValid(this);
  if ((iVar1 == 0) || (puVar5 = (undefined4 *)(DAT_0001142c + 0x11362), *(int *)*puVar5 == 0)) {
    __android_log_print(6,DAT_00011430 + 0x11404,DAT_00011434 + 0x11406,DAT_00011438 + 0x11408);
    local_6c = -0x65;
  }
  else {
    local_70 = 0;
    local_78 = *(undefined4 *)(this + 4);
    uStack_74 = *(undefined4 *)(this + 8);
    uVar7 = *(undefined4 *)(this + 0xc);
    local_80 = 0;
    uStack_7c = 0;
    uVar2 = android::hardware::hidl_handle::hidl_handle(ahStack_50);
    piVar3 = *(int **)*puVar5;
    pcVar4 = *(code **)(*piVar3 + 0x40);
    local_38 = &local_6c;
    local_3c = &local_70;
    local_40 = DAT_0001143c + 0x113a4;
    local_30 = (function<void(android::hardware::graphics::mapper::V2_0::Error,void*)> *)&local_40;
    (*pcVar4)(local_68,piVar3,uVar7,pcVar4,0x66,0,&local_80,uVar2,
              (function<void(android::hardware::graphics::mapper::V2_0::Error,void*)> *)&local_40);
    std::function<void(android::hardware::graphics::mapper::V2_0::Error,void*)>::~function
              ((function<void(android::hardware::graphics::mapper::V2_0::Error,void*)> *)&local_40);
    local_54 = 1;
    if (local_68[0] != 0) {
      __android_log_print(6,DAT_00011440 + 0x113ce,DAT_00011444 + 0x113d0);
      local_6c = -0x6f;
    }
    android::hardware::details::return_status::~return_status((return_status *)local_68);
    android::hardware::hidl_handle::~hidl_handle(ahStack_50);
    if (-1 < local_6c) {
      this[0x21] = (ImageBuf)0x0;
      *(undefined4 *)(this + 0x10) = local_70;
    }
  }
  if (*(int *)*puVar6 == local_24) {
    return;
  }
                    /* WARNING: Subroutine does not return */
  __stack_chk_fail();
}

// ====== 00011448: flushCache ======
/* cacao::ImageBuf::flushCache() */

void __thiscall cacao::ImageBuf::flushCache(ImageBuf *this)

{
  int iVar1;
  int *piVar2;
  undefined4 *puVar3;
  int local_50;
  int local_4c;
  int local_48 [5];
  undefined1 local_34;
  int local_30;
  undefined1 *local_2c;
  int *local_28;
  function<void(android::hardware::graphics::mapper::V2_0::Error,void*)> *local_20;
  int local_18;
  
  puVar3 = (undefined4 *)(DAT_00011500 + 0x11454);
  local_18 = *(int *)*puVar3;
  local_4c = 0;
  iVar1 = isValid(this);
  if ((iVar1 == 0) ||
     (piVar2 = (int *)**(undefined4 **)(DAT_00011504 + 0x1146a), piVar2 == (int *)0x0)) {
    __android_log_print(6,DAT_00011508 + 0x114e0,DAT_0001150c + 0x114e2,DAT_00011510 + 0x114e4);
    local_4c = -0x65;
  }
  else {
    local_50 = -1;
    local_28 = &local_4c;
    local_30 = DAT_00011514 + 0x11490;
    local_2c = (undefined1 *)&local_50;
    local_20 = (function<void(android::hardware::graphics::mapper::V2_0::Error,void*)> *)&local_30;
    (**(code **)(*piVar2 + 0x48))
              (local_48,piVar2,*(undefined4 *)(this + 0xc),
               (function<void(android::hardware::graphics::mapper::V2_0::Error,void*)> *)&local_30);
    std::function<void(android::hardware::graphics::mapper::V2_0::Error,void*)>::~function
              ((function<void(android::hardware::graphics::mapper::V2_0::Error,void*)> *)&local_30);
    local_34 = 1;
    if (local_48[0] != 0) {
      __android_log_print(6,DAT_00011518 + 0x114ae,DAT_0001151c + 0x114b0);
      local_4c = -0x6f;
    }
    android::hardware::details::return_status::~return_status((return_status *)local_48);
    if (-1 < local_4c) {
      this[0x21] = (ImageBuf)0x0;
    }
    close(local_50);
  }
  if (*(int *)*puVar3 == local_18) {
    return;
  }
                    /* WARNING: Subroutine does not return */
  __stack_chk_fail(local_4c);
}

// ====== 00011520: flushAndClearCache ======
/* cacao::ImageBuf::flushAndClearCache() */

void __thiscall cacao::ImageBuf::flushAndClearCache(ImageBuf *this)

{
  int iVar1;
  
  if (((this[0x21] != (ImageBuf)0x0) && (*(int *)(this + 0x10) != 0)) &&
     (iVar1 = (**(code **)(*(int *)this + 0x2c))(this), iVar1 < 0)) {
    return;
  }
                    /* WARNING: Could not recover jumptable at 0x00011544. Too many branches */
                    /* WARNING: Treating indirect jump as call */
  (**(code **)(*(int *)this + 0x28))(this);
  return;
}

// ====== 00011548: getDataSize ======
/* cacao::ImageBuf::getDataSize() const */

undefined4 __thiscall cacao::ImageBuf::getDataSize(ImageBuf *this)

{
  return *(undefined4 *)(this + 0x1c);
}

// ====== 0001154c: setDataSize ======
/* cacao::ImageBuf::setDataSize(unsigned int) */

void __thiscall cacao::ImageBuf::setDataSize(ImageBuf *this,uint param_1)

{
  int iVar1;
  
  iVar1 = isValid(this);
  if (iVar1 != 0) {
    *(uint *)(this + 0x1c) = param_1;
    return;
  }
  __android_log_print(6,DAT_00011574 + 0x11568,DAT_00011578 + 0x1156a,DAT_0001157c + 0x1156c);
  return;
}

// ====== 00011580: getDataSpace ======
/* cacao::ImageBuf::getDataSpace() const */

undefined4 __thiscall cacao::ImageBuf::getDataSpace(ImageBuf *this)

{
  return *(undefined4 *)(this + 0x18);
}

// ====== 00011584: getClassName ======
/* cacao::ImageBuf::getClassName() const */

undefined * cacao::ImageBuf::getClassName(void)

{
  return &UNK_0001158a + DAT_0001158c;
}

// ====== 00011590: getNative ======
/* cacao::ImageBuf::getNative() const */

undefined4 __thiscall cacao::ImageBuf::getNative(ImageBuf *this)

{
  return *(undefined4 *)(this + 0xc);
}

// ====== 00011594: ~__func ======
/* std::__function::__func<cacao::ImageBuf::ImageBuf(cacao::ImageSize, unsigned int,
   cacao::ImageFormat, cacao::DataSpace)::$_0,
   std::allocator<cacao::ImageBuf::ImageBuf(cacao::ImageSize, unsigned int, cacao::ImageFormat,
   cacao::DataSpace)::$_0>, void (android::hardware::graphics::mapper::V2_0::Error,
   android::hardware::hidl_vec<unsigned int> const&)>::~__func() */

void __thiscall
std::__function::
__func<cacao::ImageBuf::ImageBuf(cacao::ImageSize,unsigned_int,cacao::ImageFormat,cacao::DataSpace)::$_0,std::allocator<cacao::ImageBuf::ImageBuf(cacao::ImageSize,unsigned_int,cacao::ImageFormat,cacao::DataSpace)::$_0>,void(android::hardware::graphics::mapper::V2_0::Error,android::hardware::hidl_vec<unsigned_int>const&)>
::~__func(__func<cacao::ImageBuf::ImageBuf(cacao::ImageSize,unsigned_int,cacao::ImageFormat,cacao::DataSpace)::__0,std::allocator<cacao::ImageBuf::ImageBuf(cacao::ImageSize,unsigned_int,cacao::ImageFormat,cacao::DataSpace)::__0>,void(android::hardware::graphics::mapper::V2_0::Error,android::hardware::hidl_vec<unsigned_int>const&)>
          *this)

{
  (*(android::Parcel::Parcel + DAT_000138a8))(this);
  return;
}

// ====== 00011598: __clone ======
/* std::__function::__func<cacao::ImageBuf::ImageBuf(cacao::ImageSize, unsigned int,
   cacao::ImageFormat, cacao::DataSpace)::$_0,
   std::allocator<cacao::ImageBuf::ImageBuf(cacao::ImageSize, unsigned int, cacao::ImageFormat,
   cacao::DataSpace)::$_0>, void (android::hardware::graphics::mapper::V2_0::Error,
   android::hardware::hidl_vec<unsigned int> const&)>::__clone() const */

void __thiscall
std::__function::
__func<cacao::ImageBuf::ImageBuf(cacao::ImageSize,unsigned_int,cacao::ImageFormat,cacao::DataSpace)::$_0,std::allocator<cacao::ImageBuf::ImageBuf(cacao::ImageSize,unsigned_int,cacao::ImageFormat,cacao::DataSpace)::$_0>,void(android::hardware::graphics::mapper::V2_0::Error,android::hardware::hidl_vec<unsigned_int>const&)>
::__clone(__func<cacao::ImageBuf::ImageBuf(cacao::ImageSize,unsigned_int,cacao::ImageFormat,cacao::DataSpace)::__0,std::allocator<cacao::ImageBuf::ImageBuf(cacao::ImageSize,unsigned_int,cacao::ImageFormat,cacao::DataSpace)::__0>,void(android::hardware::graphics::mapper::V2_0::Error,android::hardware::hidl_vec<unsigned_int>const&)>
          *this)

{
  int *piVar1;
  
  piVar1 = operator_new(0x10);
  *piVar1 = DAT_000115b8 + 0x115b0;
  *(undefined8 *)(piVar1 + 1) = *(undefined8 *)(this + 4);
  piVar1[3] = *(int *)(this + 0xc);
  return;
}

// ====== 000115bc: __clone ======
/* std::__function::__func<cacao::ImageBuf::ImageBuf(cacao::ImageSize, unsigned int,
   cacao::ImageFormat, cacao::DataSpace)::$_0,
   std::allocator<cacao::ImageBuf::ImageBuf(cacao::ImageSize, unsigned int, cacao::ImageFormat,
   cacao::DataSpace)::$_0>, void (android::hardware::graphics::mapper::V2_0::Error,
   android::hardware::hidl_vec<unsigned int> const&)>::__clone(std::__function::__base<void
   (android::hardware::graphics::mapper::V2_0::Error, android::hardware::hidl_vec<unsigned int>
   const&)>*) const */

void __thiscall
std::__function::
__func<cacao::ImageBuf::ImageBuf(cacao::ImageSize,unsigned_int,cacao::ImageFormat,cacao::DataSpace)::$_0,std::allocator<cacao::ImageBuf::ImageBuf(cacao::ImageSize,unsigned_int,cacao::ImageFormat,cacao::DataSpace)::$_0>,void(android::hardware::graphics::mapper::V2_0::Error,android::hardware::hidl_vec<unsigned_int>const&)>
::__clone(__func<cacao::ImageBuf::ImageBuf(cacao::ImageSize,unsigned_int,cacao::ImageFormat,cacao::DataSpace)::__0,std::allocator<cacao::ImageBuf::ImageBuf(cacao::ImageSize,unsigned_int,cacao::ImageFormat,cacao::DataSpace)::__0>,void(android::hardware::graphics::mapper::V2_0::Error,android::hardware::hidl_vec<unsigned_int>const&)>
          *this,__base *param_1)

{
  undefined8 uVar1;
  
  *(int *)param_1 = DAT_000115d4 + 0x115ca;
  uVar1 = *(undefined8 *)(this + 4);
  *(undefined4 *)(param_1 + 0xc) = *(undefined4 *)(this + 0xc);
  *(undefined8 *)(param_1 + 4) = uVar1;
  return;
}

// ====== 000115d8: operator() ======
/* std::__function::__func<cacao::ImageBuf::ImageBuf(cacao::ImageSize, unsigned int,
   cacao::ImageFormat, cacao::DataSpace)::$_0,
   std::allocator<cacao::ImageBuf::ImageBuf(cacao::ImageSize, unsigned int, cacao::ImageFormat,
   cacao::DataSpace)::$_0>, void (android::hardware::graphics::mapper::V2_0::Error,
   android::hardware::hidl_vec<unsigned int>
   const&)>::TEMPNAMEPLACEHOLDERVALUE(android::hardware::graphics::mapper::V2_0::Error&&,
   android::hardware::hidl_vec<unsigned int> const&) */

void __thiscall
std::__function::
__func<cacao::ImageBuf::ImageBuf(cacao::ImageSize,unsigned_int,cacao::ImageFormat,cacao::DataSpace)::$_0,std::allocator<cacao::ImageBuf::ImageBuf(cacao::ImageSize,unsigned_int,cacao::ImageFormat,cacao::DataSpace)::$_0>,void(android::hardware::graphics::mapper::V2_0::Error,android::hardware::hidl_vec<unsigned_int>const&)>
::operator()(__func<cacao::ImageBuf::ImageBuf(cacao::ImageSize,unsigned_int,cacao::ImageFormat,cacao::DataSpace)::__0,std::allocator<cacao::ImageBuf::ImageBuf(cacao::ImageSize,unsigned_int,cacao::ImageFormat,cacao::DataSpace)::__0>,void(android::hardware::graphics::mapper::V2_0::Error,android::hardware::hidl_vec<unsigned_int>const&)>
             *this,Error *param_1,hidl_vec *param_2)

{
  **(undefined4 **)(this + 4) = *(undefined4 *)param_1;
  if (**(int **)(this + 4) != 0) {
    __android_log_print(6,DAT_0001160c + 0x115f2,DAT_00011610 + 0x115f4);
    **(undefined4 **)(this + 0xc) = 0xffffff91;
    return;
  }
  android::hardware::hidl_vec<unsigned_int>::operator=
            (*(hidl_vec<unsigned_int> **)(this + 8),param_2);
  return;
}

// ====== 00011614: operator= ======
/* android::hardware::hidl_vec<unsigned
   int>::TEMPNAMEPLACEHOLDERVALUE(android::hardware::hidl_vec<unsigned int> const&) */

hidl_vec<unsigned_int> * __thiscall
android::hardware::hidl_vec<unsigned_int>::operator=(hidl_vec<unsigned_int> *this,hidl_vec *param_1)

{
  if (this != (hidl_vec<unsigned_int> *)param_1) {
    if ((this[0xc] != (hidl_vec<unsigned_int>)0x0) && (*(void **)this != (void *)0x0)) {
      operator_delete__(*(void **)this);
    }
    copyFrom<android::hardware::hidl_vec<unsigned_int>>(this,param_1,*(uint *)(param_1 + 8));
  }
  return this;
}

// ====== 00011638: copyFrom<android::hardware::hidl_vec<unsigned_int>> ======
/* void android::hardware::hidl_vec<unsigned int>::copyFrom<android::hardware::hidl_vec<unsigned
   int> >(android::hardware::hidl_vec<unsigned int> const&, unsigned int) */

void __thiscall
android::hardware::hidl_vec<unsigned_int>::copyFrom<android::hardware::hidl_vec<unsigned_int>>
          (hidl_vec<unsigned_int> *this,hidl_vec *param_1,uint param_2)

{
  undefined4 *puVar1;
  int iVar2;
  uint uVar3;
  
  this[0xc] = (hidl_vec<unsigned_int>)0x1;
  *(uint *)(this + 8) = param_2;
  if (param_2 == 0) {
    *(undefined4 *)this = 0;
    return;
  }
  uVar3 = (uint)((ulonglong)param_2 * 4);
  if ((int)((ulonglong)param_2 * 4 >> 0x20) != 0) {
    uVar3 = 0xffffffff;
  }
  puVar1 = operator_new__(uVar3);
  __aeabi_memclr4(puVar1,uVar3);
  *(undefined4 **)this = puVar1;
  *puVar1 = **(undefined4 **)param_1;
  if (param_2 != 1) {
    uVar3 = 1;
    iVar2 = 4;
    do {
      uVar3 = uVar3 + 1;
      *(undefined4 *)(*(int *)this + iVar2) = *(undefined4 *)(*(int *)param_1 + iVar2);
      iVar2 = iVar2 + 4;
    } while (uVar3 != param_2);
  }
  return;
}

// ====== 000116a0: __clone ======
/* std::__function::__func<cacao::ImageBuf::ImageBuf(cacao::ImageSize, unsigned int,
   cacao::ImageFormat, cacao::DataSpace)::$_1,
   std::allocator<cacao::ImageBuf::ImageBuf(cacao::ImageSize, unsigned int, cacao::ImageFormat,
   cacao::DataSpace)::$_1>, void (android::hardware::graphics::mapper::V2_0::Error, unsigned int,
   android::hardware::hidl_vec<android::hardware::hidl_handle> const&)>::__clone() const */

void __thiscall
std::__function::
__func<cacao::ImageBuf::ImageBuf(cacao::ImageSize,unsigned_int,cacao::ImageFormat,cacao::DataSpace)::$_1,std::allocator<cacao::ImageBuf::ImageBuf(cacao::ImageSize,unsigned_int,cacao::ImageFormat,cacao::DataSpace)::$_1>,void(android::hardware::graphics::mapper::V2_0::Error,unsigned_int,android::hardware::hidl_vec<android::hardware::hidl_handle>const&)>
::__clone(__func<cacao::ImageBuf::ImageBuf(cacao::ImageSize,unsigned_int,cacao::ImageFormat,cacao::DataSpace)::__1,std::allocator<cacao::ImageBuf::ImageBuf(cacao::ImageSize,unsigned_int,cacao::ImageFormat,cacao::DataSpace)::__1>,void(android::hardware::graphics::mapper::V2_0::Error,unsigned_int,android::hardware::hidl_vec<android::hardware::hidl_handle>const&)>
          *this)

{
  int *piVar1;
  int iVar2;
  int iVar3;
  
  piVar1 = operator_new(0xc);
  iVar2 = *(int *)(this + 4);
  iVar3 = *(int *)(this + 8);
  *piVar1 = (int)&DAT_000116bc + DAT_000116bc;
  piVar1[1] = iVar2;
  piVar1[2] = iVar3;
  return;
}

// ====== 000116c0: __clone ======
/* std::__function::__func<cacao::ImageBuf::ImageBuf(cacao::ImageSize, unsigned int,
   cacao::ImageFormat, cacao::DataSpace)::$_1,
   std::allocator<cacao::ImageBuf::ImageBuf(cacao::ImageSize, unsigned int, cacao::ImageFormat,
   cacao::DataSpace)::$_1>, void (android::hardware::graphics::mapper::V2_0::Error, unsigned int,
   android::hardware::hidl_vec<android::hardware::hidl_handle>
   const&)>::__clone(std::__function::__base<void (android::hardware::graphics::mapper::V2_0::Error,
   unsigned int, android::hardware::hidl_vec<android::hardware::hidl_handle> const&)>*) const */

void __thiscall
std::__function::
__func<cacao::ImageBuf::ImageBuf(cacao::ImageSize,unsigned_int,cacao::ImageFormat,cacao::DataSpace)::$_1,std::allocator<cacao::ImageBuf::ImageBuf(cacao::ImageSize,unsigned_int,cacao::ImageFormat,cacao::DataSpace)::$_1>,void(android::hardware::graphics::mapper::V2_0::Error,unsigned_int,android::hardware::hidl_vec<android::hardware::hidl_handle>const&)>
::__clone(__func<cacao::ImageBuf::ImageBuf(cacao::ImageSize,unsigned_int,cacao::ImageFormat,cacao::DataSpace)::__1,std::allocator<cacao::ImageBuf::ImageBuf(cacao::ImageSize,unsigned_int,cacao::ImageFormat,cacao::DataSpace)::__1>,void(android::hardware::graphics::mapper::V2_0::Error,unsigned_int,android::hardware::hidl_vec<android::hardware::hidl_handle>const&)>
          *this,__base *param_1)

{
  undefined4 uVar1;
  
  *(int *)param_1 = DAT_000116d4 + 0x116ce;
  uVar1 = *(undefined4 *)(this + 8);
  *(undefined4 *)(param_1 + 4) = *(undefined4 *)(this + 4);
  *(undefined4 *)(param_1 + 8) = uVar1;
  return;
}

// ====== 000116d8: operator() ======
/* std::__function::__func<cacao::ImageBuf::ImageBuf(cacao::ImageSize, unsigned int,
   cacao::ImageFormat, cacao::DataSpace)::$_1,
   std::allocator<cacao::ImageBuf::ImageBuf(cacao::ImageSize, unsigned int, cacao::ImageFormat,
   cacao::DataSpace)::$_1>, void (android::hardware::graphics::mapper::V2_0::Error, unsigned int,
   android::hardware::hidl_vec<android::hardware::hidl_handle>
   const&)>::TEMPNAMEPLACEHOLDERVALUE(android::hardware::graphics::mapper::V2_0::Error&&, unsigned
   int&&, android::hardware::hidl_vec<android::hardware::hidl_handle> const&) */

void __thiscall
std::__function::
__func<cacao::ImageBuf::ImageBuf(cacao::ImageSize,unsigned_int,cacao::ImageFormat,cacao::DataSpace)::$_1,std::allocator<cacao::ImageBuf::ImageBuf(cacao::ImageSize,unsigned_int,cacao::ImageFormat,cacao::DataSpace)::$_1>,void(android::hardware::graphics::mapper::V2_0::Error,unsigned_int,android::hardware::hidl_vec<android::hardware::hidl_handle>const&)>
::operator()(__func<cacao::ImageBuf::ImageBuf(cacao::ImageSize,unsigned_int,cacao::ImageFormat,cacao::DataSpace)::__1,std::allocator<cacao::ImageBuf::ImageBuf(cacao::ImageSize,unsigned_int,cacao::ImageFormat,cacao::DataSpace)::__1>,void(android::hardware::graphics::mapper::V2_0::Error,unsigned_int,android::hardware::hidl_vec<android::hardware::hidl_handle>const&)>
             *this,Error *param_1,uint *param_2,hidl_vec *param_3)

{
  undefined4 *puVar1;
  int local_48 [5];
  undefined1 local_34;
  int local_30;
  undefined4 local_2c;
  undefined4 local_28;
  function<void(android::hardware::graphics::mapper::V2_0::Error,void*)> *local_20;
  int local_18;
  
  puVar1 = (undefined4 *)(DAT_0001176c + 0x116e4);
  local_18 = *(int *)*puVar1;
  if (*(int *)param_1 == 0) {
    local_2c = *(undefined4 *)(this + 4);
    local_28 = *(undefined4 *)(this + 8);
    local_30 = DAT_0001177c + 0x1172a;
    local_20 = (function<void(android::hardware::graphics::mapper::V2_0::Error,void*)> *)&local_30;
    (**(code **)(*(int *)**(undefined4 **)(DAT_00011778 + 0x11710) + 0x38))
              (local_48,(int *)**(undefined4 **)(DAT_00011778 + 0x11710),*(undefined4 *)param_3,
               (function<void(android::hardware::graphics::mapper::V2_0::Error,void*)> *)&local_30);
    function<void(android::hardware::graphics::mapper::V2_0::Error,void*)>::~function
              ((function<void(android::hardware::graphics::mapper::V2_0::Error,void*)> *)&local_30);
    local_34 = 1;
    if (local_48[0] != 0) {
      __android_log_print(6,DAT_00011780 + 0x11748,DAT_00011784 + 0x1174a);
      **(undefined4 **)(this + 8) = 0xffffff91;
    }
    android::hardware::details::return_status::~return_status((return_status *)local_48);
  }
  else {
    __android_log_print(6,DAT_00011770 + 0x116f8,DAT_00011774 + 0x116fa,*(int *)param_1);
    **(undefined4 **)(this + 8) = 0xffffff91;
  }
  if (*(int *)*puVar1 == local_18) {
    return;
  }
                    /* WARNING: Subroutine does not return */
  __stack_chk_fail();
}

// ====== 00011788: operator() ======
/* std::__function::__func<cacao::ImageBuf::ImageBuf(cacao::ImageSize, unsigned int,
   cacao::ImageFormat,
   cacao::DataSpace)::$_1::TEMPNAMEPLACEHOLDERVALUE(android::hardware::graphics::mapper::V2_0::Error
   const&, unsigned int const&, android::hardware::hidl_vec<android::hardware::hidl_handle> const&)
   const::{lambda(auto:2, auto:3)#1}, std::allocator<std::allocator>, void
   (android::hardware::graphics::mapper::V2_0::Error, void*)>::__clone() const */

void __thiscall
std::__function::
__func<cacao::ImageBuf::ImageBuf(cacao::ImageSize,unsigned_int,cacao::ImageFormat,cacao::DataSpace)::$_1::TEMPNAMEPLACEHOLDERVALUE(android::hardware::graphics::mapper::V2_0::Error_const&,unsigned_int_const&,android::hardware::hidl_vec<android::hardware::hidl_handle>const&)const::{lambda(auto:2,auto:3)#1},std::allocator<std::allocator>,void(android::hardware::graphics::mapper::V2_0::Error,void*)>
::operator()(__func<cacao::ImageBuf::ImageBuf(cacao::ImageSize,unsigned_int,cacao::ImageFormat,cacao::DataSpace)::__1::TEMPNAMEPLACEHOLDERVALUE(android::hardware::graphics::mapper::V2_0::Error_const&,unsigned_int_const&,android::hardware::hidl_vec<android::hardware::hidl_handle>const&)const::_lambda(auto:2,auto:3)_1_,std::allocator<std::allocator>,void(android::hardware::graphics::mapper::V2_0::Error,void*)>
             *this)

{
  int *piVar1;
  int iVar2;
  int iVar3;
  
  piVar1 = operator_new(0xc);
  iVar2 = *(int *)(this + 4);
  iVar3 = *(int *)(this + 8);
  *piVar1 = (int)&DAT_000117a4 + DAT_000117a4;
  piVar1[1] = iVar2;
  piVar1[2] = iVar3;
  return;
}

// ====== 000117a8: operator() ======
/* std::__function::__func<cacao::ImageBuf::ImageBuf(cacao::ImageSize, unsigned int,
   cacao::ImageFormat,
   cacao::DataSpace)::$_1::TEMPNAMEPLACEHOLDERVALUE(android::hardware::graphics::mapper::V2_0::Error
   const&, unsigned int const&, android::hardware::hidl_vec<android::hardware::hidl_handle> const&)
   const::{lambda(auto:2, auto:3)#1}, std::allocator<std::allocator>, void
   (android::hardware::graphics::mapper::V2_0::Error,
   void*)>::__clone(std::__function::__base<std::__function::__func<cacao::ImageBuf::ImageBuf(cacao::ImageSize,
   unsigned int, cacao::ImageFormat,
   cacao::DataSpace)::$_1::TEMPNAMEPLACEHOLDERVALUE(android::hardware::graphics::mapper::V2_0::Error
   const&, unsigned int const&, android::hardware::hidl_vec<android::hardware::hidl_handle> const&)
   const::{lambda(auto:2, auto:3)#1}, std::allocator<std::allocator>, void
   (android::hardware::graphics::mapper::V2_0::Error, void*)> >*) const */

void __thiscall
std::__function::
__func<cacao::ImageBuf::ImageBuf(cacao::ImageSize,unsigned_int,cacao::ImageFormat,cacao::DataSpace)::$_1::TEMPNAMEPLACEHOLDERVALUE(android::hardware::graphics::mapper::V2_0::Error_const&,unsigned_int_const&,android::hardware::hidl_vec<android::hardware::hidl_handle>const&)const::{lambda(auto:2,auto:3)#1},std::allocator<std::allocator>,void(android::hardware::graphics::mapper::V2_0::Error,void*)>
::operator()(__func<cacao::ImageBuf::ImageBuf(cacao::ImageSize,unsigned_int,cacao::ImageFormat,cacao::DataSpace)::__1::TEMPNAMEPLACEHOLDERVALUE(android::hardware::graphics::mapper::V2_0::Error_const&,unsigned_int_const&,android::hardware::hidl_vec<android::hardware::hidl_handle>const&)const::_lambda(auto:2,auto:3)_1_,std::allocator<std::allocator>,void(android::hardware::graphics::mapper::V2_0::Error,void*)>
             *this,__base *param_1)

{
  undefined4 uVar1;
  
  *(int *)param_1 = DAT_000117bc + 0x117b6;
  uVar1 = *(undefined4 *)(this + 8);
  *(undefined4 *)(param_1 + 4) = *(undefined4 *)(this + 4);
  *(undefined4 *)(param_1 + 8) = uVar1;
  return;
}

// ====== 000117c0: operator() ======
/* std::__function::__func<cacao::ImageBuf::ImageBuf(cacao::ImageSize, unsigned int,
   cacao::ImageFormat,
   cacao::DataSpace)::$_1::TEMPNAMEPLACEHOLDERVALUE<android::hardware::graphics::mapper::V2_0::Error,
   unsigned int, android::hardware::hidl_vec<android::hardware::hidl_handle>
   >(android::hardware::graphics::mapper::V2_0::Error const&, unsigned int const&,
   android::hardware::hidl_vec<android::hardware::hidl_handle> const&) const::{lambda(auto:2,
   auto:3)#1}, std::allocator<std::allocator>, void
   (android::hardware::graphics::mapper::V2_0::Error,
   void*)>::TEMPNAMEPLACEHOLDERVALUE(android::hardware::graphics::mapper::V2_0::Error&&, void
   (&&)(android::hardware::graphics::mapper::V2_0::Error, void*)) */

void __thiscall
std::__function::
__func<cacao::ImageBuf::ImageBuf(cacao::ImageSize,unsigned_int,cacao::ImageFormat,cacao::DataSpace)::$_1::TEMPNAMEPLACEHOLDERVALUE<android::hardware::graphics::mapper::V2_0::Error,unsigned_int,android::hardware::hidl_vec<android::hardware::hidl_handle>>(android::hardware::graphics::mapper::V2_0::Error_const&,unsigned_int_const&,android::hardware::hidl_vec<android::hardware::hidl_handle>const&)const::{lambda(auto:2,auto:3)#1},std::allocator<std::allocator>,void(android::hardware::graphics::mapper::V2_0::Error,void*)>
::operator()(__func<cacao::ImageBuf::ImageBuf(cacao::ImageSize,unsigned_int,cacao::ImageFormat,cacao::DataSpace)::__1::TEMPNAMEPLACEHOLDERVALUE<android::hardware::graphics::mapper::V2_0::Error,unsigned_int,android::hardware::hidl_vec<android::hardware::hidl_handle>>(android::hardware::graphics::mapper::V2_0::Error_const&,unsigned_int_const&,android::hardware::hidl_vec<android::hardware::hidl_handle>const&)const::_lambda(auto:2,auto:3)_1_,std::allocator<std::allocator>,void(android::hardware::graphics::mapper::V2_0::Error,void*)>
             *this,Error *param_1,_func_void_Error_void_ptr *param_2)

{
  undefined4 *puVar1;
  int local_30 [5];
  undefined1 local_1c;
  int local_14;
  
  puVar1 = (undefined4 *)(DAT_00011828 + 0x117ce);
  local_14 = *(int *)*puVar1;
  if (*(int *)param_1 == 0) {
    *(undefined4 *)(*(int *)(this + 4) + 0xc) = *(undefined4 *)param_2;
  }
  else {
    __android_log_print(6,DAT_0001182c + 0x117e0,DAT_00011830 + 0x117e2);
    (**(code **)(*(int *)**(undefined4 **)(DAT_00011834 + 0x117ec) + 0x3c))
              (local_30,(int *)**(undefined4 **)(DAT_00011834 + 0x117ec),*(undefined4 *)param_2);
    local_1c = 1;
    if (local_30[0] != 0) {
      **(undefined4 **)(this + 8) = 0xffffff91;
    }
    android::hardware::details::return_status::~return_status((return_status *)local_30);
  }
  if (*(int *)*puVar1 == local_14) {
    return;
  }
                    /* WARNING: Subroutine does not return */
  __stack_chk_fail();
}

// ====== 00011838: __clone ======
/* std::__function::__func<cacao::ImageBuf::clearCache()::$_2,
   std::allocator<cacao::ImageBuf::clearCache()::$_2>, void
   (android::hardware::graphics::mapper::V2_0::Error, void*)>::__clone() const */

void __thiscall
std::__function::
__func<cacao::ImageBuf::clearCache()::$_2,std::allocator<cacao::ImageBuf::clearCache()::$_2>,void(android::hardware::graphics::mapper::V2_0::Error,void*)>
::__clone(__func<cacao::ImageBuf::clearCache()::__2,std::allocator<cacao::ImageBuf::clearCache()::__2>,void(android::hardware::graphics::mapper::V2_0::Error,void*)>
          *this)

{
  int *piVar1;
  int iVar2;
  int iVar3;
  
  piVar1 = operator_new(0xc);
  iVar2 = *(int *)(this + 4);
  iVar3 = *(int *)(this + 8);
  *piVar1 = (int)&DAT_00011854 + DAT_00011854;
  piVar1[1] = iVar2;
  piVar1[2] = iVar3;
  return;
}

// ====== 00011858: __clone ======
/* std::__function::__func<cacao::ImageBuf::clearCache()::$_2,
   std::allocator<cacao::ImageBuf::clearCache()::$_2>, void
   (android::hardware::graphics::mapper::V2_0::Error, void*)>::__clone(std::__function::__base<void
   (android::hardware::graphics::mapper::V2_0::Error, void*)>*) const */

void __thiscall
std::__function::
__func<cacao::ImageBuf::clearCache()::$_2,std::allocator<cacao::ImageBuf::clearCache()::$_2>,void(android::hardware::graphics::mapper::V2_0::Error,void*)>
::__clone(__func<cacao::ImageBuf::clearCache()::__2,std::allocator<cacao::ImageBuf::clearCache()::__2>,void(android::hardware::graphics::mapper::V2_0::Error,void*)>
          *this,__base *param_1)

{
  undefined4 uVar1;
  
  *(int *)param_1 = DAT_0001186c + 0x11866;
  uVar1 = *(undefined4 *)(this + 8);
  *(undefined4 *)(param_1 + 4) = *(undefined4 *)(this + 4);
  *(undefined4 *)(param_1 + 8) = uVar1;
  return;
}

// ====== 00011870: operator() ======
/* std::__function::__func<cacao::ImageBuf::clearCache()::$_2,
   std::allocator<cacao::ImageBuf::clearCache()::$_2>, void
   (android::hardware::graphics::mapper::V2_0::Error,
   void*)>::TEMPNAMEPLACEHOLDERVALUE(android::hardware::graphics::mapper::V2_0::Error&&, void*&&) */

void __thiscall
std::__function::
__func<cacao::ImageBuf::clearCache()::$_2,std::allocator<cacao::ImageBuf::clearCache()::$_2>,void(android::hardware::graphics::mapper::V2_0::Error,void*)>
::operator()(__func<cacao::ImageBuf::clearCache()::__2,std::allocator<cacao::ImageBuf::clearCache()::__2>,void(android::hardware::graphics::mapper::V2_0::Error,void*)>
             *this,Error *param_1,void **param_2)

{
  __func<cacao::ImageBuf::clearCache()::__2,std::allocator<cacao::ImageBuf::clearCache()::__2>,void(android::hardware::graphics::mapper::V2_0::Error,void*)>
  *p_Var1;
  void *pvVar2;
  
  if (*(int *)param_1 == 0) {
    pvVar2 = *param_2;
    p_Var1 = this + 4;
  }
  else {
    __android_log_print(6,DAT_0001189c + 0x11882,DAT_000118a0 + 0x11884);
    p_Var1 = this + 8;
    pvVar2 = (void *)0xffffff91;
  }
  **(undefined4 **)p_Var1 = pvVar2;
  return;
}

// ====== 000118a4: __clone ======
/* std::__function::__func<cacao::ImageBuf::flushCache()::$_3,
   std::allocator<cacao::ImageBuf::flushCache()::$_3>, void
   (android::hardware::graphics::mapper::V2_0::Error, android::hardware::hidl_handle
   const&)>::__clone() const */

void __thiscall
std::__function::
__func<cacao::ImageBuf::flushCache()::$_3,std::allocator<cacao::ImageBuf::flushCache()::$_3>,void(android::hardware::graphics::mapper::V2_0::Error,android::hardware::hidl_handle_const&)>
::__clone(__func<cacao::ImageBuf::flushCache()::__3,std::allocator<cacao::ImageBuf::flushCache()::__3>,void(android::hardware::graphics::mapper::V2_0::Error,android::hardware::hidl_handle_const&)>
          *this)

{
  int *piVar1;
  int iVar2;
  int iVar3;
  
  piVar1 = operator_new(0xc);
  iVar2 = *(int *)(this + 4);
  iVar3 = *(int *)(this + 8);
  *piVar1 = (int)&DAT_000118c0 + DAT_000118c0;
  piVar1[1] = iVar2;
  piVar1[2] = iVar3;
  return;
}

// ====== 000118c4: __clone ======
/* std::__function::__func<cacao::ImageBuf::flushCache()::$_3,
   std::allocator<cacao::ImageBuf::flushCache()::$_3>, void
   (android::hardware::graphics::mapper::V2_0::Error, android::hardware::hidl_handle
   const&)>::__clone(std::__function::__base<void (android::hardware::graphics::mapper::V2_0::Error,
   android::hardware::hidl_handle const&)>*) const */

void __thiscall
std::__function::
__func<cacao::ImageBuf::flushCache()::$_3,std::allocator<cacao::ImageBuf::flushCache()::$_3>,void(android::hardware::graphics::mapper::V2_0::Error,android::hardware::hidl_handle_const&)>
::__clone(__func<cacao::ImageBuf::flushCache()::__3,std::allocator<cacao::ImageBuf::flushCache()::__3>,void(android::hardware::graphics::mapper::V2_0::Error,android::hardware::hidl_handle_const&)>
          *this,__base *param_1)

{
  undefined4 uVar1;
  
  *(int *)param_1 = DAT_000118d8 + 0x118d2;
  uVar1 = *(undefined4 *)(this + 8);
  *(undefined4 *)(param_1 + 4) = *(undefined4 *)(this + 4);
  *(undefined4 *)(param_1 + 8) = uVar1;
  return;
}

// ====== 000118dc: operator() ======
/* std::__function::__func<cacao::ImageBuf::flushCache()::$_3,
   std::allocator<cacao::ImageBuf::flushCache()::$_3>, void
   (android::hardware::graphics::mapper::V2_0::Error, android::hardware::hidl_handle
   const&)>::TEMPNAMEPLACEHOLDERVALUE(android::hardware::graphics::mapper::V2_0::Error&&,
   android::hardware::hidl_handle const&) */

void std::__function::
     __func<cacao::ImageBuf::flushCache()::$_3,std::allocator<cacao::ImageBuf::flushCache()::$_3>,void(android::hardware::graphics::mapper::V2_0::Error,android::hardware::hidl_handle_const&)>
     ::operator()(Error *param_1,hidl_handle *param_2)

{
  int iVar1;
  
  if (*(int *)param_2 == 0) {
    iVar1 = android::hardware::hidl_handle::getNativeHandle();
    if (((iVar1 != 0) && (*(int *)(iVar1 + 4) == 1)) &&
       (iVar1 = dup(*(int *)(iVar1 + 0xc)), -1 < iVar1)) {
      **(int **)(param_1 + 4) = iVar1;
      return;
    }
  }
  else {
    __android_log_print(6,DAT_0001191c + 0x118ee,DAT_00011920 + 0x118f0);
    **(undefined4 **)(param_1 + 8) = 0xffffff91;
  }
  return;
}

// ====== 00011924: createProcess ======
/* cacao::ProcessFactory::createProcess() */

void cacao::ProcessFactory::createProcess(void)

{
  (*(android::Cacao::getJpegBufferSize + DAT_000141d8))();
  return;
}

// ====== 00011928: createEvent ======
/* cacao::ProcessFactory::createEvent(unsigned int, cacao::ISerialize::SerializedData*) */

undefined4 cacao::ProcessFactory::createEvent(uint param_1,SerializedData *param_2)

{
  return 0;
}

// ====== 0001192c: getJpegBufferSize ======
/* cacao::ProcessFactory::getJpegBufferSize(cacao::ImageSize) */

void cacao::ProcessFactory::getJpegBufferSize(void)

{
  (*(android::Cacao::getCaps + DAT_000141e8))();
  return;
}

// ====== 00011930: getCaps ======
/* cacao::ProcessCtrlCapsFactory::getCaps(cacao::ProcessCtrlCaps::CameraIndex const&, cacao::Caps*)
    */

void cacao::ProcessCtrlCapsFactory::getCaps(CameraIndex *param_1,Caps *param_2)

{
  (*(fprintf + DAT_000141f8))(param_1);
  return;
}

// ====== 00011934: decode_eht_entry ======
uint * decode_eht_entry(uint *param_1,undefined4 *param_2,int *param_3)

{
  uint uVar1;
  uint uVar2;
  
  uVar1 = *param_1;
  if (-1 < (int)uVar1) {
    *param_2 = 1;
    *param_3 = (param_1[1] >> 0x18) * 4 + 4;
    return param_1 + 1;
  }
  uVar2 = (uVar1 << 4) >> 0x1c;
  if ((uVar2 != 3) && (uVar2 != 1)) {
    if (uVar2 == 0) {
      *param_3 = 4;
      *param_2 = 1;
      return param_1;
    }
    return (uint *)0x0;
  }
  *param_3 = ((uVar1 << 8) >> 0x18) * 4 + 4;
  *param_2 = 2;
  return param_1;
}

// ====== 00011984: _Unwind_VRS_Interpret ======
void _Unwind_VRS_Interpret(undefined4 param_1,int param_2,uint param_3,uint param_4)

{
  byte bVar1;
  byte bVar2;
  undefined4 *puVar3;
  bool bVar4;
  int iVar5;
  undefined4 uVar6;
  uint uVar7;
  uint uVar8;
  uint uVar9;
  undefined4 *puVar10;
  bool bVar11;
  undefined4 *local_2c;
  int local_28;
  
  puVar10 = (undefined4 *)(DAT_00011fc0 + 0x1199a);
  local_28 = *(int *)*puVar10;
  if (param_3 < param_4) {
    bVar4 = false;
LAB_00011f34:
    uVar8 = param_3 + 1;
    bVar2 = *(byte *)(param_2 + (param_3 ^ 3));
    uVar9 = (uint)bVar2;
    uVar7 = (uint)(char)bVar2;
    if (-1 < (int)uVar7) {
      unw_get_reg(param_1,0xd,&local_2c);
      iVar5 = (uVar9 & 0x3f) * -4 + -4;
      if ((bVar2 & 0x40) == 0) {
        iVar5 = uVar9 * 4 + 4;
      }
      local_2c = (undefined4 *)((int)local_2c + iVar5);
LAB_00011f76:
      unw_set_reg(param_1,0xd,local_2c);
      param_3 = uVar8;
      goto joined_r0x00011e38;
    }
    switch((uVar9 & 0xf0) - 0x80 >> 4) {
    case 0:
      if (param_4 <= uVar8) break;
      bVar1 = *(byte *)(param_2 + (uVar8 ^ 3));
      uVar8 = (uint)bVar1;
      if ((bVar2 & 0xf) != 0 || uVar8 != 0) {
        if ((int)(uVar9 << 0x1c) < 0) {
          bVar4 = true;
        }
        iVar5 = unw_get_reg(param_1,0xd,&local_2c);
        if (iVar5 == 0) {
          if ((bVar1 & 1) != 0) {
            uVar6 = *local_2c;
            local_2c = local_2c + 1;
            iVar5 = unw_set_reg(param_1,4,uVar6);
            if (iVar5 != 0) goto joined_r0x00011e38;
          }
          if ((int)(uVar8 << 0x1e) < 0) {
            uVar6 = *local_2c;
            local_2c = local_2c + 1;
            iVar5 = unw_set_reg(param_1,5,uVar6);
            if (iVar5 != 0) goto joined_r0x00011e38;
          }
          if ((int)(uVar8 << 0x1d) < 0) {
            uVar6 = *local_2c;
            local_2c = local_2c + 1;
            iVar5 = unw_set_reg(param_1,6,uVar6);
            if (iVar5 != 0) goto joined_r0x00011e38;
          }
          if ((int)(uVar8 << 0x1c) < 0) {
            uVar6 = *local_2c;
            local_2c = local_2c + 1;
            iVar5 = unw_set_reg(param_1,7,uVar6);
            if (iVar5 != 0) goto joined_r0x00011e38;
          }
          if ((int)(uVar8 << 0x1b) < 0) {
            uVar6 = *local_2c;
            local_2c = local_2c + 1;
            iVar5 = unw_set_reg(param_1,8,uVar6);
            if (iVar5 != 0) goto joined_r0x00011e38;
          }
          if ((int)(uVar8 << 0x1a) < 0) {
            uVar6 = *local_2c;
            local_2c = local_2c + 1;
            iVar5 = unw_set_reg(param_1,9,uVar6);
            if (iVar5 != 0) goto joined_r0x00011e38;
          }
          if ((int)(uVar8 << 0x19) < 0) {
            uVar6 = *local_2c;
            local_2c = local_2c + 1;
            iVar5 = unw_set_reg(param_1,10,uVar6);
            if (iVar5 != 0) goto joined_r0x00011e38;
          }
          if ((char)bVar1 < '\0') {
            uVar6 = *local_2c;
            local_2c = local_2c + 1;
            iVar5 = unw_set_reg(param_1,0xb,uVar6);
            if (iVar5 != 0) goto joined_r0x00011e38;
          }
          if ((bVar2 & 1) != 0) {
            uVar6 = *local_2c;
            local_2c = local_2c + 1;
            iVar5 = unw_set_reg(param_1,0xc,uVar6);
            if (iVar5 != 0) goto joined_r0x00011e38;
          }
          if ((int)(uVar9 << 0x1e) < 0) {
            uVar6 = *local_2c;
            local_2c = local_2c + 1;
            iVar5 = unw_set_reg(param_1,0xd,uVar6);
            if (iVar5 != 0) goto joined_r0x00011e38;
            bVar11 = true;
            puVar3 = local_2c;
          }
          else {
            bVar11 = false;
            puVar3 = local_2c;
          }
          local_2c = puVar3;
          if ((int)(uVar9 << 0x1d) < 0) {
            local_2c = puVar3 + 1;
            iVar5 = unw_set_reg(param_1,0xe,*puVar3);
            if (iVar5 != 0) goto joined_r0x00011e38;
          }
          if ((int)(uVar9 << 0x1c) < 0) {
            uVar6 = *local_2c;
            local_2c = local_2c + 1;
            iVar5 = unw_set_reg(param_1,0xf,uVar6);
            if (iVar5 != 0) goto joined_r0x00011e38;
          }
          if (!bVar11) {
            unw_set_reg(param_1,0xd,local_2c);
          }
        }
joined_r0x00011e38:
        param_3 = param_3 + 2;
        goto joined_r0x00011e38;
      }
      uVar6 = 9;
      goto switchD_00011b40_caseD_b4;
    case 1:
      if ((uVar7 & 0xf | 2) != 0xf) {
        unw_get_reg(param_1,uVar7 & 0xf,&local_2c);
        goto LAB_00011f76;
      }
      break;
    case 2:
      iVar5 = unw_get_reg(param_1,0xd,&local_2c);
      param_3 = uVar8;
      if (iVar5 == 0) {
        uVar6 = *local_2c;
        local_2c = local_2c + 1;
        iVar5 = unw_set_reg(param_1,4,uVar6);
        if (iVar5 == 0) {
          uVar7 = (0x20 << (uVar7 & 7)) - 0x10;
          if ((int)(uVar7 * 0x4000000) < 0) {
            uVar6 = *local_2c;
            local_2c = local_2c + 1;
            iVar5 = unw_set_reg(param_1,5,uVar6);
            if (iVar5 != 0) goto joined_r0x00011e38;
          }
          if ((int)(uVar7 * 0x2000000) < 0) {
            uVar6 = *local_2c;
            local_2c = local_2c + 1;
            iVar5 = unw_set_reg(param_1,6,uVar6);
            if (iVar5 != 0) goto joined_r0x00011e38;
          }
          if ((char)uVar7 < '\0') {
            uVar6 = *local_2c;
            local_2c = local_2c + 1;
            iVar5 = unw_set_reg(param_1,7,uVar6);
            if (iVar5 != 0) goto joined_r0x00011e38;
          }
          if ((int)(uVar7 * 0x800000) < 0) {
            uVar6 = *local_2c;
            local_2c = local_2c + 1;
            iVar5 = unw_set_reg(param_1,8,uVar6);
            if (iVar5 != 0) goto joined_r0x00011e38;
          }
          if ((int)(uVar7 * 0x400000) < 0) {
            uVar6 = *local_2c;
            local_2c = local_2c + 1;
            iVar5 = unw_set_reg(param_1,9,uVar6);
            if (iVar5 != 0) goto joined_r0x00011e38;
          }
          if ((int)(uVar7 * 0x200000) < 0) {
            uVar6 = *local_2c;
            local_2c = local_2c + 1;
            iVar5 = unw_set_reg(param_1,10,uVar6);
            if (iVar5 != 0) goto joined_r0x00011e38;
          }
          if ((int)(uVar7 * 0x100000) < 0) {
            uVar6 = *local_2c;
            local_2c = local_2c + 1;
            iVar5 = unw_set_reg(param_1,0xb,uVar6);
            if (iVar5 != 0) goto joined_r0x00011e38;
          }
          if ((int)(uVar7 * 0x80000) < 0) {
            uVar6 = *local_2c;
            local_2c = local_2c + 1;
            iVar5 = unw_set_reg(param_1,0xc,uVar6);
            if (iVar5 != 0) goto joined_r0x00011e38;
          }
          if ((int)(uVar7 * 0x40000) < 0) {
            uVar6 = *local_2c;
            local_2c = local_2c + 1;
            iVar5 = unw_set_reg(param_1,0xd,uVar6);
            if (iVar5 != 0) goto joined_r0x00011e38;
            bVar11 = true;
          }
          else {
            bVar11 = false;
          }
          if ((int)((uVar7 | uVar9 << 0xb) << 0x11) < 0) {
            uVar6 = *local_2c;
            local_2c = local_2c + 1;
            iVar5 = unw_set_reg(param_1,0xe,uVar6);
            if (iVar5 != 0) goto joined_r0x00011e38;
          }
          if (0x7fffffff < (uint)(int)(short)uVar7) {
            uVar6 = *local_2c;
            local_2c = local_2c + 1;
            iVar5 = unw_set_reg(param_1,0xf,uVar6);
            if (iVar5 != 0) goto joined_r0x00011e38;
          }
          if (!bVar11) goto LAB_00011f76;
        }
      }
joined_r0x00011e38:
      if (param_4 <= param_3) goto switchD_00011b40_caseD_b0;
      goto LAB_00011f34;
    case 3:
      uVar6 = 9;
      switch(uVar9) {
      case 0xb0:
        goto switchD_00011b40_caseD_b0;
      case 0xb1:
        if (uVar8 < param_4) {
          bVar2 = *(byte *)(param_2 + (uVar8 ^ 3));
          uVar7 = (uint)bVar2;
          uVar6 = 9;
          if ((uVar7 != 0) && ((bVar2 & 0xf0) == 0)) {
            iVar5 = unw_get_reg(param_1,0xd,&local_2c);
            uVar8 = param_3 + 2;
            param_3 = uVar8;
            if (iVar5 == 0) {
              if ((bVar2 & 1) != 0) {
                uVar6 = *local_2c;
                local_2c = local_2c + 1;
                iVar5 = unw_set_reg(param_1,0,uVar6);
                if (iVar5 != 0) goto joined_r0x00011e38;
              }
              if ((int)(uVar7 << 0x1e) < 0) {
                uVar6 = *local_2c;
                local_2c = local_2c + 1;
                iVar5 = unw_set_reg(param_1,1,uVar6);
                if (iVar5 != 0) goto joined_r0x00011e38;
              }
              if ((int)(uVar7 << 0x1d) < 0) {
                uVar6 = *local_2c;
                local_2c = local_2c + 1;
                iVar5 = unw_set_reg(param_1,2,uVar6);
                if (iVar5 != 0) goto joined_r0x00011e38;
              }
              if ((int)(uVar7 << 0x1c) < 0) {
                uVar6 = *local_2c;
                local_2c = local_2c + 1;
                iVar5 = unw_set_reg(param_1,3,uVar6);
                if (iVar5 != 0) goto joined_r0x00011e38;
              }
              if ((int)(uVar7 << 0x1b) < 0) {
                uVar6 = *local_2c;
                local_2c = local_2c + 1;
                iVar5 = unw_set_reg(param_1,4,uVar6);
                if (iVar5 != 0) goto joined_r0x00011e38;
              }
              if ((int)(uVar7 << 0x1a) < 0) {
                uVar6 = *local_2c;
                local_2c = local_2c + 1;
                iVar5 = unw_set_reg(param_1,5,uVar6);
                if (iVar5 != 0) goto joined_r0x00011e38;
              }
              if ((int)(uVar7 << 0x19) < 0) {
                uVar6 = *local_2c;
                local_2c = local_2c + 1;
                iVar5 = unw_set_reg(param_1,6,uVar6);
                if (iVar5 != 0) goto joined_r0x00011e38;
              }
              if (0x7fffffff < (uint)(int)(char)bVar2) {
                uVar6 = *local_2c;
                local_2c = local_2c + 1;
                iVar5 = unw_set_reg(param_1,7,uVar6);
                if (iVar5 != 0) goto joined_r0x00011e38;
              }
              goto LAB_00011f76;
            }
            goto joined_r0x00011e38;
          }
          goto switchD_00011b40_caseD_b4;
        }
        break;
      case 0xb2:
        if (uVar8 < param_4) {
          uVar9 = 0;
          uVar7 = 0;
          uVar8 = param_3 + 2;
          while (bVar2 = *(byte *)(param_2 + (uVar8 - 1 ^ 3)),
                uVar9 = uVar9 | (bVar2 & 0x7f) << (uVar7 & 0xff), (char)bVar2 < '\0') {
            uVar7 = uVar7 + 7;
            bVar11 = param_4 <= uVar8;
            uVar8 = uVar8 + 1;
            if (bVar11) goto switchD_000119ba_default;
          }
          unw_get_reg(param_1,0xd,&local_2c);
          local_2c = local_2c + uVar9 + 0x81;
          goto LAB_00011f76;
        }
        break;
      case 0xb3:
        uVar6 = 1;
        uVar8 = (uint)*(byte *)(param_2 + (uVar8 ^ 3));
        uVar8 = (uVar8 & 0xf) + 1 | (uVar8 & 0xf0) << 0xc;
LAB_00011e2a:
        _Unwind_VRS_Pop(param_1,1,uVar8,uVar6);
        goto joined_r0x00011e38;
      case 0xb4:
      case 0xb5:
      case 0xb6:
      case 0xb7:
        goto switchD_00011b40_caseD_b4;
      default:
        uVar6 = 1;
LAB_00011dc8:
        _Unwind_VRS_Pop(param_1,1,(uVar7 & 7) + 1 | 0x80000,uVar6);
        param_3 = uVar8;
        goto joined_r0x00011e38;
      }
      break;
    case 4:
      if ((uVar9 & 0xfe) == 200) {
        bVar2 = *(byte *)(param_2 + (uVar8 ^ 3));
        uVar9 = bVar2 & 0xf;
        uVar8 = (uint)(bVar2 >> 4) | (uint)(uVar7 == 0xffffffc8) << 4;
        if (uVar8 + uVar9 < 0x20) {
          uVar6 = 5;
          uVar8 = uVar9 + 1 | uVar8 << 0x10;
          goto LAB_00011e2a;
        }
      }
      break;
    case 5:
      if (-1 < (int)(uVar9 << 0x1c)) {
        uVar6 = 5;
        goto LAB_00011dc8;
      }
    }
switchD_000119ba_default:
    uVar6 = 9;
    goto switchD_00011b40_caseD_b4;
  }
LAB_00011f88:
  unw_get_reg(param_1,0xe,&local_2c);
  unw_set_reg(param_1,0xf,local_2c);
LAB_00011f9c:
  uVar6 = 8;
switchD_00011b40_caseD_b4:
  if (*(int *)*puVar10 != local_28) {
                    /* WARNING: Subroutine does not return */
    __stack_chk_fail(uVar6);
  }
  return;
switchD_00011b40_caseD_b0:
  if (bVar4) goto LAB_00011f9c;
  goto LAB_00011f88;
}

// ====== 00011fc4: _Unwind_VRS_Get ======
undefined4
_Unwind_VRS_Get(undefined4 param_1,int param_2,uint param_3,int param_4,undefined4 param_5)

{
  int iVar1;
  undefined4 uVar2;
  FILE *__stream;
  
  if (param_2 == 1) {
    if (param_4 == 5) {
      if (0x1f < param_3) {
        return 2;
      }
    }
    else {
      if ((param_4 != 1) || (0xf < param_3)) {
        return 2;
      }
      unw_save_vfp_as_X(param_1);
    }
    iVar1 = unw_get_fpreg(param_1,param_3 + 0x100,param_5);
  }
  else {
    if (param_2 != 0) {
      __stream = (FILE *)(*(int *)(DAT_00012054 + 0x1203a) + 0xa8);
      fprintf(__stream,(char *)(DAT_00012058 + 0x1202e),DAT_0001205c + 0x12030,
              DAT_00012060 + 0x12032,0x35f,DAT_00012050 + 0x12034);
      fflush(__stream);
                    /* WARNING: Subroutine does not return */
      abort();
    }
    if (0xf < param_3) {
      return 2;
    }
    if (param_4 != 0) {
      return 2;
    }
    iVar1 = unw_get_reg(param_1,param_3,param_5);
  }
  uVar2 = 0;
  if (iVar1 != 0) {
    uVar2 = 2;
  }
  return uVar2;
}

// ====== 00012064: _Unwind_VRS_Set ======
undefined4
_Unwind_VRS_Set(undefined4 param_1,int param_2,uint param_3,int param_4,undefined4 *param_5)

{
  int iVar1;
  undefined4 uVar2;
  FILE *__stream;
  
  if (param_2 == 1) {
    if (param_4 == 5) {
      if (0x1f < param_3) {
        return 2;
      }
    }
    else {
      if ((param_4 != 1) || (0xf < param_3)) {
        return 2;
      }
      unw_save_vfp_as_X(param_1);
    }
    iVar1 = unw_set_fpreg(param_1,param_3 + 0x100,*param_5,param_5[1]);
  }
  else {
    if (param_2 != 0) {
      __stream = (FILE *)(*(int *)(DAT_000120f4 + 0x120dc) + 0xa8);
      fprintf(__stream,(char *)(DAT_000120f8 + 0x120d0),DAT_000120fc + 0x120d2,
              DAT_00012100 + 0x120d4,0x329,DAT_000120f0 + 0x120d6);
      fflush(__stream);
                    /* WARNING: Subroutine does not return */
      abort();
    }
    if (0xf < param_3) {
      return 2;
    }
    if (param_4 != 0) {
      return 2;
    }
    iVar1 = unw_set_reg(param_1,param_3,*param_5);
  }
  uVar2 = 0;
  if (iVar1 != 0) {
    uVar2 = 2;
  }
  return uVar2;
}

// ====== 00012104: _Unwind_VRS_Pop ======
undefined4 _Unwind_VRS_Pop(undefined4 param_1,int param_2,uint param_3,uint param_4)

{
  bool bVar1;
  int iVar2;
  int *piVar3;
  FILE *__stream;
  uint uVar4;
  undefined4 uVar5;
  uint uVar6;
  undefined4 *puVar7;
  int *local_30;
  int iStack_2c;
  int *local_24;
  int local_20;
  
  puVar7 = (undefined4 *)(DAT_00012394 + 0x1211a);
  local_20 = *(int *)*puVar7;
  if (param_2 == 1) {
    if (((param_4 | 4) != 5) || (iVar2 = unw_get_reg(param_1,0xd,&local_24), iVar2 != 0))
    goto LAB_00012284;
    if ((param_3 & 0xffff) != 0) {
      uVar6 = param_3 >> 0x10;
      uVar4 = uVar6 + (param_3 & 0xffff);
      do {
        local_30 = (int *)*local_24;
        iStack_2c = local_24[1];
        local_24 = local_24 + 2;
        iVar2 = _Unwind_VRS_Set(param_1,1,uVar6,param_4,&local_30);
        if (iVar2 != 0) goto LAB_00012284;
        uVar6 = uVar6 + 1;
      } while (uVar6 < uVar4);
    }
    piVar3 = local_24;
    if (param_4 == 1) {
      local_24 = local_24 + 1;
      piVar3 = local_24;
    }
  }
  else {
    if (param_2 != 0) {
      __stream = (FILE *)(*(int *)(DAT_0001239c + 0x12380) + 0xa8);
      fprintf(__stream,(char *)(DAT_000123a0 + 0x12374),DAT_000123a4 + 0x12376,
              DAT_000123a8 + 0x12378,0x3b9,DAT_00012398 + 0x1237a);
      fflush(__stream);
                    /* WARNING: Subroutine does not return */
      abort();
    }
    if ((param_4 != 0) || (iVar2 = unw_get_reg(param_1,0xd,&local_30), iVar2 != 0)) {
LAB_00012284:
      uVar5 = 2;
      goto LAB_00012286;
    }
    if ((param_3 & 1) != 0) {
      iVar2 = *local_30;
      local_30 = local_30 + 1;
      iVar2 = unw_set_reg(param_1,0,iVar2);
      if (iVar2 != 0) goto LAB_00012284;
    }
    if ((int)(param_3 << 0x1e) < 0) {
      iVar2 = *local_30;
      local_30 = local_30 + 1;
      iVar2 = unw_set_reg(param_1,1,iVar2);
      if (iVar2 != 0) goto LAB_00012284;
    }
    if ((int)(param_3 << 0x1d) < 0) {
      uVar5 = 2;
      iVar2 = *local_30;
      local_30 = local_30 + 1;
      iVar2 = unw_set_reg(param_1,2,iVar2);
      if (iVar2 != 0) goto LAB_00012286;
    }
    if ((int)(param_3 << 0x1c) < 0) {
      iVar2 = *local_30;
      local_30 = local_30 + 1;
      iVar2 = unw_set_reg(param_1,3,iVar2);
      if (iVar2 != 0) goto LAB_00012284;
    }
    if ((int)(param_3 << 0x1b) < 0) {
      iVar2 = *local_30;
      local_30 = local_30 + 1;
      iVar2 = unw_set_reg(param_1,4,iVar2);
      if (iVar2 != 0) goto LAB_00012284;
    }
    if ((int)(param_3 << 0x1a) < 0) {
      iVar2 = *local_30;
      local_30 = local_30 + 1;
      iVar2 = unw_set_reg(param_1,5,iVar2);
      if (iVar2 != 0) goto LAB_00012284;
    }
    if ((int)(param_3 << 0x19) < 0) {
      iVar2 = *local_30;
      local_30 = local_30 + 1;
      iVar2 = unw_set_reg(param_1,6,iVar2);
      if (iVar2 != 0) goto LAB_00012284;
    }
    if ((char)param_3 < '\0') {
      iVar2 = *local_30;
      local_30 = local_30 + 1;
      iVar2 = unw_set_reg(param_1,7,iVar2);
      if (iVar2 != 0) goto LAB_00012284;
    }
    if ((int)(param_3 << 0x17) < 0) {
      iVar2 = *local_30;
      local_30 = local_30 + 1;
      iVar2 = unw_set_reg(param_1,8,iVar2);
      if (iVar2 != 0) goto LAB_00012284;
    }
    if ((int)(param_3 << 0x16) < 0) {
      iVar2 = *local_30;
      local_30 = local_30 + 1;
      iVar2 = unw_set_reg(param_1,9,iVar2);
      if (iVar2 != 0) goto LAB_00012284;
    }
    if ((int)(param_3 << 0x15) < 0) {
      iVar2 = *local_30;
      local_30 = local_30 + 1;
      iVar2 = unw_set_reg(param_1,10,iVar2);
      if (iVar2 != 0) goto LAB_00012284;
    }
    if ((int)(param_3 << 0x14) < 0) {
      iVar2 = *local_30;
      local_30 = local_30 + 1;
      iVar2 = unw_set_reg(param_1,0xb,iVar2);
      if (iVar2 != 0) goto LAB_00012284;
    }
    if ((int)(param_3 << 0x13) < 0) {
      iVar2 = *local_30;
      local_30 = local_30 + 1;
      iVar2 = unw_set_reg(param_1,0xc,iVar2);
      if (iVar2 != 0) goto LAB_00012284;
    }
    if ((int)(param_3 << 0x12) < 0) {
      iVar2 = *local_30;
      local_30 = local_30 + 1;
      iVar2 = unw_set_reg(param_1,0xd,iVar2);
      if (iVar2 != 0) goto LAB_00012284;
      bVar1 = true;
      piVar3 = local_30;
    }
    else {
      bVar1 = false;
      piVar3 = local_30;
    }
    local_30 = piVar3;
    if ((int)(param_3 << 0x11) < 0) {
      local_30 = piVar3 + 1;
      iVar2 = unw_set_reg(param_1,0xe,*piVar3);
      if (iVar2 != 0) goto LAB_00012284;
    }
    if ((short)param_3 < 0) {
      iVar2 = *local_30;
      local_30 = local_30 + 1;
      iVar2 = unw_set_reg(param_1,0xf,iVar2);
      if (iVar2 != 0) goto LAB_00012284;
    }
    piVar3 = local_30;
    if (bVar1) {
      uVar5 = 0;
      goto LAB_00012286;
    }
  }
  iVar2 = unw_set_reg(param_1,0xd,piVar3);
  uVar5 = 0;
  if (iVar2 != 0) {
    uVar5 = 2;
  }
LAB_00012286:
  if (*(int *)*puVar7 == local_20) {
    return uVar5;
  }
                    /* WARNING: Subroutine does not return */
  __stack_chk_fail();
}

// ====== 000123ac: __aeabi_unwind_cpp_pr0 ======
void __aeabi_unwind_cpp_pr0(undefined4 param_1,int param_2,undefined4 param_3)

{
  undefined4 uVar1;
  int iVar2;
  undefined4 *puVar3;
  undefined1 auStack_3c [8];
  int *local_34;
  int local_14;
  
  puVar3 = (undefined4 *)(DAT_000123fc + 0x123ba);
  local_14 = *(int *)*puVar3;
  unw_get_proc_info(param_3,auStack_3c);
  if (((*(byte *)(param_2 + 0x50) & 1) == 0) && (*local_34 != 0)) {
    uVar1 = 9;
  }
  else {
    iVar2 = unw_step(param_3);
    uVar1 = 9;
    if (iVar2 == 1) {
      uVar1 = 8;
    }
  }
  if (*(int *)*puVar3 == local_14) {
    return;
  }
                    /* WARNING: Subroutine does not return */
  __stack_chk_fail(uVar1);
}

// ====== 00012400: __aeabi_unwind_cpp_pr1 ======
void __aeabi_unwind_cpp_pr1(undefined4 param_1,int param_2,undefined4 param_3)

{
  undefined4 uVar1;
  int iVar2;
  undefined4 *puVar3;
  undefined1 auStack_3c [8];
  int *local_34;
  int local_14;
  
  puVar3 = (undefined4 *)(DAT_00012450 + 0x1240e);
  local_14 = *(int *)*puVar3;
  unw_get_proc_info(param_3,auStack_3c);
  if (((*(byte *)(param_2 + 0x50) & 1) == 0) && (*local_34 != 0)) {
    uVar1 = 9;
  }
  else {
    iVar2 = unw_step(param_3);
    uVar1 = 9;
    if (iVar2 == 1) {
      uVar1 = 8;
    }
  }
  if (*(int *)*puVar3 == local_14) {
    return;
  }
                    /* WARNING: Subroutine does not return */
  __stack_chk_fail(uVar1);
}

// ====== 00012454: __aeabi_unwind_cpp_pr2 ======
void __aeabi_unwind_cpp_pr2(undefined4 param_1,int param_2,undefined4 param_3)

{
  undefined4 uVar1;
  int iVar2;
  undefined4 *puVar3;
  undefined1 auStack_3c [8];
  int *local_34;
  int local_14;
  
  puVar3 = (undefined4 *)(DAT_000124a4 + 0x12462);
  local_14 = *(int *)*puVar3;
  unw_get_proc_info(param_3,auStack_3c);
  if (((*(byte *)(param_2 + 0x50) & 1) == 0) && (*local_34 != 0)) {
    uVar1 = 9;
  }
  else {
    iVar2 = unw_step(param_3);
    uVar1 = 9;
    if (iVar2 == 1) {
      uVar1 = 8;
    }
  }
  if (*(int *)*puVar3 == local_14) {
    return;
  }
                    /* WARNING: Subroutine does not return */
  __stack_chk_fail(uVar1);
}

// ====== 000124a8: _Unwind_RaiseException ======
void _Unwind_RaiseException(_Unwind_Control_Block *param_1)

{
  int iVar1;
  undefined4 uVar2;
  undefined4 *puVar3;
  undefined4 local_8a0 [3];
  code *local_894;
  undefined4 local_88c;
  undefined4 local_880;
  unw_cursor_t auStack_878 [1120];
  unw_context_t auStack_418 [1024];
  int local_18;
  
  puVar3 = (undefined4 *)(DAT_00012544 + 0x124bc);
  local_18 = *(int *)*puVar3;
  unw_getcontext(auStack_418);
  *(undefined4 *)(param_1 + 0xc) = 0;
  unw_init_local(auStack_878,auStack_418);
  iVar1 = unw_get_proc_info(auStack_878,local_8a0);
  if (iVar1 == 0) {
    do {
      if (local_894 != (code *)0x0) {
        *(undefined4 *)(param_1 + 0x48) = local_8a0[0];
        *(undefined4 *)(param_1 + 0x4c) = local_880;
        *(undefined4 *)(param_1 + 0x50) = local_88c;
        iVar1 = (*local_894)(0,param_1,auStack_878);
        if (iVar1 != 8) {
          if (iVar1 == 6) {
            unwind_phase2(auStack_418,auStack_878,param_1,false);
            uVar2 = 2;
            goto LAB_00012510;
          }
          if (iVar1 == 9) {
            uVar2 = 9;
            goto LAB_00012510;
          }
          break;
        }
      }
      iVar1 = unw_get_proc_info(auStack_878,local_8a0);
    } while (iVar1 == 0);
  }
  uVar2 = 3;
LAB_00012510:
  if (*(int *)*puVar3 == local_18) {
    return;
  }
                    /* WARNING: Subroutine does not return */
  __stack_chk_fail(uVar2);
}

// ====== 00012548: unwind_phase2 ======
/* unwind_phase2(unw_context_t*, unw_cursor_t*, _Unwind_Control_Block*, bool) */

void unwind_phase2(unw_context_t *param_1,unw_cursor_t *param_2,_Unwind_Control_Block *param_3,
                  bool param_4)

{
  int iVar1;
  FILE *__stream;
  byte bVar2;
  int iVar3;
  undefined4 *puVar4;
  undefined4 local_58;
  undefined4 local_54 [3];
  code *local_48;
  undefined4 local_40;
  undefined4 local_34;
  int local_2c;
  int local_28;
  
  puVar4 = (undefined4 *)(DAT_0001263c + 0x1255e);
  local_28 = *(int *)*puVar4;
  unw_init_local(param_2,param_1);
  iVar3 = 0;
  do {
    bVar2 = param_4 & 1;
    if ((iVar3 == 1) && (bVar2 != 0)) {
      unw_set_reg(param_2,0xffffffff,*(undefined4 *)(param_3 + 0x10));
      param_4 = false;
    }
    unw_get_reg(param_2,0xfffffffe,&local_2c);
    iVar1 = unw_get_proc_info(param_2,local_54);
    if (iVar1 != 0) goto LAB_00012624;
    if (local_48 != (code *)0x0) {
      *(undefined4 *)(param_3 + 0x48) = local_54[0];
      *(undefined4 *)(param_3 + 0x4c) = local_34;
      *(undefined4 *)(param_3 + 0x50) = local_40;
      iVar1 = (*local_48)(bVar2 + 1,param_3,param_2);
      if (iVar1 == 7) {
        unw_get_reg(param_2,0xffffffff,&local_58);
        *(undefined4 *)(param_3 + 0x10) = local_58;
        unw_resume(param_2);
LAB_00012624:
        if (*(int *)*puVar4 == local_28) {
          return;
        }
                    /* WARNING: Subroutine does not return */
        __stack_chk_fail();
      }
      if (iVar1 != 8) {
        if (iVar1 == 9) {
                    /* WARNING: Subroutine does not return */
          abort();
        }
        goto LAB_00012624;
      }
      if (local_2c == *(int *)(param_3 + 0x20)) {
        __stream = (FILE *)(*(int *)(DAT_00012644 + 0x125f2) + 0xa8);
        fprintf(__stream,(char *)(DAT_00012648 + 0x125e6),DAT_0001264c + 0x125e8,
                DAT_00012650 + 0x125ea,0x269,DAT_00012640 + 0x125ec);
        fflush(__stream);
                    /* WARNING: Subroutine does not return */
        abort();
      }
    }
    iVar3 = iVar3 + 1;
  } while( true );
}

// ====== 00012654: _Unwind_Resume ======
void _Unwind_Resume(_Unwind_Control_Block *param_1)

{
  FILE *__stream;
  unw_cursor_t auStack_870 [1120];
  unw_context_t auStack_410 [1024];
  
  unw_getcontext(auStack_410);
  unwind_phase2(auStack_410,auStack_870,param_1,true);
  __stream = (FILE *)(*(int *)(DAT_000126ac + 0x12690) + 0xa8);
  fprintf(__stream,(char *)(DAT_000126b0 + 0x12692),DAT_000126b4 + 0x12684,DAT_000126b8 + 0x12686,
          0x2cf,DAT_000126a8 + 0x12682);
  fflush(__stream);
                    /* WARNING: Subroutine does not return */
  abort();
}

// ====== 000126bc: _Unwind_GetLanguageSpecificData ======
void _Unwind_GetLanguageSpecificData(undefined4 param_1)

{
  int iVar1;
  undefined4 uVar2;
  undefined4 *puVar3;
  undefined1 auStack_34 [8];
  undefined4 local_2c;
  int local_c;
  
  puVar3 = (undefined4 *)(DAT_000126f0 + 0x126c6);
  local_c = *(int *)*puVar3;
  iVar1 = unw_get_proc_info(param_1,auStack_34);
  uVar2 = 0;
  if (iVar1 == 0) {
    uVar2 = local_2c;
  }
  if (*(int *)*puVar3 == local_c) {
    return;
  }
                    /* WARNING: Subroutine does not return */
  __stack_chk_fail(uVar2);
}

// ====== 000126f4: _Unwind_GetRegionStart ======
void _Unwind_GetRegionStart(undefined4 param_1)

{
  int iVar1;
  undefined4 uVar2;
  undefined4 *puVar3;
  undefined4 local_34 [10];
  int local_c;
  
  puVar3 = (undefined4 *)(DAT_00012728 + 0x126fe);
  local_c = *(int *)*puVar3;
  iVar1 = unw_get_proc_info(param_1,local_34);
  uVar2 = 0;
  if (iVar1 == 0) {
    uVar2 = local_34[0];
  }
  if (*(int *)*puVar3 == local_c) {
    return;
  }
                    /* WARNING: Subroutine does not return */
  __stack_chk_fail(uVar2);
}

// ====== 0001272c: _Unwind_DeleteException ======
void _Unwind_DeleteException(int param_1)

{
  if (*(code **)(param_1 + 8) != (code *)0x0) {
                    /* WARNING: Could not recover jumptable at 0x00012734. Too many branches */
                    /* WARNING: Treating indirect jump as call */
    (**(code **)(param_1 + 8))(1);
    return;
  }
  return;
}

// ====== 00012738: __gnu_unwind_frame ======
undefined4 __gnu_unwind_frame(undefined4 param_1,undefined4 param_2)

{
  int iVar1;
  undefined4 uVar2;
  
  iVar1 = unw_step(param_2);
  uVar2 = 9;
  if (iVar1 == 1) {
    uVar2 = 0;
  }
  return uVar2;
}

// ====== 0001274c: unw_getcontext ======
undefined4 unw_getcontext(int param_1,undefined4 param_2,undefined4 param_3,undefined4 param_4)

{
  undefined4 unaff_r4;
  undefined4 unaff_r5;
  undefined4 unaff_r6;
  undefined4 unaff_r7;
  undefined4 unaff_r8;
  undefined4 unaff_r9;
  undefined4 unaff_r10;
  undefined4 unaff_r11;
  undefined4 in_r12;
  undefined4 unaff_lr;
  
  *(int *)param_1 = param_1;
  *(undefined4 *)(param_1 + 4) = param_2;
  *(undefined4 *)(param_1 + 8) = param_3;
  *(undefined4 *)(param_1 + 0xc) = param_4;
  *(undefined4 *)(param_1 + 0x10) = unaff_r4;
  *(undefined4 *)(param_1 + 0x14) = unaff_r5;
  *(undefined4 *)(param_1 + 0x18) = unaff_r6;
  *(undefined4 *)(param_1 + 0x1c) = unaff_r7;
  *(undefined4 *)(param_1 + 0x20) = unaff_r8;
  *(undefined4 *)(param_1 + 0x24) = unaff_r9;
  *(undefined4 *)(param_1 + 0x28) = unaff_r10;
  *(undefined4 *)(param_1 + 0x2c) = unaff_r11;
  *(undefined4 *)(param_1 + 0x30) = in_r12;
  *(BADSPACEBASE **)(param_1 + 0x34) = register0x00000054;
  *(undefined4 *)(param_1 + 0x38) = unaff_lr;
  *(undefined4 *)(param_1 + 0x3c) = unaff_lr;
  return 0;
}

// ====== 00012764: saveVFPWithFSTMD ======
/* libunwind::Registers_arm::saveVFPWithFSTMD(unsigned long long*) */

void libunwind::Registers_arm::saveVFPWithFSTMD(ulonglong *param_1)

{
  ulonglong in_d0;
  ulonglong in_d1;
  ulonglong in_d2;
  ulonglong in_d3;
  ulonglong in_d4;
  ulonglong in_d5;
  ulonglong in_d6;
  ulonglong in_d7;
  ulonglong unaff_d8;
  ulonglong unaff_d9;
  ulonglong unaff_d10;
  ulonglong unaff_d11;
  ulonglong unaff_d12;
  ulonglong unaff_d13;
  ulonglong unaff_d14;
  ulonglong unaff_d15;
  
  *param_1 = in_d0;
  param_1[1] = in_d1;
  param_1[2] = in_d2;
  param_1[3] = in_d3;
  param_1[4] = in_d4;
  param_1[5] = in_d5;
  param_1[6] = in_d6;
  param_1[7] = in_d7;
  param_1[8] = unaff_d8;
  param_1[9] = unaff_d9;
  param_1[10] = unaff_d10;
  param_1[0xb] = unaff_d11;
  param_1[0xc] = unaff_d12;
  param_1[0xd] = unaff_d13;
  param_1[0xe] = unaff_d14;
  param_1[0xf] = unaff_d15;
  return;
}

// ====== 0001276c: saveVFPWithFSTMX ======
/* libunwind::Registers_arm::saveVFPWithFSTMX(unsigned long long*) */

void libunwind::Registers_arm::saveVFPWithFSTMX(ulonglong *param_1)

{
  ulonglong in_d0;
  ulonglong in_d1;
  ulonglong in_d2;
  ulonglong in_d3;
  ulonglong in_d4;
  ulonglong in_d5;
  ulonglong in_d6;
  ulonglong in_d7;
  ulonglong unaff_d8;
  ulonglong unaff_d9;
  ulonglong unaff_d10;
  ulonglong unaff_d11;
  ulonglong unaff_d12;
  ulonglong unaff_d13;
  ulonglong unaff_d14;
  ulonglong unaff_d15;
  
  *param_1 = in_d0;
  param_1[1] = in_d1;
  param_1[2] = in_d2;
  param_1[3] = in_d3;
  param_1[4] = in_d4;
  param_1[5] = in_d5;
  param_1[6] = in_d6;
  param_1[7] = in_d7;
  param_1[8] = unaff_d8;
  param_1[9] = unaff_d9;
  param_1[10] = unaff_d10;
  param_1[0xb] = unaff_d11;
  param_1[0xc] = unaff_d12;
  param_1[0xd] = unaff_d13;
  param_1[0xe] = unaff_d14;
  param_1[0xf] = unaff_d15;
  return;
}

// ====== 00012774: saveVFPv3 ======
/* libunwind::Registers_arm::saveVFPv3(unsigned long long*) */

void libunwind::Registers_arm::saveVFPv3(ulonglong *param_1)

{
  ulonglong in_d16;
  ulonglong in_d17;
  ulonglong in_d18;
  ulonglong in_d19;
  ulonglong in_d20;
  ulonglong in_d21;
  ulonglong in_d22;
  ulonglong in_d23;
  ulonglong in_d24;
  ulonglong in_d25;
  ulonglong in_d26;
  ulonglong in_d27;
  ulonglong in_d28;
  ulonglong in_d29;
  ulonglong in_d30;
  ulonglong in_d31;
  
  *param_1 = in_d16;
  param_1[1] = in_d17;
  param_1[2] = in_d18;
  param_1[3] = in_d19;
  param_1[4] = in_d20;
  param_1[5] = in_d21;
  param_1[6] = in_d22;
  param_1[7] = in_d23;
  param_1[8] = in_d24;
  param_1[9] = in_d25;
  param_1[10] = in_d26;
  param_1[0xb] = in_d27;
  param_1[0xc] = in_d28;
  param_1[0xd] = in_d29;
  param_1[0xe] = in_d30;
  param_1[0xf] = in_d31;
  return;
}

// ====== 0001277c: unw_init_local ======
undefined4 unw_init_local(int *param_1,int *param_2)

{
  int iVar1;
  int iVar2;
  int iVar3;
  int iVar4;
  undefined8 uVar5;
  undefined8 uVar6;
  undefined8 uVar7;
  undefined8 uVar8;
  undefined8 uVar9;
  
  iVar2 = DAT_00012850;
  iVar1 = DAT_0001284c;
  *(undefined2 *)(param_1 + 0x12) = 0;
  *(undefined1 *)((int)param_1 + 0x4a) = 0;
  *param_1 = iVar2 + 0x1279e;
  param_1[1] = iVar1 + 0x12798;
  iVar1 = *param_2;
  iVar2 = param_2[1];
  iVar3 = param_2[2];
  iVar4 = param_2[3];
  uVar5 = *(undefined8 *)(param_2 + 10);
  uVar6 = *(undefined8 *)(param_2 + 4);
  uVar7 = *(undefined8 *)(param_2 + 6);
  uVar8 = *(undefined8 *)(param_2 + 0xc);
  uVar9 = *(undefined8 *)(param_2 + 0xe);
  *(undefined8 *)(param_1 + 10) = *(undefined8 *)(param_2 + 8);
  *(undefined8 *)(param_1 + 0xc) = uVar5;
  *(undefined8 *)(param_1 + 6) = uVar6;
  *(undefined8 *)(param_1 + 8) = uVar7;
  param_1[2] = iVar1;
  param_1[3] = iVar2;
  param_1[4] = iVar3;
  param_1[5] = iVar4;
  *(undefined8 *)(param_1 + 0xe) = uVar8;
  *(undefined8 *)(param_1 + 0x10) = uVar9;
  __aeabi_memclr(param_1 + 0x14,0x88);
  param_1[0x52] = 0;
  param_1[0x53] = 0;
  param_1[0x54] = 0;
  param_1[0x55] = 0;
  param_1[0x4e] = 0;
  param_1[0x4f] = 0;
  param_1[0x50] = 0;
  param_1[0x51] = 0;
  param_1[0x4a] = 0;
  param_1[0x4b] = 0;
  param_1[0x4c] = 0;
  param_1[0x4d] = 0;
  param_1[0x46] = 0;
  param_1[0x47] = 0;
  param_1[0x48] = 0;
  param_1[0x49] = 0;
  param_1[0x42] = 0;
  param_1[0x43] = 0;
  param_1[0x44] = 0;
  param_1[0x45] = 0;
  param_1[0x3e] = 0;
  param_1[0x3f] = 0;
  param_1[0x40] = 0;
  param_1[0x41] = 0;
  param_1[0x3a] = 0;
  param_1[0x3b] = 0;
  param_1[0x3c] = 0;
  param_1[0x3d] = 0;
  param_1[0x36] = 0;
  param_1[0x37] = 0;
  param_1[0x38] = 0;
  param_1[0x39] = 0;
  *(undefined2 *)(param_1 + 0x60) = 0;
  param_1[0x5f] = 0;
  param_1[0x5e] = 0;
  param_1[0x5a] = 0;
  param_1[0x5b] = 0;
  param_1[0x5c] = 0;
  param_1[0x5d] = 0;
  param_1[0x56] = 0;
  param_1[0x57] = 0;
  param_1[0x58] = 0;
  param_1[0x59] = 0;
  (**(code **)(*param_1 + 0x34))(param_1,0);
  return 0;
}

// ====== 00012854: unw_get_reg ======
undefined4 unw_get_reg(int *param_1,undefined4 param_2,undefined4 *param_3)

{
  int iVar1;
  undefined4 uVar2;
  
  iVar1 = (**(code **)(*param_1 + 8))(param_1);
  if (iVar1 != 0) {
    uVar2 = (**(code **)(*param_1 + 0xc))(param_1,param_2);
    *param_3 = uVar2;
    return 0;
  }
  return 0xffffe672;
}

// ====== 00012880: unw_set_reg ======
undefined4 unw_set_reg(int *param_1,int param_2,undefined4 param_3)

{
  int iVar1;
  
  iVar1 = (**(code **)(*param_1 + 8))(param_1);
  if (iVar1 == 0) {
    return 0xffffe672;
  }
  (**(code **)(*param_1 + 0x10))(param_1,param_2,param_3);
  if (param_2 != -1) {
    return 0;
  }
  (**(code **)(*param_1 + 0x34))(param_1,0);
  return 0;
}

// ====== 000128c4: unw_get_fpreg ======
undefined4 unw_get_fpreg(int *param_1,undefined4 param_2,undefined8 *param_3)

{
  int iVar1;
  undefined8 uVar2;
  
  iVar1 = (**(code **)(*param_1 + 0x14))(param_1);
  if (iVar1 != 0) {
    uVar2 = (**(code **)(*param_1 + 0x18))(param_1,param_2);
    *param_3 = uVar2;
    return 0;
  }
  return 0xffffe672;
}

// ====== 000128f2: unw_set_fpreg ======
undefined4 unw_set_fpreg(int *param_1,undefined4 param_2,undefined4 param_3,undefined4 param_4)

{
  int iVar1;
  
  iVar1 = (**(code **)(*param_1 + 0x14))(param_1);
  if (iVar1 != 0) {
    (**(code **)(*param_1 + 0x1c))(param_1,param_2,param_3,param_4);
    return 0;
  }
  return 0xffffe672;
}

// ====== 00012928: unw_step ======
void unw_step(int *param_1)

{
                    /* WARNING: Could not recover jumptable at 0x0001292c. Too many branches */
                    /* WARNING: Treating indirect jump as call */
  (**(code **)(*param_1 + 0x20))();
  return;
}

// ====== 0001292e: unw_get_proc_info ======
undefined4 unw_get_proc_info(int *param_1,int param_2)

{
  undefined4 uVar1;
  
  (**(code **)(*param_1 + 0x24))(param_1,param_2);
  uVar1 = 0;
  if (*(int *)(param_2 + 4) == 0) {
    uVar1 = 0xffffe66b;
  }
  return uVar1;
}

// ====== 0001294e: unw_resume ======
undefined4 unw_resume(int *param_1)

{
  (**(code **)(*param_1 + 0x28))();
  return 0xffffe674;
}

// ====== 00012960: unw_save_vfp_as_X ======
void unw_save_vfp_as_X(int *param_1)

{
                    /* WARNING: Could not recover jumptable at 0x00012964. Too many branches */
                    /* WARNING: Treating indirect jump as call */
  (**(code **)(*param_1 + 0x3c))();
  return;
}

// ====== 00012966: validReg ======
/* libunwind::UnwindCursor<libunwind::LocalAddressSpace, libunwind::Registers_arm>::validReg(int) */

bool __thiscall
libunwind::UnwindCursor<libunwind::LocalAddressSpace,libunwind::Registers_arm>::validReg
          (UnwindCursor<libunwind::LocalAddressSpace,libunwind::Registers_arm> *this,int param_1)

{
  return 0xffffffed < param_1 - 0x10U;
}

// ====== 00012974: getReg ======
/* libunwind::UnwindCursor<libunwind::LocalAddressSpace, libunwind::Registers_arm>::getReg(int) */

undefined4 __thiscall
libunwind::UnwindCursor<libunwind::LocalAddressSpace,libunwind::Registers_arm>::getReg
          (UnwindCursor<libunwind::LocalAddressSpace,libunwind::Registers_arm> *this,int param_1)

{
  FILE *__stream;
  
  switch(param_1) {
  case 0xe:
    return *(undefined4 *)(this + 0x40);
  case -2:
  case 0xd:
    return *(undefined4 *)(this + 0x3c);
  case -1:
  case 0xf:
    return *(undefined4 *)(this + 0x44);
  }
  if ((uint)param_1 < 0xd) {
    return *(undefined4 *)(this + param_1 * 4 + 8);
  }
  __stream = (FILE *)(*(int *)(DAT_000129ec + 0x129d2) + 0xa8);
  fprintf(__stream,(char *)(DAT_000129f0 + 0x129c6),DAT_000129f4 + 0x129c8,DAT_000129f8 + 0x129ca,
          0x618,DAT_000129e8 + 0x129cc);
  fflush(__stream);
                    /* WARNING: Subroutine does not return */
  abort();
}

// ====== 000129fc: setReg ======
/* libunwind::UnwindCursor<libunwind::LocalAddressSpace, libunwind::Registers_arm>::setReg(int,
   unsigned int) */

void __thiscall
libunwind::UnwindCursor<libunwind::LocalAddressSpace,libunwind::Registers_arm>::setReg
          (UnwindCursor<libunwind::LocalAddressSpace,libunwind::Registers_arm> *this,int param_1,
          uint param_2)

{
  FILE *__stream;
  
  switch(param_1) {
  case 0xe:
    *(uint *)(this + 0x40) = param_2;
    return;
  case -2:
  case 0xd:
    *(uint *)(this + 0x3c) = param_2;
    return;
  case -1:
  case 0xf:
    *(uint *)(this + 0x44) = param_2;
    return;
  }
  if ((uint)param_1 < 0xd) {
    *(uint *)(this + param_1 * 4 + 8) = param_2;
    return;
  }
  __stream = (FILE *)(*(int *)(DAT_00012a74 + 0x12a5a) + 0xa8);
  fprintf(__stream,(char *)(DAT_00012a78 + 0x12a4e),DAT_00012a7c + 0x12a50,DAT_00012a80 + 0x12a52,
          0x63b,DAT_00012a70 + 0x12a54);
  fflush(__stream);
                    /* WARNING: Subroutine does not return */
  abort();
}

// ====== 00012a84: validFloatReg ======
/* libunwind::UnwindCursor<libunwind::LocalAddressSpace,
   libunwind::Registers_arm>::validFloatReg(int) */

bool __thiscall
libunwind::UnwindCursor<libunwind::LocalAddressSpace,libunwind::Registers_arm>::validFloatReg
          (UnwindCursor<libunwind::LocalAddressSpace,libunwind::Registers_arm> *this,int param_1)

{
  return (param_1 & 0xffffffe0U) == 0x100;
}

// ====== 00012a94: getFloatReg ======
/* libunwind::UnwindCursor<libunwind::LocalAddressSpace, libunwind::Registers_arm>::getFloatReg(int)
    */

undefined8 __thiscall
libunwind::UnwindCursor<libunwind::LocalAddressSpace,libunwind::Registers_arm>::getFloatReg
          (UnwindCursor<libunwind::LocalAddressSpace,libunwind::Registers_arm> *this,int param_1)

{
  int iVar1;
  FILE *__stream;
  
  if ((param_1 & 0xfffffff0U) == 0x110) {
    if (this[0x4a] == (UnwindCursor<libunwind::LocalAddressSpace,libunwind::Registers_arm>)0x0) {
      this[0x4a] = (UnwindCursor<libunwind::LocalAddressSpace,libunwind::Registers_arm>)0x1;
      Registers_arm::saveVFPv3((ulonglong *)(this + 0xd8));
    }
    iVar1 = param_1 * 8 + -0x7a8;
  }
  else {
    if ((param_1 & 0xfffffff0U) != 0x100) {
      __stream = (FILE *)(*(int *)(DAT_00012b30 + 0x12b18) + 0xa8);
      fprintf(__stream,(char *)(DAT_00012b34 + 0x12b0c),DAT_00012b38 + 0x12b0e,
              DAT_00012b3c + 0x12b10,0x70f,DAT_00012b2c + 0x12b12);
      fflush(__stream);
                    /* WARNING: Subroutine does not return */
      abort();
    }
    if (this[0x49] == (UnwindCursor<libunwind::LocalAddressSpace,libunwind::Registers_arm>)0x0) {
      this[0x49] = (UnwindCursor<libunwind::LocalAddressSpace,libunwind::Registers_arm>)0x1;
      if (this[0x48] == (UnwindCursor<libunwind::LocalAddressSpace,libunwind::Registers_arm>)0x0) {
        Registers_arm::saveVFPWithFSTMD((ulonglong *)(this + 0x50));
      }
      else {
        Registers_arm::saveVFPWithFSTMX((ulonglong *)(this + 0x50));
      }
    }
    iVar1 = param_1 * 8 + -0x7b0;
  }
  return *(undefined8 *)(this + iVar1);
}

// ====== 00012b40: setFloatReg ======
/* libunwind::UnwindCursor<libunwind::LocalAddressSpace, libunwind::Registers_arm>::setFloatReg(int,
   unsigned long long) */

void libunwind::UnwindCursor<libunwind::LocalAddressSpace,libunwind::Registers_arm>::setFloatReg
               (int param_1,ulonglong param_2)

{
  int iVar1;
  uint in_r1;
  uint uVar2;
  undefined4 uVar3;
  FILE *__stream;
  
  uVar3 = (undefined4)(param_2 >> 0x20);
  if ((in_r1 & 0xfffffff0) == 0x110) {
    if (*(char *)(param_1 + 0x4a) == '\0') {
      *(undefined1 *)(param_1 + 0x4a) = 1;
      Registers_arm::saveVFPv3((ulonglong *)(param_1 + 0xd8));
    }
    iVar1 = param_1 + in_r1 * 8;
    *(undefined4 *)(iVar1 + -0x7a4) = uVar3;
    uVar2 = 0xf858;
  }
  else {
    if ((in_r1 & 0xfffffff0) != 0x100) {
      __stream = (FILE *)(*(int *)(DAT_00012bf8 + 0x12bde) + 0xa8);
      fprintf(__stream,(char *)(DAT_00012bfc + 0x12bd2),DAT_00012c00 + 0x12bd4,
              DAT_00012c04 + 0x12bd6,0x733,DAT_00012bf4 + 0x12bd8);
      fflush(__stream);
                    /* WARNING: Subroutine does not return */
      abort();
    }
    if (*(char *)(param_1 + 0x49) == '\0') {
      *(undefined1 *)(param_1 + 0x49) = 1;
      if (*(char *)(param_1 + 0x48) == '\0') {
        Registers_arm::saveVFPWithFSTMD((ulonglong *)(param_1 + 0x50));
      }
      else {
        Registers_arm::saveVFPWithFSTMX((ulonglong *)(param_1 + 0x50));
      }
    }
    iVar1 = param_1 + in_r1 * 8;
    *(undefined4 *)(iVar1 + -0x7ac) = uVar3;
    uVar2 = 0xf850;
  }
  *(int *)(iVar1 + (uVar2 | 0xffff0000)) = (int)param_2;
  return;
}

// ====== 00012c08: step ======
/* libunwind::UnwindCursor<libunwind::LocalAddressSpace, libunwind::Registers_arm>::step() */

undefined4 __thiscall
libunwind::UnwindCursor<libunwind::LocalAddressSpace,libunwind::Registers_arm>::step
          (UnwindCursor<libunwind::LocalAddressSpace,libunwind::Registers_arm> *this)

{
  undefined4 uVar1;
  int iVar2;
  undefined4 uVar3;
  code *pcVar4;
  undefined4 *puVar5;
  undefined4 local_1c;
  undefined4 uStack_18;
  int local_14;
  
  uVar3 = 0;
  puVar5 = (undefined4 *)(DAT_00012c8c + 0x12c16);
  local_14 = *(int *)*puVar5;
  if (this[0x180] == (UnwindCursor<libunwind::LocalAddressSpace,libunwind::Registers_arm>)0x0) {
    local_1c = 0;
    uStack_18 = 0;
    uVar1 = decode_eht_entry(*(undefined4 *)(this + 0x178),&local_1c,&uStack_18);
    iVar2 = _Unwind_VRS_Interpret(this,uVar1,local_1c,uStack_18);
    if (iVar2 == 8) {
      (**(code **)(*(int *)this + 0x34))(this,1);
      if (this[0x180] == (UnwindCursor<libunwind::LocalAddressSpace,libunwind::Registers_arm>)0x0) {
        if (*(int *)(this + 0x168) != 0) {
          pcVar4 = *(code **)(*(int *)this + 0x10);
          iVar2 = (**(code **)(*(int *)this + 0xc))(this,0xfffffffe);
          (*pcVar4)(this,0xfffffffe,*(int *)(this + 0x168) + iVar2);
        }
        uVar3 = 1;
      }
      else {
        uVar3 = 0;
      }
    }
  }
  if (*(int *)*puVar5 == local_14) {
    return uVar3;
  }
                    /* WARNING: Subroutine does not return */
  __stack_chk_fail();
}

// ====== 00012c90: getInfo ======
/* libunwind::UnwindCursor<libunwind::LocalAddressSpace,
   libunwind::Registers_arm>::getInfo(unw_proc_info_t*) */

void __thiscall
libunwind::UnwindCursor<libunwind::LocalAddressSpace,libunwind::Registers_arm>::getInfo
          (UnwindCursor<libunwind::LocalAddressSpace,libunwind::Registers_arm> *this,
          unw_proc_info_t *param_1)

{
  undefined8 uVar1;
  undefined8 uVar2;
  undefined8 uVar3;
  undefined8 uVar4;
  
  uVar4 = *(undefined8 *)(this + 0x178);
  uVar3 = *(undefined8 *)(this + 0x160);
  uVar1 = *(undefined8 *)(this + 0x168);
  uVar2 = *(undefined8 *)(this + 0x170);
  *(undefined8 *)param_1 = *(undefined8 *)(this + 0x158);
  *(undefined8 *)(param_1 + 8) = uVar3;
  *(undefined8 *)(param_1 + 0x10) = uVar1;
  *(undefined8 *)(param_1 + 0x18) = uVar2;
  *(undefined8 *)(param_1 + 0x20) = uVar4;
  return;
}

// ====== 00012cb8: jumpto ======
/* libunwind::UnwindCursor<libunwind::LocalAddressSpace, libunwind::Registers_arm>::jumpto() */

void __thiscall
libunwind::UnwindCursor<libunwind::LocalAddressSpace,libunwind::Registers_arm>::jumpto
          (UnwindCursor<libunwind::LocalAddressSpace,libunwind::Registers_arm> *this)

{
  if (this[0x49] != (UnwindCursor<libunwind::LocalAddressSpace,libunwind::Registers_arm>)0x0) {
    if (this[0x48] == (UnwindCursor<libunwind::LocalAddressSpace,libunwind::Registers_arm>)0x0) {
      Registers_arm::restoreVFPWithFLDMD((ulonglong *)(this + 0x50));
    }
    else {
      Registers_arm::restoreVFPWithFLDMX((ulonglong *)(this + 0x50));
    }
  }
  if (this[0x4a] != (UnwindCursor<libunwind::LocalAddressSpace,libunwind::Registers_arm>)0x0) {
    Registers_arm::restoreVFPv3((ulonglong *)(this + 0xd8));
  }
                    /* WARNING: Could not recover jumptable at 0x000142b4. Too many branches */
                    /* WARNING: Treating indirect jump as call */
  (*(dladdr + DAT_000142b8))(this + 8);
  return;
}

// ====== 00012cf2: isSignalFrame ======
/* libunwind::UnwindCursor<libunwind::LocalAddressSpace, libunwind::Registers_arm>::isSignalFrame()
    */

UnwindCursor<libunwind::LocalAddressSpace,libunwind::Registers_arm> __thiscall
libunwind::UnwindCursor<libunwind::LocalAddressSpace,libunwind::Registers_arm>::isSignalFrame
          (UnwindCursor<libunwind::LocalAddressSpace,libunwind::Registers_arm> *this)

{
  return this[0x181];
}

// ====== 00012cf8: getFunctionName ======
/* libunwind::UnwindCursor<libunwind::LocalAddressSpace,
   libunwind::Registers_arm>::getFunctionName(char*, unsigned int, unsigned int*) */

void __thiscall
libunwind::UnwindCursor<libunwind::LocalAddressSpace,libunwind::Registers_arm>::getFunctionName
          (UnwindCursor<libunwind::LocalAddressSpace,libunwind::Registers_arm> *this,char *param_1,
          uint param_2,uint *param_3)

{
  int iVar1;
  int iVar2;
  undefined4 uVar3;
  undefined4 *puVar4;
  undefined1 auStack_2c [8];
  int local_24;
  int local_20;
  int local_1c;
  
  puVar4 = (undefined4 *)(DAT_00012d5c + 0x12d0c);
  local_1c = *(int *)*puVar4;
  iVar1 = (**(code **)(*(int *)this + 0xc))(this,0xffffffff);
  iVar2 = dladdr(iVar1,auStack_2c);
  if ((iVar2 == 0) || (local_24 == 0)) {
    uVar3 = 0;
  }
  else {
    snprintf(param_1,0xffffffff,(char *)param_2);
    *param_3 = iVar1 - local_20;
    uVar3 = 1;
  }
  if (*(int *)*puVar4 == local_1c) {
    return;
  }
                    /* WARNING: Subroutine does not return */
  __stack_chk_fail(uVar3);
}

// ====== 00012d60: setInfoBasedOnIPRegister ======
/* libunwind::UnwindCursor<libunwind::LocalAddressSpace,
   libunwind::Registers_arm>::setInfoBasedOnIPRegister(bool) */

void __thiscall
libunwind::UnwindCursor<libunwind::LocalAddressSpace,libunwind::Registers_arm>::
setInfoBasedOnIPRegister
          (UnwindCursor<libunwind::LocalAddressSpace,libunwind::Registers_arm> *this,bool param_1)

{
  uint uVar1;
  int iVar2;
  int iVar3;
  undefined4 *puVar4;
  int local_28 [2];
  undefined4 local_20;
  undefined1 *local_1c;
  uint local_18;
  int local_14;
  
  puVar4 = (undefined4 *)(DAT_00012dcc + 0x12d72);
  local_14 = *(int *)*puVar4;
  uVar1 = (**(code **)(*(int *)this + 0xc))(this,0xffffffff);
  local_20 = *(undefined4 *)(this + 4);
  uVar1 = (uVar1 & 0xfffffffe) - (uint)param_1;
  local_1c = (undefined1 *)local_28;
  local_18 = uVar1;
  iVar2 = dl_iterate_phdr(DAT_00012dd0 + 0x12d96,&local_20);
  iVar3 = 0;
  if (iVar2 != 0) {
    iVar3 = local_28[0];
  }
  if ((iVar2 == 0 || iVar3 == 0) ||
     (iVar3 = getInfoFromEHABISection(this,uVar1,(UnwindInfoSections *)local_28), iVar3 == 0)) {
    this[0x180] = (UnwindCursor<libunwind::LocalAddressSpace,libunwind::Registers_arm>)0x1;
  }
  if (*(int *)*puVar4 == local_14) {
    return;
  }
                    /* WARNING: Subroutine does not return */
  __stack_chk_fail();
}

// ====== 00012dd4: getRegisterName ======
/* libunwind::UnwindCursor<libunwind::LocalAddressSpace,
   libunwind::Registers_arm>::getRegisterName(int) */

void __thiscall
libunwind::UnwindCursor<libunwind::LocalAddressSpace,libunwind::Registers_arm>::getRegisterName
          (UnwindCursor<libunwind::LocalAddressSpace,libunwind::Registers_arm> *this,int param_1)

{
  Registers_arm::getRegisterName((Registers_arm *)(this + 8),param_1);
  return;
}

// ====== 00012dda: saveVFPAsX ======
/* libunwind::UnwindCursor<libunwind::LocalAddressSpace, libunwind::Registers_arm>::saveVFPAsX() */

void __thiscall
libunwind::UnwindCursor<libunwind::LocalAddressSpace,libunwind::Registers_arm>::saveVFPAsX
          (UnwindCursor<libunwind::LocalAddressSpace,libunwind::Registers_arm> *this)

{
  this[0x48] = (UnwindCursor<libunwind::LocalAddressSpace,libunwind::Registers_arm>)0x1;
  return;
}

// ====== 00012de4: snprintf ======
/* snprintf(char*, unsigned int pass_object_size1, char const*, ...) */

void snprintf(char *param_1,uint param_2,char *param_3,...)

{
  int iVar1;
  undefined4 *puVar2;
  
  puVar2 = (undefined4 *)(DAT_00012e1c + 0x12df0);
  iVar1 = *(int *)*puVar2;
  __vsnprintf_chk(param_1,param_3,0,param_2,DAT_00012e20 + 0x12e00,&stack0x00000000,&stack0x00000000
                 );
  if (*(int *)*puVar2 == iVar1) {
    return;
  }
                    /* WARNING: Subroutine does not return */
  __stack_chk_fail();
}

// ====== 00012e24: getInfoFromEHABISection ======
/* libunwind::UnwindCursor<libunwind::LocalAddressSpace,
   libunwind::Registers_arm>::getInfoFromEHABISection(unsigned int, libunwind::UnwindInfoSections
   const&) */

undefined4 __thiscall
libunwind::UnwindCursor<libunwind::LocalAddressSpace,libunwind::Registers_arm>::
getInfoFromEHABISection
          (UnwindCursor<libunwind::LocalAddressSpace,libunwind::Registers_arm> *this,uint param_1,
          UnwindInfoSections *param_2)

{
  uint uVar1;
  uint uVar2;
  int *piVar3;
  int iVar4;
  uint uVar5;
  char *__format;
  uint *puVar6;
  int iVar7;
  uint *puVar8;
  bool bVar9;
  uint uVar10;
  undefined4 uVar11;
  uint uVar12;
  bool bVar13;
  uint uVar14;
  int iVar15;
  int iVar16;
  
  if (*(uint *)(param_2 + 4) >> 3 == 0) {
    return 0;
  }
  iVar15 = *(int *)param_2;
  uVar14 = *(uint *)(param_2 + 4) >> 3;
  uVar12 = 0;
  uVar5 = uVar14;
  do {
    iVar7 = uVar12 + (uVar5 >> 1);
    uVar10 = *(uint *)(iVar15 + iVar7 * 8);
    uVar1 = (uVar5 - 1) - (uVar5 >> 1);
    uVar2 = iVar7 + 1;
    if (param_1 < (uVar10 | (uVar10 & 0x40000000) << 1) + iVar15 + iVar7 * 8) {
      uVar1 = uVar5 >> 1;
      uVar2 = uVar12;
    }
    uVar12 = uVar2;
    uVar5 = uVar1;
  } while (uVar5 != 0);
  if (uVar12 == 0) {
    return 0;
  }
  uVar5 = *(uint *)(iVar15 + (uVar12 - 1) * 8);
  iVar7 = iVar15 + (uVar12 - 1) * 8;
  if (uVar12 == uVar14) {
    iVar15 = -1;
  }
  else {
    uVar14 = *(uint *)(iVar15 + uVar12 * 8);
    iVar15 = (uVar14 | (uVar14 & 0x40000000) << 1) + iVar15 + uVar12 * 8;
  }
  if (iVar7 == -4) {
    return 0;
  }
  puVar8 = (uint *)(iVar7 + 4);
  uVar12 = *puVar8;
  if (uVar12 == 1) {
    return 0;
  }
  if ((int)uVar12 < 0) {
    bVar13 = true;
LAB_00012ef6:
    uVar14 = (uVar12 << 4) >> 0x1c;
    if (uVar14 == 2) {
      iVar16 = *(int *)(DAT_00012fcc + 0x12f2c);
      bVar9 = true;
    }
    else {
      if (uVar14 != 1) {
        if (uVar14 != 0) {
          uVar11 = 0x35a;
          __format = (char *)(DAT_00012ff0 + 0x12f88);
          iVar7 = DAT_00012ff4 + 0x12f8a;
          iVar16 = DAT_00012ff8 + 0x12f8c;
          iVar15 = DAT_00012fe8 + 0x12f8e;
          piVar3 = (int *)(DAT_00012fec + 0x12f94);
          goto LAB_00012fb0;
        }
        puVar6 = puVar8 + 1;
        if (bVar13) {
          puVar6 = (uint *)0x0;
        }
        iVar16 = *(int *)(DAT_00012fe4 + 0x12f14);
        goto LAB_00012f16;
      }
      iVar16 = *(int *)(DAT_00012fc8 + 0x12f20);
      bVar9 = false;
    }
    uVar12 = (uVar12 << 8) >> 0x18;
    if ((bool)(uVar12 != 0 & bVar13)) {
      uVar11 = 0x361;
      __format = (char *)(DAT_00012fd8 + 0x12fa6);
      iVar7 = DAT_00012fdc + 0x12fa8;
      iVar16 = DAT_00012fe0 + 0x12faa;
      iVar15 = DAT_00012fd0 + 0x12fac;
      piVar3 = (int *)(DAT_00012fd4 + 0x12fb2);
LAB_00012fb0:
      iVar4 = *piVar3;
      fprintf((FILE *)(iVar4 + 0xa8),__format,iVar7,iVar16,uVar11,iVar15);
      fflush((FILE *)(iVar4 + 0xa8));
                    /* WARNING: Subroutine does not return */
      abort();
    }
    puVar6 = puVar8 + uVar12 + 1;
  }
  else {
    uVar12 = uVar12 | (uVar12 & 0x40000000) << 1;
    puVar6 = (uint *)(uVar12 + (int)puVar8);
    uVar12 = *(uint *)(uVar12 + (int)puVar8);
    puVar8 = puVar6;
    if ((int)uVar12 < 0) {
      bVar13 = false;
      goto LAB_00012ef6;
    }
    bVar13 = false;
    iVar16 = (uVar12 | (uVar12 & 0x40000000) << 1) + (int)puVar6;
    puVar6 = puVar6 + 1 + (puVar6[1] >> 0x18) + 1;
LAB_00012f16:
    bVar9 = false;
  }
  *(uint **)(this + 0x178) = puVar8;
  *(uint *)(this + 0x158) = (uVar5 | (uVar5 & 0x40000000) << 1) + iVar7;
  *(int *)(this + 0x15c) = iVar15;
  *(uint **)(this + 0x160) = puVar6;
  *(int *)(this + 0x164) = iVar16;
  uVar11 = 0;
  if (bVar9) {
    uVar11 = 2;
  }
  if (bVar13) {
    uVar11 = 1;
  }
  *(undefined4 *)(this + 0x16c) = uVar11;
  return 1;
}

// ====== 00012ffc: __invoke ======
/* __invoke(dl_phdr_info*, unsigned int, void*) */

uint libunwind::LocalAddressSpace::findUnwindSections(unsigned_int,libunwind::UnwindInfoSections&)::
     {lambda(dl_phdr_info*,unsigned_int,void*)#1}::__invoke
               (dl_phdr_info *param_1,uint param_2,void *param_3)

{
  int iVar1;
  uint uVar2;
  uint uVar3;
  uint uVar4;
  int iVar5;
  uint uVar6;
  uint uVar7;
  
  if (*(uint *)param_1 <= *(uint *)((int)param_3 + 8)) {
    uVar2 = (uint)*(ushort *)(param_1 + 0xc);
    if (uVar2 == 0) {
      uVar6 = 0;
      uVar7 = 0;
    }
    else {
      iVar1 = 0;
      uVar7 = 0;
      uVar6 = 0;
      uVar3 = 0;
      do {
        iVar5 = *(int *)(param_1 + 8);
        if (*(int *)(iVar5 + iVar1) == 0x70000001) {
          uVar6 = 1;
          **(int **)((int)param_3 + 4) = *(int *)(iVar5 + iVar1 + 8) + *(int *)param_1;
          *(undefined4 *)(*(int *)((int)param_3 + 4) + 4) = *(undefined4 *)(iVar5 + iVar1 + 0x14);
          uVar2 = (uint)*(ushort *)(param_1 + 0xc);
        }
        else if (*(int *)(iVar5 + iVar1) == 1) {
          uVar4 = *(int *)(iVar5 + iVar1 + 8) + *(int *)param_1;
          if ((uVar4 <= *(uint *)((int)param_3 + 8)) &&
             (*(uint *)((int)param_3 + 8) < uVar4 + *(int *)(iVar5 + iVar1 + 0x14))) {
            uVar7 = 1;
          }
        }
        iVar1 = iVar1 + 0x20;
        uVar3 = uVar3 + 1;
      } while (uVar3 < uVar2);
    }
    return uVar6 & uVar7;
  }
  return 0;
}

// ====== 00013084: getRegisterName ======
/* libunwind::Registers_arm::getRegisterName(int) */

int __thiscall libunwind::Registers_arm::getRegisterName(Registers_arm *this,int param_1)

{
  if (param_1 + 2U >> 1 < 0x91) {
    switch(param_1) {
    case 0:
      return DAT_000135f0 + 0x132e8;
    case 1:
      return DAT_000135ec + 0x132ee;
    case 2:
      return DAT_000135e8 + 0x132f4;
    case 3:
      return DAT_000135e4 + 0x132fa;
    case 4:
      return DAT_000135e0 + 0x13300;
    case 5:
      return DAT_000135dc + 0x13306;
    case 6:
      return DAT_000135d8 + 0x1330c;
    case 7:
      return DAT_000135d4 + 0x13312;
    case 8:
      return DAT_000135d0 + 0x13318;
    case 9:
      return DAT_000135cc + 0x1331e;
    case 10:
      return DAT_000135c8 + 0x13324;
    case 0xb:
      return DAT_000135c4 + 0x1332a;
    case 0xc:
      return DAT_000135c0 + 0x13330;
    case 0xe:
      return DAT_000135f8 + 0x13336;
    case 0x40:
      return DAT_000135bc + 0x13342;
    case 0x41:
      return DAT_000135b8 + 0x13348;
    case 0x42:
      return DAT_000135b4 + 0x1334e;
    case 0x43:
      return DAT_000135b0 + 0x13354;
    case 0x44:
      return DAT_000135ac + 0x1335a;
    case 0x45:
      return DAT_000135a8 + 0x13360;
    case 0x46:
      return DAT_000135a4 + 0x13366;
    case 0x47:
      return DAT_000135a0 + 0x1336c;
    case 0x48:
      return DAT_0001359c + 0x13372;
    case 0x49:
      return DAT_00013598 + 0x13378;
    case 0x4a:
      return DAT_00013594 + 0x1337e;
    case 0x4b:
      return DAT_00013590 + 0x13384;
    case 0x4c:
      return DAT_0001358c + 0x1338a;
    case 0x4d:
      return DAT_00013588 + 0x13390;
    case 0x4e:
      return DAT_00013584 + 0x13396;
    case 0x4f:
      return DAT_00013580 + 0x1339c;
    case 0x50:
      return DAT_0001357c + 0x133a2;
    case 0x51:
      return DAT_00013578 + 0x133a8;
    case 0x52:
      return DAT_00013574 + 0x133ae;
    case 0x53:
      return DAT_00013570 + 0x133b4;
    case 0x54:
      return DAT_0001356c + 0x133ba;
    case 0x55:
      return DAT_00013568 + 0x133c0;
    case 0x56:
      return DAT_00013564 + 0x133c6;
    case 0x57:
      return DAT_00013560 + 0x133cc;
    case 0x58:
      return DAT_0001355c + 0x133d2;
    case 0x59:
      return DAT_00013558 + 0x133d8;
    case 0x5a:
      return DAT_00013554 + 0x133de;
    case 0x5b:
      return DAT_00013550 + 0x133e4;
    case 0x5c:
      return DAT_0001354c + 0x133ea;
    case 0x5d:
      return DAT_00013548 + 0x133f0;
    case 0x5e:
      return DAT_00013544 + 0x133f6;
    case 0x5f:
      return DAT_00013540 + 0x133fc;
    case 0x100:
      return DAT_0001353c + 0x13402;
    case 0x101:
      return DAT_00013538 + 0x13408;
    case 0x102:
      return DAT_00013534 + 0x1340e;
    case 0x103:
      return DAT_00013530 + 0x13414;
    case 0x104:
      return DAT_0001352c + 0x1341a;
    case 0x105:
      return DAT_00013528 + 0x13420;
    case 0x106:
      return DAT_00013524 + 0x13426;
    case 0x107:
      return DAT_00013520 + 0x1342c;
    case 0x108:
      return DAT_0001351c + 0x13432;
    case 0x109:
      return DAT_00013518 + 0x13438;
    case 0x10a:
      return DAT_00013514 + 0x1343e;
    case 0x10b:
      return DAT_00013510 + 0x13444;
    case 0x10c:
      return DAT_0001350c + 0x1344a;
    case 0x10d:
      return DAT_00013508 + 0x13450;
    case 0x10e:
      return DAT_00013504 + 0x13456;
    case 0x10f:
      return DAT_00013500 + 0x1345c;
    case 0x110:
      return DAT_000134fc + 0x13462;
    case 0x111:
      return DAT_000134f8 + 0x13468;
    case 0x112:
      return DAT_000134f4 + 0x1346e;
    case 0x113:
      return DAT_000134f0 + 0x13474;
    case 0x114:
      return DAT_000134ec + 0x1347a;
    case 0x115:
      return DAT_000134e8 + 0x13480;
    case 0x116:
      return DAT_000134e4 + 0x13486;
    case 0x117:
      return DAT_000134e0 + 0x1348c;
    case 0x118:
      return DAT_000134dc + 0x13492;
    case 0x119:
      return DAT_000134d8 + 79000;
    case 0x11a:
      return DAT_000134d4 + 0x1349e;
    case 0x11b:
      return DAT_000134d0 + 0x134a4;
    case 0x11c:
      return DAT_000134cc + 0x134aa;
    case 0x11d:
      return DAT_000134c8 + 0x134b0;
    case 0x11e:
      return DAT_000134c4 + 0x134b6;
    case 0x11f:
      return (int)&DAT_000134bc + DAT_000134c0;
    case -2:
    case 0xd:
      return DAT_000135f4 + 0x132e2;
    case -1:
    case 0xf:
      return DAT_000134bc + 0x13096;
    }
  }
  return DAT_000135fc + 0x1333c;
}

// ====== 00013600: restoreCoreAndJumpTo ======
/* libunwind::Registers_arm::restoreCoreAndJumpTo() */

undefined8 __thiscall libunwind::Registers_arm::restoreCoreAndJumpTo(Registers_arm *this)

{
  return *(undefined8 *)this;
}

// ====== 00013614: restoreVFPWithFLDMD ======
/* libunwind::Registers_arm::restoreVFPWithFLDMD(unsigned long long*) */

undefined4 libunwind::Registers_arm::restoreVFPWithFLDMD(ulonglong *param_1)

{
  return (int)*param_1;
}

// ====== 0001361c: restoreVFPWithFLDMX ======
/* libunwind::Registers_arm::restoreVFPWithFLDMX(unsigned long long*) */

undefined4 libunwind::Registers_arm::restoreVFPWithFLDMX(ulonglong *param_1)

{
  return (int)*param_1;
}

// ====== 00013624: restoreVFPv3 ======
/* libunwind::Registers_arm::restoreVFPv3(unsigned long long*) */

void libunwind::Registers_arm::restoreVFPv3(ulonglong *param_1)

{
  return;
}
