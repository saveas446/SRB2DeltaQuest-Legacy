cmake_minimum_required(VERSION 3.10)

project(demo2ultimate C)

add_executable(${PROJECT_NAME}
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
  
  win32/dx_error.c  
  win32/mid2strm.c	
  win32/win_dbg.c  
  win32/win_net.c  
  win32/win_sys.c
  win32/fabdxlib.c  
  win32/win_cd.c	
  win32/win_dll.c  
  win32/win_snd.c
  win32/win_main.c
  win32/win_vid.c
)



target_link_directories(${PROJECT_NAME} PRIVATE "${CMAKE_SOURCE_DIR}/../lib")

target_link_libraries(${PROJECT_NAME} PRIVATE 
	fmod
	dinput
	dxguid 
	dsound 
	ddraw 
	gdi32
	winmm 
	wsock32
)

# NOW I do. 
target_compile_options(${PROJECT_NAME} PRIVATE
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


target_include_directories(${PROJECT_NAME} PRIVATE 
	"${CMAKE_SOURCE_DIR}/../include"
)
