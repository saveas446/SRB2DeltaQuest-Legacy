set(BACKEND_SOURCES
    dosstr.c
    i_cdmus.c
    i_main.c
    i_net.c
    i_sound.c
    i_system.c
    i_video.c
)

list(TRANSFORM BACKEND_SOURCES PREPEND "${CMAKE_SOURCE_DIR}/src/sdl2/")

# defined in src/CMakeLists.txt
add_executable(SRB2DQSDL2 "${GENERAL_SOURCES}" "${BACKEND_SOURCES}")
target_link_libraries(SRB2DQSDL2 PRIVATE "${SRB2DQ_LINKLIBS}")
target_link_directories(SRB2DQSDL2 PRIVATE "${SRB2DQ_LINKDIRS}")
target_include_directories(SRB2DQSDL2 PRIVATE "${SRB2DQ_INCLUDEDIRS}")
target_compile_definitions(SRB2DQSDL2 PRIVATE "${SRB2DQ_DEFINITIONS}")
target_link_options(SRB2DQSDL2 PRIVATE "${SRB2DQ_COMPILEOPTS}")

# todo: add mixer when sound is added
target_link_libraries(SRB2DQSDL2 PRIVATE SDL2)
target_compile_definitions(SRB2DQSDL2 PRIVATE SDL SRB2_SDL2)
