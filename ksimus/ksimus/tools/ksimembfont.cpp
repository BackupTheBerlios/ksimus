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


// C-Includes

// QT-Includes
#include <qglobal.h>
#include <qpainter.h>
#include <qrect.h>
#include <qpoint.h>
#include <qimage.h>
#include <qstringlist.h>

// KDE-Includes

// Project-Includes
#include "ksimdebug.h"
#include "ksimembfont.h"
#include "globals.h"

// Forward declaration


//###########################################################################
//###########################################################################


struct tGlyphList
{
	unsigned short unicode;
	const char ** xpm;
};

struct tFontInfo
{
	const struct tGlyphList * glyphList;
	unsigned short glyphListLength;
	const char ** unknownGlyph_xpm;
	unsigned int glyphSpace;
};



//###########################################################################
//###########################################################################


#define static static const   // XPM defines the array not const :(

#include "font08/font080020.xpm"
#include "font08/font080021.xpm"
#include "font08/font080022.xpm"
#include "font08/font080023.xpm"
#include "font08/font080024.xpm"
#include "font08/font080025.xpm"
#include "font08/font080026.xpm"
#include "font08/font080027.xpm"
#include "font08/font080028.xpm"
#include "font08/font080029.xpm"
#include "font08/font08002a.xpm"
#include "font08/font08002b.xpm"
#include "font08/font08002c.xpm"
#include "font08/font08002d.xpm"
#include "font08/font08002e.xpm"
#include "font08/font08002f.xpm"

#include "font08/font080030.xpm"
#include "font08/font080031.xpm"
#include "font08/font080032.xpm"
#include "font08/font080033.xpm"
#include "font08/font080034.xpm"
#include "font08/font080035.xpm"
#include "font08/font080036.xpm"
#include "font08/font080037.xpm"
#include "font08/font080038.xpm"
#include "font08/font080039.xpm"
#include "font08/font08003a.xpm"
#include "font08/font08003b.xpm"
#include "font08/font08003c.xpm"
#include "font08/font08003d.xpm"
#include "font08/font08003e.xpm"
#include "font08/font08003f.xpm"

#include "font08/font080040.xpm"
#include "font08/font080041.xpm"
#include "font08/font080042.xpm"
#include "font08/font080043.xpm"
#include "font08/font080044.xpm"
#include "font08/font080045.xpm"
#include "font08/font080046.xpm"
#include "font08/font080047.xpm"
#include "font08/font080048.xpm"
#include "font08/font080049.xpm"
#include "font08/font08004a.xpm"
#include "font08/font08004b.xpm"
#include "font08/font08004c.xpm"
#include "font08/font08004d.xpm"
#include "font08/font08004e.xpm"
#include "font08/font08004f.xpm"

#include "font08/font080050.xpm"
#include "font08/font080051.xpm"
#include "font08/font080052.xpm"
#include "font08/font080053.xpm"
#include "font08/font080054.xpm"
#include "font08/font080055.xpm"
#include "font08/font080056.xpm"
#include "font08/font080057.xpm"
#include "font08/font080058.xpm"
#include "font08/font080059.xpm"
#include "font08/font08005a.xpm"
#include "font08/font08005b.xpm"
#include "font08/font08005c.xpm"
#include "font08/font08005d.xpm"
#include "font08/font08005e.xpm"
#include "font08/font08005f.xpm"

#include "font08/font080060.xpm"
#include "font08/font080061.xpm"
#include "font08/font080062.xpm"
#include "font08/font080063.xpm"
#include "font08/font080064.xpm"
#include "font08/font080065.xpm"
#include "font08/font080066.xpm"
#include "font08/font080067.xpm"
#include "font08/font080068.xpm"
#include "font08/font080069.xpm"
#include "font08/font08006a.xpm"
#include "font08/font08006b.xpm"
#include "font08/font08006c.xpm"
#include "font08/font08006d.xpm"
#include "font08/font08006e.xpm"
#include "font08/font08006f.xpm"

