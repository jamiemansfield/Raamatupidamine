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

#include "Migrations.h"

#include <QSqlError>

namespace DB {

void registerMigrations(MigrationManager& manager)
{
    // Initial migrations
    manager.addMigration(0, [](QSqlQuery& query) {
        query.exec(R"(
            CREATE TABLE IF NOT EXISTS accounts (
                id INTEGER,
                code TEXT UNIQUE,
                title TEXT,
                type INTEGER,
                PRIMARY KEY (id)
            );
        )");

        query.exec(R"(
            CREATE TABLE IF NOT EXISTS journals (
                id INTEGER,
                date TEXT NOT NULL DEFAULT CURRENT_TIMESTAMP,
                PRIMARY KEY (id)
            );
        )");

        query.exec(R"(
            CREATE TABLE IF NOT EXISTS transactions (
                id INTEGER,
                journal_id INTEGER REFERENCES journals,
                account_id INTEGER REFERENCES accounts,
                description TEXT NOT NULL,
                value INTEGER,
                PRIMARY KEY (id)
            );
        )");
    });
}

}
