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
#include <QVariant>
#include <QtDebug>

class EPGModelItem {
 public:
    EPGModelItem(const QString& key, const int& xCoordinate, const int& column, const int& width, const int& height,
                 const QString& type, const QString& epgItemColor, const QString& epgItemTextColor,
                 const QString& title, const QString& subtitle, const QString& description, const QString& startTime,
                 const QString& endTime, const QString& imageUrl, const QVariant& commands)
        : m_key(key),
          m_xCoordinate(xCoordinate),
          m_column(column),
          m_width(width),
          m_height(height),
          m_type(type),
          m_epgItemColor(epgItemColor),
          m_epgItemTextColor(epgItemTextColor),
          m_title(title),
          m_subtitle(subtitle),
          m_description(description),
          m_startTime(startTime),
          m_endTime(endTime),
          m_imageUrl(imageUrl),
          m_commands(commands) {}

    QString  itemKey() const { return m_key; }
    int      itemXCoordinate() const { return m_xCoordinate; }
    int      itemColumn() const { return m_column; }
    int      itemWidth() const { return m_width; }
    int      itemHeight() const { return m_height; }
    QString  itemType() const { return m_type; }
    QString  itemEpgItemColor() const { return m_epgItemColor; }
    QString  itemEpgItemTextColor() const { return m_epgItemTextColor; }
    QString  itemTitle() const { return m_title; }
    QString  itemSubTitle() const { return m_subtitle; }
    QString  itemDescription() const { return m_description; }
    QString  itemStartTime() const { return m_startTime; }
    QString  itemEndTime() const { return m_endTime; }
    QString  itemImageUrl() const { return m_imageUrl; }
    QVariant itemCommands() const { return m_commands; }

 private:
    QString  m_key;
    int      m_xCoordinate;
    int      m_column;
    int      m_width;
    int      m_height;
    QString  m_type;
    QString  m_epgItemColor;
    QString  m_epgItemTextColor;
    QString  m_title;
    QString  m_subtitle;
    QString  m_description;
    QString  m_startTime;
    QString  m_endTime;
    QString  m_imageUrl;
    QVariant m_commands;
};

class ListEPGModel : public QAbstractListModel {
    Q_OBJECT
    Q_PROPERTY(int count READ count WRITE setCount NOTIFY countChanged)

 public:
    enum SearchRoles {
        KeyRole = Qt::UserRole + 1,
        XCoordinateRole,
        ColumnRole,
        WidthRole,
        HeightRole,
        TypeRole,
        EpgItemColorRole,
        EpgItemTextColorRole,
        AnchorleftRole,
        TitleRole,
        SubTitleRole,
        DescriptionRole,
        StartTimeRole,
        EndTimeRole,
        ImageUrlRole,
        CommandsRole
    };

    explicit ListEPGModel(QObject* parent = nullptr);
    ~ListEPGModel() {}

    int                    count() const;
    int                    rowCount(const QModelIndex& p = QModelIndex()) const;
    QVariant               data(const QModelIndex& index, int role = Qt::DisplayRole) const;
    QHash<int, QByteArray> roleNames() const;

    void append(const EPGModelItem& o);
    void reset();

 public slots:
    Q_INVOKABLE void setCount(int count);

 signals:
    void countChanged(int count);

 private:
    int                 m_count;
    QList<EPGModelItem> m_data;
};

class BrowseEPGModel : public QObject {
    Q_OBJECT
    Q_PROPERTY(QString id READ id WRITE setId NOTIFY idChanged)
    Q_PROPERTY(int xCoordinate READ xCoordinate WRITE setXCoordinate NOTIFY xCoordinateChanged)
    Q_PROPERTY(int column READ column WRITE setColumn NOTIFY columnChanged)
    Q_PROPERTY(int width READ width WRITE setWidth NOTIFY widthChanged)
    Q_PROPERTY(int height READ height WRITE setHeight NOTIFY heightChanged)
    Q_PROPERTY(QString type READ type WRITE setType NOTIFY typeChanged)
    Q_PROPERTY(QString epgItemColor READ epgItemColor WRITE setEpgItemColor NOTIFY epgItemColorChanged)
    Q_PROPERTY(QString epgItemTextColor READ epgItemTextColor WRITE setEpgItemTextColor NOTIFY epgItemTextColorChanged)
    Q_PROPERTY(QString title READ title WRITE setTitle NOTIFY titleChanged)
    Q_PROPERTY(QString subtitle READ subtitle WRITE setSubtitle NOTIFY subtitleChanged)
    Q_PROPERTY(QString description READ description WRITE setDescription NOTIFY descriptionChanged)
    Q_PROPERTY(QString startTime READ startTime WRITE setStartTime NOTIFY startTimeChanged)
    Q_PROPERTY(QString endTime READ endTime WRITE setEndTime NOTIFY endTimeChanged)
    Q_PROPERTY(QString imageUrl READ imageUrl WRITE setImageUrl NOTIFY imageUrlChanged)
    Q_PROPERTY(QObject* model READ model NOTIFY modelChanged)
    Q_PROPERTY(QStringList commands READ commands WRITE setCommands NOTIFY commandsChanged)

