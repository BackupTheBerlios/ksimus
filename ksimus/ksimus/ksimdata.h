/***************************************************************************
                          ksimdata.h  -  description
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

#ifndef KSIMDATA_H
#define KSIMDATA_H

// C-Includes

// QT-Includes
#include <qstring.h>
#include <qstringlist.h>
#include <qvaluelist.h>
#include <qsize.h>
#include <qpoint.h>
#include <qrect.h>
#include <qvariant.h>

// KDE-Includes

// Project-Includes

// Forward declaration
class QDateTime;
class QStrList;


/** The data format for KSimus documents.
  *
  * Copy some comments from KSimpleConfig and KConfigBase. Thanks KDE, Kalle Dalheimer and Preston Brown.
  * @author Rasmus Diekenbrock
  */

class KSimData
{
class Private;
public: 

  enum eVersionType { versionError,   // Use this in @ref convertVersionType to detect an error. It is not used elsewhere.
                      versionAsIs,    // Leaves the doc version unchanged
                      versionUnknown, // The doc version is pre 0.3.5
                      version0_3_5,   // The version 0.3.5 or 0.3.6
                      version0_3_7,   // The current version
                      versionTopical = version0_3_7 // Points to the current versions  ***Last item***
                    };

  /**
   * Construct a KSimData object and make it either read-write
   * or read-only.
   *
   * @param pFileName   The file used for saving the config data. Either
   *                    a full path can be specified or just the filename.
   *                    If only a flename is specified, the default
   *                    directory for "config" files is used.
   * @param versionType The document version to use.
   * @param bReadOnly   Whether the object should be read-only.
   */
   KSimData(const QString &pFileName, eVersionType versionType = versionTopical, bool bReadOnly = false);

  /**
   * Destructor.
   *
   * Writes back any dirty configuration entries.
   */
   ~KSimData();


  /**
   * Returns the document version. See @ref eVersionType.
   */
  eVersionType getVersionType() const;

  /**
   * Converts a docType to a const char *.
   *
   * @param docType        The eDcoType to convert.
   */
  static const char * convertVersionType(KSimData::eVersionType versionType);

  /**
   * Converts a const char * to a docType.
   *
   * @param versionType        The string to convert.
   * @param defaultVersionType Used if versionType is not valid.
   */
  static KSimData::eVersionType convertVersionType(const char * versionType, KSimData::eVersionType defaultVersionType);

  /**
   * Specify the group in which keys will be searched.
   *
   *  Subsequent
   * calls to @ref readEntry() will look only for keys in the currently
   * activated group.
   *
   * Switch back to the default group by passing an empty string.
   * @param group The name of the new group.
   */
  void setGroup( const QString& group );

  /**
   * Specify the group in which keys will be searched.
   *
   *  Subsequent
   * calls to @ref readEntry() will look only for keys in the currently
   * activated group.
   *
   * @param group The name of the new group.
   */
  void setGroup( const char * group );

  /**
   * Specify the group in which keys will be searched.
   *
   *  Subsequent
   * calls to @ref readEntry() will look only for keys in the currently
   * activated group.
   *
   * @param groupRel The group relative to the current group.
   */
  void setGroupRel( const QString& groupRel );

  /**
   * Specify the group in which keys will be searched.
   *
   *  Subsequent
   * calls to @ref readEntry() will look only for keys in the currently
   * activated group.
   *
   * @param groupRel The group relative to the current group.
   */
  void setGroupRel( const char * groupRel );

  /**
   * Retrieve the name of the group in which we are
   *  searching for keys and from which we are retrieving entries.
   *
   * @return The current group.
   */
  QString group() const;

  /**
   * Specify the group in which keys will be searched and push the current group onto the stack.
   *
   *  Subsequent
   * calls to @ref readEntry() will look only for keys in the currently
   * activated group.
   *
   * @param group The name of the new group.
   */
  void pushGroup( const QString& group );
	
  /**
   * Specify the group in which keys will be searched and push the current group onto the stack.
   *
   *  Subsequent
   * calls to @ref readEntry() will look only for keys in the currently
   * activated group.
   *
   * @param group The name of the new group.
   */
  void pushGroup( const char * group );
	
  /**
   * Specify the group in which keys will be searched and push the current group onto the stack.
   *
   *  Subsequent
   * calls to @ref readEntry() will look only for keys in the currently
   * activated group.
   *
   * @param groupRel The name of the new group relative to the current group.
   */
  void pushGroupRel( const QString& groupRel );
	
