/***************************************************************************
                          rowlayoutwidget.cpp  -  description
                             -------------------
    begin                : Sat May 25 2002
    copyright            : (C) 2002 by Rasmus Diekenbrock
    email                : ksimus@gmx.de
 ***************************************************************************/

/***************************************************************************
 *                                                                         *
 *   This program is free software; you can redistribute it and/or modify  *
 *   it under the terms of the GNU General Public License as published by  *
 *   the Free Software Foundation; either version 2 of the License, or     *
 *   (at your option) any later version.                                   *
 *                                                                         *
 ***************************************************************************/

// C-Includes

// QT-Includes
#include <qvbox.h>
#include <qhbox.h>
#include <qgrid.h>
#include <qlayout.h>
#include <qptrlist.h>

// KDE-Includes

// Project-Includes
#include "rowlayoutwidget.h"
#include "rowlayoutwidgethelper.h"
#include "ksimdebug.h"

// Forward declaration

class RowLayoutWidget::WidgetInfo
{
public:
	WidgetInfo(QWidget * widget, int startCol, int endCol, int align)
	:	m_widget(widget),
		m_startCol(startCol),
		m_endCol(endCol),
		m_align(align)
	{};

	QWidget * getWidget() const { return m_widget; };
	int getStartCol() const { return m_startCol; };
	int getEndCol() const { return m_endCol; };
	int getAlign() const { return m_align; };
	
private:
	QWidget * m_widget;
	int m_startCol;
	int m_endCol;
	int m_align;
};

class RowLayoutWidget::WidgetInfoList : public QPtrList<RowLayoutWidget::WidgetInfo>
{
	
public:
	const WidgetInfo * findWidget(const QWidget * widget);

};

const RowLayoutWidget::WidgetInfo * RowLayoutWidget::WidgetInfoList::findWidget(const QWidget * widget)
{
	for(QPtrListIterator<RowLayoutWidget::WidgetInfo> it(*this);it.current();++it)
	{
		if (it.current()->getWidget() == widget)
			return it.current();
	}
	return (const RowLayoutWidget::WidgetInfo *)0;
}

//#######################################################################
//#######################################################################


class RowLayoutWidget::Private
{
public:
	Private()
		:	nextRow(0),
			nextCol(0)
	{
		widgetList.setAutoDelete(true);
	};
	
	void next();
	void newRow(bool force = false);

	int maxCol() const { return layout->numCols()-1; };
	
	QGridLayout * masterLayout;
	QGridLayout * layout;
	int nextRow;
	int nextCol;
	RowLayoutWidget::WidgetInfoList widgetList;
};

void RowLayoutWidget::Private::next()
{
	if ((nextCol+1) < layout->numCols())
	{
		nextCol++;
	}
	else
	{
		nextCol = 0;
		nextRow++;
	}
}

void RowLayoutWidget::Private::newRow(bool force)
{
	if (force || (nextCol != 0))
	{
		nextCol = 0;
		nextRow++;
	}
}


//#######################################################################
//#######################################################################


RowLayoutWidget::RowLayoutWidget(int cols, QWidget *parent, const char *name, WFlags f)
	:	QFrame(parent,name,f)
{
	m_p = new Private();
	Q_CHECK_PTR(m_p);
	
	m_p->masterLayout = new QGridLayout(this, 3, 3, 0, 0, "MainLayout");
	Q_CHECK_PTR(m_p->masterLayout);
	m_p->layout = new QGridLayout(-1, cols, 0, name);
	Q_CHECK_PTR(m_p->layout);
	m_p->masterLayout->addLayout(m_p->layout,1,1);

	m_p->masterLayout->setRowStretch(0,0);
	m_p->masterLayout->setRowStretch(1,1);
	m_p->masterLayout->setRowStretch(2,0);
	m_p->masterLayout->setColStretch(0,0);
	m_p->masterLayout->setColStretch(1,1);
	m_p->masterLayout->setColStretch(2,0);
	
}

RowLayoutWidget::~RowLayoutWidget()
{
	delete m_p;
}

void RowLayoutWidget::setSpacing(int space)
{
	m_p->layout->setSpacing(space);
}

void RowLayoutWidget::setMargin(int w)
{
	QFrame::setMargin(w);
	m_p->masterLayout->setMargin(w);
}


void RowLayoutWidget::setColStretch(int col, int stretch)
{
	if ((col >= 0) && (col <= m_p->maxCol()))
	{
		m_p->layout->setColStretch(col, stretch);
	}
	else
	{
		KSIMDEBUG_VAR("col out of range", col);
	}
}

