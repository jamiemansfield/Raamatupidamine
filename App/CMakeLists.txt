qt_add_executable(Raamatupidamine
    DB/MigrationManager.cpp
    DB/Migrations.cpp

    Dialogs/EditAccountDialog.cpp

    Models/Account.cpp

    ChartOfAccountsFilterModel.cpp
    ChartOfAccountsListModel.cpp
    MainWindow.cpp
    main.cpp
)

target_link_libraries(Raamatupidamine PRIVATE Qt6::Widgets Qt6::Sql)
