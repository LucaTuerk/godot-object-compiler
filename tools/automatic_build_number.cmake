string(RANDOM
    LENGTH 20
    BUILD_NUMBER
)
file(WRITE ${FILE} "#pragma once\n\n#define GOC_BUILD_NUMBER \"${BUILD_NUMBER}\"\n\n")