#include "font08/font080070.xpm"
#include "font08/font080071.xpm"
#include "font08/font080072.xpm"
#include "font08/font080073.xpm"
#include "font08/font080074.xpm"
#include "font08/font080075.xpm"
#include "font08/font080076.xpm"
#include "font08/font080077.xpm"
#include "font08/font080078.xpm"
#include "font08/font080079.xpm"
#include "font08/font08007a.xpm"
#include "font08/font08007b.xpm"
#include "font08/font08007c.xpm"
#include "font08/font08007d.xpm"
#include "font08/font08007e.xpm"

#include "font08/font082264.xpm"
#include "font08/font082265.xpm"

#include "font08/font08unknown.xpm"

#undef static



static const struct tGlyphList glyphList08[] = {
	{ 0x0020, font080020_xpm },
	{ 0x0021, font080021_xpm },
	{ 0x0022, font080022_xpm },
	{ 0x0023, font080023_xpm },
	{ 0x0024, font080024_xpm },
	{ 0x0025, font080025_xpm },
	{ 0x0026, font080026_xpm },
	{ 0x0027, font080027_xpm },
	{ 0x0028, font080028_xpm },
	{ 0x0029, font080029_xpm },
	{ 0x002a, font08002a_xpm },
	{ 0x002b, font08002b_xpm },
	{ 0x002c, font08002c_xpm },
	{ 0x002d, font08002d_xpm },
	{ 0x002e, font08002e_xpm },
	{ 0x002f, font08002f_xpm },

	{ 0x0030, font080030_xpm },
	{ 0x0031, font080031_xpm },
	{ 0x0032, font080032_xpm },
	{ 0x0033, font080033_xpm },
	{ 0x0034, font080034_xpm },
	{ 0x0035, font080035_xpm },
	{ 0x0036, font080036_xpm },
	{ 0x0037, font080037_xpm },
	{ 0x0038, font080038_xpm },
	{ 0x0039, font080039_xpm },
	{ 0x003a, font08003a_xpm },
	{ 0x003b, font08003b_xpm },
	{ 0x003c, font08003c_xpm },
	{ 0x003d, font08003d_xpm },
	{ 0x003e, font08003e_xpm },
	{ 0x003f, font08003f_xpm },

	{ 0x0040, font080040_xpm },
	{ 0x0041, font080041_xpm },
	{ 0x0042, font080042_xpm },
	{ 0x0043, font080043_xpm },
	{ 0x0044, font080044_xpm },
	{ 0x0045, font080045_xpm },
	{ 0x0046, font080046_xpm },
	{ 0x0047, font080047_xpm },
	{ 0x0048, font080048_xpm },
	{ 0x0049, font080049_xpm },
	{ 0x004a, font08004a_xpm },
	{ 0x004b, font08004b_xpm },
	{ 0x004c, font08004c_xpm },
	{ 0x004d, font08004d_xpm },
	{ 0x004e, font08004e_xpm },
	{ 0x004f, font08004f_xpm },

	{ 0x0050, font080050_xpm },
	{ 0x0051, font080051_xpm },
	{ 0x0052, font080052_xpm },
	{ 0x0053, font080053_xpm },
	{ 0x0054, font080054_xpm },
	{ 0x0055, font080055_xpm },
	{ 0x0056, font080056_xpm },
	{ 0x0057, font080057_xpm },
	{ 0x0058, font080058_xpm },
	{ 0x0059, font080059_xpm },
	{ 0x005a, font08005a_xpm },
	{ 0x005b, font08005b_xpm },
	{ 0x005c, font08005c_xpm },
	{ 0x005d, font08005d_xpm },
	{ 0x005e, font08005e_xpm },
	{ 0x005f, font08005f_xpm },

	{ 0x0060, font080060_xpm },
	{ 0x0061, font080061_xpm },
	{ 0x0062, font080062_xpm },
	{ 0x0063, font080063_xpm },
	{ 0x0064, font080064_xpm },
	{ 0x0065, font080065_xpm },
	{ 0x0066, font080066_xpm },
	{ 0x0067, font080067_xpm },
	{ 0x0068, font080068_xpm },
	{ 0x0069, font080069_xpm },
	{ 0x006a, font08006a_xpm },
	{ 0x006b, font08006b_xpm },
	{ 0x006c, font08006c_xpm },
	{ 0x006d, font08006d_xpm },
	{ 0x006e, font08006e_xpm },
	{ 0x006f, font08006f_xpm },

	{ 0x0070, font080070_xpm },
	{ 0x0071, font080071_xpm },
	{ 0x0072, font080072_xpm },
	{ 0x0073, font080073_xpm },
	{ 0x0074, font080074_xpm },
	{ 0x0075, font080075_xpm },
	{ 0x0076, font080076_xpm },
	{ 0x0077, font080077_xpm },
	{ 0x0078, font080078_xpm },
	{ 0x0079, font080079_xpm },
	{ 0x007a, font08007a_xpm },
	{ 0x007b, font08007b_xpm },
	{ 0x007c, font08007c_xpm },
	{ 0x007d, font08007d_xpm },
	{ 0x007e, font08007e_xpm },

	{ 0x2264, font082264_xpm },
	{ 0x2265, font082265_xpm }
};

