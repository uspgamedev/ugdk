/*
 * gamedata.cc
 *
 *  Created on: Aug 30, 2011
 *      Author: kazuo
 */

#include "gamedata.h"

namespace ugdk {

GameData::~GameData() {
    for (size_t i = 0; i < segments_.size(); i++)
        delete segments_.at(i);
}

} /* namespace ugdk */
