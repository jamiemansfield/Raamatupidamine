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

#include "../TransactionsListModel.h"

namespace Ui {

class TransactionsWindow;

}

namespace Windows {

class TransactionsWindow : public QMainWindow {
    Q_OBJECT

public:
    TransactionsWindow(QWidget* parent);
    ~TransactionsWindow() override;

    TransactionsListModel* list_model() const { return m_list_model; }

private slots:
    void on_treeView_customContextMenuRequested(QPoint const& point);

private:
    Ui::TransactionsWindow* m_ui;

    TransactionsListModel* m_list_model;
};

}