static const struct tFontInfo font08Info =
{
	glyphList08,
	sizeof(glyphList08) / sizeof(struct tGlyphList),
	font08unknown_xpm,
	1
};


//###########################################################################
//###########################################################################


#define static static const   // XPM defines the array not const :(

#include "font10/font100020.xpm"
#include "font10/font100021.xpm"
#include "font10/font100022.xpm"
#include "font10/font100023.xpm"
#include "font10/font100024.xpm"
#include "font10/font100025.xpm"
#include "font10/font100026.xpm"
#include "font10/font100027.xpm"
#include "font10/font100028.xpm"
#include "font10/font100029.xpm"
#include "font10/font10002a.xpm"
#include "font10/font10002b.xpm"
#include "font10/font10002c.xpm"
#include "font10/font10002d.xpm"
#include "font10/font10002e.xpm"
#include "font10/font10002f.xpm"

#include "font10/font100030.xpm"
#include "font10/font100031.xpm"
#include "font10/font100032.xpm"
#include "font10/font100033.xpm"
#include "font10/font100034.xpm"
#include "font10/font100035.xpm"
#include "font10/font100036.xpm"
#include "font10/font100037.xpm"
#include "font10/font100038.xpm"
#include "font10/font100039.xpm"
#include "font10/font10003a.xpm"
#include "font10/font10003b.xpm"
#include "font10/font10003c.xpm"
#include "font10/font10003d.xpm"
#include "font10/font10003e.xpm"
#include "font10/font10003f.xpm"

#include "font10/font100040.xpm"
#include "font10/font100041.xpm"
#include "font10/font100042.xpm"
#include "font10/font100043.xpm"
#include "font10/font100044.xpm"
#include "font10/font100045.xpm"
#include "font10/font100046.xpm"
#include "font10/font100047.xpm"
#include "font10/font100048.xpm"
#include "font10/font100049.xpm"
#include "font10/font10004a.xpm"
#include "font10/font10004b.xpm"
#include "font10/font10004c.xpm"
#include "font10/font10004d.xpm"
#include "font10/font10004e.xpm"
#include "font10/font10004f.xpm"

#include "font10/font100050.xpm"
#include "font10/font100051.xpm"
#include "font10/font100052.xpm"
#include "font10/font100053.xpm"
#include "font10/font100054.xpm"
#include "font10/font100055.xpm"
#include "font10/font100056.xpm"
#include "font10/font100057.xpm"
#include "font10/font100058.xpm"
#include "font10/font100059.xpm"
#include "font10/font10005a.xpm"
#include "font10/font10005b.xpm"
#include "font10/font10005c.xpm"
#include "font10/font10005d.xpm"
#include "font10/font10005e.xpm"
#include "font10/font10005f.xpm"

