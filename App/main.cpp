/*
 * Copyright 2025 Jamie Mansfield <jmansfield@cadixdev.org>
 *
 * Raamatupidamine is free software: you can redistribute it and/or modify
 * it under the terms of the GNU Lesser General Public License as
 * published by the Free Software Foundation, either version 3 of the
 * License, or (at your option) any later version.
 *
 * Raamatupidamine is distributed in the hope that it will be useful,
 * but WITHOUT ANY WARRANTY; without even the implied warranty of
 * MERCHANTABILITY or FITNESS FOR A PARTICULAR PURPOSE. See the
 * GNU Lesser General Public License for more details.
 *
 * You should have received a copy of the GNU Lesser General Public
 * License along with Raamatupidamine. If not, see <https://www.gnu.org/licenses/>.
 */

#include <QApplication>
#include <QSqlError>

#include "DB/MigrationManager.h"
#include "DB/Migrations.h"
#include "MainWindow.h"

int main(int argc, char* argv[])
{
#ifdef Q_OS_WIN
    // Set Windows Vista style if running on Windows.
    // This isn't perfect (doesn't support dark mode for example), but looks
    // better than the default (IMO).
    // I'll look into https://github.com/oclero/qlementine in the future.
    QApplication::setStyle("windowsvista");
#endif

    QApplication a(argc, argv);

    QSqlDatabase db = QSqlDatabase::addDatabase("QSQLITE");
    db.setDatabaseName("raamatupidamine.db");
    db.open();

    DB::MigrationManager migration_manager;
    DB::register_migrations(migration_manager);

    MainWindow window;
    window.show();

    return QApplication::exec();
}
