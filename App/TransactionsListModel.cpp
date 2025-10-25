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

#include "TransactionsListModel.h"

#include <QSqlError>
#include <QSqlQuery>

TransactionsListModel* TransactionsListModel::filter_by_journal(QObject* parent, int journal_id)
{
    auto const model = new TransactionsListModel(parent, false);
    model->m_journal_id = journal_id;
    model->reload();
    return model;
}

TransactionsListModel* TransactionsListModel::filter_by_account(QObject* parent, int account_id)
{
    auto const model = new TransactionsListModel(parent, false);
    model->m_account_id = account_id;
    model->reload();
    return model;
}

TransactionsListModel::TransactionsListModel(QObject* parent, bool load)
    : QAbstractListModel(parent)
{
    if (load)
        reload();
}

int TransactionsListModel::rowCount(QModelIndex const& parent) const
{
    return m_transactions.length();
}

int TransactionsListModel::columnCount(QModelIndex const& parent) const
{
    return 8;
}

QVariant TransactionsListModel::data(QModelIndex const& index, int role) const
{
    auto const row = index.row();
    auto const column = index.column();

    auto const transaction = m_transactions.value(row);

    if (role == Qt::DisplayRole) {
        if (column == 0) {
            return transaction.date.toString("dd/MM/yyyy");
        }
        if (column == 1) {
            return transaction.post_date.toString("dd/MM/yyyy");
        }
        if (column == 2) {
            return transaction.period.name;
        }
        if (column == 3) {
            return transaction.journal;
        }
        if (column == 4) {
            return transaction.account.code;
        }
        if (column == 5) {
            return transaction.account.title;
        }
        if (column == 6) {
            return QString("%1").arg(static_cast<double>(transaction.value) / 100.0, 0, 'f', 2);
        }
        if (column == 7) {
            return transaction.description;
        }
    }

    return {};
}

QVariant TransactionsListModel::headerData(int section, Qt::Orientation orientation, int role) const
{
    if (role == Qt::DisplayRole) {
        if (section == 0) {
            return "Date";
        }
        if (section == 1) {
            return "Post Date";
        }
        if (section == 2) {
            return "Period";
        }
        if (section == 3) {
            return "Journal";
        }
        if (section == 4) {
            return "Nominal";
        }
        if (section == 5) {
            return "Account";
        }
        if (section == 6) {
            return "Value";
        }
        if (section == 7) {
            return "Description";
        }
    }

    return {};
}

void TransactionsListModel::reload()
{
    beginResetModel();
    m_transactions.clear();

    QString sql = R"(
        SELECT accounts.id AS account_id, accounts.code, accounts.title, accounts.type, journals.id AS journal_id, journals.date, journals.post_date, periods.id AS period_id, periods.name AS period_name, transactions.value, transactions.description
        FROM transactions
        INNER JOIN accounts on accounts.id = transactions.account_id
        INNER JOIN journals on journals.id = transactions.journal_id
        INNER JOIN periods on periods.id = journals.period_id
    )";
    if (m_journal_id != -1)
        sql += " WHERE journals.id = " + QString::number(m_journal_id);
    if (m_account_id != -1)
        sql += " WHERE accounts.id = " + QString::number(m_account_id);
    sql += " ORDER BY date(journals.date) ASC, journals.id DESC, transactions.id;";

    QSqlQuery query(sql);
    if (!query.exec()) {
        endResetModel();
        return;
    }

    while (query.next()) {
        Models::Account account;
        account.id = query.value("account_id").toInt();
        account.code = query.value("code").toString();
        account.title = query.value("title").toString();
        account.type = Models::account_type_from_id(query.value("type").toInt());

        Models::PeriodOfAccount period;
        period.id = query.value("period_id").toInt();
        period.name = query.value("period_name").toString();

        Transaction transaction;
        transaction.journal = query.value("journal_id").toInt();
        transaction.date = QDate::fromString(query.value("date").toString(), "dd/MM/yyyy");
        transaction.post_date = QDate::fromString(query.value("post_date").toString(), "dd/MM/yyyy");
        transaction.period = period;
        transaction.account = account;
        transaction.value = query.value("value").toInt();
        transaction.description = query.value("description").toString();

        m_transactions.append(transaction);
    }

    endResetModel();
}
