/**************************************************************************
* Otter Browser: Web browser controlled by the user, not vice-versa.
* Copyright (C) 2022 Michal Dutkiewicz aka Emdek <michal@emdek.pl>
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

#include "CategoriesTabWidget.h"

#include <QtCore/QEvent>
#include <QtWidgets/QScrollArea>
#include <QtWidgets/QTabBar>

namespace Otter
{

CategoriesTabWidget::CategoriesTabWidget(QWidget *parent) : QTabWidget(parent)
{
	updateStyle();
}

void CategoriesTabWidget::changeEvent(QEvent *event)
{
	QTabWidget::changeEvent(event);

	switch (event->type())
	{
		case QEvent::FontChange:
		case QEvent::LayoutDirectionChange:
		case QEvent::StyleChange:
			updateStyle();

			break;
		case QEvent::LanguageChange:
			for (int i = 0; i < tabBar()->count(); ++i)
			{
				CategoryPage *page(getPage(i));

				if (page)
				{
					setTabText(i, page->getTitle());
				}
			}

			break;
		default:
			break;
	}
}

void CategoriesTabWidget::addPage(CategoryPage *page)
{
	QScrollArea *scrollArea(new QScrollArea(this));
	scrollArea->setFrameShape(QFrame::NoFrame);
	scrollArea->setWidgetResizable(true);
	scrollArea->setWidget(page);
	scrollArea->viewport()->setAutoFillBackground(false);

	page->setAutoFillBackground(false);

	m_pages.append(page);

	addTab(scrollArea, page->getTitle());
}

void CategoriesTabWidget::updateStyle()
{
	QFont font(this->font());

	if (font.pixelSize() > 0)
	{
		font.setPixelSize(font.pixelSize() * 1.5);
	}
	else
	{
		font.setPointSize(font.pointSize() * 1.5);
	}

	setTabPosition(isLeftToRight() ? QTabWidget::West : QTabWidget::East);
	tabBar()->setFont(font);
}

CategoryPage* CategoriesTabWidget::getPage(int index)
{
	return ((index >= 0 && index < m_pages.count()) ? m_pages.at(index) : nullptr);
}

CategoryPage::CategoryPage(QWidget *parent) : QWidget(parent),
	m_wasLoaded(false)
{
}

void CategoryPage::showEvent(QShowEvent *event)
{
	load();

	QWidget::showEvent(event);
}

void CategoryPage::markAsLoaded()
{
	if (!m_wasLoaded)
	{
		m_wasLoaded = true;

		emit loaded();
	}
}

bool CategoryPage::wasLoaded() const
{
	return m_wasLoaded;
}

}
