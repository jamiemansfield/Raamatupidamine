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

#include "MigrationManager.h"

#include <functional>

#include <QSqlQuery>

namespace DB {

MigrationManager::MigrationManager()
{
    // Ensure schema_migrations table exists
    QSqlQuery query(QSqlDatabase::database());
    query.exec("CREATE TABLE IF NOT EXISTS schema_migrations (version INTEGER);");

    // Get last migration version
    query.exec("SELECT version from schema_migrations LIMIT 1;");
    if (query.next()) {
        m_last_migration_version = query.value(0).toInt();
    }
}

void MigrationManager::add_migration(int version, std::function<void(QSqlQuery&)> const& function)
{
    if (version > m_last_migration_version) {
        auto const db = QSqlDatabase::database();

        QSqlQuery migration_query(db);
        function(migration_query);

        // Set last migration version
        QSqlQuery schema_query(db);
        schema_query.exec("DELETE FROM schema_migrations;");
        schema_query.prepare("INSERT INTO schema_migrations (version) VALUES (?);");
        schema_query.addBindValue(version);
        schema_query.exec();
        m_last_migration_version = version;
    }
}

}
