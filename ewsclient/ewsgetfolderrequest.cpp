/*  This file is part of Akonadi EWS Resource
    Copyright (C) 2015 Krzysztof Nowicki <krissn@op.pl>

    This library is free software; you can redistribute it and/or
    modify it under the terms of the GNU Library General Public
    License as published by the Free Software Foundation; either
    version 2 of the License, or (at your option) any later version.

    This library is distributed in the hope that it will be useful,
    but WITHOUT ANY WARRANTY; without even the implied warranty of
    MERCHANTABILITY or FITNESS FOR A PARTICULAR PURPOSE.  See the GNU
    Library General Public License for more details.

    You should have received a copy of the GNU Library General Public License
    along with this library; see the file COPYING.LIB.  If not, write to
    the Free Software Foundation, Inc., 51 Franklin Street, Fifth Floor,
    Boston, MA 02110-1301, USA.
*/

#include <QtCore/QXmlStreamWriter>

#include "ewsgetfolderrequest.h"
#include "ewsclient_debug.h"

EwsGetFolderRequest::EwsGetFolderRequest(EwsClient &client, QObject *parent)
    : EwsRequest(client, parent), mFolder(0)
{
}

EwsGetFolderRequest::~EwsGetFolderRequest()
{
}

void EwsGetFolderRequest::setFolderId(const EwsId &id)
{
    mId = id;
}

void EwsGetFolderRequest::setFolderShape(const EwsFolderShape &shape)
{
    mShape = shape;
}

void EwsGetFolderRequest::start()
{
    QString reqString;
    QXmlStreamWriter writer(&reqString);

    startSoapDocument(writer);

    writer.writeStartElement(ewsMsgNsUri, QStringLiteral("GetFolder"));

    mShape.write(writer);

    writer.writeStartElement(ewsMsgNsUri, QStringLiteral("FolderIds"));
    mId.writeFolderIds(writer);
    writer.writeEndElement();

    writer.writeEndElement();

    endSoapDocument(writer);

    qCDebug(EWSCLIENT_LOG) << reqString;

    prepare(reqString);

    doSend();
}

bool EwsGetFolderRequest::parseResult(QXmlStreamReader &reader)
{
    return parseResponseMessage(reader, QStringLiteral("GetFolder"),
                                [this](QXmlStreamReader &reader){return parseFoldersResponse(reader);});
}

bool EwsGetFolderRequest::parseFoldersResponse(QXmlStreamReader &reader)
{
    if (reader.namespaceUri() != ewsMsgNsUri || reader.name() != QStringLiteral("Folders"))
        return setErrorMsg(QStringLiteral("Failed to read EWS request - expected Folders element (got %1).")
                        .arg(reader.qualifiedName().toString()));

    if (!reader.readNextStartElement())
        return setErrorMsg(QStringLiteral("Failed to read EWS request - expected a child element in Folders element."));

    if (reader.namespaceUri() != ewsTypeNsUri)
        return setErrorMsg(QStringLiteral("Failed to read EWS request - expected child element from types namespace."));

    mFolder = new EwsFolder(reader);
    if (!mFolder->isValid())
        return setErrorMsg(QStringLiteral("Failed to read EWS request - invalid Folder element."));

    return true;
}
