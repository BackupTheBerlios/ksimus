/***************************************************************************
                          ksimbaseintedit.h  -  description
                             -------------------
    begin                : Sat Jul 12 2003
    copyright            : (C) 2003 by Rasmus Diekenbrock
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

#ifndef KSIMBASEINTEDIT_H
#define KSIMBASEINTEDIT_H

// C-Includes

// QT-Includes
#include <qvalidator.h>

// KDE-Includes

// Project-Includes
#include "ksimbaseint.h"
#include "ksimlineedit.h"

// Forward declaration


/** A validator for @ref KSimBaseInt objects.
  *
  *@author Rasmus Diekenbrock
  */


class KSimBaseIntValidator : public QValidator
{
public:
	/** Creates a validator with range INT_MIN to INT_MAX. */
	KSimBaseIntValidator(QWidget * parent, const char * name = 0);
	/** Creates a validator with range bottom to top. */
	KSimBaseIntValidator(int bottom, int top, QWidget * parent, const char * name = 0);
	/** The destructor. */
	~KSimBaseIntValidator();

	/** Reimplemented for valdiate @ref KSimBaseUInt. */
	virtual State validate(QString & str, int & pos) const;
	
	/** Sets the minimum value. */
	void setBottom(int bottom);
	/** Sets the maximum value. */
	void setTop(int top);
	/** Sets the valid range. */
	void setRange(int bottom, int top);

	/** Returns the minimum value. */
	int bottom() const { return m_bottom; }
	/** Returns the maximum value. */
	int top() const { return m_top; }


private:
	int m_bottom, m_top;

};


//#######################################################################################
//#######################################################################################

/** A line edit for @ref KSimBaseInt objects.
  *
  *@author Rasmus Diekenbrock
  */

class KSimBaseIntEdit : public KSimLineEdit
{
class Private;

	Q_OBJECT

public:
	/** Constructs a @ref Edit widget for @ref KSimBaseInt objects. The start value is a empty string.
	  * The allowed range is from  INT_MIN to INT_MAX. The validator @ref KSimBaseIntValidator is used. */
	KSimBaseIntEdit(QWidget * parent = 0, const char * name = 0);
	/** Constructs a @ref Edit widget for @ref KSimBaseInt objects. The start value is a empty string.
	  * The allowed range is from bottom to top. The validator @ref KSimBaseIntValidator is used. */
	KSimBaseIntEdit(int bottom, int top, QWidget * parent = 0, const char * name = 0);
	/** Constructs a @ref Edit widget for @ref KSimBaseInt objects. The start value is given by value.
	  * The allowed range is from  INT_MIN to INT_MAX. The validator @ref KSimBaseIntValidator is used. */
	KSimBaseIntEdit(const KSimBaseInt & value, QWidget * parent = 0, const char * name = 0);
	/** Constructs a @ref Edit widget for @ref KSimBaseInt objects. The start value is given by value.
	  * The allowed range is from  bottom to top. The validator @ref KSimBaseIntValidator is used. */
	KSimBaseIntEdit(const KSimBaseInt & value, int bottom, int top, QWidget * parent = 0, const char * name = 0);
	
	/** The destructor */
	~KSimBaseIntEdit();
	
	/** Returns the current value. */
	KSimBaseInt value() const;

  /** Sets the minimum value. */
  void setBottom(int minimum);
  /** Sets the maximum value. */
  void setTop(int maximum);
	/** Sets the valid range. */
	void setRange(int bottom, int top);

  /** Returns the minimum value. */
  int bottom() const;
  /** Returns the maximum value. */
  int top() const;

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
	void setValue(const KSimBaseInt & newValue);
	/** Sets a new value. Leave base unchanged. */
	void setValue(int newValue);


private slots:
	void slotTextChanged(const QString & text);
	void slotReady();
	void slotAboutToShowContextMenu(QPopupMenu* popup);
	void slotConvertToBinary();
	void slotConvertToOctal();
	void slotConvertToDecimal();
	void slotConvertToHexadecimal();

private:
	void init(int bottom, int top);
	void restoreValue();
	Private * m_p;


signals:
	void valueChangedTracking(const KSimBaseInt & value);
	void valueChanged(const KSimBaseInt & value);
};



#endif
