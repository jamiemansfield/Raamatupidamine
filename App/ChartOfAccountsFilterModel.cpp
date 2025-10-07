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

#include "ChartOfAccountsFilterModel.h"

#include "Models/Account.h"

ChartOfAccountsFilterModel::ChartOfAccountsFilterModel(QObject* parent) : QSortFilterProxyModel(parent) {}

bool ChartOfAccountsFilterModel::filterAcceptsRow(int sourceRow, const QModelIndex& sourceParent) const
{
    return true;
}

bool ChartOfAccountsFilterModel::lessThan(const QModelIndex& left, const QModelIndex& right) const
{
    auto leftAccount = sourceModel()->data(left, Qt::UserRole).value<Models::Account>();
    auto rightAccount = sourceModel()->data(right, Qt::UserRole).value<Models::Account>();

    return QString::compare(leftAccount.code, rightAccount.code) > 0;
}
