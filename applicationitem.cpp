#include "applicationitem.h"

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
