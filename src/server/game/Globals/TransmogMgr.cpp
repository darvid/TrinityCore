#include "AccountMgr.h"
#include "Chat.h"
#include "DatabaseEnv.h"
#include "TransmogMgr.h"

TransmogMgr::TransmogMgr() { }
TransmogMgr::~TransmogMgr() {
    _transmogSetStore.clear();
    _transmogrifiedItemsStore.clear();
}

void TransmogMgr::LoadTransmog()
{
    LoadTransmogrifiedItems();
    LoadPredefinedSets();
    sLog->outString();
}

int8 TransmogMgr::TransmogFieldForSlot(int8 slot)
{
    switch (slot)
    {
        case EQUIPMENT_SLOT_HEAD:
            return TRANSMOG_FIELD_HEAD;
        case EQUIPMENT_SLOT_SHOULDERS:
            return TRANSMOG_FIELD_SHOULDERS;
        case EQUIPMENT_SLOT_CHEST:
            return TRANSMOG_FIELD_CHEST;
        case EQUIPMENT_SLOT_WRISTS:
            return TRANSMOG_FIELD_WRISTS;
        case EQUIPMENT_SLOT_HANDS:
            return TRANSMOG_FIELD_HANDS;
        case EQUIPMENT_SLOT_WAIST:
            return TRANSMOG_FIELD_WAIST;
        case EQUIPMENT_SLOT_LEGS:
            return TRANSMOG_FIELD_LEGS;
        case EQUIPMENT_SLOT_FEET:
            return TRANSMOG_FIELD_FEET;
        default:
            return -1;
    }
}

TransmogResult TransmogMgr::TransmogItem(Player* player, uint32 newEntry,
                                         int8 slot, bool silent)
{
    int8 field = TransmogFieldForSlot(slot);
    if (field == -1) return TRANSMOG_ERR_INVALID_SLOT;

    Item* item = player->GetItemByPos(INVENTORY_SLOT_BAG_0, slot);
    if (!item) return TRANSMOG_ERR_UNEQUIPPED_ITEM;

    TransmogResult result = item->Transmogrify(newEntry);
    if (result == TRANSMOG_ERR_OK)
    {
        WorldPacket data;
        data << uint8(INVENTORY_SLOT_BAG_0);
        data << uint8(slot);
        player->GetSession()->HandleAutoEquipItemOpcode(data);
        player->SetVisibleItemSlot(slot, item);
        return TRANSMOG_ERR_OK;
    }
    else if (!silent)
    {
        ChatHandler handler(player);
        switch (result)
        {
            case TRANSMOG_ERR_CANT_FIND_ITEM:
                handler.PSendSysMessage("You can't transmog something you "
                    "haven't equipped, genius.");
                break;
            case TRANSMOG_ERR_WRONG_QUALITY:
                handler.PSendSysMessage("You can't transmog poor items!");
                break;
            case TRANSMOG_ERR_UNMET_REQUIREMENTS:
                handler.PSendSysMessage("You don't meet the requirements "
                    "for that!");
                break;
        }
    }
    return result;
}

TransmogResult TransmogMgr::TransmogSet(Player* player, RaceClass raceClass,
                                        uint32 setId)
{
    TransmogSetsContainer::iterator it = _transmogSetStore.find(raceClass);
    if (it == _transmogSetStore.end() || it->second.empty())
        return TRANSMOG_ERR_INVALID_SET;

    std::vector<uint32> setInfo;
    for (int setNo = 0; setNo != it->second.size(); ++setNo)
    {
        // have to iterate somewhere, better to do it here with a subset of
        // total transmog sets (for specific race-class combination) than over
        // *all* sets (when the primary key is the entry field). this is 
        // probably slightly more efficient when there's a few hundred sets.
        if (it->second[setNo][TRANSMOG_FIELD_ENTRY] == setId)
            setInfo = it->second[setNo];
    }
    if (setInfo.empty()) return TRANSMOG_ERR_INVALID_SET;

    if (player->GetArenaPersonalRating(setInfo[TRANSMOG_FIELD_BRACKET])
            < setInfo[TRANSMOG_FIELD_RATING]
        && AccountMgr::IsPlayerAccount(player->GetSession()->GetSecurity()))
        return TRANSMOG_ERR_UNMET_REQUIREMENTS;

    EquipmentSlots equipmentSet[8] = {
        EQUIPMENT_SLOT_HEAD,
        EQUIPMENT_SLOT_SHOULDERS,
        EQUIPMENT_SLOT_CHEST,
        EQUIPMENT_SLOT_LEGS,
        EQUIPMENT_SLOT_WRISTS,
        EQUIPMENT_SLOT_HANDS,
        EQUIPMENT_SLOT_WAIST,
        EQUIPMENT_SLOT_FEET
    };

    TransmogResult lastResult;
    for (int slot = 0; slot < 8; slot++)
    {
        int8 field = TransmogFieldForSlot(equipmentSet[slot]);
        lastResult = TransmogItem(player, setInfo[field], equipmentSet[slot]);
        if (lastResult != TRANSMOG_ERR_OK)
            break;
    }
    return lastResult;
}

