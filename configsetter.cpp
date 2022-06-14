/*
 * Copyright (C) 2020 ~ 2021 Uniontech Software Technology Co., Ltd.
 *
 * Author:     LiuMingHang <liuminghang@uniontech.com>
 *
 * Maintainer: ZhangYong <ZhangYong@uniontech.com>
 *
 * This program is free software: you can redistribute it and/or modify
 * it under the terms of the GNU General Public License as published by
 * the Free Software Foundation, either version 3 of the License, or
 * any later version.
 *
 * This program is distributed in the hope that it will be useful,
 * but WITHOUT ANY WARRANTY; without even the implied warranty of
 * MERCHANTABILITY or FITNESS FOR A PARTICULAR PURPOSE.  See the
 * GNU General Public License for more details.
 *
 * You should have received a copy of the GNU General Public License
 * along with this program.  If not, see <http://www.gnu.org/licenses/>.
 */
#include "configsetter.h"

#include <QDebug>
#include <QDir>
#include <QFileInfo>

const QString CONFIG_PATH =   QDir::homePath() + "/.config/openUrls/config.ini";

ConfigSetter::ConfigSetter(QObject *parent) : QObject(parent)
{
    m_settings = new QSettings(CONFIG_PATH, QSettings::IniFormat);
}

ConfigSetter *ConfigSetter::m_setter = nullptr;
ConfigSetter *ConfigSetter::instance()
{
    if (! m_setter) {
        m_setter = new ConfigSetter();
    }

    return m_setter;
}

void ConfigSetter::setValue(const QString &key,
                               const QVariant &value)
{
    m_settings->setValue(key, value);
}

QVariant ConfigSetter::value(const QString &key,
                                const QVariant &defaultValue)
{
    QMutexLocker locker(&m_mutex);
    QVariant value;
    value = m_settings->value(key, defaultValue);
    return value;
}

void ConfigSetter::clear()
{
    m_settings->clear();
}
