/* ScriptData
SDName: Transmog_NPC
SDAuthor: Dave
SD%Complete: 90
SDComment: 
SDCategory: passivewow
EndScriptData */
#include "ScriptPCH.h"
#include "AccountMgr.h"
#include <sstream>
#include <vector>


enum TransmogSetFields
{
    TMS_FIELD_ENTRY,
    TMS_FIELD_CLASS,
    TMS_FIELD_RACE,
    TMS_FIELD_RATING,
    TMS_FIELD_BRACKET,
    TMS_FIELD_HELM,
    TMS_FIELD_SHOULDER,
    TMS_FIELD_CHEST,
    TMS_FIELD_WRISTS,
    TMS_FIELD_HANDS,
    TMS_FIELD_BELT,
    TMS_FIELD_LEGS,
    TMS_FIELD_BOOTS,
};


enum TransmogResult
{
    TM_RESULT_UNMET_REQUIREMENTS,
    TM_RESULT_CANT_FIND_ITEM,
    TM_RESULT_UNEQUIPPED_ITEM,
    TM_RESULT_WRONG_QUALITY,
    TM_RESULT_DIFF_SLOTS,
    TM_RESULT_INVALID_SLOT,
    TM_RESULT_NO_SETS,
    TM_RESULT_OK
};


class transmog_npc : public CreatureScript
{
    std::vector<std::vector<uint32> > m_transmogSetStore;

    public:
        transmog_npc() : CreatureScript("transmog_npc")
        {
            QueryResult result = WorldDatabase.Query(
                "SELECT * FROM transmog_sets ORDER BY bracket, rating");
            uint32 entry = 0;
            do {
                Field* fields = result->Fetch();
                std::vector<uint32> setinfo;
                // probably not very efficient
                setinfo.push_back(entry);
                setinfo.push_back((uint32)fields[0].GetInt8());
                setinfo.push_back((uint32)fields[1].GetUInt8());
                setinfo.push_back((uint32)fields[2].GetUInt16());
                setinfo.push_back((uint32)fields[3].GetUInt16());
                setinfo.push_back((uint32)fields[4].GetUInt32());
                setinfo.push_back((uint32)fields[5].GetUInt32());
                setinfo.push_back((uint32)fields[6].GetUInt32());
                setinfo.push_back((uint32)fields[7].GetUInt32());
                setinfo.push_back((uint32)fields[8].GetUInt32());
                setinfo.push_back((uint32)fields[9].GetUInt32());
                setinfo.push_back((uint32)fields[10].GetUInt32());
                setinfo.push_back((uint32)fields[11].GetUInt32());
                setinfo.push_back((uint32)fields[12].GetUInt32());
                m_transmogSetStore.push_back(setinfo);
                entry++;
            } while (result->NextRow());
        }
        
