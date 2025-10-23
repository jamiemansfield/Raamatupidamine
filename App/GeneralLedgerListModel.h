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
#include <QDateTime>

#include "Models/Account.h"

struct GeneralLedgerTransaction {
    QDate date;
    int journal;
    Models::Account account;
    int value;
    QString description;
};

class GeneralLedgerListModel : public QAbstractListModel {
    Q_OBJECT

public:
    explicit GeneralLedgerListModel(QObject* parent);

    int rowCount(QModelIndex const& parent) const override;
    int columnCount(QModelIndex const& parent) const override;

    QVariant data(QModelIndex const& index, int role) const override;
    QVariant headerData(int section, Qt::Orientation orientation, int role) const override;

    void reload();

private:
    QVector<GeneralLedgerTransaction> m_transactions;
};
