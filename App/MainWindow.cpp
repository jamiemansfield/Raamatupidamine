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

#include "MainWindow.h"
#include "ui_MainWindow.h"

#include <QMessageBox>

#include "Dialogs/CreateJournalDialog.h"
#include "Reports/TrialBalanceWindow.h"
#include "Windows/ChartOfAccountsWindow.h"
#include "Windows/PeriodsOfAccountWindow.h"
#include "Windows/TransactionsWindow.h"

MainWindow::MainWindow()
    : m_ui(new Ui::MainWindow)
    , m_list_model(new TransactionsListModel(this))
{
    m_ui->setupUi(this);

    m_list_model->reload();

    m_ui->treeView->setModel(m_list_model);
    m_ui->treeView->setContextMenuPolicy(Qt::CustomContextMenu);
    m_ui->treeView->header()->setSectionResizeMode(0, QHeaderView::ResizeToContents);
    m_ui->treeView->header()->setSectionResizeMode(1, QHeaderView::ResizeToContents);
    m_ui->treeView->header()->setSectionResizeMode(2, QHeaderView::ResizeToContents);
    m_ui->treeView->header()->setSectionResizeMode(3, QHeaderView::ResizeToContents);
    m_ui->treeView->header()->setSectionResizeMode(4, QHeaderView::ResizeToContents);
    m_ui->treeView->header()->setSectionResizeMode(5, QHeaderView::ResizeToContents);
    m_ui->treeView->header()->setSectionResizeMode(6, QHeaderView::ResizeToContents);
}

MainWindow::~MainWindow()
{
    delete m_ui;
}

void MainWindow::on_treeView_customContextMenuRequested(QPoint const& point)
{
    auto const index = m_ui->treeView->indexAt(point);
    if (!index.isValid())
        return;

    auto const transaction = m_list_model->transactions().at(index.row());

    QMenu context_menu("Context Menu", this);

    QAction view_journal("View Journal", this);
    connect(&view_journal, &QAction::triggered, [this, transaction]() {
        auto window = new Windows::TransactionsWindow(this);
        window->list_model()->set_journal(transaction.journal);
        window->list_model()->reload();
        window->show();
    });
    context_menu.addAction(&view_journal);

    QAction view_account("View Account", this);
    connect(&view_account, &QAction::triggered, [this, transaction]() {
        auto window = new Windows::TransactionsWindow(this);
        window->list_model()->set_account(transaction.account.id);
        window->list_model()->reload();
        window->show();
    });
    context_menu.addAction(&view_account);

    context_menu.exec(m_ui->treeView->viewport()->mapToGlobal(point));
}

void MainWindow::on_actionChart_of_Accounts_triggered()
{
    auto window = new Windows::ChartOfAccountsWindow(this);
    window->show();
}

void MainWindow::on_actionPeriods_of_Account_triggered()
{
    auto window = new Windows::PeriodsOfAccountWindow(this);
    window->show();
}

void MainWindow::on_actionCreate_Journal_triggered()
{
    Dialogs::CreateJournalDialog dialog(this);
    if (dialog.exec() != QDialog::Accepted) {
        return;
    }
    dialog.save_to_database();
    m_list_model->reload();
}

void MainWindow::on_actionTrial_Balance_triggered()
{
    auto window = new Reports::TrialBalanceWindow(this);
    window->show();
}

void MainWindow::on_actionAbout_Raamatupidamine_triggered()
{
    QString text;
    text += "<h2>About Raamatupidamine</h2>";
    text += "<p><a href=\"https://sonaveeb.ee/search/unif/dlall/dsall/raamatupidamine/1/est?uilang=en\">Raamatupidamine</a> is basic bookkeeping software written in Qt.</p>";
    text += "<p>Raamatupidamine is made available under the GNU Lesser General Public License v3.0.</p>";

    QMessageBox::about(this, "Raamatupidamine", text);
}

void MainWindow::on_actionAbout_Qt_triggered()
{
    QMessageBox::aboutQt(this, "Raamatupidamine");
}
