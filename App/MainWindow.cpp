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

#include "ChartOfAccountsWindow.h"
#include "Dialogs/CreateJournalDialog.h"

MainWindow::MainWindow()
    : m_ui(new Ui::MainWindow)
{
    m_ui->setupUi(this);
}

MainWindow::~MainWindow()
{
    delete m_ui;
}

void MainWindow::on_actionChart_of_Accounts_triggered()
{
    auto window = new ChartOfAccountsWindow(this);
    window->show();
}

void MainWindow::on_actionCreate_Journal_triggered()
{
    Dialogs::CreateJournalDialog dialog(this);
    if (dialog.exec() != QDialog::Accepted) {
        return;
    }
    dialog.save_to_database();
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