uint32 TransmogMgr::GetTransmogrifiedItemEntry(uint32 itemGuid)
{
    TransmogrifiedItemsContainer::const_iterator itr;
    itr = _transmogrifiedItemsStore.find(itemGuid);

    if (itr != _transmogrifiedItemsStore.end())
        return itr->second;

    return 0;
}

void TransmogMgr::RemoveTransmogrifiedItem(uint32 itemGuid)
{
    TransmogrifiedItemsContainer::iterator itr;
    itr = _transmogrifiedItemsStore.find(itemGuid);
    if (itr != _transmogrifiedItemsStore.end())
        _transmogrifiedItemsStore.erase(itr);
}

void TransmogMgr::SetTransmogrifiedItem(uint32 itemGuid, uint32 newEntry)
{
    _transmogrifiedItemsStore[itemGuid] = newEntry;
}

void TransmogMgr::LoadPredefinedSets()
{
    QueryResult result = WorldDatabase.Query("SELECT * FROM `" 
        TRANSMOG_PREDEFINED_SETS_TABLE "` ORDER BY bracket, rating");
    uint32 entry = 0;
    do {
        Field* fields = result->Fetch();
        std::vector<uint32> setInfo;
        setInfo.push_back(entry);
        setInfo.push_back((uint32)fields[0].GetUInt8());
        setInfo.push_back((uint32)fields[1].GetUInt8());
        setInfo.push_back((uint32)fields[2].GetUInt16());
        setInfo.push_back((uint32)fields[3].GetUInt16());
        setInfo.push_back((uint32)fields[4].GetUInt32());
        setInfo.push_back((uint32)fields[5].GetUInt32());
        setInfo.push_back((uint32)fields[6].GetUInt32());
        setInfo.push_back((uint32)fields[7].GetUInt32());
        setInfo.push_back((uint32)fields[8].GetUInt32());
        setInfo.push_back((uint32)fields[9].GetUInt32());
        setInfo.push_back((uint32)fields[10].GetUInt32());
        setInfo.push_back((uint32)fields[11].GetUInt32());
        setInfo.push_back((uint32)fields[12].GetUInt32());

        RaceClass rc(fields[1].GetInt8(), fields[0].GetUInt8());
        _transmogSetStore[rc].push_back(setInfo);
        entry++;
    } while (result->NextRow());
    sLog->outString(">> Loaded %u predefined transmog sets.",
        _transmogSetStore.size());
}

void TransmogMgr::LoadTransmogrifiedItems()
{
    QueryResult result = CharacterDatabase.Query("SELECT `guid`, `entry` "
        "FROM `" TRANSMOG_ITEMS_TABLE "`");

    if (!result)
        return;

    do
    {
        Field* fields   = result->Fetch();
        uint32 guid     = fields[0].GetUInt32();
        uint32 newEntry = fields[1].GetUInt32();

        _transmogrifiedItemsStore[guid] = newEntry;
    }
    while (result->NextRow());

    sLog->outString(">> Loaded %u transmogrified items.", 
        _transmogrifiedItemsStore.size());
}