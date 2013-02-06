#include "AccountMgr.h"
#include "Chat.h"
#include "DatabaseEnv.h"
#include "Log.h"
#include "TransmogMgr.h"


uint8 NormalizedArenaBrackets[3] = {
    ARENA_TEAM_2v2,
    ARENA_TEAM_3v3,
    ARENA_TEAM_5v5
};

TransmogMgr::TransmogMgr() { }
TransmogMgr::~TransmogMgr() {
    _transmogSetStore.clear();
    _transmogrifiedItemsStore.clear();
}

void TransmogMgr::LoadTransmog()
{
    LoadTransmogrifiedItems();
    LoadPredefinedSets();
}

void TransmogMgr::LoadTransmogVendorEntries()
{
    // TODO: reloadable, db-bound transmog vendors for multi-vendor
}

uint32 TransmogMgr::GetVendorForSlotAndClass(int8 playerClass, EquipmentSlots slot)
{
    uint32 vendorEntry = 0;

    switch (slot)
    {
        case EQUIPMENT_SLOT_HEAD:
            switch (playerClass)
            {
                case CLASS_PRIEST:
                case CLASS_MAGE:
                case CLASS_WARLOCK:
                    vendorEntry = 800041;
                    break;
                case CLASS_HUNTER:
                case CLASS_SHAMAN:
                    vendorEntry = 800058;
                    break;
                case CLASS_PALADIN:
                case CLASS_WARRIOR:
                case CLASS_DEATH_KNIGHT:
                    vendorEntry = 800065;
                    break;
                case CLASS_DRUID:
                case CLASS_ROGUE:
                    vendorEntry = 800050;
                    break;
            }
            break;
        case EQUIPMENT_SLOT_SHOULDERS:
            switch (playerClass)
            {
                case CLASS_PRIEST:
                case CLASS_MAGE:
                case CLASS_WARLOCK:
                    vendorEntry = 800017;
                    break;
                case CLASS_HUNTER:
                case CLASS_SHAMAN:
                    vendorEntry = 800059;
                    break;
                case CLASS_PALADIN:
                case CLASS_WARRIOR:
                case CLASS_DEATH_KNIGHT:
                    vendorEntry = 800067;
                    break;
                case CLASS_DRUID:
                case CLASS_ROGUE:
                    vendorEntry = 800029;
                    break;
            }
            break;
        case EQUIPMENT_SLOT_WRISTS:
            switch (playerClass)
            {
                case CLASS_PRIEST:
                case CLASS_MAGE:
                case CLASS_WARLOCK:
                    vendorEntry = 800040;
                    break;
                case CLASS_HUNTER:
                case CLASS_SHAMAN:
                    vendorEntry = 800062;
                    break;
                case CLASS_PALADIN:
                case CLASS_WARRIOR:
                case CLASS_DEATH_KNIGHT:
                    vendorEntry = 800080;
                    break;
                case CLASS_DRUID:
                case CLASS_ROGUE:
                    vendorEntry = 800062;
                    break;
            }
            break;
        case EQUIPMENT_SLOT_HANDS:
            switch (playerClass)
            {
                case CLASS_PRIEST:
                case CLASS_MAGE:
                case CLASS_WARLOCK:
                    vendorEntry = 800022;
                    break;
                case CLASS_HUNTER:
                case CLASS_SHAMAN:
                    vendorEntry = 800060;
                    break;
                case CLASS_PALADIN:
                case CLASS_WARRIOR:
                case CLASS_DEATH_KNIGHT:
                    vendorEntry = 800064;
                    break;
                case CLASS_DRUID:
                case CLASS_ROGUE:
                    vendorEntry = 800053;
                    break;
            }
            break;
        case EQUIPMENT_SLOT_CHEST:
            switch (playerClass)
            {
                case CLASS_PRIEST:
                case CLASS_MAGE:
                case CLASS_WARLOCK:
                    vendorEntry = 800011;
                    break;
                case CLASS_HUNTER:
                    vendorEntry = 800033;
                    break;
                case CLASS_SHAMAN:
                    vendorEntry = 800061;
                    break;
                case CLASS_PALADIN:
                case CLASS_WARRIOR:
                case CLASS_DEATH_KNIGHT:
                    vendorEntry = 800068;
                    break;
                case CLASS_DRUID:
                case CLASS_ROGUE:
                    vendorEntry = 800025;
                    break;
            }
            break;
        case EQUIPMENT_SLOT_WAIST:
            switch (playerClass)
            {
                case CLASS_PRIEST:
                case CLASS_MAGE:
                case CLASS_WARLOCK:
                    vendorEntry = 800043;
                    break;
                case CLASS_HUNTER:
                case CLASS_SHAMAN:
                    vendorEntry = 800055;
                    break;
                case CLASS_PALADIN:
                case CLASS_WARRIOR:
                case CLASS_DEATH_KNIGHT:
                    vendorEntry = 800081;
                    break;
                case CLASS_DRUID:
                case CLASS_ROGUE:
                    vendorEntry = 800054;
                    break;
            }
            break;
        case EQUIPMENT_SLOT_LEGS:
            switch (playerClass)
            {
                case CLASS_PRIEST:
                case CLASS_MAGE:
                case CLASS_WARLOCK:
                    vendorEntry = 800018;
                    break;
                case CLASS_HUNTER:
                case CLASS_SHAMAN:
                    vendorEntry = 800056;
                    break;
                case CLASS_PALADIN:
                case CLASS_WARRIOR:
                case CLASS_DEATH_KNIGHT:
                    vendorEntry = 800066;
                    break;
                case CLASS_DRUID:
                case CLASS_ROGUE:
                    vendorEntry = 800052;
                    break;
            }
            break;
        case EQUIPMENT_SLOT_FEET:
            switch (playerClass)
            {
                case CLASS_PRIEST:
                case CLASS_MAGE:
                case CLASS_WARLOCK:
                    vendorEntry = 800030;
                    break;
                case CLASS_HUNTER:
                case CLASS_SHAMAN:
                    vendorEntry = 800057;
                    break;
                case CLASS_PALADIN:
                case CLASS_WARRIOR:
                case CLASS_DEATH_KNIGHT:
                    vendorEntry = 800063;
                    break;
                case CLASS_DRUID:
                case CLASS_ROGUE:
                    vendorEntry = 800039;
                    break;
            }
            break;
    }
    return vendorEntry;
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

bool TransmogMgr::CanAfford(Player* player, const std::vector<uint32>* setInfo)
{
    uint32 bracket = setInfo->at(TRANSMOG_FIELD_BRACKET);
    if (player->GetLifetimeStats(bracket).PersonalRating >=
            setInfo->at(TRANSMOG_FIELD_RATING)
        || player->GetArenaPersonalRating(bracket)
            >= setInfo->at(TRANSMOG_FIELD_RATING)
        || (!AccountMgr::IsPlayerAccount(player->GetSession()->GetSecurity())))
        return true;
    return false;
}

TransmogResult TransmogMgr::TransmogItem(Player* player, uint32 newEntry,
                                         int8 slot, bool silent)
{
    if (slot == EQUIPMENT_SLOT_FINGER1
        || slot == EQUIPMENT_SLOT_FINGER2
        || slot == EQUIPMENT_SLOT_TRINKET1
        || slot == EQUIPMENT_SLOT_TRINKET2
        || slot == EQUIPMENT_SLOT_TABARD)
        return TRANSMOG_ERR_INVALID_SLOT;

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
            case TRANSMOG_ERR_DIFF_CLASS:
            case TRANSMOG_ERR_DIFF_RACE:
                handler.PSendSysMessage("You don't meet the class/race "
                    "requirements for that item.");
                break;
            case TRANSMOG_ERR_DIFF_ARMOR:
                handler.PSendSysMessage("You can't transmog into that armor "
                    "type.");
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
        if (it->second[setNo][TRANSMOG_FIELD_ENTRY] == setId)
            return TransmogSet(player, &it->second[setNo]);

    return TransmogSet(player, player->GetSession()->GetAccountId(), setId);
}

TransmogResult TransmogMgr::TransmogSet(Player* player, uint32 accountId,
                                        uint32 setId)
{
    const TransmogSets* sets = GetAllSets(accountId);
    if (sets->empty()) return TRANSMOG_ERR_INVALID_SET;

    for (TransmogSets::const_iterator set = sets->begin(); set != sets->end();
         ++set)
    {
        if ((*set)[TRANSMOG_FIELD_ENTRY] == setId)
        {
            return TransmogSet(player, &(*set));
            break;
        }
    }
    return TRANSMOG_ERR_INVALID_SET;
}

TransmogResult TransmogMgr::TransmogSet(Player* player,
                                        const std::vector<uint32>* setInfo)
{
    if (setInfo->empty()) return TRANSMOG_ERR_INVALID_SET;
    if ((*setInfo)[TRANSMOG_FIELD_EMPTY]) return TRANSMOG_ERR_EMPTY;

    if (!CanAfford(player, setInfo))
        return TRANSMOG_ERR_UNMET_REQUIREMENTS;

    EquipmentSlots equipmentSet[8] = {
        EQUIPMENT_SLOT_HEAD,
        EQUIPMENT_SLOT_SHOULDERS,
        EQUIPMENT_SLOT_CHEST,
        EQUIPMENT_SLOT_WRISTS,
        EQUIPMENT_SLOT_HANDS,
        EQUIPMENT_SLOT_WAIST,
        EQUIPMENT_SLOT_LEGS,
        EQUIPMENT_SLOT_FEET
    };

    TransmogResult lastResult;
    for (int slot = 0; slot < 8; slot++)
    {
        int8 field = TransmogFieldForSlot(equipmentSet[slot]);
        lastResult = TransmogItem(player, (*setInfo)[field], equipmentSet[slot]);
        if (lastResult != TRANSMOG_ERR_OK)
            break;
    }
    return lastResult;
}

bool TransmogMgr::SaveCustomSet(Player* player, uint32 accountId, uint32 setId)
{
    EquipmentSlots equipmentSet[8] = {
        EQUIPMENT_SLOT_HEAD,
        EQUIPMENT_SLOT_SHOULDERS,
        EQUIPMENT_SLOT_CHEST,
        EQUIPMENT_SLOT_WRISTS,
        EQUIPMENT_SLOT_HANDS,
        EQUIPMENT_SLOT_WAIST,
        EQUIPMENT_SLOT_LEGS,
        EQUIPMENT_SLOT_FEET
    };
    uint32 entries[8];

    for (int slot = 0; slot < 8; slot++)
    {
        Item* item = player->GetItemByPos(INVENTORY_SLOT_BAG_0,
            equipmentSet[slot]);
        if (!item) return false;
        entries[slot] = item->GetTemplate()->ItemId;
    }

    WorldDatabase.PExecute("UPDATE `" TRANSMOG_PREDEFINED_SETS_TABLE "` SET "
        "empty=0, helm_id=%u, shoulder_id=%u, chest_id=%u, wrist_id=%u, "
        "hands_id=%u, waist_id=%u, legs_id=%u, boots_id=%u "
        "WHERE account_id=%u AND id=%u", entries[0], entries[1], entries[2],
        entries[3], entries[4], entries[5], entries[6], entries[7], accountId,
        setId);
    for (TransmogSets::iterator it = _transmogCustomSetStore[accountId].begin();
         it != _transmogCustomSetStore[accountId].end(); it++)
    {
        if ((*it)[TRANSMOG_FIELD_ENTRY] == setId)
        {
            (*it)[TRANSMOG_FIELD_EMPTY]     = 0;
            (*it)[TRANSMOG_FIELD_HEAD]      = entries[0];
            (*it)[TRANSMOG_FIELD_SHOULDERS] = entries[1];
            (*it)[TRANSMOG_FIELD_CHEST]     = entries[2];
            (*it)[TRANSMOG_FIELD_WRISTS]    = entries[3];
            (*it)[TRANSMOG_FIELD_HANDS]     = entries[4];
            (*it)[TRANSMOG_FIELD_WAIST]     = entries[5];
            (*it)[TRANSMOG_FIELD_LEGS]      = entries[6];
            (*it)[TRANSMOG_FIELD_FEET]      = entries[7];
        }
    }
    return true;
}

uint32 TransmogMgr::GetTransmogrifiedItemEntry(uint32 itemGuid)
{
    TransmogrifiedItemsContainer::const_iterator itr;
    itr = _transmogrifiedItemsStore.find(itemGuid);

    if (itr != _transmogrifiedItemsStore.end())
        return itr->second;

    return 0;
}

int8 TransmogMgr::RemoveAllTransmogrifiedItems(Player* player)
{
    int8 slots[] = {
        EQUIPMENT_SLOT_HEAD,
        EQUIPMENT_SLOT_NECK,
        EQUIPMENT_SLOT_SHOULDERS,
        EQUIPMENT_SLOT_BODY,
        EQUIPMENT_SLOT_CHEST,
        EQUIPMENT_SLOT_WAIST,
        EQUIPMENT_SLOT_LEGS,
        EQUIPMENT_SLOT_FEET,
        EQUIPMENT_SLOT_WRISTS,
        EQUIPMENT_SLOT_HANDS,
        EQUIPMENT_SLOT_BACK,
        EQUIPMENT_SLOT_MAINHAND,
        EQUIPMENT_SLOT_OFFHAND,
        EQUIPMENT_SLOT_RANGED,
    };
    int8 count = 0;
    for (int i=0; i < 14; i++)
    {
        Item* item = player->GetItemByPos(INVENTORY_SLOT_BAG_0, slots[i]);
        if (!item) continue;
        else if (item->GetTransmog() != 0)
        {
            RemoveTransmogrifiedItem(player, item);
            count++;
        }
    }
    return count;
}

void TransmogMgr::RemoveTransmogrifiedItem(Player* player, Item* item)
{
    if (!item) return;
    item->RemoveTransmog();
    TransmogrifiedItemsContainer::iterator itr;
    itr = _transmogrifiedItemsStore.find(item->GetGUIDLow());
    if (itr != _transmogrifiedItemsStore.end())
        _transmogrifiedItemsStore.erase(itr);

    WorldPacket data;
    data << uint8(INVENTORY_SLOT_BAG_0);
    data << uint8(item->GetSlot());
    player->GetSession()->HandleAutoEquipItemOpcode(data);
    player->SetVisibleItemSlot(item->GetSlot(), item);
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

void TransmogMgr::LoadCustomSets(uint32 accountId)
{
    _transmogCustomSetStore[accountId].clear();
    QueryResult result = WorldDatabase.PQuery("SELECT * FROM `"
        TRANSMOG_PREDEFINED_SETS_TABLE "` WHERE account_id = '%u' ORDER BY "
        "`id` DESC", accountId);
    if (!result) return;
    do
    {
        Field* fields = result->Fetch();
        std::vector<uint32> setInfo;
        setInfo.push_back((uint32)fields[TRANSMOG_FIELD_ENTRY].GetUInt32());
        setInfo.push_back((uint32)fields[TRANSMOG_FIELD_ACCOUNTID].GetUInt32());
        setInfo.push_back((uint32)fields[TRANSMOG_FIELD_EMPTY].GetUInt8());
        setInfo.push_back((uint32)fields[TRANSMOG_FIELD_CLASS].GetUInt8());
        setInfo.push_back((uint32)fields[TRANSMOG_FIELD_RACE].GetUInt8());
        setInfo.push_back((uint32)fields[TRANSMOG_FIELD_RATING].GetUInt16());
        setInfo.push_back((uint32)fields[TRANSMOG_FIELD_BRACKET].GetUInt16());
        setInfo.push_back((uint32)fields[TRANSMOG_FIELD_HEAD].GetUInt32());
        setInfo.push_back((uint32)fields[TRANSMOG_FIELD_SHOULDERS].GetUInt32());
        setInfo.push_back((uint32)fields[TRANSMOG_FIELD_CHEST].GetUInt32());
        setInfo.push_back((uint32)fields[TRANSMOG_FIELD_WRISTS].GetUInt32());
        setInfo.push_back((uint32)fields[TRANSMOG_FIELD_HANDS].GetUInt32());
        setInfo.push_back((uint32)fields[TRANSMOG_FIELD_WAIST].GetUInt32());
        setInfo.push_back((uint32)fields[TRANSMOG_FIELD_LEGS].GetUInt32());
        setInfo.push_back((uint32)fields[TRANSMOG_FIELD_FEET].GetUInt32());
        _transmogCustomSetStore[accountId].push_back(setInfo);
    } while (result->NextRow());
}

void TransmogMgr::LoadPredefinedSets()
{
    QueryResult result = WorldDatabase.Query("SELECT * FROM `"
        TRANSMOG_PREDEFINED_SETS_TABLE "` WHERE account_id IS NULL "
        "ORDER BY bracket, rating");
    if (!result) return;
    do
    {
        Field* fields = result->Fetch();
        std::vector<uint32> setInfo;
        setInfo.push_back(fields[TRANSMOG_FIELD_ENTRY].GetUInt32());
        setInfo.push_back(fields[TRANSMOG_FIELD_ACCOUNTID].GetUInt32());
        setInfo.push_back(fields[TRANSMOG_FIELD_EMPTY].GetUInt8());
        setInfo.push_back(fields[TRANSMOG_FIELD_CLASS].GetUInt8());
        setInfo.push_back(fields[TRANSMOG_FIELD_RACE].GetUInt8());
        setInfo.push_back(fields[TRANSMOG_FIELD_RATING].GetUInt16());
        setInfo.push_back(fields[TRANSMOG_FIELD_BRACKET].GetUInt16());
        setInfo.push_back(fields[TRANSMOG_FIELD_HEAD].GetUInt32());
        setInfo.push_back(fields[TRANSMOG_FIELD_SHOULDERS].GetUInt32());
        setInfo.push_back(fields[TRANSMOG_FIELD_CHEST].GetUInt32());
        setInfo.push_back(fields[TRANSMOG_FIELD_WRISTS].GetUInt32());
        setInfo.push_back(fields[TRANSMOG_FIELD_HANDS].GetUInt32());
        setInfo.push_back(fields[TRANSMOG_FIELD_WAIST].GetUInt32());
        setInfo.push_back(fields[TRANSMOG_FIELD_LEGS].GetUInt32());
        setInfo.push_back(fields[TRANSMOG_FIELD_FEET].GetUInt32());

        RaceClass rc(fields[TRANSMOG_FIELD_RACE].GetInt8(),
            fields[TRANSMOG_FIELD_CLASS].GetUInt8());
        _transmogSetStore[rc].push_back(setInfo);
    } while (result->NextRow());
    sLog->outInfo(LOG_FILTER_SERVER_LOADING, ">> Loaded %u predefined transmog sets.",
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

    sLog->outInfo(LOG_FILTER_SERVER_LOADING, ">> Loaded %u transmogrified items.",
        _transmogrifiedItemsStore.size());
}
