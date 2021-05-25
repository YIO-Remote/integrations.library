# Project Include file for remote-software and plugin projects.
# Kind of a "remote-software-dev" package with all required header files for
# integration plugin development.
#
# MUST be included by remote-software & integration plugins.

INCLUDEPATH += $$PWD/src

HEADERS += \
    $$PWD/src/yio-interface/configinterface.h \
    $$PWD/src/yio-interface/integrationconnectionstates.h \
    $$PWD/src/yio-interface/integrationinterface.h \
    $$PWD/src/yio-interface/notificationsinterface.h \
    $$PWD/src/yio-interface/plugininterface.h \
    $$PWD/src/yio-interface/unitsystem.h \
    $$PWD/src/yio-interface/yioapiinterface.h \
    $$PWD/src/yio-interface/entities/blindinterface.h \
    $$PWD/src/yio-interface/entities/climateinterface.h \
    $$PWD/src/yio-interface/entities/entitiesinterface.h \
    $$PWD/src/yio-interface/entities/entityinterface.h \
    $$PWD/src/yio-interface/entities/lightinterface.h \
    $$PWD/src/yio-interface/entities/mediaplayerinterface.h \
    $$PWD/src/yio-interface/entities/remoteinterface.h \
    $$PWD/src/yio-interface/entities/switchinterface.h \
    $$PWD/src/yio-interface/entities/weatherinterface.h

# DO NOT DEFINE ANY SOURCE FILES in this project include!
# Source files must be defined in a shared model project file.
