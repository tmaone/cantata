/*
 * Cantata
 *
 * Copyright (c) 2011-2015 Craig Drummond <craig.p.drummond@gmail.com>
 *
 * ----
 *
 * This program is free software; you can redistribute it and/or modify
 * it under the terms of the GNU General Public License as published by
 * the Free Software Foundation; either version 2 of the License, or
 * (at your option) any later version.
 *
 * This program is distributed in the hope that it will be useful,
 * but WITHOUT ANY WARRANTY; without even the implied warranty of
 * MERCHANTABILITY or FITNESS FOR A PARTICULAR PURPOSE.  See the GNU
 * General Public License for more details.
 *
 * You should have received a copy of the GNU General Public License
 * along with this program; see the file COPYING.  If not, write to
 * the Free Software Foundation, Inc., 51 Franklin Street, Fifth Floor,
 * Boston, MA 02110-1301, USA.
 */

#include "icon.h"
#include "utils.h"
#include <QApplication>
#include <QToolButton>

int Icon::stdSize(int v)
{
    if (v<19) {
        return 16;
    } else if (v<=28) {
        return 22;
    } else if (v<=40) {
        return 32;
    } else if (v<=56) {
        return 48;
    } else if (v<=90) {
        return 64;
    }

    if (Utils::isHighDpi()) {
        if (v<=160) {
            return 128;
        } else {
            return 256;
        }
    }
    return 128;
}

int Icon::dlgIconSize()
{
    static int size=-1;

    if (-1==size) {
        size=stdSize(QApplication::fontMetrics().height()*3.5);
    }
    return size;
}

void Icon::init(QToolButton *btn, bool setFlat)
{
    static int size=-1;

    if (-1==size) {
        size=QApplication::fontMetrics().height();
        if (size>18) {
            size=stdSize(size*1.25);
        } else {
            size=16;
        }
    }
    btn->setIconSize(QSize(size, size));
    if (setFlat) {
        btn->setAutoRaise(true);
    }
}

#ifndef ENABLE_KDE_SUPPORT
Icon Icon::getMediaIcon(const QString &name)
{
    static QList<QIcon::Mode> modes=QList<QIcon::Mode>() << QIcon::Normal << QIcon::Disabled << QIcon::Active << QIcon::Selected;
    Icon icn;
    Icon icon(name);

    foreach (QIcon::Mode mode, modes) {
        icn.addPixmap(icon.pixmap(QSize(64, 64), mode).scaled(QSize(28, 28), Qt::KeepAspectRatio, Qt::SmoothTransformation), mode);
        icn.addPixmap(icon.pixmap(QSize(48, 48), mode), mode);
        icn.addPixmap(icon.pixmap(QSize(32, 32), mode), mode);
        icn.addPixmap(icon.pixmap(QSize(24, 24), mode), mode);
        icn.addPixmap(icon.pixmap(QSize(22, 22), mode), mode);
        icn.addPixmap(icon.pixmap(QSize(16, 16), mode), mode);
    }

    return icn;
}
#endif

Icon Icon::create(const QString &name, const QList<int> &sizes, bool andSvg)
{
    Icon icon;
    foreach (int s, sizes) {
        icon.addFile(QLatin1Char(':')+name+QString::number(s), QSize(s, s));
    }
    if (andSvg) {
        icon.addFile(QLatin1Char(':')+name+QLatin1String(".svg"));
    }
    return icon;
}

#ifdef Q_OS_MAC
Icon Icon::std(Std i)
{
    static bool initResource=true;
    if (initResource) {
        Q_INIT_RESOURCE(support_mac_icons);
        initResource=false;
    }
    Icon icon;
    switch (i) {
    case Close: icon.addFile(":support-mac-close"); break;
    case Clear: icon.addFile(":support-mac-clear"); break;
    default: break;
    }
    return icon;
}
#else
static QIcon closeIcon;
static QIcon clearIcon;

QIcon Icon::std(Std i)
{
    switch (i) {
    case Close:
        if (closeIcon.isNull()) {
            closeIcon=Icon("dialog-close");
            if (closeIcon.isNull()) {
                closeIcon=Icon("window-close");
            }
        }
        return closeIcon;
    case Clear:
        if (clearIcon.isNull()) {
            clearIcon=Icon(qApp->isRightToLeft() ? "edit-clear-locationbar-ltr" : "edit-clear-locationbar-rtl");
            if (clearIcon.isNull()) {
                clearIcon=Icon("edit-clear");
            }
        }
        return clearIcon;
    default: break;
    }
    return QIcon();
}

void Icon::setStd(Std i, const QIcon &icon)
{
    switch (i) {
    case Close:
        closeIcon=icon;
        break;
    case Clear:
        clearIcon=icon;
    default: break;
    }
}

#endif
