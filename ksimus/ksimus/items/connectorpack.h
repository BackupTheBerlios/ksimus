/***************************************************************************
                          connectorpack.h  -  description
                             -------------------
    begin                : Tue Nov 27 2001
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

#ifndef CONNECTORPACK_H
#define CONNECTORPACK_H

// C-Includes

// QT-Includes

// KDE-Includes

// Project-Includes
#include "connectorbase.h"
#include "componentaddon.h"
#include "types.h"


// Forward declaration
class ConnectorInfo;


/**Class which support your component with a variable number of connectors.

  *@author Rasmus Diekenbrock
  */

class ConnectorPack : public ComponentAddOn
{
  Q_OBJECT

public:
	
	/** Constructs a new connector pack.
		*
		* @param comp     The parent component.
		* @param name     The name is used for the storages of the connector pack. It is also used as default
		*                 connector name.
		* @param connInfo The info (means type) of the connector to add by this connector pack.
		* @param minConn  The minimum count of connectors. The default is one connector.
		* @param maxConn  The maximum count of connectors. The default are ten connecotrs.
		*/
	ConnectorPack(Component * comp, const QString & name, const ConnectorInfo * connInfo,
	              unsigned int minConn = 1, unsigned int maxConn = 10);
	/** The destructor.
	 */
	~ConnectorPack();

	/** Returns the list of the current connectors.
	 */
	ConnectorList * getConnList() const { return m_connList; };
		
	/** Sets the current count of connectors. Connecotrs are created or deleted if the count not match.
	 */
	void setConnectorCount(unsigned int connCount);
	/** Returns the current count of connectors.
	 */
	unsigned int getConnectorCount() const;
	
	/** Sets the minimum count of connectors. The function creates no missing connectors!
	 */
	void setConnectorMinimum(unsigned int minConn);
	/** Returns the minimum count of connectors.
	 */
	unsigned int getConnectorMinimum() const { return m_minConn; };
	
	/** Sets the maximum count of connectors. The function delete no supernumeray connectors!
	 */
	void setConnectorMaximum(unsigned int minConn);
	/** Returns the maximum count of connectors.
	 */
	unsigned int getConnectorMaximum() const { return m_maxConn; };
	
  /** Returns the info (means type) of the connector.
   */
  const ConnectorInfo * getConnectorInfo() const { return m_connInfo; };
  /** Returns the name of this connector pack. It is also used as default connector name. */
	QString getName() const { return m_name; };

	/** Initialize the component popup menu.
	  *	Return true, if items are added.
		* This function takes care about the KSimAction information.
		*/
	bool initPopupMenu(QPopupMenu * popup);
	/** Save component add on's properties.
		* The default implementation does nothing.
		*/
	virtual void save(KSimData & file) const;
	/** Load component add on's properties.
		*	Returns true if successful.
		* The default implementation does nothing.
		*/
	virtual bool load(KSimData & file);
	
	/** Set the connector name here. The set connector name before calling first @ref setConnectorCount
	  * Don't forget the following %1.
	  */
	void setConnectorName(const QString & connName);
	/** Returns the current connector name.
	  */
	QString getConnectorName() const;
	
	/** The connectors are named with letters if ture, or with numbers is false. The default are letters. */
	void setLetter(bool letter);
	/** Returns ture if the connector are named with letter, or false if named with numbers. */
	bool isLetter() const;
	/** Sets the orientation of the connectors. */
  void setOrientation(ConnOrientationType orient);
  /** Returns the orientations of the connectors. */
  ConnOrientationType getOrientation() const { return m_orientation; };

  /** Set true, if only the last of all connector shall be deleted. If false, the last unwired connector
    * will delete. The default is delete last only.
    *
    * TODO: Do not use it! Works not properly!
    */
  void setDeleteLastOnly(bool lastOnly);
  /** Returns true, if only the last of all connector shall be deleted. If false, the last unwired connector
    * will delete.
    */
  bool isDeleteLastOnly() const;


public slots:
	/** Call this slot to add a connector. The slot is not successful if the maximum count is reached. */
	void slotAddConnector();
	/** Call this slot to delete a connector. The slot tests if one or the last connector is not connected
	  * and deletes it. */
	void slotDeleteConnector();
	/** Call this slot to delete a certain connector. The slot does nothing, if the pack don't contain the
	  * connector or the miminim connector count is reached.
	  */
	void slotDeleteConnector(ConnectorBase * conn);

protected:
	/** The function creates a name for a new connector. Per default it is the pack name and a letter or
	  * a number depenning on the setting of @ref setLetter.
	  */
	virtual QString createNewName();
	/** Returns the next connector to delete or zero if all connector are connected.
	  */
	virtual ConnectorBase * getDeletableConnector();
		
private: 		//functions
	/** This internal function adds a new connector.
	  *
	  * It tests the maximum connector count (set by @ref setConnectorMaximum) and if the maximum is
	  * not reached it creates a new connector and after that emits the signal @ref signalAddConnector.
	  * The function returns a pointer to the created connector.
	  */
	ConnectorBase * internalAddConnector();
	
	/** This functions deletes a coonnector.
	  *
	  * The function tests if one or the last connector is not connected and deletets it. The signal
	  * @ref signalDeleteConnector is emitted before the connector is deleted and the signal
	  * @ref signalDeletedConnector is emitted after the connector is deleted. The function returns
	  * true, if a connector is deleted and false if no connector is deleted.
	  */
	bool internalDeleteConnector();

private:
	QString m_name;
	ConnectorList * m_connList;
	const ConnectorInfo * m_connInfo;
	unsigned int m_minConn;
	unsigned int m_maxConn;
	QString m_connName;
	ConnOrientationType m_orientation;
	Q_UINT32 m_flags;
	
signals:
	/** This signal is emitted after a connectoer is created.
	  */
	void signalAddConnector(ConnectorBase * conn);
	/** This signal is emitted before a connector is deleted.
	  */
	void signalDeleteConnector(ConnectorBase * conn);
	/** This signal is emitted after a connector is deleted.
	  */
	void signalDeletedConnector();

};

#endif
