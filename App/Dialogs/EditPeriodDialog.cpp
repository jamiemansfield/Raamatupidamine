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

#include "EditPeriodDialog.h"
#include "ui_EditPeriodDialog.h"

namespace Dialogs {

EditPeriodDialog::EditPeriodDialog(QWidget* parent)
    : QDialog(parent)
    , m_ui(new Ui::EditPeriodDialog)
{
    m_ui->setupUi(this);
}

EditPeriodDialog::EditPeriodDialog(QWidget* parent, Models::PeriodOfAccount period)
    : EditPeriodDialog(parent)
{
    m_period_id = period.id;
    m_ui->nameLineEdit->setText(period.name);
    m_ui->startDateEdit->setDate(period.start_date);
    m_ui->endDateEdit->setDate(period.end_date);
    m_ui->openRadioButton->setChecked(period.open);
}

EditPeriodDialog::~EditPeriodDialog()
{
    delete m_ui;
}

Models::PeriodOfAccount EditPeriodDialog::period() const
{
    return {
        .id = m_period_id,
        .name = m_ui->nameLineEdit->text(),
        .start_date = m_ui->startDateEdit->date(),
        .end_date = m_ui->endDateEdit->date(),
        .open = m_ui->openRadioButton->isChecked()
    };
}

}
