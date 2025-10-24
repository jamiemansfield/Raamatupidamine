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

EditAccountDialog::EditAccountDialog(QWidget* parent)
    : QDialog(parent)
    , m_ui(new Ui::EditAccountDialog)
{
    m_ui->setupUi(this);

    m_ui->typeComboBox->addItem("Non-current Asset", QVariant::fromValue(Models::Account::Type::NonCurrentAsset));
    m_ui->typeComboBox->addItem("Current Asset", QVariant::fromValue(Models::Account::Type::CurrentAsset));
    m_ui->typeComboBox->addItem("Non-current Liability", QVariant::fromValue(Models::Account::Type::NonCurrentLiability));
    m_ui->typeComboBox->addItem("Current Liability", QVariant::fromValue(Models::Account::Type::CurrentLiability));
    m_ui->typeComboBox->addItem("Equity", QVariant::fromValue(Models::Account::Type::Equity));
    m_ui->typeComboBox->addItem("Income", QVariant::fromValue(Models::Account::Type::Income));
    m_ui->typeComboBox->addItem("Expense", QVariant::fromValue(Models::Account::Type::Expense));
}

EditAccountDialog::EditAccountDialog(QWidget* parent, Models::Account account)
    : EditAccountDialog(parent)
{
    m_account_id = account.id;
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
    return {
        .id = m_account_id,
        .code = m_ui->nominalLineEdit->text(),
        .title = m_ui->titleLineEdit->text(),
        .type = m_ui->typeComboBox->currentData().value<Models::Account::Type>(),
    };
}

}
