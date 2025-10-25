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

#include "PeriodsOfAccountWindow.h"
#include "../Dialogs/EditPeriodDialog.h"
#include "ui_PeriodsOfAccountWindow.h"

namespace Windows {

PeriodsOfAccountWindow::PeriodsOfAccountWindow(QWidget* parent)
    : QMainWindow(parent)
    , m_ui(new Ui::PeriodsOfAccountWindow)
    , m_list_model(new PeriodsOfAccountListModel(this))
{
    m_ui->setupUi(this);

    m_ui->treeView->setModel(m_list_model);
    m_ui->treeView->header()->setSectionResizeMode(0, QHeaderView::ResizeToContents);
    m_ui->treeView->header()->setSectionResizeMode(1, QHeaderView::ResizeToContents);
    m_ui->treeView->header()->setSectionResizeMode(2, QHeaderView::ResizeToContents);
    m_ui->treeView->header()->setSectionResizeMode(3, QHeaderView::ResizeToContents);
}

PeriodsOfAccountWindow::~PeriodsOfAccountWindow()
{
    delete m_ui;
}

void PeriodsOfAccountWindow::on_treeView_doubleClicked(QModelIndex const& index)
{
    auto const period = m_list_model->data(index, Qt::UserRole).value<Models::PeriodOfAccount>();

    Dialogs::EditPeriodDialog dialog(this, period);
    if (dialog.exec() != QDialog::Accepted) {
        return;
    }

    // Update model with new information.
    m_list_model->save_period(dialog.period());
}

void PeriodsOfAccountWindow::on_actionAdd_Period_of_Account_triggered()
{
    Dialogs::EditPeriodDialog dialog(this);
    dialog.setWindowTitle("Add Period of Account");
    if (dialog.exec() != QDialog::Accepted) {
        return;
    }

    // Add new period of account to the model.
    m_list_model->save_period(dialog.period());
}

}
