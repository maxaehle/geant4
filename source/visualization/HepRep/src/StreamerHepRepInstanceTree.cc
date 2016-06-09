//
// ********************************************************************
// * DISCLAIMER                                                       *
// *                                                                  *
// * The following disclaimer summarizes all the specific disclaimers *
// * of contributors to this software. The specific disclaimers,which *
// * govern, are listed with their locations in:                      *
// *   http://cern.ch/geant4/license                                  *
// *                                                                  *
// * Neither the authors of this software system, nor their employing *
// * institutes,nor the agencies providing financial support for this *
// * work  make  any representation or  warranty, express or implied, *
// * regarding  this  software system or assume any liability for its *
// * use.                                                             *
// *                                                                  *
// * This  code  implementation is the  intellectual property  of the *
// * GEANT4 collaboration.                                            *
// * By copying,  distributing  or modifying the Program (or any work *
// * based  on  the Program)  you indicate  your  acceptance of  this *
// * statement, and all its terms.                                    *
// ********************************************************************
//

#include "StreamerHepRepInstanceTree.h"

using namespace std;
using namespace HEPREP;

StreamerHepRepInstanceTree::StreamerHepRepInstanceTree(HepRepWriter* stream, string name, string version, HepRepTreeID* typeTree)
    : DefaultHepRepTreeID(name, version), streamer(stream), typeTree(typeTree) {

    stream->write(this);
}

StreamerHepRepInstanceTree::~StreamerHepRepInstanceTree() {
}

HepRepTreeID* StreamerHepRepInstanceTree::copy() {
    return DefaultHepRepTreeID::copy();
}

HepRepInstanceTree* StreamerHepRepInstanceTree::copy(HepRep*, HepRepSelectFilter*) {
    return NULL;
}

bool StreamerHepRepInstanceTree::addInstance(HepRepInstance*) {
    return true;
}

void StreamerHepRepInstanceTree::removeInstance(HepRepInstance*) {
}

vector<HepRepInstance*>* StreamerHepRepInstanceTree::getInstances() {
    return NULL;
}

bool StreamerHepRepInstanceTree::addInstanceTree(HepRepTreeID* treeID) {
    streamer->write(treeID);
    return true;
}

HepRepTreeID* StreamerHepRepInstanceTree::getTypeTree() {
    return typeTree;
}

vector<HepRepInstanceTree*>* StreamerHepRepInstanceTree::getInstanceTrees() {
    return NULL;
}
