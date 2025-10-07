qt_add_executable(Raamatupidamine
    MainWindow.cpp
    main.cpp
)

target_link_libraries(Raamatupidamine PRIVATE Qt6::Widgets)
