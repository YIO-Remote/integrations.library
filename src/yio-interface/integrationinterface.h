/******************************************************************************
 *
 * Copyright (C) 2018-2019 Marton Borzak <hello@martonborzak.com>
 * Copyright (C) 2021 Michael Lörcher <MichaelLoercher@web.de>
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

#include <QVariant>
#include "integrationconnectionstates.h"

// FIXME provide complete API documentation with all QML requirements of the implementation!
/**
 * @brief The IntegrationInterface must be implemented by the integration plugins.
 */
class IntegrationInterface {
 public:
    // enum States { CONNECTED = 0, CONNECTING = 1, DISCONNECTED = 2, STANDBY = 3 };
    typedef IntegrationConnectionStates::States States;


    virtual ~IntegrationInterface();

    /**
     * @brief getAllAvailableEntities Can be implemented by integration
     * @returns list of QVariantMap
     * @details
     * https://github.com/YIO-Remote/documentation/wiki/developer-API-remote#get-available-entities-json-of-loaded-entities
     */
    virtual QVariantList getAllAvailableEntities() = 0;

    /**
     * @brief adds available entity from integration. custom features list is optional
     * @param entityId
     * @param type
     * @param integration
     * @param friendlyName
     * @param supportedFeatures
     */
    virtual bool addAvailableEntity(const QString& entityId, const QString& type, const QString& integration,
                                    const QString& friendlyName, const QStringList& supportedFeatures,
                                    const QStringList& customFeatures = QStringList()) = 0;

    /**
     * @brief sendCommand Must be implemented
     * @param type
     * @param entityId
     * @param command
     * @param param
     */
    virtual void sendCommand(const QString& type, const QString& entityId, int command, const QVariant& param) = 0;

    /**
     * @brief state Returns the current state. See States enum definition.
     * @details A Q_PROPERTY must be implemented with this method as READ accessor.
     */
    virtual int state() = 0;

    /**
     * @brief state Returns the integration identifier.
     * @details A Q_PROPERTY must be implemented with this method as READ accessor.
     */
    virtual QString integrationId() = 0;

    /**
     * @brief state Returns the friendly human readable name of the integration.
     * @details A Q_PROPERTY must be implemented with this method as READ accessor.
     */
    virtual QString friendlyName() = 0;

 public slots:
    /**
     * @brief connect Must be implemented by integration
     */
    virtual void connect() = 0;

    /**
     * @brief disconnect Must be implemented by integration
     */
    virtual void disconnect() = 0;

    /**
     * @brief enterStandby Can be implemented by integration
     */
    virtual void enterStandby() = 0;

    /**
     * @brief leaveStandby Can be implemented by integration
     */
    virtual void leaveStandby() = 0;
};

QT_BEGIN_NAMESPACE
#define IntegrationInterface_iid "YIO.IntegrationInterface"
Q_DECLARE_INTERFACE(IntegrationInterface, IntegrationInterface_iid)
QT_END_NAMESPACE
