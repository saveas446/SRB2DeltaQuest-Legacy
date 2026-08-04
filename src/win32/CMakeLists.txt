
set(BACKEND_SOURCES
    dx_error.c  
    mid2strm.c	
    win_dbg.c  
    win_net.c  
    win_sys.c
    fabdxlib.c  
    win_cd.c	
    win_dll.c  
    win_snd.c
    win_main.c
    win_vid.c
)

list(TRANSFORM BACKEND_SOURCES PREPEND "${CMAKE_SOURCE_DIR}/src/win32/")

# defined in src/CMakeLists.txt
add_executable(SRB2DQDD "${GENERAL_SOURCES}" "${BACKEND_SOURCES}")
target_link_libraries(SRB2DQSDL2 PRIVATE "${SRB2DQ_LINKLIBS}")
target_link_directories(SRB2DQSDL2 PRIVATE "${SRB2DQ_LINKDIRS}")
target_include_directories(SRB2DQDD PRIVATE "${SRB2DQ_INCLUDEDIRS}")
target_compile_definitions(SRB2DQSDL2 PRIVATE "${SRB2DQ_DEFINITIONS}")
target_link_options(SRB2DQSDL2 PRIVATE "${SRB2DQ_COMPILEOPTS}")

# DD stuff
target_link_libraries(SRB2DQDD PRIVATE 
	fmod
	dinput
	dxguid
	dsound
	ddraw 
	gdi32
	winmm
	wsock32
)

# WINDOWS is used for the platform, _WIN32 is when using directdraw specifically
target_compile_definitions(SRB2DQDD PRIVATE _WIN32)
