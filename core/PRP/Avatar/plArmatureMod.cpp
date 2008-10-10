#include "plArmatureMod.h"

/* plArmatureModBase */
plArmatureModBase::plArmatureModBase() { }

plArmatureModBase::~plArmatureModBase() {
    for (size_t i=0; i<fBrains.getSize(); i++)
        delete fBrains[i];
}

IMPLEMENT_CREATABLE(plArmatureModBase, kArmatureModBase, plAGMasterMod)

void plArmatureModBase::read(hsStream* S, plResManager* mgr) {
    plAGMasterMod::read(S, mgr);

    fMeshKeys.setSize(S->readInt());
    fUnusedBones.setSize(fMeshKeys.getSize());
    for (size_t i=0; i<fMeshKeys.getSize(); i++) {
        fMeshKeys[i] = mgr->readKey(S);
        fUnusedBones[i].setSize(S->readInt());
        for (size_t j=0; j<fUnusedBones[i].getSize(); j++)
            fUnusedBones[i][j] = mgr->readKey(S);
    }

    fBrains.setSize(S->readInt());
    for (size_t i=0; i<fBrains.getSize(); i++)
        fBrains[i] = plArmatureBrain::Convert(mgr->ReadCreatable(S));
}

void plArmatureModBase::write(hsStream* S, plResManager* mgr) {
    plAGMasterMod::write(S, mgr);

    S->writeInt(fMeshKeys.getSize());
    for (size_t i=0; i<fMeshKeys.getSize(); i++) {
        mgr->writeKey(S, fMeshKeys[i]);
        S->writeInt(fUnusedBones[i].getSize());
        for (size_t j=0; j<fUnusedBones[i].getSize(); j++)
            mgr->writeKey(S, fUnusedBones[i][j]);
    }

    S->writeInt(fBrains.getSize());
    for (size_t i=0; i<fBrains.getSize(); i++)
        mgr->WriteCreatable(S, fBrains[i]);
}

void plArmatureModBase::IPrcWrite(pfPrcHelper* prc) {
    plAGMasterMod::IPrcWrite(prc);

    prc->writeSimpleTag("Meshes");
    for (size_t i=0; i<fMeshKeys.getSize(); i++) {
        prc->writeSimpleTag("Mesh");
        fMeshKeys[i]->prcWrite(prc);
        prc->writeSimpleTag("UnusedBones");
        for (size_t j=0; j<fUnusedBones[i].getSize(); j++)
            fUnusedBones[i][j]->prcWrite(prc);
        prc->closeTag(); // UnusedBones
        prc->closeTag(); // Mesh
    }
    prc->closeTag();

    prc->writeSimpleTag("Brains");
    for (size_t i=0; i<fBrains.getSize(); i++)
        fBrains[i]->prcWrite(prc);
    prc->closeTag();
}

void plArmatureModBase::IPrcParse(const pfPrcTag* tag, plResManager* mgr) {
    if (tag->getName() == "Meshes") {
        fMeshKeys.setSize(tag->countChildren());
        fUnusedBones.setSize(fMeshKeys.getSize());
        const pfPrcTag* child = tag->getFirstChild();
        for (size_t i=0; i<fMeshKeys.getSize(); i++) {
            if (child->getName() != "Mesh")
                throw pfPrcTagException(__FILE__, __LINE__, child->getName());

            const pfPrcTag* subchild = child->getFirstChild();
            while (subchild != NULL) {
                if (subchild->getName() == "plKey") {
                    fMeshKeys[i] = mgr->prcParseKey(subchild);
                } else if (subchild->getName() == "UnusedBones") {
                    fUnusedBones[i].setSize(subchild->countChildren());
                    const pfPrcTag* boneChild = subchild->getFirstChild();
                    for (size_t j=0; j<fUnusedBones[i].getSize(); j++) {
                        fUnusedBones[i][j] = mgr->prcParseKey(boneChild);
                        boneChild = boneChild->getNextSibling();
                    }
                } else {
                    throw pfPrcTagException(__FILE__, __LINE__, subchild->getName());
                }
                subchild = subchild->getNextSibling();
            }
            child = child->getNextSibling();
        }
    } else if (tag->getName() == "Brains") {
        fBrains.setSize(tag->countChildren());
        const pfPrcTag* child = tag->getFirstChild();
        for (size_t i=0; i<fBrains.getSize(); i++) {
            fBrains[i] = plArmatureBrain::Convert(mgr->prcParseCreatable(child));
            child = child->getNextSibling();
        }
    } else {
        plAGMasterMod::IPrcParse(tag, mgr);
    }
}


