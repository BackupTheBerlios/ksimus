/***************************************************************************
                          ksimbaseuintedit.h  -  description
                             -------------------
    begin                : Sun Dec 23 2001
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

#ifndef KSIMBASEUINTEDIT_H
#define KSIMBASEUINTEDIT_H

// C-Includes

// QT-Includes
#include <qvalidator.h>

// KDE-Includes

// Project-Includes
#include "ksimbaseuint.h"
#include "ksimlineedit.h"

// Forward declaration



/** A validator for @ref KSimBaseUInt objects.
  *
  *@author Rasmus Diekenbrock
  */


class KSimBaseUIntValidator : public QValidator
{
public:
	/** Creates a validator with range 0 to UINT_MAX. */
	KSimBaseUIntValidator(QWidget * parent, const char * name = 0);
	/** Creates a validator with range bottom to top. */
	KSimBaseUIntValidator(unsigned int bottom, unsigned int top, QWidget * parent, const char * name = 0);
	/** The destructor. */
	~KSimBaseUIntValidator();

	/** Reimplemented for valdiate @ref KSimBaseUInt. */
	virtual State validate(QString & str, int & pos) const;

	/** Sets the minimum value. */
	void setBottom(unsigned int bottom);
	/** Sets the maximum value. */
	void setTop(unsigned int top);
	/** Sets the valid range. */
	void setRange(unsigned int bottom, unsigned int top);

	/** Returns the minimum value. */
	unsigned int bottom() const { return m_bottom; }
	/** Returns the maximum value. */
	unsigned int top() const { return m_top; }


private:
	unsigned int m_bottom, m_top;

};


//#######################################################################################
//#######################################################################################

/** A edit for @ref KSimBaseUInt objects.
  *
  *@author Rasmus Diekenbrock
  */

class KSimBaseUIntEdit : public KSimLineEdit
{

	Q_OBJECT

class Private;

public:
	/** Constructs a @ref LineEdit widget for @ref KSimBaseUInt objects. The start value is a empty string.
	  * The allowed range is from  0 to UINT_MAX. The validator @ref KSimBaseUIntValidator is used. */
	KSimBaseUIntEdit(QWidget * parent = 0, const char * name = 0);
	/** Constructs a @ref LineEdit widget for @ref KSimBaseUInt objects. The start value is a empty string.
	  * The allowed range is from bottom to top. The validator @ref KSimBaseUIntValidator is used. */
	KSimBaseUIntEdit(unsigned int bottom, unsigned int top, QWidget * parent = 0, const char * name = 0);
	/** Constructs a @ref LineEdit widget for @ref KSimBaseUInt objects. The start value is given by value.
	  * The allowed range is from  0 to UINT_MAX. The validator @ref KSimBaseUIntValidator is used. */
	KSimBaseUIntEdit(const KSimBaseUInt & value, QWidget * parent = 0, const char * name = 0);
	/** Constructs a @ref LineEdit widget for @ref KSimBaseUInt objects. The start value is given by value.
	  * The allowed range is from  bottom to top. The validator @ref KSimBaseUIntValidator is used. */
	KSimBaseUIntEdit(const KSimBaseUInt & value, unsigned int bottom, unsigned int top, QWidget * parent = 0, const char * name = 0);

	/** The destructor */
	~KSimBaseUIntEdit();

	/** Returns the current value. */
	KSimBaseUInt value() const;

  /** Sets the minimum value. */
  void setBottom(unsigned int minimum);
  /** Sets the maximum value. */
  void setTop(unsigned int maximum);
	/** Sets the valid range. */
	void setRange(unsigned int bottom, unsigned int top);

  /** Returns the minimum value. */
  unsigned int bottom() const;
  /** Returns the maximum value. */
  unsigned int top() const;

	/** Enables the value tracking during the input. If the current input is a valid value
	  * and is in the given range the signal @ref valueChanged is emitted. If tracking is
	  * disabled the signal is emitted only after retrun pressed or the input line was leaved.
	  * The default is disabled.
	  */
	void setTrackingEnabled(bool tracking);
	/** Returns true if tracking is enabled.
	  * @see setTrackingEnabled
	  */
	bool isTrackingEnabled() const;

	/** Returns a help text which explains the usage of this widget. Add this text to the whatsThis text. */
	static const QString & getWhatsThisHelp();

public slots:
	/** Sets a new value. */
	void setValue(const KSimBaseUInt & newValue);
	/** Sets a new value. Leave base unchanged. */
	void setValue(unsigned int newValue);


private slots:
	void slotTextChanged(const QString & text);
	void slotReady();
	void slotAboutToShowContextMenu(QPopupMenu* popup);
	void slotConvertToBinary();
	void slotConvertToOctal();
	void slotConvertToDecimal();
	void slotConvertToHexadecimal();

private:
	void init(unsigned int bottom, unsigned int top);
	void restoreValue();
	Private * m_p;


signals:
	void valueChangedTracking(const KSimBaseUInt & value);
	void valueChanged(const KSimBaseUInt & value);
};

#endif
