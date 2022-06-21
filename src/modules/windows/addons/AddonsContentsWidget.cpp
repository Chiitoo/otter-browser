/**************************************************************************
* Otter Browser: Web browser controlled by the user, not vice-versa.
* Copyright (C) 2016 - 2022 Michal Dutkiewicz aka Emdek <michal@emdek.pl>
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

#include "AddonsContentsWidget.h"
#include "UserScriptsPage.h"
#include "../../../core/ThemesManager.h"

#include "ui_AddonsContentsWidget.h"

#include <QtWidgets/QTabBar>

namespace Otter
{

AddonsContentsWidget::AddonsContentsWidget(const QVariantMap &parameters, Window *window, QWidget *parent) : ContentsWidget(parameters, window, parent),
	m_currentPage(new UserScriptsPage(this)),
	m_ui(new Ui::AddonsContentsWidget)
{
	m_ui->setupUi(this);
	m_ui->categoriesTabWidget->addPage(m_currentPage);
	m_ui->categoriesTabWidget->tabBar()->addTab(tr("User Styles"));
	m_ui->categoriesTabWidget->tabBar()->setTabEnabled(1, false);
	m_ui->categoriesTabWidget->tabBar()->addTab(tr("Dictionaries"));
	m_ui->categoriesTabWidget->tabBar()->setTabEnabled(2, false);
	m_ui->categoriesTabWidget->tabBar()->addTab(tr("Translations"));
	m_ui->categoriesTabWidget->tabBar()->setTabEnabled(3, false);

	if (isSidebarPanel())
	{
		m_ui->detailsWidget->hide();
	}
	else
	{
		updateDetails(m_currentPage->getCurrentAddon());

		connect(m_currentPage, &AddonsPage::currentAddonChanged, this, &AddonsContentsWidget::updateDetails);
	}

	connect(m_ui->saveButton, &QPushButton::clicked, this, [&]()
	{
		m_currentPage->save();

		m_ui->saveButton->setEnabled(false);
	});
	connect(m_ui->addButton, &QPushButton::clicked, m_currentPage, &AddonsPage::addAddon);
	connect(m_currentPage, &AddonsPage::settingsModified, this, [&]()
	{
		m_ui->saveButton->setEnabled(true);
	});
	connect(m_currentPage, &AddonsPage::needsActionsUpdate, [&]()
	{
		emit arbitraryActionsStateChanged({ActionsManager::DeleteAction});
	});
}

AddonsContentsWidget::~AddonsContentsWidget()
{
	delete m_ui;
}

void AddonsContentsWidget::changeEvent(QEvent *event)
{
	ContentsWidget::changeEvent(event);

	if (event->type() == QEvent::LanguageChange)
	{
		m_ui->retranslateUi(this);
		m_ui->categoriesTabWidget->tabBar()->setTabText(1, tr("User Styles"));
		m_ui->categoriesTabWidget->tabBar()->setTabText(2, tr("Dictionaries"));
		m_ui->categoriesTabWidget->tabBar()->setTabText(3, tr("Translations"));
	}
}

void AddonsContentsWidget::print(QPrinter *printer)
{
	m_currentPage->print(printer);
}

void AddonsContentsWidget::triggerAction(int identifier, const QVariantMap &parameters, ActionsManager::TriggerType trigger)
{
	switch (identifier)
	{
		case ActionsManager::SelectAllAction:
		case ActionsManager::DeleteAction:
		case ActionsManager::FindAction:
		case ActionsManager::QuickFindAction:
		case ActionsManager::ActivateContentAction:
			m_currentPage->triggerAction(identifier, parameters, trigger);

			break;
		default:
			ContentsWidget::triggerAction(identifier, parameters, trigger);

			break;
	}
}

void AddonsContentsWidget::updateDetails(Addon *addon)
{
	m_ui->titleLabelWidget->clear();
	m_ui->updateUrlLabelWidget->clear();

	if (addon)
	{
		m_ui->titleLabelWidget->setText(addon->getTitle());
		m_ui->updateUrlLabelWidget->setText(addon->getUpdateUrl().toString());
	}
}

QString AddonsContentsWidget::getTitle() const
{
	return tr("Addons");
}

QLatin1String AddonsContentsWidget::getType() const
{
	return QLatin1String("addons");
}

QUrl AddonsContentsWidget::getUrl() const
{
	return QUrl(QLatin1String("about:addons"));
}

QIcon AddonsContentsWidget::getIcon() const
{
	return ThemesManager::createIcon(QLatin1String("preferences-plugin"), false);
}

ActionsManager::ActionDefinition::State AddonsContentsWidget::getActionState(int identifier, const QVariantMap &parameters) const
{
	return m_currentPage->getActionState(identifier, parameters);
}

WebWidget::LoadingState AddonsContentsWidget::getLoadingState() const
{
	return m_currentPage->getLoadingState();
}

}