/* plArmatureMod */
plArmatureMod::plArmatureMod()
             : fBodyType(0), fPhysHeight(0.0f), fPhysWidth(0.0f) { }

plArmatureMod::~plArmatureMod() { }

IMPLEMENT_CREATABLE(plArmatureMod, kArmatureMod, plArmatureModBase)

void plArmatureMod::read(hsStream* S, plResManager* mgr) {
    plAGMasterMod::read(S, mgr);

    fMeshKeys.append(mgr->readKey(S));
    fRootName = S->readSafeStr();

    fBrains.setSize(S->readInt());
    for (size_t i=0; i<fBrains.getSize(); i++)
        fBrains[i] = plArmatureBrain::Convert(mgr->ReadCreatable(S));

    if (S->readBool())
        fClothingOutfit = mgr->readKey(S);
    else
        fClothingOutfit = plKey();

    fBodyType = S->readInt();
    if (S->readBool())
        fEffects = mgr->readKey(S);
    else
        fEffects = plKey();

    if (S->getVer() < pvLive) {
        fUruVec1.read(S);
        fUruVec2.read(S);
    }

    fPhysHeight = S->readFloat();
    fPhysWidth = S->readFloat();

    if (S->getVer() == pvLive) {
        fFootstepType = S->readSafeStr();
        fFootstepAge = S->readSafeStr();
        fFootstepPage = S->readSafeStr();
    } else {
        fFootstepAge = "CustomAvatars";
        fFootstepPage = "Audio";
        if (fBodyType == kBoneBaseCritter) {
            fFootstepPage = "Quab";
            fFootstepType = "Quab";
        } else if (fBodyType == kBoneBaseMale) {
            fFootstepType = "Male";
        } else if (fBodyType == kBoneBaseFemale) {
            fFootstepType = "Female";
        }
    }
}

void plArmatureMod::write(hsStream* S, plResManager* mgr) {
    plAGMasterMod::write(S, mgr);

    mgr->writeKey(S, fMeshKeys[0]);
    S->writeSafeStr(fRootName);

    S->writeInt(fBrains.getSize());
    for (size_t i=0; i<fBrains.getSize(); i++)
        mgr->WriteCreatable(S, fBrains[i]);

    S->writeBool(fClothingOutfit.Exists());
    if (fClothingOutfit.Exists())
        mgr->writeKey(S, fClothingOutfit);

    S->writeInt(fBodyType);
    S->writeBool(fEffects.Exists());
    if (fEffects.Exists())
        mgr->writeKey(S, fEffects);

    if (S->getVer() < pvLive) {
        fUruVec1.write(S);
        fUruVec2.write(S);
    }

    S->writeFloat(fPhysHeight);
    S->writeFloat(fPhysWidth);

    if (S->getVer() == pvLive) {
        S->writeSafeStr(fFootstepType);
        S->writeSafeStr(fFootstepAge);
        S->writeSafeStr(fFootstepPage);
    }
}

void plArmatureMod::IPrcWrite(pfPrcHelper* prc) {
    plAGMasterMod::IPrcWrite(prc);

    prc->writeSimpleTag("DefaultMesh");
    fMeshKeys[0]->prcWrite(prc);
    prc->closeTag();

    prc->startTag("ArmatureParams");
    prc->writeParam("Root", fRootName);
    prc->writeParam("BodyType", fBodyType);
    prc->writeParam("PhysHeight", fPhysHeight);
    prc->writeParam("PhysWidth", fPhysWidth);
    prc->endTag();
      prc->writeSimpleTag("Mins");
      fUruVec1.prcWrite(prc);
      prc->closeTag();
      prc->writeSimpleTag("Maxs");
      fUruVec2.prcWrite(prc);
      prc->closeTag();
    prc->closeTag();

    prc->startTag("Footsteps");
    prc->writeParam("Age", fFootstepAge);
    prc->writeParam("Page", fFootstepPage);
    prc->writeParam("Type", fFootstepType);
    prc->endTag(true);

    prc->writeSimpleTag("Brains");
    for (size_t i=0; i<fBrains.getSize(); i++)
        fBrains[i]->prcWrite(prc);
    prc->closeTag();

    prc->writeSimpleTag("ClothingOutfit");
    fClothingOutfit->prcWrite(prc);
    prc->closeTag();

    prc->writeSimpleTag("Effects");
    fEffects->prcWrite(prc);
    prc->closeTag();
}

