/***************************************************************************
                          componentblocklayout.h  -  description
                             -------------------
    begin                : Sun Dec 16 2001
    copyright            : (C) 2001 by Rasmus Diekenbrock
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

#ifndef COMPONENTBLOCKLAYOUT_H
#define COMPONENTBLOCKLAYOUT_H

// C-Includes

// QT-Includes
#include <qstring.h>

// KDE-Includes

// Project-Includes
#include "componentaddon.h"
#include "doublelayout.h"


// Forward declaration
class CompView;


//#######################################################################
//#######################################################################

/**Layout class for arrange connectors in a block on both sides of a component.
  *@author Rasmus Diekenbrock
  */

class ComponentBlockLayout : public ComponentAddOn, public DoubleLayout
{
public:
	ComponentBlockLayout(CompView * sheetView, ComponentLayout * compLayout, Orientation orient, QString blockName);
	~ComponentBlockLayout();

	/** Draws a rectangle frame and a text.
		*/
	virtual void drawSheetView (QPainter *p) const;

  /** If enable is true a rectangle frame will be drawn.
    * If false no frame will be drawn. */
  void setDrawFrame(bool enable);
  /** Returns true if a rectangle frame will be drawn. */
  bool isDrawFrame() const;

  /** Set a text which will be darw in the frame. */
  void setText(const QString & text) { m_text = text; };
  /** Returns the text which will be darw in the frame. */
  QString getText() const { return m_text; };

	/** Set the alignment of the drawn text. */
	void setTextAlign(int align) { m_textAlign = align; };
	/** Returns the alignment of the drawn text. */
	int getTextAlign() const { return m_textAlign; };

private:
	/** Some internal flags. */
	Q_UINT32 m_flags;
	QString m_text;
	int m_textAlign;
};

//#######################################################################
//#######################################################################

class ComponentControlBlock : public ComponentBlockLayout
{
public:
	ComponentControlBlock(CompView * sheetView, ComponentLayout * compLayout, QString blockName = QString::fromLatin1("Control Block"));
	~ComponentControlBlock();

	/** Draws a control block and a text.
		*/
	virtual void drawSheetView (QPainter *p) const;
};


#endif
