#  This file is part of Akonadi EWS Resource
#  Copyright (C) 2015-2017 Krzysztof Nowicki <krissn@op.pl>
#
#  This library is free software; you can redistribute it and/or
#  modify it under the terms of the GNU Library General Public
#  License as published by the Free Software Foundation; either
#  version 2 of the License, or (at your option) any later version.
#
#  This library is distributed in the hope that it will be useful,
#  but WITHOUT ANY WARRANTY; without even the implied warranty of
#  MERCHANTABILITY or FITNESS FOR A PARTICULAR PURPOSE.  See the GNU
#  Library General Public License for more details.
#
#  You should have received a copy of the GNU Library General Public License
#  along with this library; see the file COPYING.LIB.  If not, write to
#  the Free Software Foundation, Inc., 51 Franklin Street, Fifth Floor,
#  Boston, MA 02110-1301, USA.

cmake_minimum_required(VERSION 3.1.0)

project(akonadi-ews)

find_package(ECM 5.16.0 NO_MODULE)
set(CMAKE_MODULE_PATH ${CMAKE_CURRENT_SOURCE_DIR}/cmake/modules ${ECM_MODULE_PATH} ${ECM_KDE_MODULE_DIR})

set(REQUIRED_QT_VERSION 5.4.0)
if(ENABLE_TESTS)
  set(EXTRA_QT_PACKAGES Test XmlPatterns)
endif(ENABLE_TESTS)
find_package(Qt5 ${REQUIRED_QT_VERSION} CONFIG REQUIRED Core ${EXTRA_QT_PACKAGES})

include(KDEInstallDirs)
include(KDEFrameworkCompilerSettings)
include(KDECMakeSettings)
include(FeatureSummary)

find_package(KF5KIO)
find_package(KF5Akonadi)
find_package(KF5AkonadiMime)
find_package(Xsltproc)
find_package(KF5Mime)
find_package(KF5CalendarCore)
find_package(KF5Contacts)
find_package(KF5Codecs)
find_package(KF5KDELibs4Support)
find_package(KF5Wallet)
find_package(KF5WidgetsAddons)

option(HAVE_SEPARATE_MTA_RESOURCE "Build a separate resource for sending e-mail." TRUE)
option(HAVE_INBOX_FILTERING_WORKAROUND "Include inbox filtering workaround." TRUE)
option(ENABLE_TESTS "Build the test suite." FALSE)

set(AKONADI_EWS_VERSION "0.8.1-dev")

add_subdirectory(ewsclient)

configure_file(${CMAKE_CURRENT_SOURCE_DIR}/config.h.cmake ${CMAKE_CURRENT_BINARY_DIR}/config.h)

include_directories(${CMAKE_CURRENT_SOURCE_DIR}/ewsclient)