int RowLayoutWidget::colStretch(int col) const
{
	if ((col >= 0) && (col <= m_p->maxCol()))
	{
		return m_p->layout->colStretch(col);
	}
	else
	{
		KSIMDEBUG_VAR("col out of range", col);
	}
	return 0;
}

void RowLayoutWidget::addColSpacing(int col, int minsize)
{
	if ((col >= 0) && (col <= m_p->maxCol()))
	{
		m_p->layout->addColSpacing(col, minsize);
	}
	else
	{
		KSIMDEBUG_VAR("col out of range", col);
	}
}

void RowLayoutWidget::setLeftColStretch(int stretch)
{
	m_p->masterLayout->setColStretch(0,stretch);
}
int RowLayoutWidget::leftColStretch() const
{
	return m_p->masterLayout->colStretch(0);
}

void RowLayoutWidget::setCenterColStretch(int stretch)
{
	m_p->masterLayout->setColStretch(1,stretch);
}
int RowLayoutWidget::centerColStretch() const
{
	return m_p->masterLayout->colStretch(1);
}

void RowLayoutWidget::setRightColStretch(int stretch)
{
	m_p->masterLayout->setColStretch(2,stretch);
}
int RowLayoutWidget::rightColStretch() const
{
	return m_p->masterLayout->colStretch(2);
}

void RowLayoutWidget::setTopRowStretch(int stretch)
{
	m_p->masterLayout->setRowStretch(0,stretch);
}
int RowLayoutWidget::topRowStretch() const
{
	return m_p->masterLayout->rowStretch(0);
}

void RowLayoutWidget::setCenterRowStretch(int stretch)
{
	m_p->masterLayout->setRowStretch(1,stretch);
}
int RowLayoutWidget::centerRowStretch() const
{
	return m_p->masterLayout->rowStretch(1);
}

void RowLayoutWidget::setBottomRowStretch(int stretch)
{
	m_p->masterLayout->setRowStretch(2,stretch);
}
int RowLayoutWidget::bottomRowStretch() const
{
	return m_p->masterLayout->rowStretch(2);
}

void RowLayoutWidget::setLeftColSpacing(int minsize)
{
	m_p->masterLayout->addColSpacing(0, minsize);
}
int RowLayoutWidget::leftColSpacing() const
{
	return 0;//	return m_p->masterLayout->colSpacing(0);
}

void RowLayoutWidget::setCenterColSpacing(int minsize)
{
	m_p->masterLayout->addColSpacing(1, minsize);
}
int RowLayoutWidget::centerColSpacing() const
{
	return 0;//	return m_p->masterLayout->colSpacing(1);
}

void RowLayoutWidget::setRightColSpacing(int minsize)
{
	m_p->masterLayout->addColSpacing(2, minsize);
}
int RowLayoutWidget::rightColSpacing() const
{
	return 0;//	return m_p->masterLayout->colSpacing(2);
}

void RowLayoutWidget::setTopRowSpacing(int minsize)
{
	m_p->masterLayout->addRowSpacing(0, minsize);
}
int RowLayoutWidget::topRowSpacing() const
{
	return 0;//	return m_p->masterLayout->rowSpacing(0);
}

void RowLayoutWidget::setCenterRowSpacing(int minsize)
{
	m_p->masterLayout->addRowSpacing(1, minsize);
}
int RowLayoutWidget::centerRowSpacing() const
{
	return 0;//	return m_p->masterLayout->rowSpacing(1);
}

void RowLayoutWidget::setBottomRowSpacing(int minsize)
{
	m_p->masterLayout->addRowSpacing(2, minsize);
}
int RowLayoutWidget::bottomRowSpacing() const
{
	return 0;//	return m_p->masterLayout->rowSpacing(2);
}





