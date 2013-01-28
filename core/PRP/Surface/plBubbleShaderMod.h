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

#ifndef _PLBUBBLESHADERMOD_H
#define _PLBUBBLESHADERMOD_H

#include "PRP/Modifier/plModifier.h"

class PLASMA_DLL plBubbleWave {
protected:
    float fHeight, fOffset, fDirX, fDirY, fSpeed;

public:
    plBubbleWave() : fHeight(0.0f), fOffset(0.0f),
        fDirX(0.0f), fDirY(0.0f), fSpeed(0.0f) { }

    void read(hsStream* S);
    void write(hsStream* S);
    void prcWrite(pfPrcHelper* prc);
    void prcParse(const pfPrcTag* tag);
};

class PLASMA_DLL plBubbleShaderMod : public virtual plModifier {
    CREATABLE(plBubbleShaderMod, kBubbleShaderMod, plModifier)

public:
    enum { kNumWaves = 4 };

protected:
    plKey fMaterialOuter, fMaterialInner, fFilmTexture;
    std::vector<plKey> fEnvironMaps;
    plBubbleWave fWaves[kNumWaves];

    // Somewhere in here is the Max and Min Fade distances,
    // as well as Max and Min Fresnel Fade angles...
    float fUnknown1, fUnknown2, fUnknown3, fUnknown4, fUnknown5,
        fUnknown6, fUnknown7, fUnknown8, fUnknown9;

public:
    plBubbleShaderMod() : fUnknown1(0.0f), fUnknown2(0.0f), fUnknown3(0.0f),
    fUnknown4(0.0f), fUnknown5(0.0f), fUnknown6(0.0f), fUnknown7(0.0f),
    fUnknown8(0.0f), fUnknown9(0.0f) { };

    virtual void read(hsStream* S, plResManager* mgr);
    virtual void write(hsStream* S, plResManager* mgr);

protected:
    virtual void IPrcWrite(pfPrcHelper* prc);
    virtual void IPrcParse(const pfPrcTag* tag, plResManager* mgr);
};

#endif
