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

ChartOfAccountsListModel::ChartOfAccountsListModel(QObject* parent)
    : QAbstractListModel(parent)
{
    m_accounts = Models::DefaultAccounts;
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
            return "Nominal code";
        }
        if (section == 1) {
            return "Account Type";
        }
        if (section == 2) {
            return "Account Title";
        }
    }

    return {};
}

void ChartOfAccountsListModel::setAccount(QModelIndex const& index, Models::Account const& account)
{
    m_accounts[index.row()] = account;

    emit dataChanged(index, index, { Qt::DisplayRole, Qt::UserRole });
}