#include "font10/font100060.xpm"
#include "font10/font100061.xpm"
#include "font10/font100062.xpm"
#include "font10/font100063.xpm"
#include "font10/font100064.xpm"
#include "font10/font100065.xpm"
#include "font10/font100066.xpm"
#include "font10/font100067.xpm"
#include "font10/font100068.xpm"
#include "font10/font100069.xpm"
#include "font10/font10006a.xpm"
#include "font10/font10006b.xpm"
#include "font10/font10006c.xpm"
#include "font10/font10006d.xpm"
#include "font10/font10006e.xpm"
#include "font10/font10006f.xpm"

#include "font10/font100070.xpm"
#include "font10/font100071.xpm"
#include "font10/font100072.xpm"
#include "font10/font100073.xpm"
#include "font10/font100074.xpm"
#include "font10/font100075.xpm"
#include "font10/font100076.xpm"
#include "font10/font100077.xpm"
#include "font10/font100078.xpm"
#include "font10/font100079.xpm"
#include "font10/font10007a.xpm"
#include "font10/font10007b.xpm"
#include "font10/font10007c.xpm"
#include "font10/font10007d.xpm"
#include "font10/font10007e.xpm"

#include "font10/font102264.xpm"
#include "font10/font102265.xpm"

#include "font10/font10unknown.xpm"

#undef static




static const struct tGlyphList glyphList10[] = {
	{ 0x0020, font100020_xpm },
	{ 0x0021, font100021_xpm },
	{ 0x0022, font100022_xpm },
	{ 0x0023, font100023_xpm },
	{ 0x0024, font100024_xpm },
	{ 0x0025, font100025_xpm },
	{ 0x0026, font100026_xpm },
	{ 0x0027, font100027_xpm },
	{ 0x0028, font100028_xpm },
	{ 0x0029, font100029_xpm },
	{ 0x002a, font10002a_xpm },
	{ 0x002b, font10002b_xpm },
	{ 0x002c, font10002c_xpm },
	{ 0x002d, font10002d_xpm },
	{ 0x002e, font10002e_xpm },
	{ 0x002f, font10002f_xpm },

	{ 0x0030, font100030_xpm },
	{ 0x0031, font100031_xpm },
	{ 0x0032, font100032_xpm },
	{ 0x0033, font100033_xpm },
	{ 0x0034, font100034_xpm },
	{ 0x0035, font100035_xpm },
	{ 0x0036, font100036_xpm },
	{ 0x0037, font100037_xpm },
	{ 0x0038, font100038_xpm },
	{ 0x0039, font100039_xpm },
	{ 0x003a, font10003a_xpm },
	{ 0x003b, font10003b_xpm },
	{ 0x003c, font10003c_xpm },
	{ 0x003d, font10003d_xpm },
	{ 0x003e, font10003e_xpm },
	{ 0x003f, font10003f_xpm },

	{ 0x0040, font100040_xpm },
	{ 0x0041, font100041_xpm },
	{ 0x0042, font100042_xpm },
	{ 0x0043, font100043_xpm },
	{ 0x0044, font100044_xpm },
	{ 0x0045, font100045_xpm },
	{ 0x0046, font100046_xpm },
	{ 0x0047, font100047_xpm },
	{ 0x0048, font100048_xpm },
	{ 0x0049, font100049_xpm },
	{ 0x004a, font10004a_xpm },
	{ 0x004b, font10004b_xpm },
	{ 0x004c, font10004c_xpm },
	{ 0x004d, font10004d_xpm },
	{ 0x004e, font10004e_xpm },
	{ 0x004f, font10004f_xpm },

	{ 0x0050, font100050_xpm },
	{ 0x0051, font100051_xpm },
	{ 0x0052, font100052_xpm },
	{ 0x0053, font100053_xpm },
	{ 0x0054, font100054_xpm },
	{ 0x0055, font100055_xpm },
	{ 0x0056, font100056_xpm },
	{ 0x0057, font100057_xpm },
	{ 0x0058, font100058_xpm },
	{ 0x0059, font100059_xpm },
	{ 0x005a, font10005a_xpm },
	{ 0x005b, font10005b_xpm },
	{ 0x005c, font10005c_xpm },
	{ 0x005d, font10005d_xpm },
	{ 0x005e, font10005e_xpm },
	{ 0x005f, font10005f_xpm },

	{ 0x0060, font100060_xpm },
	{ 0x0061, font100061_xpm },
	{ 0x0062, font100062_xpm },
	{ 0x0063, font100063_xpm },
	{ 0x0064, font100064_xpm },
	{ 0x0065, font100065_xpm },
	{ 0x0066, font100066_xpm },
	{ 0x0067, font100067_xpm },
	{ 0x0068, font100068_xpm },
	{ 0x0069, font100069_xpm },
	{ 0x006a, font10006a_xpm },
	{ 0x006b, font10006b_xpm },
	{ 0x006c, font10006c_xpm },
	{ 0x006d, font10006d_xpm },
	{ 0x006e, font10006e_xpm },
	{ 0x006f, font10006f_xpm },

	{ 0x0070, font100070_xpm },
	{ 0x0071, font100071_xpm },
	{ 0x0072, font100072_xpm },
	{ 0x0073, font100073_xpm },
	{ 0x0074, font100074_xpm },
	{ 0x0075, font100075_xpm },
	{ 0x0076, font100076_xpm },
	{ 0x0077, font100077_xpm },
	{ 0x0078, font100078_xpm },
	{ 0x0079, font100079_xpm },
	{ 0x007a, font10007a_xpm },
	{ 0x007b, font10007b_xpm },
	{ 0x007c, font10007c_xpm },
	{ 0x007d, font10007d_xpm },
	{ 0x007e, font10007e_xpm },

	{ 0x2264, font102264_xpm },
	{ 0x2265, font102265_xpm }
};

