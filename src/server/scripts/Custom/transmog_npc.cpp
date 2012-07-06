/* ScriptData
SDName: Transmog_NPC
SDAuthor: Dave
SD%Complete: 92
SDComment: 
SDCategory: passivewow
EndScriptData */
#include "ScriptPCH.h"
#include "AccountMgr.h"
#include "TransmogMgr.h"
#include <sstream>
#include <vector>

class transmog_npc : public CreatureScript
{
    public:
        transmog_npc() : CreatureScript("transmog_npc") { }

        bool OnGossipHello(Player* player, Creature* creature)
        {
            raceClass = std::make_pair(player->getRace(), player->getClass());
            TransmogMgr::TransmogSets sets = sTransmogMgr->GetAllSets(raceClass);
            for (TransmogMgr::TransmogSets::iterator set = sets.begin(); 
                 set != sets.end(); ++set)
            {
                std::stringstream gossip_text;
                gossip_text << ((!(*set)[TRANSMOG_FIELD_BRACKET])
                    ? "2v2" : "3v3");
                gossip_text << " - ";
                gossip_text << (*set)[TRANSMOG_FIELD_RATING];
                gossip_text << " rating";

                if (TransmogMgr::CanAfford(player, &(*set)))
                    gossip_text << " [unlocked]";

                player->ADD_GOSSIP_ITEM(GOSSIP_ICON_CHAT, gossip_text.str(),
                    GOSSIP_SENDER_MAIN, (*set)[TRANSMOG_FIELD_ENTRY]);
            }
            // blindly assuming that sets exist in db for all classes
            player->SEND_GOSSIP_MENU(1, creature->GetGUID());
            return true;
        }
        
        bool OnGossipSelect(Player* player, Creature* creature,
                            uint32 /*sender*/, uint32 action)
        {
            sTransmogMgr->TransmogSet(player, raceClass, action);
            player->CLOSE_GOSSIP_MENU();
            return true;
        }

    private:
        TransmogMgr::RaceClass raceClass;
};

void AddSC_transmog_npc()
{
    new transmog_npc();
}