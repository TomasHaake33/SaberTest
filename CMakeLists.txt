add_executable(SaberTest
    src/main.cpp
    src/FirstTask.cpp
    src/SecondTask.cpp
    src/ThirdTask.cpp
    include/FirstTask.h
    include/SecondTask.h
    include/ThirdTask.h)

target_include_directories(SaberTest PRIVATE include)