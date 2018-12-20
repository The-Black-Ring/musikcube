//////////////////////////////////////////////////////////////////////////////
//
// Copyright (c) 2007-2017 musikcube team
//
// All rights reserved.
//
// Redistribution and use in source and binary forms, with or without
// modification, are permitted provided that the following conditions are met:
//
//    * Redistributions of source code must retain the above copyright notice,
//      this list of conditions and the following disclaimer.
//
//    * Redistributions in binary form must reproduce the above copyright
//      notice, this list of conditions and the following disclaimer in the
//      documentation and/or other materials provided with the distribution.
//
//    * Neither the name of the author nor the names of other contributors may
//      be used to endorse or promote products derived from this software
//      without specific prior written permission.
//
// THIS SOFTWARE IS PROVIDED BY THE COPYRIGHT HOLDERS AND CONTRIBUTORS "AS IS"
// AND ANY EXPRESS OR IMPLIED WARRANTIES, INCLUDING, BUT NOT LIMITED TO, THE
// IMPLIED WARRANTIES OF MERCHANTABILITY AND FITNESS FOR A PARTICULAR PURPOSE
// ARE DISCLAIMED. IN NO EVENT SHALL THE COPYRIGHT OWNER OR CONTRIBUTORS BE
// LIABLE FOR ANY DIRECT, INDIRECT, INCIDENTAL, SPECIAL, EXEMPLARY, OR
// CONSEQUENTIAL DAMAGES (INCLUDING, BUT NOT LIMITED TO, PROCUREMENT OF
// SUBSTITUTE GOODS OR SERVICES; LOSS OF USE, DATA, OR PROFITS; OR BUSINESS
// INTERRUPTION) HOWEVER CAUSED AND ON ANY THEORY OF LIABILITY, WHETHER IN
// CONTRACT, STRICT LIABILITY, OR TORT (INCLUDING NEGLIGENCE OR OTHERWISE)
// ARISING IN ANY WAY OUT OF THE USE OF THIS SOFTWARE, EVEN IF ADVISED OF THE
// POSSIBILITY OF SUCH DAMAGE.
//
//////////////////////////////////////////////////////////////////////////////

#include "stdafx.h"

#include "SimpleScrollAdapter.h"
#include "SingleLineEntry.h"
#include "MultiLineEntry.h"
#include "ScrollableWindow.h"
#include "Colors.h"
#include <utf8/utf8/unchecked.h>

using namespace cursespp;

#define MAX_ENTRY_COUNT 0xffffffff

typedef IScrollAdapter::EntryPtr EntryPtr;

SimpleScrollAdapter::SimpleScrollAdapter() {
    this->maxEntries = MAX_ENTRY_COUNT;
    this->selectable = false;
}

SimpleScrollAdapter::~SimpleScrollAdapter() {

}

void SimpleScrollAdapter::SetSelectable(bool selectable) {
    this->selectable = selectable;
}

void SimpleScrollAdapter::Clear() {
    this->entries.clear();
    this->Changed(this);
}

size_t SimpleScrollAdapter::GetEntryCount() {
    return this->entries.size();
}

void SimpleScrollAdapter::SetMaxEntries(size_t maxEntries) {
    this->maxEntries = maxEntries;
}

EntryPtr SimpleScrollAdapter::GetEntry(cursespp::ScrollableWindow* window, size_t index) {
    auto entry = this->entries.at(index);

    /* this is pretty damned gross, but super convenient. */
    if (window && selectable) {
        SingleLineEntry* single = static_cast<SingleLineEntry*>(entry.get());
        single->SetAttrs(Color(Color::Default));
            if (index == window->GetScrollPosition().logicalIndex) {
            single->SetAttrs(Color(Color::ListItemHighlighted));
            }
        }

    return entry;
    }

std::string SimpleScrollAdapter::StringAt(size_t index) {
    auto entry = this->entries.at(index);
    return static_cast<SingleLineEntry*>(entry.get())->GetValue();
}

void SimpleScrollAdapter::AddEntry(std::shared_ptr<IEntry> entry) {
    entry->SetWidth(this->GetWidth());
    entries.push_back(entry);

    while (entries.size() > this->maxEntries) {
        entries.pop_front();
    }

    this->Changed(this);
}

void SimpleScrollAdapter::AddEntry(const std::string& value) {
    EntryPtr entry(new SingleLineEntry(value));
    this->AddEntry(entry);
}
