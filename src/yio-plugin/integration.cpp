/******************************************************************************
 *
 * Copyright (C) 2019 Christian Riedl <ric@rts.co.at>
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

#include "integration.h"

// clang-format off
const QString Integration::KEY_ID                 = CFG_KEY_ID;
const QString Integration::KEY_FRIENDLYNAME       = CFG_KEY_FRIENDLYNAME;
const QString Integration::KEY_ENTITY_ID          = CFG_KEY_ENTITY_ID;
const QString Integration::KEY_AREA               = CFG_KEY_AREA;
const QString Integration::KEY_INTEGRATION        = CFG_KEY_INTEGRATION;
const QString Integration::KEY_SUPPORTED_FEATURES = CFG_KEY_SUPPORTED_FEATURES;
const QString Integration::KEY_CUSTOM_FEATURES    = CFG_KEY_CUSTOM_FEATURES;
const QString Integration::KEY_TYPE               = CFG_KEY_TYPE;
const QString Integration::KEY_MDNS               = CFG_KEY_MDNS;
const QString Integration::KEY_WORKERTHREAD       = CFG_KEY_WORKERTHREAD;
const QString Integration::OBJ_DATA               = CFG_OBJ_DATA;
const QString Integration::KEY_DATA_IP            = CFG_KEY_DATA_IP;
const QString Integration::KEY_DATA_URL           = CFG_KEY_DATA_URL;
const QString Integration::KEY_DATA_TOKEN         = CFG_KEY_DATA_TOKEN;
const QString Integration::KEY_DATA_SSL           = CFG_KEY_DATA_SSL;
const QString Integration::KEY_DATA_SSL_IGNORE    = CFG_KEY_DATA_SSL_IGNORE;
// clang-format on

IntegrationInterface::~IntegrationInterface() {}

Integration::Integration(const QVariantMap& config, EntitiesInterface* entities, NotificationsInterface* notifications,
                         YioAPIInterface* api, ConfigInterface* configObj, Plugin* plugin)
    : QObject(plugin->m_useWorkerThread ? nullptr : plugin),
      m_state(States::DISCONNECTED),
      m_entities(entities),
      m_useWorkerThread(plugin->m_useWorkerThread),
      m_notifications(notifications),
      m_yioapi(api),
      m_config(configObj),
      m_logCategory(plugin->m_logCategory) {
    // FIXME remove QVariantMap indirection for friendlyName and integrationId:
    //       plugins MUST set them themself. Otherwise it's just very confusing without any benefits.
    for (QVariantMap::const_iterator iter = config.cbegin(); iter != config.cend(); ++iter) {
        if (iter.key() == Integration::KEY_FRIENDLYNAME)
            m_friendlyName = iter.value().toString();
        else if (iter.key() == Integration::KEY_ID)
            m_integrationId = iter.value().toString();
    }
}

// Used for integration threading adapter
Integration::Integration(Plugin* plugin)
    : QObject(plugin),
      m_state(States::DISCONNECTED),
      m_entities(nullptr),
      m_notifications(nullptr),
      m_yioapi(nullptr),
      m_config(nullptr),
      m_logCategory(plugin->m_logCategory) {}

Integration::~Integration() {}

bool Integration::addAvailableEntity(const QString& entityId, const QString& type, const QString& integration,
                                     const QString& friendlyName, const QStringList& supportedFeatures,
                                     const QStringList& customFeatures) {
    // if the entity is already in the list, skip
    for (int i = 0; i < m_allAvailableEntities.length(); i++) {
        if (m_allAvailableEntities[i].toMap().value(Integration::KEY_ENTITY_ID).toString() == entityId) {
            return false;
        }
    }

    if (m_entities->isSupportedEntityType(type)) {
        QVariantMap entity;
        entity.insert(Integration::KEY_ENTITY_ID, entityId);
        entity.insert(Integration::KEY_TYPE, type);
        entity.insert(Integration::KEY_INTEGRATION, integration);
        entity.insert(Integration::KEY_FRIENDLYNAME, friendlyName);
        entity.insert(Integration::KEY_SUPPORTED_FEATURES, supportedFeatures);
        if (customFeatures.size() > 0) {
            entity.insert(Integration::KEY_CUSTOM_FEATURES, customFeatures);
        }
        m_allAvailableEntities.append(entity);
        return true;
    } else {
        // return false if the type is not supported
        return false;
    }
}

void Integration::setState(int state) {
    if (state == m_state) {
        return;
    }
    m_state = state;

    qCDebug(m_logCategory) << "Set state:" << static_cast<States>(state);

    emit stateChanged();
    switch (state) {
        case States::CONNECTING:
            if (!m_useWorkerThread) {
                emit connecting();
            }
            break;
        case States::CONNECTED:
            if (!m_useWorkerThread) {
                emit connected();
            }
            m_entities->setConnected(m_integrationId, true);
            break;
        case States::DISCONNECTED:
            if (!m_useWorkerThread) {
                emit disconnected();
            }
            m_entities->setConnected(m_integrationId, false);
            break;
    }
}