static const struct tFontInfo font10Info =
{
	glyphList10,
	sizeof(glyphList10) / sizeof(struct tGlyphList),
	font10unknown_xpm,
	1
};


//###########################################################################
//###########################################################################


KSimEmbFont::KSimEmbFont(enum eFont font)
	:	m_height(-1)
{
	const struct tFontInfo * fontInfo;

	if (font == FONT_08)
	{
		fontInfo = &font08Info;
	}
	else
	{
		fontInfo = &font10Info;
	}

	m_glyphDict.resize(getGreaterPrim(fontInfo->glyphListLength));
	m_glyphDict.setAutoDelete(true);

	for (unsigned int i = 0; i < fontInfo->glyphListLength; i++)
	{
		QBitmap * pBitmap = new QBitmap();
		CHECK_PTR(pBitmap);
		*pBitmap = QImage(fontInfo->glyphList[i].xpm);
		pBitmap->setMask(*pBitmap); // selfMask 
		m_glyphDict.insert(fontInfo->glyphList[i].unicode, pBitmap);
		if (m_height == -1)
		{
			m_height = pBitmap->height();
		}
		else if (pBitmap->height() != m_height)
		{
			KSIMDEBUG_VAR("Font with different heights!", fontInfo->glyphList[i].unicode);
		}
	}

	m_glyphSpace = fontInfo->glyphSpace;
	m_unknownGlyph = QImage(fontInfo->unknownGlyph_xpm);
}


KSimEmbFont::~KSimEmbFont()
{
}

const QBitmap & KSimEmbFont::getBitmap(const QChar & c) const
{
	const QBitmap * p = m_glyphDict.find(c.unicode());
	if (p)
	{
		return *p;
	}
	return m_unknownGlyph;
}

bool KSimEmbFont::exist(const QChar & c) const
{
	return (m_glyphDict.find(c.unicode()) != 0);
}

unsigned char KSimEmbFont::toRotMode(double rot)
{
	// Only angles 0°, 90°, 180° and 270° are allowed.
	if ((rot <= 45.0) || (rot > 315.0)) return 0; // 0 degree
	else if (rot <= 135.0)              return 1; // 90 degree
	else if (rot <= 225.0)              return 2; // 180 degree
	return 3; // 270 degree
}

