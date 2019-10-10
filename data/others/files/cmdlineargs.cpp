/*
 * Copyright (c) 2015-2016 Nicola Corna (nicola@corna.info)
 *
 * This file is part of pcb2gcodeGUI.
 *
 * pcb2gcodeGUI is free software: you can redistribute it and/or modify
 * it under the terms of the GNU General Public License as published by
 * the Free Software Foundation, either version 3 of the License, or
 * (at your option) any later version.
 *
 * pcb2gcodeGUI is distributed in the hope that it will be useful,
 * but WITHOUT ANY WARRANTY; without even the implied warranty of
 * MERCHANTABILITY or FITNESS FOR A PARTICULAR PURPOSE.  See the
 * GNU General Public License for more details.
 *
 * You should have received a copy of the GNU General Public License
 * along with pcb2gcodeGUI.  If not, see <http://www.gnu.org/licenses/>.
 */

#include "cmdlineargs.h"
#include "ui_cmdlineargs.h"

CmdLineArgs::CmdLineArgs(QWidget *parent) :
    QDialog(parent),
    ui(new Ui::CmdLineArgs)
{
    ui->setupUi(this);
}

CmdLineArgs::~CmdLineArgs()
{
    delete ui;
}

void CmdLineArgs::setText(QString text)
{
    ui->plainTextEdit->setPlainText(text);
}