  /**
   * Specify the group in which keys will be searched and push the current group onto the stack.
   *
   *  Subsequent
   * calls to @ref readEntry() will look only for keys in the currently
   * activated group.
   *
   * @param groupRel The name of the new group relative to the current group.
   */
  void pushGroupRel( const char * groupRel );
	
  /**
   * Pop the last pushed group from the stack.
   *
   *  Subsequent
   * calls to @ref readEntry() will look only for keys in the currently
   * activated group.
   *
   */
  void popGroup(void);


  /**
   * Returns @p true if the specified group is known about.
   *
   * @param group The group to search for.
   * @returns Whether the group exists.
   */
  bool hasGroup(const QString &group) const;

  /**
   * Returns @p true if the specified group is known about.
   *
   * @param group The group to search for.
   * @returns Whether the group exists.
   */
  bool hasGroup(const char * group) const;

  /**
   * Returns @p true if the specified group is known about.
   *
   * @param groupRel The group relative to he current group to search for.
   * @returns Whether the group exists.
   */
  bool hasGroupRel(const QString &groupRel) const;

  /**
   * Returns @p true if the specified group is known about.
   *
   * @param groupRel The group relative to he current group to search for.
   * @returns Whether the group exists.
   */
  bool hasGroupRel(const char * groupRel) const;

  /**
   * Returns @p true if the specified key is known about.
   *
   * @param pKey The key to search for.
   * @returns Whether the key exists.
   */
  bool hasKey(const char *pKey) const;


  /**
   * Read the value of an entry specified by @p pKey in the current group.
   *
   * @param pKey The key to search for.
   * @param aDefault A default value returned if the key was not found.
   * @return The value for this key or a null string if no value
   *      was found.
   */
   QString readEntry( const char *pKey,
                     const QString& aDefault = QString::null ) const;


  /**
   * Read a list of strings.
   *
   * @deprecated
   *
   * @param pKey The key to search for
   * @param list In this object, the read list will be returned.
   * @param sep  The list separator (default ",")
   * @return The number of entries in the list.
   */
  int readListEntry( const char *pKey, QStrList &list, char sep = ',' ) const;


  /**
   * Read a list of strings.
   *
   * @param pKey The key to search for.
   * @param sep  The list separator (default is ",").
   * @return The list.
   */
  QStringList readListEntry( const char *pKey, char sep = ',' ) const;

  /**
   * Read a list of Integers.
   *
   * @param pKey The key to search for.
   * @return The list.
   */
  QValueList<int> readIntListEntry( const char *pKey ) const;

  /**
   * Read a path.
   *
   * Read the value of an entry specified by @p pKey in the current group
   * and interpret it as a path. This means, dollar expansion is activated
   * for this value, so that e.g. $HOME gets expanded.
   *
   * @param pKey The key to search for.
   * @param aDefault A default value returned if the key was not found.
   * @return The value for this key or a null string if no value was found.
   */
  QString readPathEntry( const char *pKey, const QString & aDefault = QString::null ) const;

  /**
   * Read a numerical value.
   *
   * Read the value of an entry specified by @p pKey in the current group
   * and interpret it numerically.
   *
   * @param pKey The key to search for.
   * @param nDefault A default value returned if the key was not found.
   * @return The value for this key or 0 if no value was found.
   */
  int readNumEntry( const char *pKey, int nDefault = 0 ) const;

  /**
   * Read a numerical value.
   *
   * Read the value of an entry specified by @p pKey in the current group
   * and interpret it numerically.
   *
   * @param pKey The key to search for.
   * @param nDefault A default value returned if the key was not found.
   * @return The value for this key or 0 if no value was found.
   */
  unsigned int readUnsignedNumEntry( const char *pKey, unsigned int nDefault = 0 ) const;


  /**
   * Read a numerical value.
   *
   * Read the value of an entry specified by @p pKey in the current group
   * and interpret it numerically.
   *
   * @param pKey The key to search for.
   * @param nDefault A default value returned if the key was not found.
   * @return The value for this key or 0 if no value was found.
   */
  long readLongNumEntry( const char *pKey, long nDefault = 0 ) const;

