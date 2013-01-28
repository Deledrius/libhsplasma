/* This file is part of HSPlasma.
 *
 * HSPlasma is free software: you can redistribute it and/or modify
 * it under the terms of the GNU General Public License as published by
 * the Free Software Foundation, either version 3 of the License, or
 * (at your option) any later version.
 *
 * HSPlasma is distributed in the hope that it will be useful,
 * but WITHOUT ANY WARRANTY; without even the implied warranty of
 * MERCHANTABILITY or FITNESS FOR A PARTICULAR PURPOSE.  See the
 * GNU General Public License for more details.
 *
 * You should have received a copy of the GNU General Public License
 * along with HSPlasma.  If not, see <http://www.gnu.org/licenses/>.
 */

#include "plBubbleShaderMod.h"

/* plBubbleWave */
void plBubbleWave::read(hsStream* S) {
    // Not certain of the order here.
    fHeight = S->readFloat();
    fOffset = S->readFloat();
    fDirX = S->readFloat();
    fDirY = S->readFloat();
    fSpeed = S->readFloat();
}

void plBubbleWave::write(hsStream* S) {
    S->writeFloat(fHeight);
    S->writeFloat(fOffset);
    S->writeFloat(fDirX);
    S->writeFloat(fDirY);
    S->writeFloat(fSpeed);
}

void plBubbleWave::prcWrite(pfPrcHelper* prc) {
    prc->startTag("plBubbleWave");
    prc->writeParam("Height", fHeight);
    prc->writeParam("Offset", fOffset);
    prc->writeParam("DirX", fDirX);
    prc->writeParam("DirY", fDirY);
    prc->writeParam("Speed", fSpeed);
    prc->endTag(true);
}

void plBubbleWave::prcParse(const pfPrcTag* tag) {
    if (tag->getName() != "plBubbleWave")
        throw pfPrcTagException(__FILE__, __LINE__, tag->getName());

    fHeight = tag->getParam("Height", "0").toFloat();
    fOffset = tag->getParam("Offset", "0").toFloat();
    fDirX = tag->getParam("DirX", "0").toFloat();
    fDirY = tag->getParam("DirY", "0").toFloat();
    fSpeed = tag->getParam("Speed", "0").toFloat();
}

/* plBubbleShaderMod */
void plBubbleShaderMod::read(hsStream* S, plResManager* mgr) {
    plModifier::read(S, mgr);

    fMaterialOuter = mgr->readKey(S);
    fMaterialInner = mgr->readKey(S);
    fFilmTexture = mgr->readKey(S);
    fUnknown1 = S->readFloat();

    fEnvironMaps.resize(S->readInt());
    for (size_t i=0; i<fEnvironMaps.size(); i++)
        fEnvironMaps[i] = mgr->readKey(S);

    for (uint32_t i=0; i<kNumWaves; i++)
        fWaves[i].read(S);

    fUnknown2 = S->readFloat();
    fUnknown3 = S->readFloat();
    fUnknown4 = S->readFloat();
    fUnknown5 = S->readFloat();
    fUnknown6 = S->readFloat();
    fUnknown7 = S->readFloat();
    fUnknown8 = S->readFloat();
    fUnknown9 = S->readFloat();
}

void plBubbleShaderMod::write(hsStream* S, plResManager* mgr) {
    plModifier::write(S, mgr);

    mgr->writeKey(S, fMaterialOuter);
    mgr->writeKey(S, fMaterialInner);
    mgr->writeKey(S, fFilmTexture);
    S->writeFloat(fUnknown1);

    for (size_t i=0; i<fEnvironMaps.size(); i++)
        mgr->writeKey(S, fEnvironMaps[i]);

    for (uint32_t i=0; i<kNumWaves; i++)
        fWaves[i].write(S);

    S->writeFloat(fUnknown2);
    S->writeFloat(fUnknown3);
    S->writeFloat(fUnknown4);
    S->writeFloat(fUnknown5);
    S->writeFloat(fUnknown6);
    S->writeFloat(fUnknown7);
    S->writeFloat(fUnknown8);
    S->writeFloat(fUnknown9);
}