void RowLayoutWidget::childEvent(QChildEvent * ev)
{
	QFrame::childEvent(ev);
	
	if (ev->inserted())
	{
		if (ev->child()->isWidgetType())
		{
			// A widget
			QWidget * wid = (QWidget *)ev->child();
			const WidgetInfo * info = m_p->widgetList.findWidget(wid);
			if (info != 0)
			{
				//KSIMDEBUG_VAR("Has widgetinfo", wid->name());
				if((info->getStartCol() == -1) && (info->getEndCol() == -1))
				{
					// Add to single cell
					//KSIMDEBUG("is single cell");
					m_p->layout->addWidget(wid, m_p->nextRow, m_p->nextCol, info->getAlign());
					m_p->next();
				}
				else
				{
					// Multi cell widget
					//KSIMDEBUG("is multicell cell");
					if (m_p->nextCol > info->getStartCol())
					{
						// Need a new row
						m_p->newRow();
					}
					m_p->layout->addMultiCellWidget(wid, m_p->nextRow, m_p->nextRow,
					                                     info->getStartCol(),
					                                     info->getEndCol(),
					                                     info->getAlign());
					m_p->nextCol = info->getEndCol();
					m_p->next();
				}
				m_p->widgetList.removeRef(info);
			}
			else
			{
				m_p->layout->addWidget((QWidget*)ev->child(), m_p->nextRow, m_p->nextCol);
				m_p->next();
			}
		}
		else
		{
			// Not a widget
			if(ev->child()->isA("RowLayoutWidgetHelperObj"))
			{
				// A special object
				RowLayoutWidgetHelperObj * helperObj = (RowLayoutWidgetHelperObj*)ev->child();
				Q_ASSERT(helperObj);
				
				switch(helperObj->getType())
				{
					case RowLayoutNewRow:
						m_p->newRow();
						break;
						
					case RowLayoutAddEmptyCell:
						m_p->next();
						break;
						
					case RowLayoutSetRowStretch:
					{
						int row = m_p->nextRow;
						if ((m_p->nextCol == 0) && (row > 0))
						{
							row--;
						}
						m_p->layout->setRowStretch(row,
						  ((RowLayoutWidgetHelperRowStretch*)helperObj)->getStretch());
					}
					break;
					
					default:
						KSIMDEBUG_VAR("Unknown type", helperObj->getType())
						break;
				}
				// We dont need the helper any more
				delete helperObj;
			}
			else
			{
				// A "regular" object
				// nothing todo
			}
		}
	}
}

void RowLayoutWidget::newRow()
{
	RowLayoutWidgetHelperObj * obj = new RowLayoutWidgetHelperObj(this, RowLayoutNewRow);
	Q_CHECK_PTR(obj);
}

void RowLayoutWidget::addEmptyCell()
{
	RowLayoutWidgetHelperObj * obj = new RowLayoutWidgetHelperObj(this, RowLayoutAddEmptyCell);
	Q_CHECK_PTR(obj);
}

void RowLayoutWidget::setRowStretch(int stretch)
{
	RowLayoutWidgetHelperRowStretch * obj = new RowLayoutWidgetHelperRowStretch(this, stretch);
	Q_CHECK_PTR(obj);
}


void RowLayoutWidget::addWidgetInternal(QWidget * widget, int startCol, int endCol, int align)
{
	if (widget)
	{
		if ((startCol != -1) || (endCol != -1))
		{
			if (startCol < 0)
			{
				startCol = 0;
				KSIMDEBUG("startCol < 0 ! Use 0");
			}
			if (endCol > m_p->maxCol())
			{
				endCol = m_p->maxCol();
				KSIMDEBUG("endCol > maxCol ! Use maxCol");
			}
			if (startCol > endCol)
			{
				startCol = -1;
				endCol = -1;
				KSIMDEBUG("startCol > endCol ! Use single cell");
			}
		}
		WidgetInfo * info = new WidgetInfo(widget, startCol, endCol, align);
		Q_CHECK_PTR(info);
		m_p->widgetList.append(info);
	}
}

void RowLayoutWidget::addRowWidgetInternal(QWidget * widget, int align)
{
	addWidgetInternal(widget, 0, m_p->maxCol(), align);
}

// ############   Widget    ###################

QWidget * RowLayoutWidget::newWidget(int align, const char * name)
{
	QWidget * widget = new QWidget(this, name);
	Q_CHECK_PTR(widget);
	addWidgetInternal(widget, -1, -1, align);
	
	return widget;
}

QWidget * RowLayoutWidget::newWidget(int startCol, int endCol, int align, const char * name)
{
	QWidget * widget = new QWidget(this, name);
	Q_CHECK_PTR(widget);
	addWidgetInternal(widget, startCol, endCol, align);
	
	return widget;
}

QWidget * RowLayoutWidget::newRowWidget(int align, const char * name)
{
	QWidget * widget = new QWidget(this, name);
	Q_CHECK_PTR(widget);
	addRowWidgetInternal(widget, align);
	return widget;
}

