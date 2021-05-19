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

#include "integration_threadadapter.h"

IntegrationThreadAdapter::IntegrationThreadAdapter(Integration& integration, Plugin* plugin)
    : Integration(plugin), m_integration(integration) {
    setFriendlyName(integration.friendlyName());
    setIntegrationId(integration.integrationId());

    integration.moveToThread(&m_thread);

    // connect signals and slots
    QObject::connect(&m_thread, &QThread::finished, &integration, &QObject::deleteLater);

    QObject::connect(this, &IntegrationThreadAdapter::connectSignal, &integration, &Integration::connect);
    QObject::connect(this, &IntegrationThreadAdapter::disconnectSignal, &integration, &Integration::disconnect);
    QObject::connect(this, &IntegrationThreadAdapter::enterStandbySignal, &integration, &Integration::enterStandby);
    QObject::connect(this, &IntegrationThreadAdapter::leaveStandbySignal, &integration, &Integration::leaveStandby);
    QObject::connect(this, &IntegrationThreadAdapter::sendCommandSignal, &integration, &Integration::sendCommand);

    QObject::connect(&integration, &Integration::stateChanged, this, &IntegrationThreadAdapter::onStateChanged);

    m_thread.start();
}

IntegrationThreadAdapter::~IntegrationThreadAdapter() {
    if (m_thread.isRunning()) {
        m_thread.exit();
        m_thread.wait(5000);
    }
}

void IntegrationThreadAdapter::connect() {
    qCDebug(m_logCategory) << "ThreadAdapter connect";
    emit connectSignal();
}

void IntegrationThreadAdapter::disconnect() {
    qCDebug(m_logCategory) << "ThreadAdapter disconnect";
    emit disconnectSignal();
}

void IntegrationThreadAdapter::enterStandby() {
    qCDebug(m_logCategory) << "ThreadAdapter entering standby";
    emit enterStandbySignal();
}

void IntegrationThreadAdapter::leaveStandby() {
    qCDebug(m_logCategory) << "ThreadAdapter leaving standby";
    emit leaveStandbySignal();
}

QVariantList IntegrationThreadAdapter::getAllAvailableEntities() {
    qCDebug(m_logCategory) << "ThreadAdapter getAllAvailableEntities";
    return m_integration.getAllAvailableEntities();
}

void IntegrationThreadAdapter::sendCommand(const QString& type, const QString& entityId, int command,
                                           const QVariant& param) {
    qCDebug(m_logCategory) << "ThreadAdapter sendCommand" << type << entityId << command << param;
    emit sendCommandSignal(type, entityId, command, param);
}

void IntegrationThreadAdapter::onStateChanged() {
    m_state = m_integration.state();
    qCDebug(m_logCategory) << "ThreadAdapter state changed" << static_cast<States>(m_state);
    emit stateChanged();
    switch (m_state) {
        case States::CONNECTING:
            emit connecting();
            break;
    case States::CONNECTED:
            emit connected();
            break;
    case States::DISCONNECTED:
            emit disconnected();
            break;
    }
}
