#ifndef APPLICATIONITEM_H
#define APPLICATIONITEM_H

#include <QString>

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

#endif // APPLICATIONITEM_H