 public:
    BrowseEPGModel(QObject* parent = nullptr, const QString& id = "", const int& xCoordinate = 0, const int& column = 0,
                   const int& width = 0, const int& height = 0, const QString& type = "",
                   const QString& epgItemColor = "", const QString& epgItemTextColor = "", const QString& title = "",
                   const QString& subtitle = "", const QString& description = "", const QString& startTime = "",
                   const QString& endTime = "", const QString& imageUrl = "", const QStringList& commands = {})
        : m_id(id),
          m_xCoordinate(xCoordinate),
          m_column(column),
          m_width(width),
          m_height(height),
          m_type(type),
          m_epgItemColor(epgItemColor),
          m_epgItemTextColor(epgItemTextColor),
          m_title(title),
          m_subtitle(subtitle),
          m_description(description),
          m_startTime(startTime),
          m_endTime(endTime),
          m_imageUrl(imageUrl),
          m_commands(commands) {
        Q_UNUSED(parent)
    }

    ~BrowseEPGModel() {}

    QString     id() { return m_id; }
    int         xCoordinate() { return m_xCoordinate; }
    int         column() { return m_column; }
    int         width() { return m_width; }
    int         height() { return m_height; }
    QString     type() { return m_type; }
    QString     epgItemColor() { return m_epgItemColor; }
    QString     epgItemTextColor() { return m_epgItemTextColor; }
    QString     title() { return m_title; }
    QString     subtitle() { return m_subtitle; }
    QString     description() { return m_description; }
    QString     startTime() { return m_startTime; }
    QString     endTime() { return m_endTime; }
    QString     imageUrl() { return m_imageUrl; }
    QObject*    model() { return m_model; }
    QStringList commands() { return m_commands; }

    // Since we reuse BrowseModel we need setters
    void setId(const QString& id) { m_id = id; }
    void setXCoordinate(const int& xCoordinate) { m_xCoordinate = xCoordinate; }
    void setColumn(const int& column) { m_column = column; }
    void setWidth(const int& width) { m_width = width; }
    void setHeight(const int& height) { m_height = height; }
    void setType(const QString& type) { m_type = type; }
    void setEpgItemColor(const QString& epgItemColor) { m_epgItemColor = epgItemColor; }
    void setEpgItemTextColor(const QString& epgItemTextColor) { m_epgItemTextColor = epgItemTextColor; }
    void setTitle(const QString& title) { m_title = title; }
    void setSubtitle(const QString& subtitle) { m_subtitle = subtitle; }
    void setDescription(const QString& description) { m_description = description; }
    void setStartTime(const QString& startTime) { m_startTime = startTime; }
    void setEndTime(const QString& endTime) { m_endTime = endTime; }
    void setImageUrl(const QString& imageUrl) { m_imageUrl = imageUrl; }

    void setCommands(const QStringList& commands) { m_commands = commands; }

    void addEPGItem(const QString& key, const int& xCoordinate, const int& column, const int& width, const int& height,
                    const QString& type, const QString& epgItemColor, const QString& epgItemTextColor,
                    const QString& title, const QString& subtitle, const QString& description, const QString& startTime,
                    const QString& endTime, const QString& imageUrl, const QVariant& commands);
    // Since we reuse BrowseModel we need to clear former items
    void clearItems();

    // Since we reuse BrowseModel we need to clear former properties
    void clearProperties();
    void reset();
    void update();
 signals:
    void idChanged();
    void xCoordinateChanged();
    void columnChanged();
    void widthChanged();
    void heightChanged();
    void typeChanged();
    void epgItemColorChanged();
    void epgItemTextColorChanged();
    void titleChanged();
    void subtitleChanged();
    void descriptionChanged();
    void startTimeChanged();
    void endTimeChanged();
    void imageUrlChanged();
    void modelChanged();
    void commandsChanged();

 private:
    QString     m_id;
    int         m_xCoordinate;
    int         m_column;
    int         m_width;
    int         m_height;
    QString     m_type;
    QString     m_epgItemColor;
    QString     m_epgItemTextColor;
    QString     m_title;
    QString     m_subtitle;
    QString     m_description;
    QString     m_startTime;
    QString     m_endTime;
    QString     m_imageUrl;
    QObject*    m_model = new ListEPGModel();
    QStringList m_commands;
};
