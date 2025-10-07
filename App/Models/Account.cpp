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

#include "Account.h"

namespace Models {

QString account_type_to_name(Account::Type type)
{
    switch (type) {
    case Account::Type::NonCurrentAsset:
        return "Non-current Asset";
    case Account::CurrentAsset:
        return "Current Asset";
    case Account::NonCurrentLiability:
        return "Non-current Liability";
    case Account::CurrentLiability:
        return "Current Liability";
    case Account::Equity:
        return "Equity";
    case Account::Income:
        return "Income";
    case Account::Expense:
        return "Expense";
    }

    return {};
}

}
