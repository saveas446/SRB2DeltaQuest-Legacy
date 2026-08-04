set(GENERAL_SOURCES
  am_map.c
  command.c
  comptime.c
  d_clisrv.c
  d_main.c
  d_net.c
  d_netfil.c
  dehacked.c
  dstrings.c
  f_finale.c
  f_wipe.c
  filesrch.c
  g_input.c
  hu_stuff.c
  i_tcp.c
  m_argv.c
  m_bbox.c
  m_cheat.c
  m_filewriter.c
  m_fixed.c
  m_misc.c
  m_random.c
  md5.c
  mserv.c
  p_ceilng.c
  p_doors.c
  p_lights.c
  p_map.c
  p_maputl.c
  p_saveg.c
  p_sight.c
  p_telept.c
  p_tick.c
  r_data.c
  r_draw.c
  r_draw16.c
  r_draw8.c
  r_main.c
  r_plane.c
  r_segs.c
  r_sky.c
  r_splats.c
  screen.c
  sounds.c
  st_stuff.c
  string.c
  tables.c
  v_video.c
  w_wad.c
  y_inter.c
  z_zone.c
  console.c
  d_netcmd.c
  g_game.c
  info.c
  m_menu.c
  p_enemy.c
  p_fab.c
  p_floor.c
  p_inter.c
  p_mobj.c
  p_setup.c
  p_spec.c
  p_user.c
  r_bsp.c
  r_things.c
  s_sound.c
)

list(TRANSFORM GENERAL_SOURCES PREPEND "${CMAKE_SOURCE_DIR}/src/")

# OS specific stuff
# i dont know if windows works
if ("${CMAKE_SYSTEM_NAME}" STREQUAL "Windows")
    set(SRB2DQ_LINKDIRS "${CMAKE_SOURCE_DIR}/../lib")
    set(SRB2DQ_INCLUDEDIRS "${CMAKE_SOURCE_DIR}/../include")
    set(SRB2DQ_COMPILEOPTS
      $<$<C_COMPILER_ID:GNU,Clang>:
        -fno-pie
        -Wno-int-conversion
        -fpermissive
      >
      $<$<C_COMPILER_ID:MSVC>:
        /wd4244         # disable conversion warnings
        /permissive-    # make MSVC less permissive (closer to standard)
      >
    )
    set(SRB2DQ_DEFINITIONS WINDOWS)
elseif ("${CMAKE_SYSTEM_NAME}" STREQUAL "Linux")
    set(SRB2DQ_LINKLIBS m z png) # math
    set(SRB2DQ_DEFINITIONS UNIXLIKE)
endif()

if ("${SRB2DQ_CONFIG_BACKEND}" STREQUAL "sdl2")
  add_subdirectory(sdl2)
elseif ("${SRB2DQ_CONFIG_BACKEND}" STREQUAL "win32")
  add_subdirectory(win32)
else()
  message(FATAL_ERROR "Unknown Backend")
endif()
