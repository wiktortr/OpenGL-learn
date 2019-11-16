
include(ProcessorCount)
ProcessorCount(CORES)

option(HUNTER_NO_TOOLCHAIN_ID_RECALCULATION "No Toolchain-ID recalculation" ON)

include("${CMAKE_CURRENT_LIST_DIR}/HunterGate.cmake")

set(HUNTER_JOBS_NUMBER ${CORES} CACHE STRING "Hunter jobs number")

set(
    HUNTER_CACHE_SERVERS
    "https://github.com/elucideye/hunter-cache;https://github.com/ingenue/hunter-cache"
    CACHE
    STRING
    "Hunter cache servers"
)

set(HUNTER_USE_CACHE_SERVERS "YES" CACHE STRING "Hunter use cache servers")

HunterGate(
    URL "https://github.com/ruslo/hunter/archive/v0.23.148.tar.gz"
    SHA1 "39855eac4375c6fb1d42fd0f13f03040621cb7b6"
)
