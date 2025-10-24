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

#pragma once

#include <QMainWindow>

#include "ChartOfAccountsListModel.h"

namespace Ui {

class ChartOfAccountsWindow;

}

namespace Windows {

class ChartOfAccountsWindow : public QMainWindow {
    Q_OBJECT

public:
    ChartOfAccountsWindow(QWidget* parent = nullptr);
    ~ChartOfAccountsWindow() override;

private slots:
    void on_treeView_doubleClicked(QModelIndex const& index);

    void on_actionAdd_Account_triggered();
    void on_actionAdd_Default_Accounts_triggered();

private:
    Ui::ChartOfAccountsWindow* m_ui;

    ChartOfAccountsListModel* m_list_model;
};

}
