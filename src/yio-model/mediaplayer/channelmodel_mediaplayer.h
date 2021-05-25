/******************************************************************************
 *
 * Copyright (C) 2020 Michael Lörcher <MichaelLoecher@web.de>
 *
 * This file is part of the YIO-Remote software project.
 *
 * YIO-Remote software is free software: you can redistribute it and/or modify
 * it under the terms of the GNU General Public License as published by
 * the Free Software Foundation, either version 3 of the License, or
 * (at your option) any later version.
 *
 * YIO-Remote software is distributed in the hope that it will be useful,
 * but WITHOUT ANY WARRANTY; without even the implied warranty of
 * MERCHANTABILITY or FITNESS FOR A PARTICULAR PURPOSE.  See the
 * GNU General Public License for more details.
 *
 * You should have received a copy of the GNU General Public License
 * along with YIO-Remote software. If not, see <https://www.gnu.org/licenses/>.
 *
 * SPDX-License-Identifier: GPL-3.0-or-later
 *****************************************************************************/

#pragma once

#include <QAbstractListModel>
#include <QObject>
#include <QQmlListProperty>
#include <QVariant>
#include <QtDebug>
#include <QtQml>

class ChannelModelItem {
 public:
    ChannelModelItem(const QString& key, const QString& time, const QString& title, const QString& subtitle,
                     const QString& type, const QString& imageUrl, const QVariant& commands)
        : m_key(key),
          m_time(time),
          m_title(title),
          m_subtitle(subtitle),
          m_type(type),
          m_imageUrl(imageUrl),
          m_commands(commands) {}

    QString  itemKey() const { return m_key; }
    QString  itemTime() const { return m_time; }
    QString  itemTitle() const { return m_title; }
    QString  itemSubtitle() const { return m_subtitle; }
    QString  itemType() const { return m_type; }
    QString  itemImageUrl() const { return m_imageUrl; }
    QVariant itemCommands() const { return m_commands; }

 private:
    QString  m_key;
    QString  m_time;
    QString  m_title;
    QString  m_subtitle;
    QString  m_type;
    QString  m_imageUrl;
    QVariant m_commands;
};

class ListChannelModel : public QAbstractListModel {
    Q_OBJECT
    Q_PROPERTY(int count READ count WRITE setCount NOTIFY countChanged)

 public:
    enum SearchRoles {
        KeyRole = Qt::UserRole + 1,
        TimeRole,
        TitleRole,
        SubTitleRole,
        TypeRole,
        ImageUrlRole,
        CommandsRole
    };

    explicit ListChannelModel(QObject* parent = nullptr);
    ~ListChannelModel() {}

    int                    count() const;
    int                    rowCount(const QModelIndex& p = QModelIndex()) const;
    QVariant               data(const QModelIndex& index, int role = Qt::DisplayRole) const;
    QHash<int, QByteArray> roleNames() const;

 public slots:
    Q_INVOKABLE void append(const ChannelModelItem& o);
    Q_INVOKABLE void reset();
    Q_INVOKABLE void setCount(int count);

 signals:
    void countChanged(int count);

 private:
    int                     m_count;
    QList<ChannelModelItem> m_data;
};

class BrowseChannelModel : public QObject {
    Q_OBJECT
    Q_PROPERTY(QString id READ id WRITE setId NOTIFY idChanged)
    Q_PROPERTY(QString time READ time WRITE setId NOTIFY timeChanged)
    Q_PROPERTY(QString title READ title WRITE setTitle NOTIFY titleChanged)
    Q_PROPERTY(QString subtitle READ subtitle WRITE setSubtitle NOTIFY subtitleChanged)
    Q_PROPERTY(QString type READ type WRITE setType NOTIFY typeChanged)
    Q_PROPERTY(QString imageUrl READ imageUrl WRITE setImageUrl NOTIFY imageUrlChanged)
    Q_PROPERTY(QObject* model READ model NOTIFY modelChanged)
    Q_PROPERTY(QStringList commands READ commands WRITE setCommands NOTIFY commandsChanged)

 public:
    BrowseChannelModel(QObject* parent = nullptr, const QString& id = "", const QString& time = "",
                       const QString& title = "", const QString& subtitle = "", const QString& type = "",
                       const QString& imageUrl = "", const QStringList& commands = {})
        : m_id(id),
          m_time(time),
          m_title(title),
          m_subtitle(subtitle),
          m_type(type),
          m_imageUrl(imageUrl),
          m_commands(commands) {
        Q_UNUSED(parent)
    }

    ~BrowseChannelModel() {}

    QString     id() { return m_id; }
    QString     time() { return m_time; }
    QString     title() { return m_title; }
    QString     subtitle() { return m_subtitle; }
    QString     type() { return m_type; }
    QString     imageUrl() { return m_imageUrl; }
    QObject*    model() { return m_model; }
    QStringList commands() { return m_commands; }

    // Since we reuse BrowseModel we need setters
    void setId(const QString& id) { m_id = id; }
    void setTime(const QString& time) { m_time = time; }
    void setTitle(const QString& title) { m_title = title; }
    void setSubtitle(const QString& subtitle) { m_subtitle = subtitle; }
    void setType(const QString& type) { m_type = type; }
    void setImageUrl(const QString& imageUrl) { m_imageUrl = imageUrl; }
    void setCommands(const QStringList& commands) { m_commands = commands; }

    void addchannelItem(const QString& key, const QString& time, const QString& title, const QString& subtitle,
                        const QString& type, const QString& imageUrl, const QVariant& commands);

    // Since we reuse BrowseModel we need to clear former items
    void clearItems();

    // Since we reuse BrowseModel we need to clear former properties
    void clearProperties();

    void reset();
    void update();
 signals:
    void idChanged();
    void timeChanged();
    void titleChanged();
    void subtitleChanged();
    void typeChanged();
    void imageUrlChanged();
    void modelChanged();
    void commandsChanged();

 private:
    QString     m_id;
    QString     m_time;
    QString     m_title;
    QString     m_subtitle;
    QString     m_type;
    QString     m_imageUrl;
    QObject*    m_model = new ListChannelModel();
    QStringList m_commands;
};
