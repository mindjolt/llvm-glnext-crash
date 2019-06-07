//
// Created by Jesus Lopez on 4/19/19.
//
#include <sys/mman.h>
#include <sys/stat.h>
#include <pthread.h>
#include <cerrno>

#include "Obliviate.h"

static const size_t BUGGY_SIZE = 11806716;
static const int SQUAT_SIZE = 0x10000;

#ifndef MAP_FIXED_NOREPLACE
#define MAP_FIXED_NOREPLACE 0x100000
#endif

Obliviate::Obliviate() :
    Logger("Obliviate"),
    state(kState_InitialState),
    isBuggyQualcommDevice(detectBuggyQualcommDevice()),
    isThreadIdInitialized(false) {

    bless();
}

bool Obliviate::transitionTo(Obliviate::State newState) {
    if (!isBuggyQualcommDevice)
        return false;

    if (state == newState)
        return false;

    if (isThreadIdInitialized) {
        pthread_t currentThreadId = pthread_self();
        if (currentThreadId != threadId) {
            error("Ignoring request coming from different thread %d != %d",
                    currentThreadId, threadId);
            return false;
        }
    } else if (state != kState_InitialState) {
        threadId = pthread_self();
        isThreadIdInitialized = true;
    }

    // info("Entering mode %d", newState);
    state = newState;
    return true;
}

void Obliviate::bless() {
    if (transitionTo(kState_Protected)) {
        unsquat();
        squat(0x40000000, 0x80000000);
    }
}

void Obliviate::curse() {
    if (transitionTo(kState_Exposed)) {
        unsquat();
        squat(0x00000000, 0x40000000);
        squat(0x80000000, 0x00000000);
    }
}

void Obliviate::disable() {
    if (transitionTo(kState_Disabled)) {
        unsquat();
    }
}

void Obliviate::squat(uint32_t sa, uint32_t ea) {
    int flags = MAP_PRIVATE | MAP_ANONYMOUS | MAP_FIXED_NOREPLACE; // NOLINT(hicpp-signed-bitwise)
    int mappedCount = 0;
    int remappedCount = 0;
    int rejectedCount = 0;
    int failedCount = 0;
    for (uint32_t addr = sa; addr != ea; addr += SQUAT_SIZE) {
        void *ptr = mmap(reinterpret_cast<void *>(addr), SQUAT_SIZE, PROT_NONE, flags, -1, 0);
        auto obtained = reinterpret_cast<uint32_t>(ptr);
        if (ptr == MAP_FAILED) {
            debug("Unable to map 0x%08X: %d %s", addr, errno, strerror(errno));
            failedCount++;
        } else if (obtained != addr) {
            if (obtained >= sa && obtained < ea) {
                debug("Remapped but accepting it: 0x%08x != 0x%08x", obtained, addr);
                remappedCount++;
            } else {
                debug("Bad mapping obtained. 0x%08x != 0x%08x. Dropping.", obtained, addr);
                int st = munmap(ptr, SQUAT_SIZE);
                if (st == -1)
                    error("Unable to unmap %p: %d %s", ptr, errno, strerror(errno));
                rejectedCount++;
            }
        } else {
            squattedAddresses.push_back(ptr);
            mappedCount++;
        }
    }
    // info("Squat: Mapped %d, Remapped %d, Rejected %d, Failed %d", mappedCount, remappedCount, rejectedCount, failedCount);
}

void Obliviate::unsquat() {
    for (auto ptr : squattedAddresses) {
        int st = munmap(ptr, SQUAT_SIZE);
        if (st == -1) {
            error("Unable to unmap 0x%08X: %d %s", ptr, errno, strerror(errno));
        }
    }
    squattedAddresses.clear();
}

#pragma clang diagnostic push
#pragma ide diagnostic ignored "OCSimplifyInspection"
#pragma ide diagnostic ignored "OCDFAInspection"
bool Obliviate::detectBuggyQualcommDevice() {
    if (sizeof(void *) != 4) {
        info("Running in 64-bit mode. Obliviate disabled");
        return false;
    }

    struct stat sb = { 0 };
    int st = stat("/vendor/lib/libllvm-glnext.so", &sb);
    if (st == -1) {
        info("libllvm-glnext.so library not found. Obliviate disabled");
        return false;
    }
    if (sb.st_size != BUGGY_SIZE) {
        info("libllvm-glnext.so has different size than buggy version: %d != %d. Obliviate disabled",
                sb.st_size, BUGGY_SIZE);
        return false;
    }
    warn("Buggy libllvm-glnext.so detected");
    return true;
}

#pragma clang diagnostic pop