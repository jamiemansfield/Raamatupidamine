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

#include "CreateJournalDialog.h"
#include "ui_CreateJournalDialog.h"
#include "ui_CreateTransactionDialog.h"

#include <QMenu>
#include <QSqlQuery>

#include "../Models/PeriodOfAccount.h"

namespace Dialogs {

CreateJournalDialog::CreateJournalDialog(QWidget* parent)
    : QDialog(parent)
    , m_ui(new Ui::CreateJournalDialog)
    , m_list_model(new CreateJournalListModel(this))
{
    m_ui->setupUi(this);

    m_ui->dateEdit->setDate(QDate::currentDate());

    for (auto const& period : Models::get_periods(true)) {
        m_ui->periodComboBox->addItem(period.name, QVariant::fromValue(period));
    }

    m_ui->treeView->setModel(m_list_model);
    m_ui->treeView->setContextMenuPolicy(Qt::CustomContextMenu);
    m_ui->treeView->header()->setSectionResizeMode(0, QHeaderView::ResizeToContents);
    m_ui->treeView->header()->setSectionResizeMode(1, QHeaderView::ResizeToContents);
    m_ui->treeView->header()->setSectionResizeMode(2, QHeaderView::ResizeToContents);

    m_ui->buttonBox->button(QDialogButtonBox::Ok)->setEnabled(false);
}

CreateJournalDialog::~CreateJournalDialog()
{
    delete m_ui;
}

void CreateJournalDialog::save_to_database()
{
    // Create journals entry
    QSqlQuery journal_query(QSqlDatabase::database());
    journal_query.prepare("INSERT INTO journals (date, post_date, period_id) VALUES (:date, :post_date, :period_id);");
    journal_query.bindValue(":date", m_ui->dateEdit->date().toString("dd/MM/yyyy"));
    journal_query.bindValue(":post_date", QDate::currentDate().toString("dd/MM/yyyy"));
    journal_query.bindValue(":period_id", m_ui->periodComboBox->currentData().value<Models::PeriodOfAccount>().id);
    journal_query.exec();
    auto const journal_id = journal_query.lastInsertId().toInt();

    // Create transactions entries
    for (auto const& transaction : m_list_model->transactions()) {
        QSqlQuery transaction_query(QSqlDatabase::database());
        transaction_query.prepare("INSERT INTO transactions (journal_id, account_id, description, value) VALUES (:journal_id, :account_id, :description, :value);");
        transaction_query.bindValue(":journal_id", journal_id);
        transaction_query.bindValue(":account_id", transaction.account.id);
        transaction_query.bindValue(":description", transaction.description);
        transaction_query.bindValue(":value", transaction.value);
        transaction_query.exec();
    }
}

void CreateJournalDialog::update_balance()
{
    auto const balance = static_cast<double>(m_list_model->current_balance()) / 100;
    m_ui->balanceLineEdit->setText(QString("%1").arg(balance, 0, 'f', 2));

    if (m_list_model->transactions().empty()) {
        m_ui->buttonBox->button(QDialogButtonBox::Ok)->setEnabled(false);
    } else {
        m_ui->buttonBox->button(QDialogButtonBox::Ok)->setEnabled(m_list_model->current_balance() == 0);
    }
}

void CreateJournalDialog::on_addTransactionButton_clicked()
{
    CreateTransactionDialog dialog(this);
    if (dialog.exec() != Accepted) {
        return;
    }

    // Add transaction to list model
    m_list_model->add_transaction(dialog.transaction());

    // Update balance
    update_balance();
}

void CreateJournalDialog::on_treeView_doubleClicked(QModelIndex const& index)
{
    if (!index.isValid())
        return;

    auto const transaction = m_list_model->transactions().value(index.row());

    CreateTransactionDialog dialog(this, transaction);
    if (dialog.exec() != Accepted) {
        return;
    }

    m_list_model->replace_transaction(index.row(), dialog.transaction());
    update_balance();
}

void CreateJournalDialog::on_treeView_customContextMenuRequested(QPoint const& point)
{
    auto const index = m_ui->treeView->indexAt(point);
    if (!index.isValid())
        return;

    auto const transaction = m_list_model->transactions().value(index.row());

    QMenu context_menu("Context Menu", this);

    QAction delete_transaction("Delete", this);
    connect(&delete_transaction, &QAction::triggered, [this, index]() {
        m_list_model->remove_transaction(index.row());
        update_balance();
    });
    context_menu.addAction(&delete_transaction);

    context_menu.exec(m_ui->treeView->viewport()->mapToGlobal(point));
}

CreateTransactionDialog::CreateTransactionDialog(QWidget* parent)
    : QDialog(parent)
    , m_ui(new Ui::CreateTransactionDialog)
{
    m_ui->setupUi(this);

    // Query nominals from the database
    QSqlQuery query("SELECT id, code, title FROM accounts ORDER BY code;");
    if (query.exec()) {
        while (query.next()) {
            Models::Account account;
            account.id = query.value("id").toInt();
            account.code = query.value("code").toString();
            account.title = query.value("title").toString();

            m_ui->nominalComboBox->addItem(
                account.code + " - " + account.title,
                QVariant::fromValue(account));
        }
    }
}

CreateTransactionDialog::CreateTransactionDialog(QWidget* parent, Transaction const& transaction)
    : CreateTransactionDialog(parent)
{
    setWindowTitle("Edit transaction");

    auto const value = static_cast<double>(transaction.value) / 100;

    m_ui->nominalComboBox->setCurrentText(transaction.account.code + " - " + transaction.account.title);
    m_ui->valueLineEdit->setText(QString("%1").arg(value, 0, 'f', 2));
    m_ui->descriptionLineEdit->setText(transaction.description);
}

CreateTransactionDialog::~CreateTransactionDialog()
{
    delete m_ui;
}

Transaction CreateTransactionDialog::transaction() const
{
    auto const account = m_ui->nominalComboBox->currentData().value<Models::Account>();
    auto const value = static_cast<int>(m_ui->valueLineEdit->text().toDouble() * 100);
    auto const description = m_ui->descriptionLineEdit->text();

    return { account, value, description };
}

CreateJournalListModel::CreateJournalListModel(QObject* parent)
    : QAbstractListModel(parent)
{
}

int CreateJournalListModel::rowCount(QModelIndex const& parent) const
{
    return m_transactions.size();
}

int CreateJournalListModel::columnCount(QModelIndex const& parent) const
{
    return 4;
}

QVariant CreateJournalListModel::data(QModelIndex const& index, int role) const
{
    auto const row = index.row();
    auto const column = index.column();

    auto const transaction = m_transactions.value(row);

    if (role == Qt::DisplayRole) {
        if (column == 0) {
            return transaction.account.code;
        }
        if (column == 1) {
            return transaction.account.title;
        }
        if (column == 2) {
            auto const value = static_cast<double>(transaction.value) / 100.0;
            return QString("%1").arg(value, 0, 'f', 2);
        }
        if (column == 3) {
            return transaction.description;
        }
    }

    return {};
}

QVariant CreateJournalListModel::headerData(int section, Qt::Orientation orientation, int role) const
{
    if (role == Qt::DisplayRole) {
        if (section == 0) {
            return "Nominal";
        }
        if (section == 1) {
            return "Account";
        }
        if (section == 2) {
            return "Value";
        }
        if (section == 3) {
            return "Description";
        }
    }

    return {};
}

void CreateJournalListModel::add_transaction(Transaction const& transaction)
{
    beginInsertRows(QModelIndex(), m_transactions.size(), m_transactions.size());
    m_transactions.push_back(transaction);
    endInsertRows();
}

int CreateJournalListModel::current_balance() const
{
    int balance = 0;
    for (auto const& transaction : m_transactions) {
        balance += transaction.value;
    }
    return balance;
}

void CreateJournalListModel::remove_transaction(int row)
{
    beginRemoveRows(QModelIndex(), row, row);
    m_transactions.remove(row);
    endRemoveRows();
}

void CreateJournalListModel::replace_transaction(int row, Transaction const& transaction)
{
    m_transactions.replace(row, transaction);

    auto const left = index(row, 0);
    auto const right = index(row, 3);
    emit dataChanged(left, right);
}

}
