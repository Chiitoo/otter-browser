/**************************************************************************
* Otter Browser: Web browser controlled by the user, not vice-versa.
* Copyright (C) 2014 - 2015 Piotr Wójcik <chocimier@tlen.pl>
* Copyright (C) 2015 - 2021 Michal Dutkiewicz aka Emdek <michal@emdek.pl>
*
* This program is free software: you can redistribute it and/or modify
* it under the terms of the GNU General Public License as published by
* the Free Software Foundation, either version 3 of the License, or
* (at your option) any later version.
*
* This program is distributed in the hope that it will be useful,
* but WITHOUT ANY WARRANTY; without even the implied warranty of
* MERCHANTABILITY or FITNESS FOR A PARTICULAR PURPOSE. See the
* GNU General Public License for more details.
*
* You should have received a copy of the GNU General Public License
* along with this program. If not, see <http://www.gnu.org/licenses/>.
*
**************************************************************************/

#ifndef OTTER_IMPORTDIALOG_H
#define OTTER_IMPORTDIALOG_H

#include "Dialog.h"
#include "../core/DataExchanger.h"

namespace Otter
{

namespace Ui
{
	class ImportDialog;
}

class ImportDialog final : public Dialog
{
	Q_OBJECT

public:
	~ImportDialog();

	static void createDialog(const QString &importerName, QWidget *parent = nullptr);

protected:
	explicit ImportDialog(DataExchanger *importer, QWidget *parent);

	void closeEvent(QCloseEvent *event) override;
	void changeEvent(QEvent *event) override;

protected slots:
	void handleImportRequested();
	void handleImportStarted(DataExchanger::ExchangeType type, int total);
	void handleImportProgress(DataExchanger::ExchangeType type, int total, int amount);
	void handleImportFinished(DataExchanger::ExchangeType type, DataExchanger::OperationResult result, int total);
	void setPath(const QString &path);

private:
	DataExchanger *m_importer;
	QString m_path;
	Ui::ImportDialog *m_ui;
};

}

#endif
