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

#ifndef EWSITEMBASE_H
#define EWSITEMBASE_H

#include <QtCore/QSharedDataPointer>

#include "ewsid.h"
#include "ewspropertyfield.h"

class EwsItemBasePrivate;

class EwsItemBase
{
public:
    EwsItemBase();
    EwsItemBase(const EwsItemBase &other);
    EwsItemBase(EwsItemBase &&other);
    virtual ~EwsItemBase();
    EwsItemBase& operator=(const EwsItemBase &other);
    EwsItemBase& operator=(EwsItemBase &&other);

    bool isValid() const;

    bool hasField(EwsItemFields f) const;
    QVariant operator[](EwsItemFields f) const;

    QStringRef operator[](const EwsPropertyField &prop) const;

protected:
    EwsItemBase(QSharedDataPointer<EwsItemBasePrivate> priv);

    void resetFields();
    bool readExtendedProperty(QXmlStreamReader &reader);

    QSharedDataPointer<EwsItemBasePrivate> d;
};

#endif