void plArmatureMod::IPrcParse(const pfPrcTag* tag, plResManager* mgr) {
    if (tag->getName() == "DefaultMesh") {
        fMeshKeys.append(mgr->prcParseKey(tag->getFirstChild()));
    } else if (tag->getName() == "ArmatureParams") {
        fRootName = tag->getParam("Root", "");
        fBodyType = tag->getParam("BodyType", "0").toInt();
        fPhysHeight = tag->getParam("PhysHeight", "0").toFloat();
        fPhysWidth = tag->getParam("PhysWidth", "0").toFloat();
    } else if (tag->getName() == "Footsteps") {
        fFootstepAge = tag->getParam("Age", "");
        fFootstepPage = tag->getParam("Page", "");
        fFootstepType = tag->getParam("Type", "");
    } else if (tag->getName() == "Brains") {
        fBrains.setSize(tag->countChildren());
        const pfPrcTag* child = tag->getFirstChild();
        for (size_t i=0; i<fBrains.getSize(); i++) {
            fBrains[i] = plArmatureBrain::Convert(mgr->prcParseCreatable(child));
            child = child->getNextSibling();
        }
    } else if (tag->getName() == "ClothingOutfit") {
        fClothingOutfit = mgr->prcParseKey(tag->getFirstChild());
    } else if (tag->getName() == "Effects") {
        fEffects = mgr->prcParseKey(tag->getFirstChild());
    } else {
        plAGMasterMod::IPrcParse(tag, mgr);
    }
}


/* plArmatureLODMod */
plArmatureLODMod::plArmatureLODMod() { }
plArmatureLODMod::~plArmatureLODMod() { }

IMPLEMENT_CREATABLE(plArmatureLODMod, kArmatureLODMod, plArmatureMod)

void plArmatureLODMod::read(hsStream* S, plResManager* mgr) {
    plArmatureMod::read(S, mgr);

    fMeshKeys.setSize(S->readInt());
    fUnusedBones.setSize(fMeshKeys.getSize());
    for (size_t i=0; i<fMeshKeys.getSize(); i++) {
        fMeshKeys[i] = mgr->readKey(S);
        fUnusedBones[i].setSize(S->readInt());
        for (size_t j=0; j<fUnusedBones[i].getSize(); j++)
            fUnusedBones[i][j] = mgr->readKey(S);
    }
}

void plArmatureLODMod::write(hsStream* S, plResManager* mgr) {
    plArmatureMod::write(S, mgr);

    S->writeInt(fMeshKeys.getSize());
    for (size_t i=0; i<fMeshKeys.getSize(); i++) {
        mgr->writeKey(S, fMeshKeys[i]);
        S->writeInt(fUnusedBones[i].getSize());
        for (size_t j=0; j<fUnusedBones[i].getSize(); j++)
            mgr->writeKey(S, fUnusedBones[i][j]);
    }
}

void plArmatureLODMod::IPrcWrite(pfPrcHelper* prc) {
    plArmatureMod::IPrcWrite(prc);

    prc->writeSimpleTag("Meshes");
    for (size_t i=0; i<fMeshKeys.getSize(); i++) {
        prc->writeSimpleTag("Mesh");
        fMeshKeys[i]->prcWrite(prc);
        prc->writeSimpleTag("UnusedBones");
        for (size_t j=0; j<fUnusedBones[i].getSize(); j++)
            fUnusedBones[i][j]->prcWrite(prc);
        prc->closeTag(); // UnusedBones
        prc->closeTag(); // Mesh
    }
    prc->closeTag();
}

void plArmatureLODMod::IPrcParse(const pfPrcTag* tag, plResManager* mgr) {
    if (tag->getName() == "Meshes") {
        fMeshKeys.setSize(tag->countChildren());
        fUnusedBones.setSize(fMeshKeys.getSize());
        const pfPrcTag* child = tag->getFirstChild();
        for (size_t i=0; i<fMeshKeys.getSize(); i++) {
            if (child->getName() != "Mesh")
                throw pfPrcTagException(__FILE__, __LINE__, child->getName());

            const pfPrcTag* subchild = child->getFirstChild();
            while (subchild != NULL) {
                if (subchild->getName() == "plKey") {
                    fMeshKeys[i] = mgr->prcParseKey(subchild);
                } else if (subchild->getName() == "UnusedBones") {
                    fUnusedBones[i].setSize(subchild->countChildren());
                    const pfPrcTag* boneChild = subchild->getFirstChild();
                    for (size_t j=0; j<fUnusedBones[i].getSize(); j++) {
                        fUnusedBones[i][j] = mgr->prcParseKey(boneChild);
                        boneChild = boneChild->getNextSibling();
                    }
                } else {
                    throw pfPrcTagException(__FILE__, __LINE__, subchild->getName());
                }
                subchild = subchild->getNextSibling();
            }
            child = child->getNextSibling();
        }
    } else {
        plArmatureMod::IPrcParse(tag, mgr);
    }
}