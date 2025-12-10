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

#include "TrialBalanceWindow.h"
#include "ui_TrialBalanceWindow.h"

#include <QFileDialog>

namespace Reports {

TrialBalanceWindow::TrialBalanceWindow(QWidget* parent)
    : QMainWindow(parent)
    , m_ui(new Ui::TrialBalanceWindow)
    , m_list_model(new TrialBalanceListModel(this))
{
    m_ui->setupUi(this);

    m_ui->treeView->setModel(m_list_model);
    m_ui->treeView->header()->setSectionResizeMode(1, QHeaderView::ResizeToContents);
}

TrialBalanceWindow::~TrialBalanceWindow()
{
    delete m_ui;
}

void TrialBalanceWindow::on_actionExport_to_CSV_triggered()
{
    QFileDialog dialog(this);
    dialog.setWindowTitle("Save Trial Balance as CSV");
    dialog.setAcceptMode(QFileDialog::AcceptSave);
    dialog.setNameFilter(tr("CSV (*.csv)"));

    if (!dialog.exec() || dialog.selectedFiles().empty())
        return;

    auto file_name = dialog.selectedFiles()[0];

    QFile file(file_name);
    if (!file.open(QFile::WriteOnly))
        return;

    QTextStream stream(&file);
    stream << "Nominal,Account,Debit,Credit\n";

    for (const auto& account : m_list_model->accounts()) {
        QString debit;
        if (account.balance > 0) {
            auto const balance = static_cast<double>(account.balance) / 100.0;
            debit = QString("%1").arg(balance, 0, 'f', 2);
        }
        QString credit;
        if (account.balance < 0) {
            auto const balance = static_cast<double>(-account.balance) / 100.0;
            credit = QString("%1").arg(balance, 0, 'f', 2);
        }

        stream << "\"" + account.nominal + "\",\"" + account.title + "\"," + debit + "," + credit + "\n";
    }

    file.close();
}

}
