/***************************************************************************
                          ksimembfont  -  description
                             -------------------
    begin                : Sa Jan 10 2004
    copyright            : (C) 2004 by Rasmus Diekenbrock
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
#ifndef KSIMEMBFONT_H
#define KSIMEMBFONT_H

// C-Includes

// QT-Includes
#include <qbitmap.h>
#include <qintdict.h>

// KDE-Includes

// Project-Includes
#include "types.h"

// Forward declaration
class QPainter;
class QRect;
class QPoint;
class QString;


/**
A class with embedded fonts.

@author Rasmus Diekenbrock
*/
class KSimEmbFont
{
public:
	enum TextFlags { Underline       = 0x00010000,
	                 UnderlineSpace  = 0x00020000,
	                 UnderlineMask   = Underline | UnderlineSpace,
	                 Overline        = 0x00040000,
	                 OverlineSpace   = 0x00080000,
	                 OverlineMask    = Overline | OverlineSpace
	               };
	
	~KSimEmbFont();

	QSize getSize(double rot, int tf, const QString & str) const;
	QSize getSize(const QString & str) const;

	const QBitmap & getBitmap(const QChar & c) const;
	bool exist(const QChar & c) const;
	void drawText(QPainter * p, int x, int y, const QString & str) const;
	void drawText(QPainter * p, int x, int y, int tf, const QString & str) const;
	void drawText(QPainter * p, int x, int y, double rot, int tf, const QString & str) const;
	void drawText(QPainter * p, const QPoint & pos, int tf, const QString & str) const;
	void drawText(QPainter * p, const QRect & r, int tf, const QString & str) const;
	void drawText(QPainter * p, const QPoint & pos, double rot, int tf, const QString & str) const;
	void drawText(QPainter * p, const QRect & r, double rot, int tf, const QString & str) const;


	static KSimEmbFont * getFont08();
	static KSimEmbFont * f08() { return getFont08(); };
	static KSimEmbFont * getFont10();
	static KSimEmbFont * f10() { return getFont10(); };
	static KSimEmbFont * getFont(eFont font);
	static KSimEmbFont * font(eFont font) { return getFont(font); };

protected:
	KSimEmbFont(enum eFont font);

private:
	int getWidthSL(const QString & str) const;
	int drawTextSL(QPainter * p, int x, int y, const QString & str) const;
	int getLineHeight(int tf) const;
	static unsigned char toRotMode(double rot);
	static unsigned char toVerMode(int tf);
	static unsigned char toHorMode(int tf);
	
	QIntDict<QBitmap> m_glyphDict;
	QBitmap m_unknownGlyph;
	int m_height;
	int m_glyphSpace;
};

#endif
