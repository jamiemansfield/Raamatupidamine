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

#include "ChartOfAccountsWindow.h"
#include "ui_ChartOfAccountsWindow.h"

#include "Dialogs/EditAccountDialog.h"

ChartOfAccountsWindow::ChartOfAccountsWindow(QWidget* parent)
    : QMainWindow(parent)
    , m_ui(new Ui::ChartOfAccountsWindow)
    , m_list_model(new ChartOfAccountsListModel(this))
{
    m_ui->setupUi(this);

    m_ui->treeView->setModel(m_list_model);
    m_ui->treeView->header()->setSectionResizeMode(0, QHeaderView::ResizeToContents);
    m_ui->treeView->header()->setSectionResizeMode(1, QHeaderView::ResizeToContents);
}

ChartOfAccountsWindow::~ChartOfAccountsWindow()
{
    delete m_ui;
}

void ChartOfAccountsWindow::on_treeView_doubleClicked(QModelIndex const& index)
{
    auto const account = m_list_model->data(index, Qt::UserRole).value<Models::Account>();

    Dialogs::EditAccountDialog dialog(this, account);
    if (dialog.exec() != QDialog::Accepted) {
        return;
    }

    // Update model with new information.
    m_list_model->saveAccount(dialog.account());
}

void ChartOfAccountsWindow::on_actionAdd_Account_triggered()
{
    Dialogs::EditAccountDialog dialog(this);
    dialog.setWindowTitle("Add Account");
    if (dialog.exec() != QDialog::Accepted) {
        return;
    }

    // Add new account to the model.
    m_list_model->saveAccount(dialog.account());
}

void ChartOfAccountsWindow::on_actionAdd_Default_Accounts_triggered()
{
    for (auto const& account : Models::DefaultAccounts) {
        m_list_model->saveAccount(account);
    }
}