unsigned char KSimEmbFont::toVerMode(int tf)
{
	if ((tf & Qt::AlignBottom) == Qt::AlignBottom)        return 2;
	else if ((tf & Qt::AlignVCenter) == Qt::AlignVCenter) return 1;
	return 0;
}

unsigned char KSimEmbFont::toHorMode(int tf)
{
	if ((tf & Qt::AlignRight) == Qt::AlignRight)          return 2;
	else if ((tf & Qt::AlignHCenter) == Qt::AlignHCenter) return 1;
	return 0;
}

int KSimEmbFont::getWidthSL(const QString & str) const
{
	int w = 0;
	const unsigned int len = str.length();
	for (unsigned int i = 0; i < len; i++)
	{
		w += getBitmap(str[i]).width();
	}
	w += len ? m_glyphSpace * (len - 1) : 0;

	return w;
}

int KSimEmbFont::getLineHeight(int tf) const
{
	int h = m_height;
	if (tf & UnderlineMask) h += 2;
	if (tf & OverlineMask)  h += 2;
	return h;
}

QSize KSimEmbFont::getSize(double rot, int tf, const QString & str) const
{
	int w = 0;
	const QStringList list(QStringList::split(QChar('\n'), str));
	QStringList::const_iterator it;
	for ( it = list.begin(); it != list.end(); ++it )
	{
		const int w2 = getWidthSL(*it);
		if (w2 > w) 
			w = w2;
	}

	const int h = list.count() * getLineHeight(tf);
	
	if (toRotMode(rot) & 1)
	{
		return QSize(h, w);
	}
	
	return QSize(w, h);
}

QSize KSimEmbFont::getSize(const QString & str) const
{
	return getSize(0.0, 0, str);
}

int KSimEmbFont::drawTextSL(QPainter * p, int x, int y, const QString & str) const
{
	const unsigned int len = str.length();
	for (unsigned int i = 0; i < len; i++)
	{
		const QBitmap & pix = getBitmap(str[i]);
		p->drawPixmap(x, y, pix);
		x += pix.width() + m_glyphSpace;
	}
	return x;
}

void KSimEmbFont::drawText(QPainter * p, int x, int y, const QString & str) const
{
	drawText(p, QRect(QPoint(x,y), QSize (0,0)), 0.0, 0, str);
}

void KSimEmbFont::drawText(QPainter * p, int x, int y, int tf, const QString & str) const
{
	drawText(p, QRect(QPoint(x,y), QSize (0,0)), 0.0, tf, str);
}

void KSimEmbFont::drawText(QPainter * p, int x, int y, double rot, int tf, const QString & str) const
{
	drawText(p, QRect(QPoint(x,y), QSize (0,0)), rot, tf, str);
}

void KSimEmbFont::drawText(QPainter * p, const QPoint & pos, int tf, const QString & str) const
{
	drawText(p, QRect(pos, QSize (0,0)), 0.0, tf, str);
}

void KSimEmbFont::drawText(QPainter * p, const QRect & r, int tf, const QString & str) const
{
	drawText(p,r,0.0,tf,str);
}


void KSimEmbFont::drawText(QPainter * p, const QPoint & pos, double rot, int tf, const QString & str) const
{
	drawText(p, QRect(pos, QSize (0,0)), rot, tf, str);
}

#define RSWITCH3(x, v0, v1, v2) ((x == 0) ? v0 : ((x == 1) ? v1 : v2))

