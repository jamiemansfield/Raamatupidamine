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

namespace Reports {

TrialBalanceWindow::TrialBalanceWindow(QWidget* parent)
    : QMainWindow(parent)
    , m_ui(new Ui::TrialBalanceWindow)
    , m_list_model(new TrialBalanceListModel(this))
{
    m_ui->setupUi(this);

    m_ui->treeView->setModel(m_list_model);
}

TrialBalanceWindow::~TrialBalanceWindow()
{
    delete m_ui;
}

}
