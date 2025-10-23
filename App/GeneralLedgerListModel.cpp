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

#include "GeneralLedgerListModel.h"

#include <QSqlError>
#include <QSqlQuery>

GeneralLedgerListModel::GeneralLedgerListModel(QObject* parent)
    : QAbstractListModel(parent)
{
    reload();
}

int GeneralLedgerListModel::rowCount(QModelIndex const& parent) const
{
    return m_transactions.length();
}

int GeneralLedgerListModel::columnCount(QModelIndex const& parent) const
{
    return 6;
}

QVariant GeneralLedgerListModel::data(QModelIndex const& index, int role) const
{
    auto const row = index.row();
    auto const column = index.column();

    auto const transaction = m_transactions.value(row);

    if (role == Qt::DisplayRole) {
        if (column == 0) {
            return transaction.date.toString("dd/MM/yyyy");
        }
        if (column == 1) {
            return transaction.journal;
        }
        if (column == 2) {
            return transaction.account.code;
        }
        if (column == 3) {
            return transaction.account.title;
        }
        if (column == 4) {
            return QString("%1").arg(static_cast<double>(transaction.value) / 100.0, 0, 'f', 2);
        }
        if (column == 5) {
            return transaction.description;
        }
    }

    return {};
}

QVariant GeneralLedgerListModel::headerData(int section, Qt::Orientation orientation, int role) const
{
    if (role == Qt::DisplayRole) {
        if (section == 0) {
            return "Date";
        }
        if (section == 1) {
            return "Journal";
        }
        if (section == 2) {
            return "Nominal";
        }
        if (section == 3) {
            return "Account";
        }
        if (section == 4) {
            return "Value";
        }
        if (section == 5) {
            return "Description";
        }
    }

    return {};
}

void GeneralLedgerListModel::reload()
{
    beginResetModel();
    m_transactions.clear();

    QSqlQuery query(R"(
        SELECT accounts.id, accounts.code, accounts.title, accounts.type, journals.id, journals.date, transactions.value, transactions.description
        FROM transactions
        INNER JOIN accounts on accounts.id = transactions.account_id
        INNER JOIN journals on journals.id = transactions.journal_id
        ORDER BY journals.date DESC, transactions.id;
    )");
    if (!query.exec()) {
        endResetModel();
        return;
    }

    while (query.next()) {
        Models::Account account;
        account.id = query.value(0).toInt();
        account.code = query.value(1).toString();
        account.title = query.value(2).toString();
        account.type = Models::account_type_from_id(query.value(3).toInt());

        GeneralLedgerTransaction transaction;
        transaction.journal = query.value(4).toInt();
        transaction.date = QDate::fromString(query.value(5).toString());
        transaction.account = account;
        transaction.value = query.value(6).toInt();
        transaction.description = query.value(7).toString();

        m_transactions.append(transaction);
    }

    endResetModel();
}