  /**
   * Read a numerical value.
   *
   * Read the value of an entry specified by @p pKey in the current group
   * and interpret it numerically.
   *
   * @param pKey The key to search for.
   * @param nDefault A default value returned if the key was not found.
   * @return The value for this key or 0 if no value was found.
   */
  unsigned long readUnsignedLongNumEntry( const char *pKey, unsigned long nDefault = 0 ) const;

  /**
   * Read a numerical value.
   *
   * Read the value of an entry specified by @p pKey in the current group
   * and interpret it numerically.
   *
   * @param pKey The key to search for.
   * @param nDefault A default value returned if the key was not found.
   * @return The value for this key or 0 if no value was found.
   */
  double readDoubleNumEntry( const char *pKey, double nDefault = 0.0 ) const;

  /**
   * Read a @ref QFont.
   *
   * Read the value of an entry specified by @p pKey in the current group
   * and interpret it as a font object.
   *
   * @param pKey                The key to search for.
   * @param pDefault    A default value returned if the key was not found.
   * @return The value for this key or a default font if no value was found.
   */
  QFont readFontEntry( const char *pKey, const QFont* pDefault = 0L ) const;

  /**
   * Read a boolean entry.
   *
   * Read the value of an entry specified by @p pKey in the current group
   * and interpret it as a boolean value. Currently "on" and "true" are
   * accepted as true, everything else if false.
   *
   * @param pKey                The key to search for
   * @param bDefault    A default value returned if the key was not
   *                                    found.
   * @return The value for this key or a default value if no value was
   * found.
   */
  bool readBoolEntry( const char *pKey, const bool bDefault = false ) const;

  /**
   * Read a rect entry.
   *
   * Read the value of an entry specified by pKey in the current group
   * and interpret it as a @ref QRect object.
   *
   * @param pKey                The key to search for
   * @param pDefault    A default value returned if the key was not
   *                                    found.
   * @return The value for this key or a default rectangle if no value
   * was found.
   */
  QRect readRectEntry( const char *pKey, const QRect* pDefault = 0L ) const;

  /**
   * Read a point entry.
   *
   * Read the value of an entry specified by @p pKey in the current group
   * and interpret it as a @ref QPoint object.
   *
   * @param pKey                The key to search for
   * @param pDefault    A default value returned if the key was not
   *                                    found.
   * @return The value for this key or a default point if no value
   * was found.
   */
  QPoint readPointEntry( const char *pKey, const QPoint* pDefault = 0L ) const;

  /**
   * Read a size entry.
   *
   * Read the value of an entry specified by @p pKey in the current group
   * and interpret it as a @ref QSize object.
   *
   * @param pKey                The key to search for
   * @param pDefault    A default value returned if the key was not
   *                                    found.
   * @return The value for this key or a default point if no value
   * was found.
   */
  QSize readSizeEntry( const char *pKey, const QSize* pDefault = 0L ) const;


  /**
   * Read a @ref QColor.
   *
   * Read the value of an entry specified by @p pKey in the current group
   * and interpret it as a color.
   *
   * @param pKey                The key to search for.
   * @param pDefault    A default value returned if the key was not found.
   * @return The value for this key or a default color if no value
   * was found.
   */
  QColor readColorEntry( const char *pKey, const QColor* pDefault = 0L ) const;

  /**
   * Read a @ref QDateTime.
   *
   * Read the value of an entry specified by @p pKey in the current group
   * and interpret it as a date and time.
   *
   * @param pKey                The key to search for.
   * @param pDefault    A default value returned if the key was not found.
   * @return The value for this key or a @ref currentDateTime()
   *  (Qt global function) if no value was found.
   */
  QDateTime readDateTimeEntry( const char *pKey, const QDateTime* pDefault = 0L ) const;

  /**
   * Read a @ref QPixmap.
   *
   * Read the value of an entry specified by @p pKey in the current group
   * and interpret it as a pixmap.
   *
   * @param pKey        The key to search for.
   * @param pDefault    A default value returned if the key was not found.
   * @return The value for this key or a a dafault pixmap
   *   if no value was found.
   */
  QPixmap readPixmapEntry( const char *pKey, const QPixmap* pDefault = 0L ) const;

  /**
   * Write the key/value pair.
   *
   * This is stored in the most specific config file when destroying the
   * config object or when calling @ref sync().
   *
   * @param pKey         The key to write.
   * @param pValue       The value to write.
   */
  void writeEntry( const char *pKey, const QString& pValue);


