/***************************************************************************
                          ksimdata.cpp  -  description
                             -------------------
    begin                : Fri Nov 23 2001
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

// C-Includes

// QT-Includes
#include <qstringlist.h>
#include <qpixmap.h>

// KDE-Includes
#include <ksimpleconfig.h>

// Project-Includes
#include "ksimdata.h"
#include "ksimdebug.h"
#include "enumdict.h"

// Forward declaration

class KSimData::Private
{
	public:

	Private(const QString &filename, KSimData::eVersionType versionType, bool bReadOnly)
		: m_simpleFile(filename, bReadOnly),
			m_versionType(versionType),
			m_filename(filename)
	{
	};

	KSimpleConfig m_simpleFile;
	KSimData::eVersionType m_versionType;
	QStringList m_groupStack;
	QString m_filename;
};

//#########################################################################################

typedef struct
{
	const char * docVersion;
	const char * firstKSimusVersion;
	const char * lastKSimusVersion;
} tVersionList;

static const tVersionList versionList[] =
{
	{ "KSimus <0.3.5", "0.0.0", "0.3.4" },
	{ "KSimus 0.3.5", "0.3.5", (const char *)0 }
};


static const char * const sVersionTypeKey = "Document Version";
static const char * const sKSimusKey =      "Document Type";


//#########################################################################################

EnumDict<KSimData::eVersionType>::tData EnumDict<KSimData::eVersionType>::data[]
      = { {"error",        KSimData::versionError},
          {"as is",        KSimData::versionAsIs},
          {"unknown",      KSimData::versionUnknown},
          {"KSimus 0.3.5", KSimData::version0_3_5},
          {"KSimus 0.3.7", KSimData::version0_3_7},
          {"topical",      KSimData::versionTopical},
          // Dont forget to change the EnumDict size!!!
          {0,              (KSimData::eVersionType)0}};

static const EnumDict<KSimData::eVersionType> & getVersionTypeDict()
{
	static EnumDict<KSimData::eVersionType> versionTypeDict(7);
	return versionTypeDict;
}

const char * KSimData::convertVersionType(KSimData::eVersionType versionType)
{
	return getVersionTypeDict().find(versionType);
}

KSimData::eVersionType KSimData::convertVersionType(const char * versionType, KSimData::eVersionType defaultVersionType)
{
	return getVersionTypeDict().find(versionType, defaultVersionType);
}


//#########################################################################################

KSimData::KSimData(const QString &pFileName, eVersionType versionType, bool bReadOnly)
{
	m_p = new Private(pFileName, versionType, bReadOnly);
	Q_CHECK_PTR(m_p);
	pushGroup("/");
	writeEntry(sKSimusKey, "KSimus"); // Set always
	
	switch(versionType)
	{
		case versionError:
			KSIMDEBUG("Fix me: call KSimData::KSimData with versionType == versionError");
			break;
			
		case versionAsIs:
			m_p->m_versionType = convertVersionType(m_p->m_simpleFile.readEntry(sVersionTypeKey), versionUnknown);
			break;
			
		case versionUnknown:
			KSIMDEBUG("Fix me: call KSimData::KSimData with versionType == versionUnknown");
			break;
			
		default:
			m_p->m_versionType = versionType;
			m_p->m_simpleFile.writeEntry(sVersionTypeKey, convertVersionType(m_p->m_versionType));
			break;
	}
	
	popGroup();
}

KSimData::~KSimData()
{
	if (m_p->m_groupStack.count() != 0)
	{
		KSIMDEBUG(QString::fromLatin1("close file while stack is not empty (File %1, current group %2").arg(m_p->m_filename).arg(group()));
	}
	
	delete m_p;
}

KSimData::eVersionType KSimData::getVersionType() const
{
	return m_p->m_versionType;
}

void KSimData::setGroup( const QString& pGroup )
{
	m_p->m_simpleFile.setGroup( pGroup );
}

void KSimData::setGroupRel( const QString& groupRel )
{
	m_p->m_simpleFile.setGroup(m_p->m_simpleFile.group() + groupRel);
}

void KSimData::setGroup( const char * pGroup )
{
	m_p->m_simpleFile.setGroup( pGroup );
}

void KSimData::setGroupRel( const char * groupRel )
{
	m_p->m_simpleFile.setGroup(m_p->m_simpleFile.group() + groupRel);
}

QString KSimData::group() const
{
	return m_p->m_simpleFile.group();
}

void KSimData::pushGroup( const QString& newGroup )
{
	m_p->m_groupStack.append(group());
	setGroup(newGroup);
}
	
void KSimData::pushGroup( const char * newGroup )
{
	pushGroup(QString::fromLatin1(newGroup));
}

void KSimData::pushGroupRel( const QString& groupRel )
{
	m_p->m_groupStack.append(group());
	setGroupRel(groupRel);
}
	
void KSimData::pushGroupRel( const char * groupRel )
{
	pushGroupRel(QString::fromLatin1(groupRel));
}
	
void KSimData::popGroup(void)
{
	if (m_p->m_groupStack.count())
	{
		QStringList::Iterator it(m_p->m_groupStack.end());
		it--;
		
		setGroup(*it);
		m_p->m_groupStack.remove(it);
	}
	else
	{
		KSIMDEBUG(QString::fromLatin1("pop group from an empty stack (File %1, current group %2").arg(m_p->m_filename).arg(group()));
	}
}

bool KSimData::hasGroup(const QString &group) const
{
	return m_p->m_simpleFile.hasGroup(group);	
}

bool KSimData::hasGroup(const char * group) const
{
	return m_p->m_simpleFile.hasGroup(group);
}

bool KSimData::hasGroupRel(const QString &groupRel) const
{
	return m_p->m_simpleFile.hasGroup(m_p->m_simpleFile.group() + groupRel);
}

bool KSimData::hasGroupRel(const char * groupRel) const
{
	return m_p->m_simpleFile.hasGroup(m_p->m_simpleFile.group() + groupRel);
}

bool KSimData::hasKey(const char *pKey) const
{
	return m_p->m_simpleFile.hasKey(pKey);
}

QString KSimData::readEntry( const char *pKey, const QString& aDefault) const
{
	return m_p->m_simpleFile.readEntry(pKey, aDefault);
}

int KSimData::readListEntry( const char *pKey, QStrList &list, char sep) const
{
	return m_p->m_simpleFile.readListEntry(pKey, list, sep);
}

QStringList KSimData::readListEntry( const char *pKey, char sep) const
{
	return m_p->m_simpleFile.readListEntry(pKey, sep);
}

QValueList<int> KSimData::readIntListEntry( const char *pKey ) const
{
	return m_p->m_simpleFile.readIntListEntry(pKey);
}

QString KSimData::readPathEntry( const char *pKey, const QString & aDefault ) const
{
	return m_p->m_simpleFile.readPathEntry(pKey, aDefault);
}

int KSimData::readNumEntry( const char *pKey, int nDefault ) const
{
	return m_p->m_simpleFile.readNumEntry(pKey, nDefault);
}

unsigned int KSimData::readUnsignedNumEntry( const char *pKey, unsigned int nDefault ) const
{
	return m_p->m_simpleFile.readUnsignedNumEntry(pKey, nDefault);
}

long KSimData::readLongNumEntry( const char *pKey, long nDefault ) const
{
	return m_p->m_simpleFile.readLongNumEntry(pKey, nDefault);
}

unsigned long KSimData::readUnsignedLongNumEntry( const char *pKey, unsigned long nDefault ) const
{
	return m_p->m_simpleFile.readUnsignedLongNumEntry(pKey, nDefault);
}

double KSimData::readDoubleNumEntry( const char *pKey, double nDefault ) const
{
	return m_p->m_simpleFile.readDoubleNumEntry(pKey, nDefault);
}

QFont KSimData::readFontEntry( const char *pKey, const QFont* pDefault ) const
{
	return m_p->m_simpleFile.readFontEntry(pKey, pDefault);
}

bool KSimData::readBoolEntry( const char *pKey, const bool bDefault ) const
{
	return m_p->m_simpleFile.readBoolEntry(pKey, bDefault);
}

QRect KSimData::readRectEntry( const char *pKey, const QRect* pDefault ) const
{
	return m_p->m_simpleFile.readRectEntry(pKey, pDefault);
}

QPoint KSimData::readPointEntry( const char *pKey, const QPoint* pDefault ) const
{
	return m_p->m_simpleFile.readPointEntry(pKey, pDefault);
}

QSize KSimData::readSizeEntry( const char *pKey, const QSize* pDefault ) const
{
	return m_p->m_simpleFile.readSizeEntry(pKey, pDefault);
}

QColor KSimData::readColorEntry( const char *pKey, const QColor* pDefault ) const
{
	return m_p->m_simpleFile.readColorEntry(pKey, pDefault);
}

QDateTime KSimData::readDateTimeEntry( const char *pKey, const QDateTime* pDefault ) const
{
	return m_p->m_simpleFile.readDateTimeEntry(pKey, pDefault);
}

QPixmap KSimData::readPixmapEntry( const char *pKey, const QPixmap* pDefault ) const
{
	if (hasKey(pKey))
	{
		QStringList strList(readListEntry(pKey));
		unsigned int size,i;
		size = strList[0].toUInt(0,16);
		QByteArray array(size);
		
		for (i = 0; i < size; i++)
		{
			array[i] = (char) strList[i+1].toUInt(0,16);
		}
		
		QDataStream stream(array, IO_ReadOnly);
		QPixmap pixmap;
		stream >> pixmap;
		return pixmap;
	}
	else if (pDefault)
	{
		return *pDefault;
	}
	else
	{
		return QPixmap();
	}
}

void KSimData::writeEntry( const char *pKey, const QString& pValue)
{
	m_p->m_simpleFile.writeEntry(pKey, pValue);
}

void KSimData::writeEntry( const char *pKey, const QVariant& rValue)
{
	m_p->m_simpleFile.writeEntry(pKey, rValue);
}

void KSimData::writeEntry( const char *pKey, const QStrList &rValue, char sep )
{
	m_p->m_simpleFile.writeEntry(pKey, rValue, sep);
}

void KSimData::writeEntry( const char *pKey, const QStringList &rValue, char sep )
{
	m_p->m_simpleFile.writeEntry(pKey, rValue, sep);
}

void KSimData::writeEntry( const char *pKey, const QValueList<int>& rValue)
{
	m_p->m_simpleFile.writeEntry(pKey, rValue);
}

void KSimData::writeEntry( const char *pKey, int nValue)
{
	m_p->m_simpleFile.writeEntry(pKey, nValue);
}

void KSimData::writeEntry( const char *pKey, unsigned int nValue)
{
	m_p->m_simpleFile.writeEntry(pKey, nValue);
}

void KSimData::writeEntry( const char *pKey, long nValue)
{
	m_p->m_simpleFile.writeEntry(pKey, nValue);
}

void KSimData::writeEntry( const char *pKey, unsigned long nValue)
{
	m_p->m_simpleFile.writeEntry(pKey, nValue);
}

void KSimData::writeEntry( const char *pKey, double nValue, char format, int precision )
{
	m_p->m_simpleFile.writeEntry(pKey, nValue, true, false, format, precision );
}

void KSimData::writeEntry( const char *pKey, bool bValue )
{
	m_p->m_simpleFile.writeEntry(pKey, bValue );
}

void KSimData::writeEntry( const char *pKey, const QFont& rFont )
{
	m_p->m_simpleFile.writeEntry(pKey, rFont);
}

void KSimData::writeEntry( const char *pKey, const QColor& rColor )
{
	m_p->m_simpleFile.writeEntry(pKey, rColor);
}

void KSimData::writeEntry( const char *pKey, const QDateTime& rDateTime )
{
	m_p->m_simpleFile.writeEntry(pKey, rDateTime);
}

void KSimData::writeEntry( const char *pKey, const QRect& rValue )
{
	m_p->m_simpleFile.writeEntry(pKey, rValue);
}

void KSimData::writeEntry( const char *pKey, const QPoint& rValue )
{
	m_p->m_simpleFile.writeEntry(pKey, rValue);
}

void KSimData::writeEntry( const char *pKey, const QSize& rValue )
{
	m_p->m_simpleFile.writeEntry(pKey, rValue);
}

void KSimData::writeEntry( const char *pKey, const QPixmap& rPixmap )
{
	QByteArray array;
	QDataStream stream(array, IO_WriteOnly);
	QStringList strList;
	unsigned int i;
	
	stream << rPixmap;
	
	strList.append(QString::number(array.size(),16));
	
	for (i = 0; i < array.size(); i++)
	{
		strList.append(QString::number((unsigned char)array[i],16));
	}
	writeEntry(pKey, strList);
}


