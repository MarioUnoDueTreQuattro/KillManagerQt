#ifndef APPLICATIONITEMLIST_H
#define APPLICATIONITEMLIST_H

#include <QList>
#include <QString>
#include "utility.h"

class ApplicationItem
{
private:
    QString appName;
    bool appKillEnabled;
    bool foundWhenKilling;
public:
    //ApplicationItem();
    ApplicationItem(QString, bool);
    QString getAppName() const;
    void setAppName(const QString &value);
    bool getAppKillEnabled() const;
    void setAppKillEnabled(bool value);
    bool getFoundWhenKilling() const;
    void setFoundWhenKilling(bool value);
};

class ApplicationItemsList
{
public:
    ApplicationItemsList();
    void clear();
    void removeAt (int);
    int size() const;
    int count() const;
    ApplicationItem *at(int i);
    void append( ApplicationItem);
    int findApplicationItemIndex(QString );
    ApplicationItem *findApplicationItem(QString );
    void resetAllApplicationItems();
    bool deleteApplicationItem(QString);
    bool moveApplicationItem(QString, bool );
private:
    QList<ApplicationItem> m_list;
};


#endif // APPLICATIONITEMLIST_H
