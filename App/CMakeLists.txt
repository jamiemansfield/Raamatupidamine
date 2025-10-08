qt_add_executable(Raamatupidamine
    Dialogs/EditAccountDialog.cpp

    Models/Account.cpp

    ChartOfAccountsFilterModel.cpp
    ChartOfAccountsListModel.cpp
    MainWindow.cpp
    main.cpp
)

target_link_libraries(Raamatupidamine PRIVATE Qt6::Widgets)