void plBubbleShaderMod::IPrcWrite(pfPrcHelper* prc) {
    plModifier::IPrcWrite(prc);

    prc->startTag("BubbleShaderParams");
    prc->writeParam("Unknown1", fUnknown1);
    prc->writeParam("Unknown2", fUnknown2);
    prc->writeParam("Unknown3", fUnknown3);
    prc->writeParam("Unknown4", fUnknown4);
    prc->writeParam("Unknown5", fUnknown5);
    prc->writeParam("Unknown6", fUnknown6);
    prc->writeParam("Unknown7", fUnknown7);
    prc->writeParam("Unknown8", fUnknown8);
    prc->writeParam("Unknown9", fUnknown9);
    prc->endTag(true);

    prc->writeSimpleTag("MaterialOuter");
    fMaterialOuter->prcWrite(prc);
    prc->closeTag();

    prc->writeSimpleTag("MaterialInner");
    fMaterialInner->prcWrite(prc);
    prc->closeTag();

    prc->writeSimpleTag("FilmTexture");
    fFilmTexture->prcWrite(prc);
    prc->closeTag();

    prc->writeSimpleTag("EnvironmentMaps");
    for (size_t i=0; i<fEnvironMaps.size(); i++) {
        fEnvironMaps[i]->prcWrite(prc);
    }
    prc->closeTag();

    prc->writeSimpleTag("Waves");
    for (size_t i=0; i<kNumWaves; i++)
        fWaves[i].prcWrite(prc);
    prc->closeTag();
}

void plBubbleShaderMod::IPrcParse(const pfPrcTag* tag, plResManager* mgr) {
    if (tag->getName() == "BubbleShaderParams") {
        fUnknown1 = tag->getParam("Unknown1", "0").toFloat();
        fUnknown2 = tag->getParam("Unknown2", "0").toFloat();
        fUnknown3 = tag->getParam("Unknown3", "0").toFloat();
        fUnknown4 = tag->getParam("Unknown4", "0").toFloat();
        fUnknown5 = tag->getParam("Unknown5", "0").toFloat();
        fUnknown6 = tag->getParam("Unknown6", "0").toFloat();
        fUnknown7 = tag->getParam("Unknown7", "0").toFloat();
        fUnknown8 = tag->getParam("Unknown8", "0").toFloat();
        fUnknown9 = tag->getParam("Unknown9", "0").toFloat();
    } else if (tag->getName() == "MaterialOuter") {
        if (tag->hasChildren())
            fMaterialOuter = mgr->prcParseKey(tag->getFirstChild());
    } else if (tag->getName() == "MaterialInner") {
        if (tag->hasChildren())
            fMaterialInner = mgr->prcParseKey(tag->getFirstChild());
    } else if (tag->getName() == "FilmTexture") {
        if (tag->hasChildren())
            fFilmTexture = mgr->prcParseKey(tag->getFirstChild());
    } else if (tag->getName() == "EnvironmentMaps") {
        fEnvironMaps.resize(tag->countChildren());
        const pfPrcTag* child = tag->getFirstChild();
        for (size_t i=0; i<fEnvironMaps.size(); i++) {
            fEnvironMaps[i]->PrcParse(child);
            child = child->getNextSibling();
        }
    } else if (tag->getName() == "Waves") {
        if (tag->countChildren() != kNumWaves)
            throw pfPrcParseException(__FILE__, __LINE__, "Incorrect number of waves");

        const pfPrcTag* child = tag->getFirstChild();
        for (size_t i=0; i<kNumWaves; i++) {
            fWaves[i].prcParse(child);
            child = child->getNextSibling();
        }
    } else {
        plModifier::IPrcParse(tag, mgr);
    }
}
