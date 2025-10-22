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

#include "ChartOfAccountsListModel.h"

#include <QSqlError>
#include <QSqlQuery>

ChartOfAccountsListModel::ChartOfAccountsListModel(QObject* parent)
    : QAbstractListModel(parent)
{
    reload();
}

int ChartOfAccountsListModel::rowCount(QModelIndex const& parent) const
{
    return m_accounts.length();
}

int ChartOfAccountsListModel::columnCount(QModelIndex const& parent) const
{
    return 3;
}

QVariant ChartOfAccountsListModel::data(QModelIndex const& index, int role) const
{
    auto const row = index.row();
    auto const column = index.column();

    auto const account = m_accounts.value(row);

    if (role == Qt::DisplayRole) {
        if (column == 0) {
            return account.code;
        }
        if (column == 1) {
            return Models::account_type_to_name(account.type);
        }
        if (column == 2) {
            return account.title;
        }
    }

    if (role == Qt::UserRole) {
        QVariant v;
        v.setValue(account);
        return v;
    }

    return {};
}

QVariant ChartOfAccountsListModel::headerData(int section, Qt::Orientation orientation, int role) const
{
    if (role == Qt::DisplayRole) {
        if (section == 0) {
            return "Nominal";
        }
        if (section == 1) {
            return "Type";
        }
        if (section == 2) {
            return "Account";
        }
    }

    return {};
}

void ChartOfAccountsListModel::saveAccount(Models::Account const& account)
{
    Models::save_account(account);
    reload();
}

void ChartOfAccountsListModel::reload()
{
    beginResetModel();
    m_accounts.clear();

    QSqlQuery query("SELECT id, code, title, type FROM accounts ORDER BY id;");
    if (!query.exec()) {
        endResetModel();
        return;
    }

    while (query.next()) {
        Models::Account account;
        account.id = query.value("id").toInt();
        account.code = query.value("code").toString();
        account.title = query.value("title").toString();
        account.type = Models::account_type_from_id(query.value("type").toInt());
        m_accounts.append(account);
    }

    endResetModel();
}
