#include "applicationitemslist.h"
//#include "applicationitem.h"
#include <QDebug>
#include <QMessageBox>

ApplicationItemsList::ApplicationItemsList()
{

}

void ApplicationItemsList::clear()
{
    m_ProcessItemsList.clear ();
}

void ApplicationItemsList::removeAt(int iIndex)
{
    m_ProcessItemsList.removeAt (iIndex);
}

int ApplicationItemsList::size() const
{
    return m_ProcessItemsList.size ();
}

int ApplicationItemsList::count() const
{
    return m_ProcessItemsList.count ();

}

ApplicationItem* ApplicationItemsList::at(int i)
{
    return &m_ProcessItemsList[i];
}

void ApplicationItemsList::append(ApplicationItem item)
{
    m_ProcessItemsList.append (item);
}

ApplicationItem *ApplicationItemsList::findApplicationItem(QString sFound)
{
    int i_AppItemCount = this->count ();
    ApplicationItem *foundItem;
    // for (int iCount = i_AppItemCount - 1; iCount >= 0; --iCount)
    for (int iCount = 0; iCount < i_AppItemCount; iCount++)
    {
        foundItem = this->at(iCount);
        if (foundItem->getAppName () == sFound) return foundItem;
    }
    return NULL;
}

int ApplicationItemsList::findApplicationItemIndex(QString sFound)
{
    int i_AppItemCount = this->count ();
    ApplicationItem *foundItem;
    // for (int iCount = i_AppItemCount - 1; iCount >= 0; --iCount)
    for (int iCount = 0; iCount < i_AppItemCount; iCount++)
    {
        foundItem = this->at(iCount);
        if (foundItem->getAppName () == sFound) return iCount;
    }
    return -1;
}

void ApplicationItemsList::resetAllApplicationItems()
{
    int i_AppItemCount = this->count ();
    ApplicationItem *foundItem;
    // for (int iCount = i_AppItemCount - 1; iCount >= 0; --iCount)
    for (int iCount = 0; iCount < i_AppItemCount; iCount++)
    {
        foundItem = this->at(iCount);
        foundItem->setFoundWhenKilling (true);
    }
}

bool ApplicationItemsList::deleteApplicationItem(QString deleteString)
{
    bool bFound = false;
    QString itemString = "";
    // Iterate in reverse to safely remove items while modifying the list
    //qDebug()<< "m_ApplicationItemsList.size="<<m_ApplicationItemsList.size();
    int iFoundItem = this->findApplicationItemIndex (deleteString);
    if (iFoundItem != -1)
    {
        bFound = true;
        this->removeAt (iFoundItem);
        LOG_MSG( "m_ApplicationItemsList.size= " << this->size());
    }
    // for (int i = m_ApplicationItemsList.size() - 1; i >= 0; --i)
    // {
    //        //qDebug()<< "m_ApplicationItemsList.at="<<i;
    //        //qDebug()<< m_ApplicationItemsList.at(i).getAppName();
    // if (m_ApplicationItemsList.at(i).getAppName() == deleteString)
    // {
    // m_ApplicationItemsList.removeAt(i);
    // qDebug() << "m_ApplicationItemsList.size=" << m_ApplicationItemsList.size();
    // bFound = true;
    // }
    // for (int i = m_ApplicationItemsList.count() - 1; i >= 0; --i) {
    // itemString = m_ApplicationItemsList.at(i).getAppName();
    // if (itemString == deleteString) {
    // bFound = true;
    // m_ApplicationItemsList.takeAt(i);
    // }
    // }
    if (!bFound)
       QMessageBox::information(nullptr, "NOT Removed", QString("NOT Removed all items with text '%1'.").arg(deleteString));
    return bFound;
}


bool ApplicationItemsList::moveApplicationItem(QString deleteString, bool bState)
{
    bool bFound = false;
    QString itemString = "";
    // Iterate in reverse to safely remove items while modifying the list
    LOG_MSG( "m_ApplicationItemsList.size= " << this->size());
    int iFoundItem = this->findApplicationItemIndex (deleteString);
    if (iFoundItem != -1)
    {
        bFound = true;
        this->at(iFoundItem)->setAppKillEnabled(bState);
        //LOG_MSG( "m_ApplicationItemsList.size= " << this->size());
    }
    // for (int i = m_ApplicationItemsList.size() - 1; i >= 0; --i)
    // {
    //        //qDebug()<< "m_ApplicationItemsList.at="<<i;
    //        //qDebug()<< m_ApplicationItemsList.at(i).getAppName();
    // if (m_ApplicationItemsList.at(i).getAppName() == deleteString)
    // {
    //            //qDebug()<< "m_ApplicationItemsList.at(i)="<<m_ApplicationItemsList.at(i).getAppKillEnabled ();
    //            // ApplicationItem foundItem=m_ApplicationItemsList.at(i);
    // m_ApplicationItemsList[i].setAppKillEnabled(bState);
    // bFound = true;
    //            //qDebug()<< "m_ApplicationItemsList.at(i)="<<m_ApplicationItemsList.at(i).getAppKillEnabled ();
    //            // qDebug()<< "foundItem="<<foundItem.getAppKillEnabled ();
    // }
    //        // for (int i = m_ApplicationItemsList.count() - 1; i >= 0; --i) {
    //        // itemString = m_ApplicationItemsList.at(i).getAppName();
    //        // if (itemString == deleteString) {
    //        // bFound = true;
    //        // m_ApplicationItemsList.takeAt(i);
    //        // }
    // }
    if (!bFound)
        QMessageBox::information(nullptr, "NOT moved", QString("NOT moved all items with text '%1'.").arg(deleteString));
    return bFound;
}

bool ApplicationItem::getFoundWhenKilling() const
{
    return foundWhenKilling;
}

void ApplicationItem::setFoundWhenKilling(bool value)
{
    foundWhenKilling = value;
}

ApplicationItem::ApplicationItem(QString sAppName, bool bAppKillEnabled)
{
    appName = sAppName;
    appKillEnabled = bAppKillEnabled;
    foundWhenKilling=true;
}

bool ApplicationItem::getAppKillEnabled() const
{
    return appKillEnabled;
}

void ApplicationItem::setAppKillEnabled(bool value)
{
    appKillEnabled = value;
}

QString ApplicationItem::getAppName() const
{
    return appName;
}

void ApplicationItem::setAppName(const QString& value)
{
    appName = value;
}
