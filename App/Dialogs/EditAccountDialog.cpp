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

#include "EditAccountDialog.h"
#include "ui_EditAccountDialog.h"

namespace Dialogs {

EditAccountDialog::EditAccountDialog(QWidget* parent, Models::Account account)
    : QDialog(parent)
    , m_ui(new Ui::EditAccountDialog)
{
    m_ui->setupUi(this);

    m_ui->nominalLineEdit->setText(account.code);
    m_ui->typeComboBox->setCurrentText(Models::account_type_to_name(account.type));
    m_ui->titleLineEdit->setText(account.title);
}

EditAccountDialog::~EditAccountDialog()
{
    delete m_ui;
}

Models::Account EditAccountDialog::account() const
{
    Models::Account::Type type;
    switch (m_ui->typeComboBox->currentIndex()) {
    case 0:
        type = Models::Account::Type::NonCurrentAsset;
        break;
    case 1:
        type = Models::Account::Type::CurrentAsset;
        break;
    case 2:
        type = Models::Account::Type::NonCurrentLiability;
        break;
    case 3:
        type = Models::Account::Type::CurrentLiability;
        break;
    case 4:
        type = Models::Account::Type::Equity;
        break;
    case 5:
        type = Models::Account::Type::Income;
        break;
    case 6:
        type = Models::Account::Type::Expense;
        break;

    default:
        type = Models::Account::Type::NonCurrentAsset;
        break;
    }

    return {
        .code = m_ui->nominalLineEdit->text(),
        .title = m_ui->titleLineEdit->text(),
        .type = type,
    };
}

}
