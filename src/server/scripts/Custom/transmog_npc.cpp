/* ScriptData
SDName: Transmog_NPC
SDAuthor: Dave
SD%Complete: 95
SDComment: 
SDCategory: passivewow
EndScriptData */
#include "ScriptPCH.h"
#include "AccountMgr.h"
#include "DatabaseEnv.h"
#include "TransmogMgr.h"
#include <sstream>
#include <vector>

#define CUSTOM_SET_TOKEN 49426 // EoF
#define CUSTOM_SET_PRICE 50

class transmog_npc : public CreatureScript
{
    public:
        transmog_npc() : CreatureScript("transmog_npc") { }

        bool OnGossipHello(Player* player, Creature* creature)
        {
            player->PlayerTalkClass->ClearMenus();
            editing = false;
            raceClass = std::make_pair(player->getRace(), player->getClass());
            TransmogMgr::TransmogSets sets = sTransmogMgr->GetAllSets(raceClass);

            for (TransmogMgr::TransmogSets::iterator set = sets.begin(); 
                 set != sets.end(); ++set)
            {
                std::stringstream gossip_text;
                switch (set->at(TRANSMOG_FIELD_BRACKET))
                {
                    case 0:
                        gossip_text << "2v2";
                        break;
                    case 1:
                        gossip_text << "3v3";
                        break;
                    case 2:
                        gossip_text << "5v5";
                        break;
                    default:
                        sLog->outError(LOG_FILTER_TRANSMOG, "invalid set with id %d",
                            (*set)[TRANSMOG_FIELD_ENTRY]);
                        continue;
                }
                gossip_text << " - ";
                gossip_text << (*set)[TRANSMOG_FIELD_RATING];
                gossip_text << " rating";

                if (TransmogMgr::CanAfford(player, &(*set)))
                    gossip_text << " [unlocked]";

                player->ADD_GOSSIP_ITEM(GOSSIP_ICON_CHAT, gossip_text.str(),
                    GOSSIP_SENDER_MAIN, (*set)[TRANSMOG_FIELD_ENTRY]);
            }

            player->ADD_GOSSIP_ITEM(GOSSIP_ICON_MONEY_BAG, "Custom sets",
                GOSSIP_SENDER_MAIN, GetActionIndex());

            // blindly assuming that sets exist in db for all classes
            player->SEND_GOSSIP_MENU(1, creature->GetGUID());
            return true;
        }
        
        void CreateCustomSet(Player* player, Creature* creature)
        { 
            if (player->HasItemCount(CUSTOM_SET_TOKEN, CUSTOM_SET_PRICE, false))
            {
                uint32 accountId = player->GetSession()->GetAccountId();
                player->DestroyItemCount(CUSTOM_SET_TOKEN, CUSTOM_SET_PRICE,
                    true);
                WorldDatabase.PExecute("INSERT INTO `"
                    TRANSMOG_PREDEFINED_SETS_TABLE "` (account_id, empty) "
                    "VALUES (%u, 1)", accountId);
                sTransmogMgr->LoadCustomSets(accountId);
                ShowCustomSetsMenu(player, creature, GetActionIndex());
                return;
            }
            else
                creature->MonsterWhisper("You don't have enough tokens for "
                    "that. Required tokens are rewarded for donations, "
                    "please see our website for more information!",
                    player->GetGUID());
            player->CLOSE_GOSSIP_MENU();
        }