QWidget * RowLayoutWidget::newRowWidget(const char * name)
{
	return newRowWidget(0, name);
}

// ############   QVBox    ###################

QVBox * RowLayoutWidget::newVBox(int align, const char * name, WFlags f)
{
	QVBox * widget = new QVBox(this, name, f);
	Q_CHECK_PTR(widget);
	addWidgetInternal(widget, -1, -1, align);
	
	return widget;
}

QVBox * RowLayoutWidget::newVBox(int startCol, int endCol, int align, const char * name, WFlags f)
{
	QVBox * widget = new QVBox(this, name, f);
	Q_CHECK_PTR(widget);
	addWidgetInternal(widget, startCol, endCol, align);
	
	return widget;
}

QVBox * RowLayoutWidget::newRowVBox(int align, const char * name, WFlags f)
{
	QVBox * widget = new QVBox(this, name, f);
	Q_CHECK_PTR(widget);
	addRowWidgetInternal(widget, align);
	return widget;
}

QVBox * RowLayoutWidget::newRowVBox(const char * name, WFlags f)
{
	return newRowVBox(0, name, f);
}

// ############   HBox    ###################

QHBox * RowLayoutWidget::newHBox(int align, const char * name, WFlags f)
{
	QHBox * widget = new QHBox(this, name, f);
	Q_CHECK_PTR(widget);
	addWidgetInternal(widget, -1, -1, align);
	
	return widget;
}

QHBox * RowLayoutWidget::newHBox(int startCol, int endCol, int align, const char * name, WFlags f)
{
	QHBox * widget = new QHBox(this, name, f);
	Q_CHECK_PTR(widget);
	addWidgetInternal(widget, startCol, endCol, align);
	
	return widget;
}

QHBox * RowLayoutWidget::newRowHBox(int align, const char * name, WFlags f)
{
	QHBox * widget = new QHBox(this, name, f);
	Q_CHECK_PTR(widget);
	addRowWidgetInternal(widget, align);
	return widget;
}

QHBox * RowLayoutWidget::newRowHBox(const char * name, WFlags f)
{
	return newRowHBox(0, name, f);
}

// ############   QGrid(n, Horizontal)    ###################

QGrid * RowLayoutWidget::newHGrid(int n, int align, const char * name, WFlags f)
{
	QGrid * widget = new QGrid(n, QGrid::Horizontal, this, name, f);
	Q_CHECK_PTR(widget);
	addWidgetInternal(widget, -1, -1, align);
	
	return widget;
}

QGrid * RowLayoutWidget::newHGrid(int n, int startCol, int endCol, int align, const char * name, WFlags f)
{
	QGrid * widget = new QGrid(n, QGrid::Horizontal, this, name, f);
	Q_CHECK_PTR(widget);
	addWidgetInternal(widget, startCol, endCol, align);
	
	return widget;
}

QGrid * RowLayoutWidget::newRowHGrid(int n, int align, const char * name, WFlags f)
{
	QGrid * widget = new QGrid(n, QGrid::Horizontal, this, name, f);
	Q_CHECK_PTR(widget);
	addRowWidgetInternal(widget, align);
	return widget;
}

QGrid * RowLayoutWidget::newRowHGrid(int n, const char * name, WFlags f)
{
	return newRowHGrid(n, 0, name, f);
}

// ############   QGrid(n, Vertical)    ###################

QGrid * RowLayoutWidget::newVGrid(int n, int align, const char * name, WFlags f)
{
	QGrid * widget = new QGrid(n, QGrid::Vertical, this, name, f);
	Q_CHECK_PTR(widget);
	addWidgetInternal(widget, -1, -1, align);
	
	return widget;
}

QGrid * RowLayoutWidget::newVGrid(int n, int startCol, int endCol, int align, const char * name, WFlags f)
{
	QGrid * widget = new QGrid(n, QGrid::Vertical, this, name, f);
	Q_CHECK_PTR(widget);
	addWidgetInternal(widget, startCol, endCol, align);
	
	return widget;
}

QGrid * RowLayoutWidget::newRowVGrid(int n, int align, const char * name, WFlags f)
{
	QGrid * widget = new QGrid(n, QGrid::Vertical, this, name, f);
	Q_CHECK_PTR(widget);
	addRowWidgetInternal(widget, align);
	return widget;
}

QGrid * RowLayoutWidget::newRowVGrid(int n, const char * name, WFlags f)
{
	return newRowVGrid(n, 0, name, f);
}

#include "rowlayoutwidget.moc"

