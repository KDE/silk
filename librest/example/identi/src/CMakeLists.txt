set(identi_SRCS
   main.cpp
 )

kde4_add_executable(identi ${identi_SRCS})

target_link_libraries(identi ${KDE4_KDEUI_LIBS} rest )
