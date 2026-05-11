// binder_sd_helpers.h — Binder SerializedData 序列化輔助函式（共用）
// 供 ICacao.cpp, ICacaoService.cpp, ICacaoClient.cpp, ICacaoProcessCallback.cpp 使用
// 來源: libcacao_client.so Ghidra 反編譯 — 4 個翻譯單元共用相同邏輯
#pragma once

#include <binder/IMemory.h>
#include <binder/Parcel.h>
#include <string.h>
#include <unistd.h>

#include <cacao/ProcessCtrlGatewayBase.h>

// 從 Binder Parcel 重建 ISerialize::SerializedData
// 協議: fd_count(int32) + fd_count 個 FD + IMemory 的 size/data 填入 sd
static inline void reconstruct_sd(const android::Parcel& data,
                                   const android::sp<android::IMemory>& mem,
                                   cacao::ISerialize::SerializedData& sd)
{
    memset(&sd, 0, sizeof(sd));
    int fdCount = data.readInt32();
    int* padInts = reinterpret_cast<int*>(&sd);
    padInts[0] = fdCount;
    for (int i = 0; i < fdCount && i < 95; i++) {
        padInts[i + 1] = ::dup(data.readFileDescriptor());
    }
    if (mem != nullptr) {
        sd.size = (uint32_t)mem->size();
        sd.data = mem->unsecurePointer();
    }
}

// 將 ISerialize::SerializedData 序列化到 Binder Parcel (proxy 端使用)
static inline void write_sd(android::Parcel& data,
                             const cacao::ISerialize::SerializedData& sd)
{
    const int* padInts = reinterpret_cast<const int*>(&sd);
    int fdCount = padInts[0];
    data.writeInt32(fdCount);
    for (int i = 0; i < fdCount && i < 95; i++) {
        data.writeFileDescriptor(padInts[i + 1]);
    }
}