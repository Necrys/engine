#pragma once

namespace engine {

enum Key: int {
    key_A = 65,
    key_D = 68,
    key_S = 83,
    key_W = 87,

    key_Esc = 256,

    kMax = 349
};

enum Action: int {
    aRelease = 0,
    aPress   = 1,
    aRepeat  = 2,

    aMax = 3
};

} //engine