void KSimEmbFont::drawText(QPainter * p, const QRect & r, double rot, int tf, const QString & str) const
{
	static const unsigned char horModeTransTab[3][3][4] =
	// hor, ver, degree
	{	//   left
		{	{ 0, 0, 2, 2 },    // top
			{ 0, 1, 2, 1 },    // ver center
			{ 0, 2, 2, 0 } },  // bottom
		//   hori center
		{	{ 1, 0, 1, 2 },    // top
			{ 1, 1, 1, 1 },    // ver center
			{ 1, 2, 1, 0 } },  // bottom
		//   right
		{	{ 2, 0, 0, 2 },    // top
			{ 2, 1, 0, 1 },    // ver center
			{ 2, 2, 0, 0 } } };// bottom
	
	const unsigned char rotMode = toRotMode(rot);
	KSIMASSERT_VAR(rotMode <= 3, rotMode);
	unsigned char horMode = toHorMode(tf);
	KSIMASSERT_VAR(horMode <= 2, horMode);
	const unsigned char verMode = toVerMode(tf);
	KSIMASSERT_VAR(verMode <= 2, verMode);
	int lineHeight = getLineHeight(tf) + 1;
	
	const QStringList list(QStringList::split(QChar('\n'), str));
	
	QStringList::const_iterator it;
	
	// Calc dimension
	int allW = 0;
	for (it = list.begin(); it != list.end(); ++it)
	{
		const int w = getWidthSL(*it);
		if (w > allW) allW = w;
	}
	const int maxWidth = allW;
	int allH = list.count() * lineHeight - 1;
	const int maxHeight = allH;
	
	if ((rotMode == 1) || (rotMode == 3))
	{
		const int dummy = allW;
		allW = allH;
		allH = dummy;
	}
	
	// Calc horizontal position
	const int xOff = RSWITCH3(horMode, r.left(),                           // == 0
	                                   r.left() + (r.width() - allW) / 2,  // == 1
	                                   r.right() - allW);                  // == 2
	
	// Calc vertical position
	const int yOff = RSWITCH3(verMode, r.top(),                           // == 0
	                                   r.top() + (r.height() - allH) / 2, // == 1
	                                   r.bottom() - allH);                // == 2
	
	horMode = horModeTransTab[horMode][verMode][rotMode];
	KSIMASSERT_VAR(horMode <= 2, horMode);
	
	int x = 0;
	int y = 0;
	QBitmap bitmap(maxWidth + 1, maxHeight + 1, true);
	QPainter paintBitmap;
	paintBitmap.begin(&bitmap);
	
	for (it = list.begin(); it != list.end(); ++it)
	{
		switch(horMode)
		{
			case 0: x = 0;                                break;
			case 1: x = (maxWidth - getWidthSL(*it)) / 2; break;
			case 2: x =  maxWidth - getWidthSL(*it);      break;
		}
		
		int y1 = y;
		if (tf & OverlineMask)
			y1 += 2;
		int x1 = drawTextSL(&paintBitmap, x, y1, *it) - m_glyphSpace - 1;
		if (tf & Overline)
			paintBitmap.drawLine(x, y, x1,y);
		if (tf & Underline)
		{
			y1 += m_height;
			paintBitmap.drawLine(x, y1, x1, y1);
		}
		
		y += lineHeight;
	}
	paintBitmap.end();
	
	p->save();
	switch(rotMode)
	{
		case 0: p->setWorldMatrix(QWMatrix( 1.0,  0.0,  0.0,  1.0, xOff, yOff), true);               break;
		case 1: p->setWorldMatrix(QWMatrix( 0.0,  1.0, -1.0,  0.0, xOff + allW, yOff), true);        break;
		case 2: p->setWorldMatrix(QWMatrix(-1.0,  0.0,  0.0, -1.0, xOff + allW, yOff + allH), true); break;
		case 3: p->setWorldMatrix(QWMatrix( 0.0, -1.0,  1.0,  0.0, xOff, yOff + allH), true);        break;
	}
	
	bitmap.setMask(bitmap);
	p->drawPixmap(QPoint(0,0), bitmap);
	p->restore();
}

#undef RSWITCH3


KSimEmbFont * KSimEmbFont::getFont08()
{
	static KSimEmbFont embFont(FONT_08);
	return &embFont;
}

KSimEmbFont * KSimEmbFont::getFont10()
{
	static KSimEmbFont embFont(FONT_10);
	return &embFont;
}

KSimEmbFont * KSimEmbFont::getFont(eFont font)
{
	switch(font)
	{
		case FONT_08:
			return getFont08();
		break;

		case FONT_10:
			return getFont10();
		break;
	}

	return 0;	// Not possible
}