        TransmogResult TransmogrifyItem(Player* player, Creature* creature, 
                                        int slot, uint32 new_entry)
        {
            int8 field;
            switch (slot)
            {
                case EQUIPMENT_SLOT_HEAD:
                    field = TMS_FIELD_HELM;
                    break;
                case EQUIPMENT_SLOT_SHOULDERS:
                    field = TMS_FIELD_SHOULDER;
                    break;
                case EQUIPMENT_SLOT_CHEST:
                    field = TMS_FIELD_CHEST;
                    break;
                case EQUIPMENT_SLOT_WRISTS:
                    field = TMS_FIELD_WRISTS;
                    break;
                case EQUIPMENT_SLOT_HANDS:
                    field = TMS_FIELD_HANDS;
                    break;
                case EQUIPMENT_SLOT_WAIST:
                    field = TMS_FIELD_BELT;
                    break;
                case EQUIPMENT_SLOT_LEGS:
                    field = TMS_FIELD_LEGS;
                    break;
                case EQUIPMENT_SLOT_FEET:
                    field = TMS_FIELD_BOOTS;
                    break;
            }
            if (!field) return TM_RESULT_INVALID_SLOT;

            std::vector<uint32> setinfo;
            for (std::vector<int>::size_type i = 0; 
                 i != m_transmogSetStore.size(); i++)
            {
                if (m_transmogSetStore[i][TMS_FIELD_ENTRY] == new_entry)
                    setinfo = m_transmogSetStore[i];
            }
            if (setinfo.empty()) return TM_RESULT_NO_SETS; // shouldn't happen

            // restrict by arena rating for regular players, but allow all GM
            // accounts to transmog
            if (player->GetArenaPersonalRating(setinfo[TMS_FIELD_BRACKET]) <
                setinfo[TMS_FIELD_RATING] &&
                AccountMgr::IsPlayerAccount(player->GetSession()->GetSecurity()))
                return TM_RESULT_UNMET_REQUIREMENTS;
            
            Item* item = player->GetItemByPos(INVENTORY_SLOT_BAG_0, slot);
            if (!item) {
                return TM_RESULT_UNEQUIPPED_ITEM;
            }

            uint8 result = item->SetFakeDisplay(setinfo[field]);
            switch (result)
            {
                case FAKE_ERR_CANT_FIND_ITEM:
                    return TM_RESULT_CANT_FIND_ITEM;
                    break;
                case FAKE_ERR_WRONG_QUALITY:
                    return TM_RESULT_WRONG_QUALITY;
                    break;
                case FAKE_ERR_DIFF_SLOTS:
                    return TM_RESULT_DIFF_SLOTS;
                    break;
                /*
                case FAKE_ERR_DIFF_CLASS:
                case FAKE_ERR_DIFF_RACE:
                    handler.PSendSysMessage("Wrong race or class for item.");
                    break;
                */
                case FAKE_ERR_OK:
                {
                    WorldPacket data;
                    data << uint8(INVENTORY_SLOT_BAG_0);
                    data << uint8(slot);
                    player->GetSession()->HandleAutoEquipItemOpcode(data);
                    player->SetVisibleItemSlot(slot, item);
                    return TM_RESULT_OK;
                }
            }
        }
        
        bool OnGossipHello(Player* player, Creature* creature)
        {
            for (int i = 0; i < m_transmogSetStore.size(); i++)
            {
                std::vector<uint32> setinfo = m_transmogSetStore[i];
                if (player->getClass() == setinfo[TMS_FIELD_CLASS] &&
                    player->getRace() == setinfo[TMS_FIELD_RACE])
                {
                    std::stringstream gossip_text;
                    uint32 entry, rating, bracket;

                    entry = setinfo[TMS_FIELD_ENTRY];
                    rating = setinfo[TMS_FIELD_RATING];
                    bracket = setinfo[TMS_FIELD_BRACKET];

                    gossip_text << ((!bracket) ? "2v2" : "3v3");
                    gossip_text << " - " << rating << " rating";
                    player->ADD_GOSSIP_ITEM(
                        GOSSIP_ICON_CHAT, gossip_text.str(),
                        GOSSIP_SENDER_MAIN, entry);
                }
            }
            // blindly assuming that sets exist in db for all classes
            player->SEND_GOSSIP_MENU(1, creature->GetGUID());
            return true;
        }
        
        bool OnGossipSelect(Player* player, Creature* creature, 
                            uint32 /*sender*/, uint32 action)
        {
            EquipmentSlots equipment_slots[8] = {
                EQUIPMENT_SLOT_HEAD,
                EQUIPMENT_SLOT_SHOULDERS,
                EQUIPMENT_SLOT_CHEST,
                EQUIPMENT_SLOT_LEGS,
                EQUIPMENT_SLOT_WRISTS,
                EQUIPMENT_SLOT_HANDS,
                EQUIPMENT_SLOT_WAIST,
                EQUIPMENT_SLOT_FEET
            };
            for (int i=0; i<8; i++)
            {
                TransmogResult result = TransmogrifyItem(player, creature, 
                    equipment_slots[i], action);
                if (result != TM_RESULT_OK) {
                    ChatHandler handler(player);
                    switch (result)
                    {
                        case TM_RESULT_UNMET_REQUIREMENTS:
                            handler.PSendSysMessage("You don't meet the requirements for that!");
                            goto close_menu;
                            break;
                        default:
                            break;
                    }
                }
            }
            close_menu:
            player->CLOSE_GOSSIP_MENU();
            return true;
        }
};

void AddSC_transmog_npc()
{
    new transmog_npc();
}