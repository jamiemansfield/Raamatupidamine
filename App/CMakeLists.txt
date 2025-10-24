qt_add_executable(Raamatupidamine
    DB/MigrationManager.cpp
    DB/Migrations.cpp

    Dialogs/CreateJournalDialog.cpp
    Dialogs/EditAccountDialog.cpp

    Models/Account.cpp

    Reports/TrialBalanceListModel.cpp
    Reports/TrialBalanceWindow.cpp

    Windows/ChartOfAccountsListModel.cpp
    Windows/ChartOfAccountsWindow.cpp

    GeneralLedgerListModel.cpp
    MainWindow.cpp
    main.cpp
)

target_link_libraries(Raamatupidamine PRIVATE Qt6::Widgets Qt6::Sql)
