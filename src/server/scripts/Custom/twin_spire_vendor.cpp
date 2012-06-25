/* ScriptData
SDName: Twin_Spire_NPC
SDAuthor: Dave
SD%Complete: 100
SDComment:
SDCategory: passivewow
EndScriptData */
#define TWIN_SPIRE_BLESSING 33779
#define HORDE_SCOUT_NPCTEXT 900000
#define ALLIANCE_SCOUT_NPCTEXT 900000

class twin_spire_vendor : public CreatureScript
{
    public:
        twin_spire_vendor() : CreatureScript("twin_spire_vendor") { }

        bool OnGossipHello(Player* player, Creature* creature)
        {
            if (!creature->isVendor()) return true;
            if (player->HasAura(TWIN_SPIRE_BLESSING))
                player->ADD_GOSSIP_ITEM(GOSSIP_ICON_VENDOR, 
                    GOSSIP_TEXT_BROWSE_GOODS, GOSSIP_SENDER_MAIN, 
                    GOSSIP_ACTION_TRADE);
            player->TalkedToCreature(creature->GetEntry(), creature->GetGUID());
            if (player->getFaction())   // Alliance
                player->SEND_GOSSIP_MENU(ALLIANCE_SCOUT_NPCTEXT, 
                    creature->GetGUID());
            else                        // Horde
                player->SEND_GOSSIP_MENU(HORDE_SCOUT_NPCTEXT, 
                    creature->GetGUID());
            return true;
        }
        
        bool OnGossipSelect(Player* player, Creature* creature, uint32 /*sender*/, uint32 action)
        {
            player->CLOSE_GOSSIP_MENU();
            switch (action)
            {
                case GOSSIP_ACTION_TRADE: 
                    player->GetSession()->SendListInventory(creature->GetGUID()); 
                    break;
            }
            return true;
        }
};

void AddSC_twin_spire_vendor()
{
    new twin_spire_vendor();
}