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

#include <QFileDialog>

namespace Windows {

TransactionsWindow::TransactionsWindow(QWidget* parent)
    : QMainWindow(parent)
    , m_ui(new Ui::TransactionsWindow)
    , m_list_model(new TransactionsListModel(this))
{
    m_ui->setupUi(this);

    m_ui->treeView->setModel(m_list_model);
    m_ui->treeView->setContextMenuPolicy(Qt::CustomContextMenu);
    m_ui->treeView->header()->setSectionResizeMode(0, QHeaderView::ResizeToContents);
    m_ui->treeView->header()->setSectionResizeMode(1, QHeaderView::ResizeToContents);
    m_ui->treeView->header()->setSectionResizeMode(2, QHeaderView::ResizeToContents);
    m_ui->treeView->header()->setSectionResizeMode(3, QHeaderView::ResizeToContents);
    m_ui->treeView->header()->setSectionResizeMode(4, QHeaderView::ResizeToContents);
    m_ui->treeView->header()->setSectionResizeMode(5, QHeaderView::ResizeToContents);
    m_ui->treeView->header()->setSectionResizeMode(6, QHeaderView::ResizeToContents);

    // Recalculate total
    connect(m_list_model, &TransactionsListModel::modelReset, [this]() {
        int total = 0;
        for (auto const& transaction : m_list_model->transactions()) {
            total += transaction.value;
        }
        m_ui->statusBar->showMessage(QString("%1").arg(static_cast<double>(total) / 100.0, 0, 'f', 2));
    });
}

TransactionsWindow::~TransactionsWindow()
{
    delete m_ui;
}

void TransactionsWindow::on_treeView_customContextMenuRequested(QPoint const& point)
{
    auto const index = m_ui->treeView->indexAt(point);
    if (!index.isValid())
        return;

    auto const transaction = m_list_model->transactions().at(index.row());

    QMenu context_menu("Context Menu", this);

    QAction view_journal("View Journal", this);
    connect(&view_journal, &QAction::triggered, [this, transaction]() {
        auto window = new TransactionsWindow(this->parentWidget());
        window->list_model()->set_journal(transaction.journal);
        window->list_model()->reload();
        window->show();
    });
    context_menu.addAction(&view_journal);

    QAction view_account("View Account", this);
    connect(&view_account, &QAction::triggered, [this, transaction]() {
        auto window = new TransactionsWindow(this->parentWidget());
        window->list_model()->set_account(transaction.account.id);
        window->list_model()->reload();
        window->show();
    });
    context_menu.addAction(&view_account);

    context_menu.exec(m_ui->treeView->viewport()->mapToGlobal(point));
}

void TransactionsWindow::on_actionExport_to_CSV_triggered()
{
    QFileDialog dialog(this);
    dialog.setWindowTitle("Save Transactions as CSV");
    dialog.setAcceptMode(QFileDialog::AcceptSave);
    dialog.setNameFilter(tr("CSV (*.csv)"));

    if (!dialog.exec() || dialog.selectedFiles().empty())
        return;

    auto file_name = dialog.selectedFiles()[0];

    QFile file(file_name);
    if (!file.open(QFile::WriteOnly))
        return;

    QTextStream stream(&file);
    stream << "Date,Post Date,Period,Journal,Nominal,Account,Value,Description\n";

    for (const auto& transaction : m_list_model->transactions()) {
        auto const value = static_cast<double>(transaction.value) / 100.0;

        stream << transaction.date.toString("dd/MM/yyyy") << ",";
        stream << transaction.post_date.toString("dd/MM/yyyy") << ",";
        stream << transaction.period.name << ",";
        stream << transaction.journal << ",";
        stream << transaction.account.code << ",";
        stream << "\"" << transaction.account.title << "\",";
        stream << QString("%1").arg(value, 0, 'f', 2) << ",";
        stream << "\"" << transaction.description << "\"";
        stream << "\n";
    }

    file.close();
}

}