  /**
   * Write the key/value pair.
   *
   * @param pKey       The key to write.
   * @param pValue     The value to write.
   * @see  writeEntry()
   */
  void writeEntry( const char *pKey, const char *pValue )
  { return writeEntry(pKey, QString::fromLatin1(pValue)); }

  /**
   * @ref writeEntry() Overridden to accept a property.
   *
   * @param pKey The key to write
   * @param rValue The property to write
   * @see  writeEntry()
   */
  void writeEntry( const char *pKey, const QVariant& rValue);

  /**
   * @ref writeEntry() overriden to accept a list of strings.
   *
   * @param pKey The key to write
   * @param rValue The list to write
   *
   * @see  writeEntry()
   */
  void writeEntry( const char *pKey, const QStrList &rValue, char sep = ',' );

  /**
   * @ref writeEntry() overridden to accept a list of strings.
   *
   * @param pKey The key to write
   * @param rValue The list to write
   *
   * @see  writeEntry()
   */
  void writeEntry( const char *pKey, const QStringList &rValue, char sep = ',');


 /**
   * @ref writeEntry() overridden to accept a list of Integers.
   *
   * @param pKey The key to write
   * @param rValue The list to write
   *
   * @see  writeEntry()
   */
  void writeEntry( const char *pKey, const QValueList<int>& rValue);


  /**
   * Write the key value pair.
   * Same as above, but write a numerical value.
   *
   * @param pKey The key to write.
   * @param nValue The value to write.
   */
  void writeEntry( const char *pKey, int nValue);

  /**
   * Write the key value pair.
   * Same as above, but write an unsigned numerical value.
   *
   * @param pKey The key to write.
   * @param nValue The value to write.
   */
  void writeEntry( const char *pKey, unsigned int nValue);

  /**
   * Write the key value pair.
   * Same as above, but write a long numerical value.
   *
   * @param pKey The key to write.
   * @param nValue The value to write.
   */
  void writeEntry( const char *pKey, long nValue);

  /**
   * Write the key value pair.
   * Same as above, but write an unsigned long numerical value.
   *
   * @param pKey The key to write.
   * @param nValue The value to write.
   */
  void writeEntry( const char *pKey, unsigned long nValue);

  /**
   * Write the key value pair.
   * Same as above, but write a floating-point value.*
   * @param pKey The key to write.
   * @param nValue The value to write.
   * @param format      @p format determines the format to which the value
   *  is converted. Default is 'g'.
   * @param precision   @p precision sets the precision with which the
   *  value is converted. Default is 6 as in QString.
   */
  void writeEntry( const char *pKey, double nValue,
                   char format = 'g', int precision = 6 );

  /**
   * Write the key value pair.
   * Same as above, but write a boolean value.
   *
   * @param pKey The key to write.
   * @param bValue The value to write.
   */
  void writeEntry( const char *pKey, bool bValue );

  /**
   * Write the key value pair.
   * Same as above, but write a font
   *
   * @param pKey The key to write.
   * @param rFont The font value to write.
   */
  void writeEntry( const char *pKey, const QFont& rFont );

  /**
   * Write the key value pair.
   * Same as above, but write a color.
   *
   * @param pKey The key to write.
   * @param rValue The color value to write.
   */
  void writeEntry( const char *pKey, const QColor& rColor );

  /**
   * Write the key value pair.
   * Same as above, but write a date and time.
   *
   * @param pKey The key to write.
   * @param rValue The date and time value to write.
   */
  void writeEntry( const char *pKey, const QDateTime& rDateTime );


  /**
   * Write the key value pair.
   * Same as above, but write a rectangle.
   *
   * @param pKey The key to write.
   * @param rValue The rectangle value to write.
   */
  void writeEntry( const char *pKey, const QRect& rValue );

  /**
   * Write the key value pair.
   * Same as above, but write a point.
   *
   * @param pKey The key to write.
   * @param rValue The point value to write.
   */
  void writeEntry( const char *pKey, const QPoint& rValue );

  /**
   * Write the key value pair.
   * Same as above, but write a size.
   *
   * @param pKey The key to write.
   * @param rValue The size value to write.
   */
  void writeEntry( const char *pKey, const QSize& rValue );

  /**
   * Write the key value pair.
   * Same as above, but write a pixmap.
   *
   * @param pKey The key to write.
   * @param rPixmap The pixmap to write.
   */
  void writeEntry( const char *pKey, const QPixmap& rPixmap );


private:
	Private * m_p;
};

#endif
