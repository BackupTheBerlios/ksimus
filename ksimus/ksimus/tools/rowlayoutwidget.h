/***************************************************************************
                          rowlayoutwidget.h  -  description
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

#ifndef ROWLAYOUTWIDGET_H
#define ROWLAYOUTWIDGET_H

// C-Includes

// QT-Includes
#include <qframe.h>

// KDE-Includes

// Project-Includes

// Forward declaration
class QVBox;
class QHBox;
class QGrid;

/** A widget with a row orientated layout.
  * The widget is easy to use as a QGrid
  * but gives more control about the layout.
  * @author Rasmus Diekenbrock
  */

class RowLayoutWidget : public QFrame
{
	/* Internal classes */
	class Private;
	class WidgetInfo;
	class WidgetInfoList;
	
	Q_OBJECT

public:
	RowLayoutWidget(int cols, QWidget *parent=0, const char *name=0, WFlags f=0);
	~RowLayoutWidget();

	void setSpacing(int space);
	virtual void setMargin(int w);
	
	void setLeftColStretch(int stretch);
	int leftColStretch() const;
	
	void setCenterColStretch(int stretch);
	int centerColStretch() const;
	
	void setRightColStretch(int stretch);
	int rightColStretch() const;
	
	void setTopRowStretch(int stretch);
	int topRowStretch() const;
	
	void setCenterRowStretch(int stretch);
	int centerRowStretch() const;
	
	void setBottomRowStretch(int stretch);
	int bottomRowStretch() const;
	
	void setLeftColSpacing(int minsize);
	int leftColSpacing() const;
	
	void setCenterColSpacing(int minsize);
	int centerColSpacing() const;
	
	void setRightColSpacing(int minsize);
	int rightColSpacing() const;
	
	void setTopRowSpacing(int minsize);
	int topRowSpacing() const;
	
	void setCenterRowSpacing(int minsize);
	int centerRowSpacing() const;
	
	void setBottomRowSpacing(int minsize);
	int bottomRowSpacing() const;
	
	void setColStretch(int col, int stretch);
	int colStretch(int col) const;
	void addColSpacing(int col, int minsize);
	
	void setRowStretch(int stretch);
	
	void newRow();
	void addEmptyCell();
	
	
	QWidget * newWidget(int align, const char * name=0);
	QWidget * newWidget(int startCol, int endCol, int align, const char * name=0);
	QWidget * newRowWidget(int align, const char * name=0);
	QWidget * newRowWidget(const char * name=0);

	QVBox * newVBox(int align, const char * name=0, WFlags f=0);
	QVBox * newVBox(int startCol, int endCol, int align, const char * name=0, WFlags f=0);
	QVBox * newRowVBox(int align, const char * name=0, WFlags f=0);
	QVBox * newRowVBox(const char * name=0, WFlags f=0);

	QHBox * newHBox(int align, const char * name=0, WFlags f=0);
	QHBox * newHBox(int startCol, int endCol, int align, const char * name=0, WFlags f=0);
	QHBox * newRowHBox(int align, const char * name=0, WFlags f=0);
	QHBox * newRowHBox(const char * name=0, WFlags f=0);

	QGrid * newHGrid(int n, int align, const char * name=0, WFlags f=0);
	QGrid * newHGrid(int n, int startCol, int endCol, int align, const char * name=0, WFlags f=0);
	QGrid * newRowHGrid(int n, int align, const char * name=0, WFlags f=0);
	QGrid * newRowHGrid(int n, const char * name=0, WFlags f=0);

	QGrid * newVGrid(int n, int align, const char * name=0, WFlags f=0);
	QGrid * newVGrid(int n, int startCol, int endCol, int align, const char * name=0, WFlags f=0);
	QGrid * newRowVGrid(int n, int align, const char * name=0, WFlags f=0);
	QGrid * newRowVGrid(int n, const char * name=0, WFlags f=0);


protected:
	virtual void childEvent(QChildEvent * ev);

private:
	void addWidgetInternal(QWidget * widget, int startCol, int endCol, int align);
	void addRowWidgetInternal(QWidget * widget, int align);
	Private * m_p;
};

#endif
