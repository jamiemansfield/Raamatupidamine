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

#include "PeriodsOfAccountListModel.h"

#include <QSqlQuery>

namespace Windows {

PeriodsOfAccountListModel::PeriodsOfAccountListModel(QObject* parent)
    : QAbstractListModel(parent)
{
    reload();
}

int PeriodsOfAccountListModel::rowCount(QModelIndex const& parent) const
{
    return m_periods.size();
}

int PeriodsOfAccountListModel::columnCount(QModelIndex const& parent) const
{
    return 4;
}

QVariant PeriodsOfAccountListModel::data(QModelIndex const& index, int role) const
{
    if (!index.isValid())
        return {};

    auto const row = index.row();
    auto const column = index.column();

    auto const period = m_periods.value(row);

    if (role == Qt::DisplayRole) {
        if (column == 0) {
            return period.start_date.toString("dd/MM/yyyy");
        }
        if (column == 1) {
            return period.end_date.toString("dd/MM/yyyy");
        }
        if (column == 2) {
            return period.name;
        }
        if (column == 3) {
            return period.open;
        }
    }

    if (role == Qt::UserRole) {
        return QVariant::fromValue(period);
    }

    return {};
}

QVariant PeriodsOfAccountListModel::headerData(int section, Qt::Orientation orientation, int role) const
{
    if (role == Qt::DisplayRole) {
        if (section == 0) {
            return "Start Date";
        }
        if (section == 1) {
            return "End Date";
        }
        if (section == 2) {
            return "Name";
        }
        if (section == 3) {
            return "Closed";
        }
    }

    return {};
}

void PeriodsOfAccountListModel::save_period(Models::PeriodOfAccount const& period)
{
    Models::save_period(period);
    reload();
}

void PeriodsOfAccountListModel::reload()
{
    beginResetModel();
    m_periods = Models::get_periods();
    endResetModel();
}

}
