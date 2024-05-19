set(HOT_RELOAD_LIB_NAME "HotReloadLib")
set(HOT_RELOAD_SRC "hot_reload.cpp")
set(HOT_RELOAD_HEADER "hot_reload.h")

add_library(${HOT_RELOAD_LIB_NAME} SHARED ${HOT_RELOAD_SRC} ${HOT_RELOAD_HEADER})