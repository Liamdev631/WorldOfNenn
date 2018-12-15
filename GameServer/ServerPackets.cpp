#include "ServerPackets.h"

const packet_header SP_Hello::header			= 0;
const packet_header SP_EntityStatus::header		= 1;
const packet_header SP_RemoveEntity::header		= 2;
const packet_header SP_Inventory::header		= 3;
const packet_header SP_EntityTookDamage::header = 4;
const packet_header SP_ItemPicked::header		= 5;
const packet_header SP_ItemDropped::header		= 6;
const packet_header SP_EntityMoved::header		= 7;