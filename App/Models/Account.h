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

#include <QVector>

namespace Models {

struct Account {
    enum Type : int {
        NonCurrentAsset = 0,
        CurrentAsset = 1,

        NonCurrentLiability = 2,
        CurrentLiability = 3,

        Equity = 4,

        Income = 5,
        Expense = 6,
    };

    QString code;
    QString title;
    Type type;
};

QString account_type_to_name(Account::Type type);

const QVector<Account> DefaultAccounts = {
    // Equity
    Account {
        .code = "9000",
        .title = "Opening balances",
        .type = Account::Equity,
    },
    Account {
        .code = "9001",
        .title = "Share capital",
        .type = Account::Equity,
    },
    Account {
        .code = "9002",
        .title = "Retained earnings",
        .type = Account::Equity,
    },

    // Assets
    Account {
        .code = "1000",
        .title = "Cash",
        .type = Account::CurrentAsset,
    },
    Account {
        .code = "1001",
        .title = "Bank",
        .type = Account::CurrentAsset,
    },
    Account {
        .code = "1002",
        .title = "Sales Ledger Control Account",
        .type = Account::CurrentAsset,
    },

    // Liabilities
    Account {
        .code = "6002",
        .title = "Purchase Ledger Control Account",
        .type = Account::CurrentLiability,
    },

    // Income
    Account {
        .code = "0001",
        .title = "Sales",
        .type = Account::Income,
    },

    // Expenses
    Account {
        .code = "2000",
        .title = "Purchases",
        .type = Account::Expense,
    },
};

}
