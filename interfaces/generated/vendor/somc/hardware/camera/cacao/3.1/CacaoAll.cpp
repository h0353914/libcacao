#define LOG_TAG "vendor.somc.hardware.camera.cacao@3.1::Cacao"

#include <log/log.h>
#include <cutils/trace.h>
#include <hidl/HidlTransportSupport.h>

#include <hidl/Static.h>
#include <hwbinder/ProcessState.h>
#include <utils/Trace.h>
#include <android/hidl/manager/1.0/IServiceManager.h>
#include <vendor/somc/hardware/camera/cacao/3.1/BpHwCacao.h>
#include <vendor/somc/hardware/camera/cacao/3.1/BnHwCacao.h>
#include <vendor/somc/hardware/camera/cacao/3.1/BsCacao.h>
#include <vendor/somc/hardware/camera/cacao/3.0/BpHwCacao.h>
#include <android/hidl/base/1.0/BpHwBase.h>
#include <hidl/ServiceManagement.h>

namespace vendor {
namespace somc {
namespace hardware {
namespace camera {
namespace cacao {
namespace V3_1 {

const char* ICacao::descriptor("vendor.somc.hardware.camera.cacao@3.1::ICacao");

__attribute__((constructor)) static void static_constructor() {
    ::android::hardware::details::getBnConstructorMap().set(ICacao::descriptor,
            [](void *iIntf) -> ::android::sp<::android::hardware::IBinder> {
                return new BnHwCacao(static_cast<ICacao *>(iIntf));
            });
    ::android::hardware::details::getBsConstructorMap().set(ICacao::descriptor,
            [](void *iIntf) -> ::android::sp<::android::hidl::base::V1_0::IBase> {
                return new BsCacao(static_cast<ICacao *>(iIntf));
            });
}

__attribute__((destructor))static void static_destructor() {
    ::android::hardware::details::getBnConstructorMap().erase(ICacao::descriptor);
    ::android::hardware::details::getBsConstructorMap().erase(ICacao::descriptor);
}

// Methods from ::vendor::somc::hardware::camera::cacao::V3_0::ICacao follow.
// no default implementation for: ::android::hardware::Return<::vendor::somc::hardware::camera::cacao::V3_0::ErrCode> ICacao::setCallback(const ::android::sp<::vendor::somc::hardware::camera::cacao::V3_0::ICacaoCallback>& callback)
// no default implementation for: ::android::hardware::Return<::vendor::somc::hardware::camera::cacao::V3_0::ErrCode> ICacao::start(const ::vendor::somc::hardware::camera::cacao::V3_0::ControlMode& mode)
// no default implementation for: ::android::hardware::Return<::vendor::somc::hardware::camera::cacao::V3_0::ErrCode> ICacao::stop()
// no default implementation for: ::android::hardware::Return<::vendor::somc::hardware::camera::cacao::V3_0::ErrCode> ICacao::setConfig(const ::vendor::somc::hardware::camera::cacao::V3_0::Config& config)
// no default implementation for: ::android::hardware::Return<void> ICacao::getConfig(::vendor::somc::hardware::camera::cacao::V3_0::ConfigId configId, getConfig_cb _hidl_cb)
// no default implementation for: ::android::hardware::Return<::vendor::somc::hardware::camera::cacao::V3_0::ErrCode> ICacao::process(const ::vendor::somc::hardware::camera::cacao::V3_0::Param& param)
// no default implementation for: ::android::hardware::Return<::vendor::somc::hardware::camera::cacao::V3_0::ErrCode> ICacao::processAsync(const ::vendor::somc::hardware::camera::cacao::V3_0::Param& param, const ::vendor::somc::hardware::camera::cacao::V3_0::Result& result)
// no default implementation for: ::android::hardware::Return<::vendor::somc::hardware::camera::cacao::V3_0::ErrCode> ICacao::setColorSpaceForHandle(const ::android::hardware::hidl_handle& bufHandle, ::vendor::somc::hardware::camera::cacao::V3_0::DataSpace dataSpace)
// no default implementation for: ::android::hardware::Return<void> ICacao::getCaps(::vendor::somc::hardware::camera::cacao::V3_0::CameraIndex camIdx, getCaps_cb _hidl_cb)
// no default implementation for: ::android::hardware::Return<void> ICacao::getJpegBufferSize(const ::vendor::somc::hardware::camera::cacao::V3_0::ImageSize& size, getJpegBufferSize_cb _hidl_cb)

// Methods from ::vendor::somc::hardware::camera::cacao::V3_1::ICacao follow.
// no default implementation for: ::android::hardware::Return<void> ICacao::getCapsV3_1(::vendor::somc::hardware::camera::cacao::V3_0::CameraIndex camIdx, getCapsV3_1_cb _hidl_cb)

// Methods from ::android::hidl::base::V1_0::IBase follow.
::android::hardware::Return<void> ICacao::interfaceChain(interfaceChain_cb _hidl_cb){
    _hidl_cb({
        ::vendor::somc::hardware::camera::cacao::V3_1::ICacao::descriptor,
        ::vendor::somc::hardware::camera::cacao::V3_0::ICacao::descriptor,
        ::android::hidl::base::V1_0::IBase::descriptor,
    });
    return ::android::hardware::Void();
}

::android::hardware::Return<void> ICacao::debug(const ::android::hardware::hidl_handle& fd, const ::android::hardware::hidl_vec<::android::hardware::hidl_string>& options){
    (void)fd;
    (void)options;
    return ::android::hardware::Void();
}

::android::hardware::Return<void> ICacao::interfaceDescriptor(interfaceDescriptor_cb _hidl_cb){
    _hidl_cb(::vendor::somc::hardware::camera::cacao::V3_1::ICacao::descriptor);
    return ::android::hardware::Void();
}

::android::hardware::Return<void> ICacao::getHashChain(getHashChain_cb _hidl_cb){
    _hidl_cb({
        (uint8_t[32]){0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0} /* 0000000000000000000000000000000000000000000000000000000000000000 */,
        (uint8_t[32]){39,50,28,13,5,154,44,113,61,192,123,153,73,70,57,109,35,144,56,56,135,149,248,109,21,141,67,71,107,73,76,76} /* 27321c0d059a2c713dc07b994946396d239038388795f86d158d43476b494c4c */,
        (uint8_t[32]){236,127,215,158,208,45,250,133,188,73,148,38,173,174,62,190,35,239,5,36,243,205,105,87,19,147,36,184,59,24,202,76} /* ec7fd79ed02dfa85bc499426adae3ebe23ef0524f3cd6957139324b83b18ca4c */});
    return ::android::hardware::Void();
}

::android::hardware::Return<void> ICacao::setHALInstrumentation(){
    return ::android::hardware::Void();
}

::android::hardware::Return<bool> ICacao::linkToDeath(const ::android::sp<::android::hardware::hidl_death_recipient>& recipient, uint64_t cookie){
    (void)cookie;
    return (recipient != nullptr);
}

::android::hardware::Return<void> ICacao::ping(){
    return ::android::hardware::Void();
}

::android::hardware::Return<void> ICacao::getDebugInfo(getDebugInfo_cb _hidl_cb){
    ::android::hidl::base::V1_0::DebugInfo info = {};
    info.pid = -1;
    info.ptr = 0;
    info.arch = 
    #if defined(__LP64__)
    ::android::hidl::base::V1_0::DebugInfo::Architecture::IS_64BIT
    #else
    ::android::hidl::base::V1_0::DebugInfo::Architecture::IS_32BIT
    #endif
    ;
    _hidl_cb(info);
    return ::android::hardware::Void();
}

::android::hardware::Return<void> ICacao::notifySyspropsChanged(){
    ::android::report_sysprop_change();
    return ::android::hardware::Void();
}

::android::hardware::Return<bool> ICacao::unlinkToDeath(const ::android::sp<::android::hardware::hidl_death_recipient>& recipient){
    return (recipient != nullptr);
}


::android::hardware::Return<::android::sp<::vendor::somc::hardware::camera::cacao::V3_1::ICacao>> ICacao::castFrom(const ::android::sp<::vendor::somc::hardware::camera::cacao::V3_1::ICacao>& parent, bool /* emitError */) {
    return parent;
}

::android::hardware::Return<::android::sp<::vendor::somc::hardware::camera::cacao::V3_1::ICacao>> ICacao::castFrom(const ::android::sp<::vendor::somc::hardware::camera::cacao::V3_0::ICacao>& parent, bool emitError) {
    return ::android::hardware::details::castInterface<ICacao, ::vendor::somc::hardware::camera::cacao::V3_0::ICacao, BpHwCacao>(
            parent, "vendor.somc.hardware.camera.cacao@3.1::ICacao", emitError);
}

::android::hardware::Return<::android::sp<::vendor::somc::hardware::camera::cacao::V3_1::ICacao>> ICacao::castFrom(const ::android::sp<::android::hidl::base::V1_0::IBase>& parent, bool emitError) {
    return ::android::hardware::details::castInterface<ICacao, ::android::hidl::base::V1_0::IBase, BpHwCacao>(
            parent, "vendor.somc.hardware.camera.cacao@3.1::ICacao", emitError);
}

BpHwCacao::BpHwCacao(const ::android::sp<::android::hardware::IBinder> &_hidl_impl)
        : BpInterface<ICacao>(_hidl_impl),
          ::android::hardware::details::HidlInstrumentor("vendor.somc.hardware.camera.cacao@3.1", "ICacao") {
}

void BpHwCacao::onLastStrongRef(const void* id) {
    {
        std::unique_lock<std::mutex> lock(_hidl_mMutex);
        _hidl_mDeathRecipients.clear();
    }

    BpInterface<ICacao>::onLastStrongRef(id);
}
// Methods from ::vendor::somc::hardware::camera::cacao::V3_1::ICacao follow.
::android::hardware::Return<void> BpHwCacao::_hidl_getCapsV3_1(::android::hardware::IInterface *_hidl_this, ::android::hardware::details::HidlInstrumentor *_hidl_this_instrumentor, ::vendor::somc::hardware::camera::cacao::V3_0::CameraIndex camIdx, getCapsV3_1_cb _hidl_cb) {
    #ifdef __ANDROID_DEBUGGABLE__
    bool mEnableInstrumentation = _hidl_this_instrumentor->isInstrumentationEnabled();
    const auto &mInstrumentationCallbacks = _hidl_this_instrumentor->getInstrumentationCallbacks();
    #else
    (void) _hidl_this_instrumentor;
    #endif // __ANDROID_DEBUGGABLE__
    ::android::ScopedTrace PASTE(___tracer, __LINE__) (ATRACE_TAG_HAL, "HIDL::ICacao::getCapsV3_1::client");
    #ifdef __ANDROID_DEBUGGABLE__
    if (UNLIKELY(mEnableInstrumentation)) {
        std::vector<void *> _hidl_args;
        _hidl_args.push_back((void *)&camIdx);
        for (const auto &callback: mInstrumentationCallbacks) {
            callback(InstrumentationEvent::CLIENT_API_ENTRY, "vendor.somc.hardware.camera.cacao", "3.1", "ICacao", "getCapsV3_1", &_hidl_args);
        }
    }
    #endif // __ANDROID_DEBUGGABLE__

    ::android::hardware::Parcel _hidl_data;
    ::android::hardware::Parcel _hidl_reply;
    ::android::status_t _hidl_err;
    ::android::status_t _hidl_transact_err;
    ::android::hardware::Status _hidl_status;

    _hidl_err = _hidl_data.writeInterfaceToken(BpHwCacao::descriptor);
    if (_hidl_err != ::android::OK) { goto _hidl_error; }

    _hidl_err = _hidl_data.writeInt32((int32_t)camIdx);
    if (_hidl_err != ::android::OK) { goto _hidl_error; }

    _hidl_transact_err = ::android::hardware::IInterface::asBinder(_hidl_this)->transact(11 /* getCapsV3_1 */, _hidl_data, &_hidl_reply, 0 /* flags */, [&] (::android::hardware::Parcel& _hidl_reply) {
        ::vendor::somc::hardware::camera::cacao::V3_1::CacaoCaps* _hidl_out_caps;
        ::vendor::somc::hardware::camera::cacao::V3_0::ErrCode _hidl_out_err;


        _hidl_err = ::android::hardware::readFromParcel(&_hidl_status, _hidl_reply);
        if (_hidl_err != ::android::OK) { return; }

        if (!_hidl_status.isOk()) { return; }

        size_t _hidl__hidl_out_caps_parent;

        _hidl_err = _hidl_reply.readBuffer(sizeof(*_hidl_out_caps), &_hidl__hidl_out_caps_parent,  const_cast<const void**>(reinterpret_cast<void **>(&_hidl_out_caps)));
        if (_hidl_err != ::android::OK) { return; }

        _hidl_err = readEmbeddedFromParcel(
                const_cast<::vendor::somc::hardware::camera::cacao::V3_1::CacaoCaps &>(*_hidl_out_caps),
                _hidl_reply,
                _hidl__hidl_out_caps_parent,
                0 /* parentOffset */);

        if (_hidl_err != ::android::OK) { return; }

        _hidl_err = _hidl_reply.readInt32((int32_t *)&_hidl_out_err);
        if (_hidl_err != ::android::OK) { return; }

        _hidl_cb(*_hidl_out_caps, _hidl_out_err);

        #ifdef __ANDROID_DEBUGGABLE__
        if (UNLIKELY(mEnableInstrumentation)) {
            std::vector<void *> _hidl_args;
            _hidl_args.push_back((void *)_hidl_out_caps);
            _hidl_args.push_back((void *)&_hidl_out_err);
            for (const auto &callback: mInstrumentationCallbacks) {
                callback(InstrumentationEvent::CLIENT_API_EXIT, "vendor.somc.hardware.camera.cacao", "3.1", "ICacao", "getCapsV3_1", &_hidl_args);
            }
        }
        #endif // __ANDROID_DEBUGGABLE__

    });
    if (_hidl_transact_err != ::android::OK) {
        _hidl_err = _hidl_transact_err;
        goto _hidl_error;
    }

    if (!_hidl_status.isOk()) { return _hidl_status; }
    return ::android::hardware::Return<void>();

_hidl_error:
    _hidl_status.setFromStatusT(_hidl_err);
    return ::android::hardware::Return<void>(_hidl_status);
}


// Methods from ::vendor::somc::hardware::camera::cacao::V3_0::ICacao follow.
::android::hardware::Return<::vendor::somc::hardware::camera::cacao::V3_0::ErrCode> BpHwCacao::setCallback(const ::android::sp<::vendor::somc::hardware::camera::cacao::V3_0::ICacaoCallback>& callback){
    ::android::hardware::Return<::vendor::somc::hardware::camera::cacao::V3_0::ErrCode>  _hidl_out = ::vendor::somc::hardware::camera::cacao::V3_0::BpHwCacao::_hidl_setCallback(this, this, callback);

    return _hidl_out;
}

::android::hardware::Return<::vendor::somc::hardware::camera::cacao::V3_0::ErrCode> BpHwCacao::start(const ::vendor::somc::hardware::camera::cacao::V3_0::ControlMode& mode){
    ::android::hardware::Return<::vendor::somc::hardware::camera::cacao::V3_0::ErrCode>  _hidl_out = ::vendor::somc::hardware::camera::cacao::V3_0::BpHwCacao::_hidl_start(this, this, mode);

    return _hidl_out;
}

::android::hardware::Return<::vendor::somc::hardware::camera::cacao::V3_0::ErrCode> BpHwCacao::stop(){
    ::android::hardware::Return<::vendor::somc::hardware::camera::cacao::V3_0::ErrCode>  _hidl_out = ::vendor::somc::hardware::camera::cacao::V3_0::BpHwCacao::_hidl_stop(this, this);

    return _hidl_out;
}

::android::hardware::Return<::vendor::somc::hardware::camera::cacao::V3_0::ErrCode> BpHwCacao::setConfig(const ::vendor::somc::hardware::camera::cacao::V3_0::Config& config){
    ::android::hardware::Return<::vendor::somc::hardware::camera::cacao::V3_0::ErrCode>  _hidl_out = ::vendor::somc::hardware::camera::cacao::V3_0::BpHwCacao::_hidl_setConfig(this, this, config);

    return _hidl_out;
}

::android::hardware::Return<void> BpHwCacao::getConfig(::vendor::somc::hardware::camera::cacao::V3_0::ConfigId configId, getConfig_cb _hidl_cb){
    ::android::hardware::Return<void>  _hidl_out = ::vendor::somc::hardware::camera::cacao::V3_0::BpHwCacao::_hidl_getConfig(this, this, configId, _hidl_cb);

    return _hidl_out;
}

::android::hardware::Return<::vendor::somc::hardware::camera::cacao::V3_0::ErrCode> BpHwCacao::process(const ::vendor::somc::hardware::camera::cacao::V3_0::Param& param){
    ::android::hardware::Return<::vendor::somc::hardware::camera::cacao::V3_0::ErrCode>  _hidl_out = ::vendor::somc::hardware::camera::cacao::V3_0::BpHwCacao::_hidl_process(this, this, param);

    return _hidl_out;
}

::android::hardware::Return<::vendor::somc::hardware::camera::cacao::V3_0::ErrCode> BpHwCacao::processAsync(const ::vendor::somc::hardware::camera::cacao::V3_0::Param& param, const ::vendor::somc::hardware::camera::cacao::V3_0::Result& result){
    ::android::hardware::Return<::vendor::somc::hardware::camera::cacao::V3_0::ErrCode>  _hidl_out = ::vendor::somc::hardware::camera::cacao::V3_0::BpHwCacao::_hidl_processAsync(this, this, param, result);

    return _hidl_out;
}

::android::hardware::Return<::vendor::somc::hardware::camera::cacao::V3_0::ErrCode> BpHwCacao::setColorSpaceForHandle(const ::android::hardware::hidl_handle& bufHandle, ::vendor::somc::hardware::camera::cacao::V3_0::DataSpace dataSpace){
    ::android::hardware::Return<::vendor::somc::hardware::camera::cacao::V3_0::ErrCode>  _hidl_out = ::vendor::somc::hardware::camera::cacao::V3_0::BpHwCacao::_hidl_setColorSpaceForHandle(this, this, bufHandle, dataSpace);

    return _hidl_out;
}

::android::hardware::Return<void> BpHwCacao::getCaps(::vendor::somc::hardware::camera::cacao::V3_0::CameraIndex camIdx, getCaps_cb _hidl_cb){
    ::android::hardware::Return<void>  _hidl_out = ::vendor::somc::hardware::camera::cacao::V3_0::BpHwCacao::_hidl_getCaps(this, this, camIdx, _hidl_cb);

    return _hidl_out;
}

::android::hardware::Return<void> BpHwCacao::getJpegBufferSize(const ::vendor::somc::hardware::camera::cacao::V3_0::ImageSize& size, getJpegBufferSize_cb _hidl_cb){
    ::android::hardware::Return<void>  _hidl_out = ::vendor::somc::hardware::camera::cacao::V3_0::BpHwCacao::_hidl_getJpegBufferSize(this, this, size, _hidl_cb);

    return _hidl_out;
}


// Methods from ::vendor::somc::hardware::camera::cacao::V3_1::ICacao follow.
::android::hardware::Return<void> BpHwCacao::getCapsV3_1(::vendor::somc::hardware::camera::cacao::V3_0::CameraIndex camIdx, getCapsV3_1_cb _hidl_cb){
    ::android::hardware::Return<void>  _hidl_out = ::vendor::somc::hardware::camera::cacao::V3_1::BpHwCacao::_hidl_getCapsV3_1(this, this, camIdx, _hidl_cb);

    return _hidl_out;
}


// Methods from ::android::hidl::base::V1_0::IBase follow.
::android::hardware::Return<void> BpHwCacao::interfaceChain(interfaceChain_cb _hidl_cb){
    ::android::hardware::Return<void>  _hidl_out = ::android::hidl::base::V1_0::BpHwBase::_hidl_interfaceChain(this, this, _hidl_cb);

    return _hidl_out;
}

::android::hardware::Return<void> BpHwCacao::debug(const ::android::hardware::hidl_handle& fd, const ::android::hardware::hidl_vec<::android::hardware::hidl_string>& options){
    ::android::hardware::Return<void>  _hidl_out = ::android::hidl::base::V1_0::BpHwBase::_hidl_debug(this, this, fd, options);

    return _hidl_out;
}

::android::hardware::Return<void> BpHwCacao::interfaceDescriptor(interfaceDescriptor_cb _hidl_cb){
    ::android::hardware::Return<void>  _hidl_out = ::android::hidl::base::V1_0::BpHwBase::_hidl_interfaceDescriptor(this, this, _hidl_cb);

    return _hidl_out;
}

::android::hardware::Return<void> BpHwCacao::getHashChain(getHashChain_cb _hidl_cb){
    ::android::hardware::Return<void>  _hidl_out = ::android::hidl::base::V1_0::BpHwBase::_hidl_getHashChain(this, this, _hidl_cb);

    return _hidl_out;
}

::android::hardware::Return<void> BpHwCacao::setHALInstrumentation(){
    ::android::hardware::Return<void>  _hidl_out = ::android::hidl::base::V1_0::BpHwBase::_hidl_setHALInstrumentation(this, this);

    return _hidl_out;
}

::android::hardware::Return<bool> BpHwCacao::linkToDeath(const ::android::sp<::android::hardware::hidl_death_recipient>& recipient, uint64_t cookie){
    ::android::hardware::ProcessState::self()->startThreadPool();
    ::android::hardware::hidl_binder_death_recipient *binder_recipient = new ::android::hardware::hidl_binder_death_recipient(recipient, cookie, this);
    std::unique_lock<std::mutex> lock(_hidl_mMutex);
    _hidl_mDeathRecipients.push_back(binder_recipient);
    return (remote()->linkToDeath(binder_recipient) == ::android::OK);
}

::android::hardware::Return<void> BpHwCacao::ping(){
    ::android::hardware::Return<void>  _hidl_out = ::android::hidl::base::V1_0::BpHwBase::_hidl_ping(this, this);

    return _hidl_out;
}

::android::hardware::Return<void> BpHwCacao::getDebugInfo(getDebugInfo_cb _hidl_cb){
    ::android::hardware::Return<void>  _hidl_out = ::android::hidl::base::V1_0::BpHwBase::_hidl_getDebugInfo(this, this, _hidl_cb);

    return _hidl_out;
}

::android::hardware::Return<void> BpHwCacao::notifySyspropsChanged(){
    ::android::hardware::Return<void>  _hidl_out = ::android::hidl::base::V1_0::BpHwBase::_hidl_notifySyspropsChanged(this, this);

    return _hidl_out;
}

::android::hardware::Return<bool> BpHwCacao::unlinkToDeath(const ::android::sp<::android::hardware::hidl_death_recipient>& recipient){
    std::unique_lock<std::mutex> lock(_hidl_mMutex);
    for (auto it = _hidl_mDeathRecipients.rbegin();it != _hidl_mDeathRecipients.rend();++it) {
        if ((*it)->getRecipient() == recipient) {
            ::android::status_t status = remote()->unlinkToDeath(*it);
            _hidl_mDeathRecipients.erase(it.base()-1);
            return status == ::android::OK;
        }
    }
    return false;
}


BnHwCacao::BnHwCacao(const ::android::sp<ICacao> &_hidl_impl)
        : ::android::hidl::base::V1_0::BnHwBase(_hidl_impl, "vendor.somc.hardware.camera.cacao@3.1", "ICacao") { 
            _hidl_mImpl = _hidl_impl;
            auto prio = ::android::hardware::getMinSchedulerPolicy(_hidl_impl);
            mSchedPolicy = prio.sched_policy;
            mSchedPriority = prio.prio;
            setRequestingSid(::android::hardware::getRequestingSid(_hidl_impl));
}

BnHwCacao::~BnHwCacao() {
    ::android::hardware::details::gBnMap->eraseIfEqual(_hidl_mImpl.get(), this);
}

// Methods from ::vendor::somc::hardware::camera::cacao::V3_1::ICacao follow.
::android::status_t BnHwCacao::_hidl_getCapsV3_1(
        ::android::hidl::base::V1_0::BnHwBase* _hidl_this,
        const ::android::hardware::Parcel &_hidl_data,
        ::android::hardware::Parcel *_hidl_reply,
        TransactCallback _hidl_cb) {
    #ifdef __ANDROID_DEBUGGABLE__
    bool mEnableInstrumentation = _hidl_this->isInstrumentationEnabled();
    const auto &mInstrumentationCallbacks = _hidl_this->getInstrumentationCallbacks();
    #endif // __ANDROID_DEBUGGABLE__

    ::android::status_t _hidl_err = ::android::OK;
    if (!_hidl_data.enforceInterface(BnHwCacao::Pure::descriptor)) {
        _hidl_err = ::android::BAD_TYPE;
        return _hidl_err;
    }

    ::vendor::somc::hardware::camera::cacao::V3_0::CameraIndex camIdx;

    _hidl_err = _hidl_data.readInt32((int32_t *)&camIdx);
    if (_hidl_err != ::android::OK) { return _hidl_err; }

    atrace_begin(ATRACE_TAG_HAL, "HIDL::ICacao::getCapsV3_1::server");
    #ifdef __ANDROID_DEBUGGABLE__
    if (UNLIKELY(mEnableInstrumentation)) {
        std::vector<void *> _hidl_args;
        _hidl_args.push_back((void *)&camIdx);
        for (const auto &callback: mInstrumentationCallbacks) {
            callback(InstrumentationEvent::SERVER_API_ENTRY, "vendor.somc.hardware.camera.cacao", "3.1", "ICacao", "getCapsV3_1", &_hidl_args);
        }
    }
    #endif // __ANDROID_DEBUGGABLE__

    bool _hidl_callbackCalled = false;

    ::android::hardware::Return<void> _hidl_ret = static_cast<ICacao*>(_hidl_this->getImpl().get())->getCapsV3_1(camIdx, [&](const auto &_hidl_out_caps, const auto &_hidl_out_err) {
        if (_hidl_callbackCalled) {
            LOG_ALWAYS_FATAL("getCapsV3_1: _hidl_cb called a second time, but must be called once.");
        }
        _hidl_callbackCalled = true;

        ::android::hardware::writeToParcel(::android::hardware::Status::ok(), _hidl_reply);

        size_t _hidl__hidl_out_caps_parent;

        _hidl_err = _hidl_reply->writeBuffer(&_hidl_out_caps, sizeof(_hidl_out_caps), &_hidl__hidl_out_caps_parent);
        if (_hidl_err != ::android::OK) { goto _hidl_error; }

        _hidl_err = writeEmbeddedToParcel(
                _hidl_out_caps,
                _hidl_reply,
                _hidl__hidl_out_caps_parent,
                0 /* parentOffset */);

        if (_hidl_err != ::android::OK) { goto _hidl_error; }

        _hidl_err = _hidl_reply->writeInt32((int32_t)_hidl_out_err);
        if (_hidl_err != ::android::OK) { goto _hidl_error; }

    _hidl_error:
        atrace_end(ATRACE_TAG_HAL);
        #ifdef __ANDROID_DEBUGGABLE__
        if (UNLIKELY(mEnableInstrumentation)) {
            std::vector<void *> _hidl_args;
            _hidl_args.push_back((void *)&_hidl_out_caps);
            _hidl_args.push_back((void *)&_hidl_out_err);
            for (const auto &callback: mInstrumentationCallbacks) {
                callback(InstrumentationEvent::SERVER_API_EXIT, "vendor.somc.hardware.camera.cacao", "3.1", "ICacao", "getCapsV3_1", &_hidl_args);
            }
        }
        #endif // __ANDROID_DEBUGGABLE__

        if (_hidl_err != ::android::OK) { return; }
        _hidl_cb(*_hidl_reply);
    });

    _hidl_ret.assertOk();
    if (!_hidl_callbackCalled) {
        LOG_ALWAYS_FATAL("getCapsV3_1: _hidl_cb not called, but must be called once.");
    }

    return _hidl_err;
}


// Methods from ::vendor::somc::hardware::camera::cacao::V3_0::ICacao follow.

// Methods from ::vendor::somc::hardware::camera::cacao::V3_1::ICacao follow.

// Methods from ::android::hidl::base::V1_0::IBase follow.
::android::hardware::Return<void> BnHwCacao::ping() {
    return ::android::hardware::Void();
}
::android::hardware::Return<void> BnHwCacao::getDebugInfo(getDebugInfo_cb _hidl_cb) {
    ::android::hidl::base::V1_0::DebugInfo info = {};
    info.pid = ::android::hardware::details::getPidIfSharable();
    info.ptr = ::android::hardware::details::debuggable()? reinterpret_cast<uint64_t>(this) : 0;
    info.arch = 
    #if defined(__LP64__)
    ::android::hidl::base::V1_0::DebugInfo::Architecture::IS_64BIT
    #else
    ::android::hidl::base::V1_0::DebugInfo::Architecture::IS_32BIT
    #endif
    ;
    _hidl_cb(info);
    return ::android::hardware::Void();
}

::android::status_t BnHwCacao::onTransact(
        uint32_t _hidl_code,
        const ::android::hardware::Parcel &_hidl_data,
        ::android::hardware::Parcel *_hidl_reply,
        uint32_t _hidl_flags,
        TransactCallback _hidl_cb) {
    ::android::status_t _hidl_err = ::android::OK;

    switch (_hidl_code) {
        case 1 /* setCallback */:
        {
            _hidl_err = ::vendor::somc::hardware::camera::cacao::V3_0::BnHwCacao::_hidl_setCallback(this, _hidl_data, _hidl_reply, _hidl_cb);
            break;
        }

        case 2 /* start */:
        {
            _hidl_err = ::vendor::somc::hardware::camera::cacao::V3_0::BnHwCacao::_hidl_start(this, _hidl_data, _hidl_reply, _hidl_cb);
            break;
        }

        case 3 /* stop */:
        {
            _hidl_err = ::vendor::somc::hardware::camera::cacao::V3_0::BnHwCacao::_hidl_stop(this, _hidl_data, _hidl_reply, _hidl_cb);
            break;
        }

        case 4 /* setConfig */:
        {
            _hidl_err = ::vendor::somc::hardware::camera::cacao::V3_0::BnHwCacao::_hidl_setConfig(this, _hidl_data, _hidl_reply, _hidl_cb);
            break;
        }

        case 5 /* getConfig */:
        {
            _hidl_err = ::vendor::somc::hardware::camera::cacao::V3_0::BnHwCacao::_hidl_getConfig(this, _hidl_data, _hidl_reply, _hidl_cb);
            break;
        }

        case 6 /* process */:
        {
            _hidl_err = ::vendor::somc::hardware::camera::cacao::V3_0::BnHwCacao::_hidl_process(this, _hidl_data, _hidl_reply, _hidl_cb);
            break;
        }

        case 7 /* processAsync */:
        {
            _hidl_err = ::vendor::somc::hardware::camera::cacao::V3_0::BnHwCacao::_hidl_processAsync(this, _hidl_data, _hidl_reply, _hidl_cb);
            break;
        }

        case 8 /* setColorSpaceForHandle */:
        {
            _hidl_err = ::vendor::somc::hardware::camera::cacao::V3_0::BnHwCacao::_hidl_setColorSpaceForHandle(this, _hidl_data, _hidl_reply, _hidl_cb);
            break;
        }

        case 9 /* getCaps */:
        {
            _hidl_err = ::vendor::somc::hardware::camera::cacao::V3_0::BnHwCacao::_hidl_getCaps(this, _hidl_data, _hidl_reply, _hidl_cb);
            break;
        }

        case 10 /* getJpegBufferSize */:
        {
            _hidl_err = ::vendor::somc::hardware::camera::cacao::V3_0::BnHwCacao::_hidl_getJpegBufferSize(this, _hidl_data, _hidl_reply, _hidl_cb);
            break;
        }

        case 11 /* getCapsV3_1 */:
        {
            _hidl_err = ::vendor::somc::hardware::camera::cacao::V3_1::BnHwCacao::_hidl_getCapsV3_1(this, _hidl_data, _hidl_reply, _hidl_cb);
            break;
        }

        default:
        {
            return ::android::hidl::base::V1_0::BnHwBase::onTransact(
                    _hidl_code, _hidl_data, _hidl_reply, _hidl_flags, _hidl_cb);
        }
    }

    if (_hidl_err == ::android::UNEXPECTED_NULL) {
        _hidl_err = ::android::hardware::writeToParcel(
                ::android::hardware::Status::fromExceptionCode(::android::hardware::Status::EX_NULL_POINTER),
                _hidl_reply);
    }return _hidl_err;
}

BsCacao::BsCacao(const ::android::sp<::vendor::somc::hardware::camera::cacao::V3_1::ICacao> impl) : ::android::hardware::details::HidlInstrumentor("vendor.somc.hardware.camera.cacao@3.1", "ICacao"), mImpl(impl) {
    mOnewayQueue.start(3000 /* similar limit to binderized */);
}

::android::hardware::Return<void> BsCacao::addOnewayTask(std::function<void(void)> fun) {
    if (!mOnewayQueue.push(fun)) {
        return ::android::hardware::Status::fromExceptionCode(
                ::android::hardware::Status::EX_TRANSACTION_FAILED,
                "Passthrough oneway function queue exceeds maximum size.");
    }
    return ::android::hardware::Status();
}

::android::sp<ICacao> ICacao::tryGetService(const std::string &serviceName, const bool getStub) {
    return ::android::hardware::details::getServiceInternal<BpHwCacao>(serviceName, false, getStub);
}

::android::sp<ICacao> ICacao::getService(const std::string &serviceName, const bool getStub) {
    return ::android::hardware::details::getServiceInternal<BpHwCacao>(serviceName, true, getStub);
}

::android::status_t ICacao::registerAsService(const std::string &serviceName) {
    return ::android::hardware::details::registerAsServiceInternal(this, serviceName);
}

bool ICacao::registerForNotifications(
        const std::string &serviceName,
        const ::android::sp<::android::hidl::manager::V1_0::IServiceNotification> &notification) {
    const ::android::sp<::android::hidl::manager::V1_0::IServiceManager> sm
            = ::android::hardware::defaultServiceManager();
    if (sm == nullptr) {
        return false;
    }
    ::android::hardware::Return<bool> success =
            sm->registerForNotifications("vendor.somc.hardware.camera.cacao@3.1::ICacao",
                    serviceName, notification);
    return success.isOk() && success;
}

static_assert(sizeof(::android::hardware::MQDescriptor<char, ::android::hardware::kSynchronizedReadWrite>) == 32, "wrong size");
static_assert(sizeof(::android::hardware::hidl_handle) == 16, "wrong size");
static_assert(sizeof(::android::hardware::hidl_memory) == 40, "wrong size");
static_assert(sizeof(::android::hardware::hidl_string) == 16, "wrong size");
static_assert(sizeof(::android::hardware::hidl_vec<char>) == 16, "wrong size");

}  // namespace V3_1
}  // namespace cacao
}  // namespace camera
}  // namespace hardware
}  // namespace somc
}  // namespace vendor
