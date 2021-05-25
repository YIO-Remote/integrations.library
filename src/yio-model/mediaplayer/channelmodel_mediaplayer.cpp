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

#include "channelmodel_mediaplayer.h"

ListChannelModel::ListChannelModel(QObject *parent) : QAbstractListModel(parent) {}

int ListChannelModel::count() const {
    return m_count;
}

int ListChannelModel::rowCount(const QModelIndex &p) const {
    Q_UNUSED(p)
    return m_data.size();
}

QVariant ListChannelModel::data(const QModelIndex &index, int role) const {
    if (index.row() < 0 || index.row() >= m_data.count()) return QVariant();
    const ChannelModelItem &item = m_data[index.row()];
    switch (role) {
        case KeyRole:
            return item.itemKey();
        case TimeRole:
            return item.itemTime();
        case TitleRole:
            return item.itemTitle();
        case SubTitleRole:
            return item.itemSubtitle();
        case TypeRole:
            return item.itemType();
        case ImageUrlRole:
            return item.itemImageUrl();
        case CommandsRole:
            return item.itemCommands();
    }
    return QVariant();
}

QHash<int, QByteArray> ListChannelModel::roleNames() const {
    QHash<int, QByteArray> roles;
    roles[KeyRole] = "item_key";
    roles[TimeRole] = "item_time";
    roles[TitleRole] = "item_title";
    roles[SubTitleRole] = "item_subtitle";
    roles[TypeRole] = "item_type";
    roles[ImageUrlRole] = "item_image";
    roles[CommandsRole] = "item_commands";
    return roles;
}

void ListChannelModel::append(const ChannelModelItem &o) {
    int i = m_data.size();
    beginInsertRows(QModelIndex(), i, i);
    m_data.append(o);

    // Emit changed signals
    emit countChanged(count());
    endInsertRows();
}

void ListChannelModel::setCount(int count) {
    if (m_count == count) return;

    m_count = count;
    emit countChanged(m_count);
}

void ListChannelModel::reset() {
    beginResetModel();
    m_data.clear();

    // Emit changed signals
    emit countChanged(count());
    endResetModel();
}

void BrowseChannelModel::addchannelItem(const QString &key, const QString &time, const QString &title,
                                        const QString &subtitle, const QString &type, const QString &imageUrl,
                                        const QVariant &commands) {
    ListChannelModel *model = static_cast<ListChannelModel *>(m_model);
    ChannelModelItem  item = ChannelModelItem(key, time, title, subtitle, type, imageUrl, commands);
    model->append(item);

    emit modelChanged();
}

void BrowseChannelModel::clearItems() {
    if (m_model) {
        delete m_model;
    }

    m_model = new ListChannelModel();

    emit modelChanged();
}

void BrowseChannelModel::clearProperties() {
    m_id.clear();
    m_time.clear();
    m_title.clear();
    m_subtitle.clear();
    m_type.clear();
    m_imageUrl.clear();
    m_commands.clear();
}

void BrowseChannelModel::reset() {
    ListChannelModel *model = static_cast<ListChannelModel *>(m_model);
    model->reset();
    emit modelChanged();
}

void BrowseChannelModel::update() {
    emit modelChanged();
}