add_definitions(-DTRANSLATION_DOMAIN=\"akonadi_ews_resource\")

set(ewsresource_SRCS
    abchperson/ewsabchpersonhandler.cpp
    abchperson/ewscreateabchpersonjob.cpp
    abchperson/ewsfetchabchpersondetailjob.cpp
    abchperson/ewsmodifyabchpersonjob.cpp
    calendar/ewscalendarhandler.cpp
    calendar/ewscreatecalendarjob.cpp
    calendar/ewsfetchcalendardetailjob.cpp
    calendar/ewsmodifycalendarjob.cpp
    contact/ewscontacthandler.cpp
    contact/ewscreatecontactjob.cpp
    contact/ewsfetchcontactdetailjob.cpp
    contact/ewsmodifycontactjob.cpp
    mail/ewscreatemailjob.cpp
    mail/ewsfetchmaildetailjob.cpp
    mail/ewsmailhandler.cpp
    mail/ewsmodifymailjob.cpp
    tags/ewsakonaditagssyncjob.cpp
    tags/ewsglobaltagsreadjob.cpp
    tags/ewsglobaltagswritejob.cpp
    tags/ewstagstore.cpp
    tags/ewsupdateitemstagsjob.cpp
    task/ewscreatetaskjob.cpp
    task/ewsfetchtaskdetailjob.cpp
    task/ewsmodifytaskjob.cpp
    task/ewstaskhandler.cpp
    configdialog.cpp
    ewsautodiscoveryjob.cpp
    ewscreateitemjob.cpp
    ewsfetchfoldersjob.cpp
    ewsfetchfoldersincrjob.cpp
    ewsfetchitemsjob.cpp
    ewsfetchitemdetailjob.cpp
    ewsitemhandler.cpp
    ewsmodifyitemjob.cpp
    ewsmodifyitemflagsjob.cpp
    ewsresource.cpp
    ewsresource_debug.cpp
    ewssubscribedfoldersjob.cpp
    ewssubscriptionmanager.cpp
    ewssubscriptionwidget.cpp
    progressdialog.cpp
    settings.cpp)

ki18n_wrap_ui(ewsresource_SRCS configdialog.ui)

kconfig_add_kcfg_files(ewsresource_SRCS settingsbase.kcfgc)
kcfg_generate_dbus_interface(${CMAKE_CURRENT_SOURCE_DIR}/ewsresource.kcfg org.kde.Akonadi.Ews.Settings)
qt5_add_dbus_adaptor(ewsresource_SRCS
  ${CMAKE_CURRENT_BINARY_DIR}/org.kde.Akonadi.Ews.Settings.xml settings.h Settings
)

qt5_generate_dbus_interface( ${CMAKE_CURRENT_SOURCE_DIR}/ewsresource.h org.kde.Akonadi.Ews.Resource.xml OPTIONS -a )
qt5_add_dbus_adaptor(ewsresource_SRCS
  ${CMAKE_CURRENT_BINARY_DIR}/org.kde.Akonadi.Ews.Resource.xml ewsresource.h EwsResource
)
qt5_generate_dbus_interface( ${CMAKE_CURRENT_SOURCE_DIR}/settings.h org.kde.Akonadi.Ews.Wallet.xml OPTIONS -a )
qt5_add_dbus_adaptor(ewsresource_SRCS
  ${CMAKE_CURRENT_BINARY_DIR}/org.kde.Akonadi.Ews.Wallet.xml settings.h Settings
)



add_executable(akonadi_ews_resource ${ewsresource_SRCS})

target_link_libraries(akonadi_ews_resource
    KF5::AkonadiAgentBase
    KF5::AkonadiCore
    KF5::AkonadiMime
    KF5::KIOCore
    KF5::Mime
    KF5::CalendarCore
    KF5::Contacts
    KF5::KDELibs4Support
    KF5::Wallet
    KF5::WidgetsAddons
    ewsclient)

if (HAVE_SEPARATE_MTA_RESOURCE)
  set(ewsmtaresource_SRCS
    ewsmtaresource.cpp
    mtaconfigdialog.cpp)

  ki18n_wrap_ui(ewsmtaresource_SRCS mtaconfigdialog.ui)

  kconfig_add_kcfg_files(ewsmtaresource_SRCS mtasettings.kcfgc)
  kcfg_generate_dbus_interface(${CMAKE_CURRENT_SOURCE_DIR}/ewsmtaresource.kcfg org.kde.Akonadi.EwsMta.Settings)
  qt5_add_dbus_adaptor(ewsmtaresource_SRCS
    ${CMAKE_CURRENT_BINARY_DIR}/org.kde.Akonadi.EwsMta.Settings.xml mtasettings.h MtaSettings mtasettingsadaptor
  )
  qt5_add_dbus_interface(ewsmtaresource_SRCS ${CMAKE_CURRENT_BINARY_DIR}/org.kde.Akonadi.Ews.Resource.xml resourceinterface)

  add_executable(akonadi_ewsmta_resource ${ewsmtaresource_SRCS})
  target_link_libraries(akonadi_ewsmta_resource
      KF5::AkonadiAgentBase
      KF5::AkonadiCore
      KF5::AkonadiWidgets
      KF5::Mime
      KF5::KDELibs4Support)

  set(EWS_MTA_CAPABILITIES "X-EwsMailTransport")
else (HAVE_SEPARATE_MTA_RESOURCE)
  set(EWS_MTA_CAPABILITIES "MailTransport,Synchronizable")
endif (HAVE_SEPARATE_MTA_RESOURCE)

configure_file(${CMAKE_CURRENT_SOURCE_DIR}/ewsresource.desktop.cmake ${CMAKE_CURRENT_BINARY_DIR}/ewsresource.desktop)

if (ENABLE_TESTS)
  enable_testing()
  add_subdirectory(test)
endif (ENABLE_TESTS)


install(FILES ${CMAKE_CURRENT_BINARY_DIR}/ewsresource.desktop
  DESTINATION "${CMAKE_INSTALL_PREFIX}/share/akonadi/agents" )
install(TARGETS akonadi_ews_resource ${INSTALL_TARGETS_DEFAULT_ARGS})

if (HAVE_SEPARATE_MTA_RESOURCE)
  install(FILES ${CMAKE_CURRENT_SOURCE_DIR}/ewsmtaresource.desktop
	DESTINATION "${CMAKE_INSTALL_PREFIX}/share/akonadi/agents" )
  install(TARGETS akonadi_ewsmta_resource ${INSTALL_TARGETS_DEFAULT_ARGS})
endif (HAVE_SEPARATE_MTA_RESOURCE)

foreach(icon_resolution 16 22 24 32 48 64 72 96 128)
  install(FILES ${CMAKE_CURRENT_SOURCE_DIR}/icons/akonadi-ews-${icon_resolution}.png
	DESTINATION "${CMAKE_INSTALL_PREFIX}/share/icons/hicolor/${icon_resolution}x${icon_resolution}/apps/"
	RENAME "akonadi-ews.png")
endforeach(icon_resolution)