        void ShowCustomSetsMenu(Player* player, Creature* creature, 
                                const int actionIndex)
        {
            editing = false;
            player->PlayerTalkClass->ClearMenus();
            const TransmogMgr::TransmogSets* customSets;
            customSets = sTransmogMgr->GetAllSets(player->GetSession()->GetAccountId());
            if (customSets)
            {   
                TransmogMgr::TransmogSets::const_iterator set;
                int count = 1;
                for (set = customSets->begin(); set != customSets->end(); ++set)
                {
                    player->ADD_GOSSIP_ITEM(GOSSIP_ICON_CHAT, "Custom set",
                        GOSSIP_SENDER_MAIN, set->at(TRANSMOG_FIELD_ENTRY));
                    count++;
                }
                player->ADD_GOSSIP_ITEM(GOSSIP_ICON_INTERACT_1, 
                    "Edit custom sets", GOSSIP_SENDER_MAIN, actionIndex + 2);
            }

            player->ADD_GOSSIP_ITEM(GOSSIP_ICON_MONEY_BAG, "Create custom set",
                GOSSIP_SENDER_MAIN, actionIndex + 1);
            player->ADD_GOSSIP_ITEM(GOSSIP_ICON_CHAT, "Back", 
                GOSSIP_SENDER_MAIN, actionIndex + 3);
            player->SEND_GOSSIP_MENU(1, creature->GetGUID());
        }
        
        void ShowEditCustomSetsMenu(Player* player, Creature* creature,
                                    const int actionIndex)
        {
            editing = true;
            player->PlayerTalkClass->ClearMenus();
            const TransmogMgr::TransmogSets* customSets;
            customSets = sTransmogMgr->GetAllSets(player->GetSession()->GetAccountId());
            if (customSets)
            {
                int count = 1;
                TransmogMgr::TransmogSets::const_iterator set;
                for (set = customSets->begin(); set != customSets->end(); ++set)
                {
                    player->ADD_GOSSIP_ITEM(GOSSIP_ICON_CHAT, "Save custom set",
                        GOSSIP_SENDER_MAIN, (*set)[TRANSMOG_FIELD_ENTRY]);
                    count++;
                }
            }
            player->ADD_GOSSIP_ITEM(GOSSIP_ICON_CHAT, "Back", 
                GOSSIP_SENDER_MAIN, actionIndex);
            player->SEND_GOSSIP_MENU(1, creature->GetGUID());
        }

        bool OnGossipSelect(Player* player, Creature* creature,
                            uint32 /*sender*/, uint32 action)
        {
            const int actionIndex = GetActionIndex();
            if (action == actionIndex)
                ShowCustomSetsMenu(player, creature, actionIndex);
            else if (action == actionIndex + 1)
                CreateCustomSet(player, creature);
            else if (action == actionIndex + 2)
                ShowEditCustomSetsMenu(player, creature, actionIndex);
            else if (action == actionIndex + 3)
                OnGossipHello(player, creature);
            else
            {
                uint32 accountId = player->GetSession()->GetAccountId();
                if (editing)
                {
                    if (sTransmogMgr->SaveCustomSet(player, accountId, action))
                    {
                        ChatHandler(player).PSendSysMessage("Successfully "
                            "saved transmog set.");
                        return OnGossipHello(player, creature);
                    }
                    else
                        ChatHandler(player).PSendSysMessage("Unable to save "
                            "custom set. Please ensure that you aren't missing "
                            "armor pieces.");
                    return true;
                }
                TransmogResult result;
                result = sTransmogMgr->TransmogSet(player, raceClass, action);
                switch (result)
                {
                    case TRANSMOG_ERR_INVALID_SET:
                        ChatHandler(player).PSendSysMessage("Problem applying "
                            "transmogrification. This shouldn't happen. Please "
                            "open a thread in the support forum so we can fix "
                            "this!");
                        break;
                    case TRANSMOG_ERR_EMPTY:
                        ChatHandler(player).PSendSysMessage("Nothing "
                            "saved to this set yet. Wear your desired "
                            "armor and then save your set under 'Edit "
                            "Sets'.");
                        ShowCustomSetsMenu(player, creature, actionIndex);
                        return true;
                    case TRANSMOG_ERR_UNMET_REQUIREMENTS:
                        ChatHandler(player).PSendSysMessage("You don't meet "
                            "the requirements for that set.");
                }
                player->CLOSE_GOSSIP_MENU();
            }
        }

    private:
        bool editing;
        TransmogMgr::RaceClass raceClass;
        const int GetActionIndex() {
            return sTransmogMgr->GetTotalTransmogSets() + 1;
        }
};

void AddSC_transmog_npc()
{
    new transmog_npc();
}
