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

#include "Account.h"

#include <QSqlError>
#include <QSqlQuery>

namespace Models {

QString account_type_to_name(Account::Type const& type)
{
    switch (type) {
    case Account::Type::NonCurrentAsset:
        return "Non-current Asset";
    case Account::CurrentAsset:
        return "Current Asset";
    case Account::NonCurrentLiability:
        return "Non-current Liability";
    case Account::CurrentLiability:
        return "Current Liability";
    case Account::Equity:
        return "Equity";
    case Account::Income:
        return "Income";
    case Account::Expense:
        return "Expense";
    }

    return {};
}

Account::Type account_type_from_id(int id)
{
    switch (id) {
    case Account::Type::NonCurrentAsset:
        return Account::Type::NonCurrentAsset;
    case Account::CurrentAsset:
        return Account::CurrentAsset;
    case Account::NonCurrentLiability:
        return Account::NonCurrentLiability;
    case Account::CurrentLiability:
        return Account::CurrentLiability;
    case Account::Equity:
        return Account::Equity;
    case Account::Income:
        return Account::Income;
    case Account::Expense:
        return Account::Expense;

    default:
        return Account::NonCurrentAsset;
    }
}

int save_account(Account const& account)
{
    QSqlQuery query(QSqlDatabase::database());

    // Create new account
    if (account.id == -1) {
        query.prepare("INSERT INTO accounts (code, title, type) VALUES (:code, :title, :type);");
        query.bindValue(":code", account.code);
        query.bindValue(":title", account.title);
        query.bindValue(":type", account.type);
        query.exec();
        return query.lastInsertId().toInt();
    }

    // Update existing account
    query.prepare("UPDATE accounts SET code = :code, title = :title, type = :type WHERE id = :id;");
    query.bindValue(":id", account.id);
    query.bindValue(":code", account.code);
    query.bindValue(":title", account.title);
    query.bindValue(":type", account.type);
    query.exec();
    return account.id;
}

}
