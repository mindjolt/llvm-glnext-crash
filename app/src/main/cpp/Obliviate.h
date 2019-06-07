//
// Created by Jesus Lopez on 4/19/19.
//

#ifndef OBLIVIATE_H
#define OBLIVIATE_H

#include <vector>
#include "Logger.h"

class Obliviate : private Logger {
public:
    explicit Obliviate();

    void bless();
    void curse();

private:
    bool detectBuggyQualcommDevice();
    void squat(uint32_t sa, uint32_t ea);

    enum State {
        kState_InitialState,
        kState_Protected,
        kState_Exposed,
        kState_Disabled
    };

    State state;
    bool isBuggyQualcommDevice;
    bool isThreadIdInitialized;
    pthread_t threadId;
    std::vector<void *> squattedAddresses;

    void unsquat();

    bool transitionTo(State state);

    void disable();
};

#endif //OBLIVIATE_H
