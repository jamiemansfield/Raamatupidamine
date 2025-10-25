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

#include "TransactionsWindow.h"
#include "ui_TransactionsWindow.h"

namespace Windows {

TransactionsWindow* TransactionsWindow::filter_by_journal(QWidget* parent, int journal_id)
{
    return new TransactionsWindow(parent, TransactionsListModel::filter_by_journal(parent, journal_id));
}

TransactionsWindow* TransactionsWindow::filter_by_account(QWidget* parent, int account_id)
{
    return new TransactionsWindow(parent, TransactionsListModel::filter_by_account(parent, account_id));
}

TransactionsWindow::TransactionsWindow(QWidget* parent, TransactionsListModel* list_model)
    : QMainWindow(parent)
    , m_ui(new Ui::TransactionsWindow)
    , m_list_model(list_model)
{
    m_ui->setupUi(this);

    m_ui->treeView->setModel(m_list_model);
    m_ui->treeView->header()->setSectionResizeMode(0, QHeaderView::ResizeToContents);
    m_ui->treeView->header()->setSectionResizeMode(1, QHeaderView::ResizeToContents);
    m_ui->treeView->header()->setSectionResizeMode(2, QHeaderView::ResizeToContents);
    m_ui->treeView->header()->setSectionResizeMode(3, QHeaderView::ResizeToContents);
    m_ui->treeView->header()->setSectionResizeMode(4, QHeaderView::ResizeToContents);
    m_ui->treeView->header()->setSectionResizeMode(5, QHeaderView::ResizeToContents);
    m_ui->treeView->header()->setSectionResizeMode(6, QHeaderView::ResizeToContents);
}

TransactionsWindow::~TransactionsWindow()
{
    delete m_ui;
}

}
