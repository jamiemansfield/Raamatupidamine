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

#pragma once

#include <QAbstractListModel>
#include <QDialog>

#include "../Models/Account.h"

namespace Ui {

class CreateJournalDialog;
class CreateTransactionDialog;

}

namespace Dialogs {

struct Transaction {
    Models::Account account;
    int value;
    QString description;
};

class CreateJournalListModel : public QAbstractListModel {
    Q_OBJECT

public:
    explicit CreateJournalListModel(QObject* parent);

    int rowCount(QModelIndex const& parent) const override;
    int columnCount(QModelIndex const& parent) const override;

    QVariant data(QModelIndex const& index, int role) const override;
    QVariant headerData(int section, Qt::Orientation orientation, int role) const override;

    QVector<Transaction> transactions() const { return m_transactions; }
    void add_transaction(Transaction const& transaction);

    int current_balance() const;

    void remove_transaction(int row);

private:
    QVector<Transaction> m_transactions;
};

class CreateJournalDialog : public QDialog {
    Q_OBJECT

public:
    explicit CreateJournalDialog(QWidget* parent);
    ~CreateJournalDialog() override;

    void save_to_database();

    void update_balance();

private slots:
    void on_addTransactionButton_clicked();

    void on_treeView_customContextMenuRequested(QPoint const& point);

private:
    Ui::CreateJournalDialog* m_ui;

    CreateJournalListModel* m_list_model;
};

class CreateTransactionDialog : public QDialog {
    Q_OBJECT

public:
    explicit CreateTransactionDialog(QWidget* parent);
    ~CreateTransactionDialog() override;

    Transaction transaction() const;

private:
    Ui::CreateTransactionDialog* m_ui;
};

}
