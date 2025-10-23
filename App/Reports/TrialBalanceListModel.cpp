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

#include "TrialBalanceListModel.h"

#include <QSqlQuery>

namespace Reports {

TrialBalanceListModel::TrialBalanceListModel(QObject* parent)
    : QAbstractListModel(parent)
{
    beginResetModel();
    m_accounts.clear();

    QSqlQuery query(R"(
        SELECT code, title, IFNULL((SELECT sum(value) FROM transactions WHERE transactions.account_id = accounts.id), 0) AS balance
        FROM accounts
        ORDER BY code
    )");
    if (!query.exec()) {
        endResetModel();
        return;
    }

    while (query.next()) {
        Account account;
        account.nominal = query.value("code").toString();
        account.title = query.value("title").toString();
        account.balance = query.value("balance").toInt();
        m_accounts.append(account);
    }

    endResetModel();
}

int TrialBalanceListModel::rowCount(QModelIndex const& parent) const
{
    return m_accounts.length();
}

int TrialBalanceListModel::columnCount(QModelIndex const& parent) const
{
    return 4;
}

QVariant TrialBalanceListModel::data(QModelIndex const& index, int role) const
{
    auto const row = index.row();
    auto const column = index.column();

    auto const account = m_accounts.value(row);

    if (role == Qt::DisplayRole) {
        if (column == 0) {
            return account.nominal;
        }
        if (column == 1) {
            return account.title;
        }
        if (column == 2) {
            if (account.balance > 0) {
                auto const balance = static_cast<double>(account.balance) / 100.0;
                return QString("%1").arg(balance, 0, 'f', 2);
            }
            return {};
        }
        if (column == 3) {
            if (account.balance < 0) {
                auto const balance = static_cast<double>(-account.balance) / 100.0;
                return QString("%1").arg(balance, 0, 'f', 2);
            }
            return {};
        }
    }

    return {};
}

QVariant TrialBalanceListModel::headerData(int section, Qt::Orientation orientation, int role) const
{
    if (role == Qt::DisplayRole) {
        if (section == 0) {
            return "Nominal";
        }
        if (section == 1) {
            return "Account";
        }
        if (section == 2) {
            return "Debit";
        }
        if (section == 3) {
            return "Credit";
        }
    }

    return {};
}

}
