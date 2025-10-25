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

#include "PeriodOfAccount.h"

#include <QSqlQuery>

namespace Models {

QVector<PeriodOfAccount> get_periods(bool must_be_open)
{
    QVector<PeriodOfAccount> period_of_accounts;

    QString sql = "SELECT id, start_date, end_date, name, open FROM periods";
    if (must_be_open)
        sql += " WHERE open = TRUE";
    sql += " ORDER BY name;";

    QSqlQuery query(sql);
    if (!query.exec()) {
        return period_of_accounts;
    }

    while (query.next()) {
        PeriodOfAccount period;
        period.id = query.value("id").toInt();
        period.start_date = QDate::fromString(query.value("start_date").toString(), "dd/MM/yyyy");
        period.end_date = QDate::fromString(query.value("end_date").toString(), "dd/MM/yyyy");
        period.name = query.value("name").toString();
        period.open = query.value("open").toBool();
        period_of_accounts.append(period);
    }

    return period_of_accounts;
}

int save_period(PeriodOfAccount const& period)
{
    QSqlQuery query(QSqlDatabase::database());

    // Create new period of account
    if (period.id == -1) {
        query.prepare("INSERT INTO periods (name, start_date, end_date, open) VALUES (:name, :start_date, :end_date, :open);");
        query.bindValue(":name", period.name);
        query.bindValue(":start_date", period.start_date.toString("dd/MM/yyyy"));
        query.bindValue(":end_date", period.end_date.toString("dd/MM/yyyy"));
        query.bindValue(":open", period.open);
        query.exec();
        return query.lastInsertId().toInt();
    }

    // Update existing period of account
    query.prepare("UPDATE periods SET name = :name, start_date = :start_date, end_date = :end_date, open = :open WHERE id = :id;");
    query.bindValue(":id", period.id);
    query.bindValue(":name", period.name);
    query.bindValue(":start_date", period.start_date.toString("dd/MM/yyyy"));
    query.bindValue(":end_date", period.end_date.toString("dd/MM/yyyy"));
    query.bindValue(":open", period.open);
    query.exec();
    return period.id;
}

}
