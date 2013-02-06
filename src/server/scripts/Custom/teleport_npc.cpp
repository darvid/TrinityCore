#include "ScriptPCH.h"

#define VISUAL 27123

#define STORMWIND  0,   -9006.330078f,    478.264008f,   97.294930f,   4.47967f
#define UNDERCITY  0,    1628.025024f,    206.270279f , -42.274281f,  1.791683f
#define THUNDER    1,   -1277.369995f,    124.804001f,  131.287003f,  5.222740f
#define DALARAN    571,  5833.479980f,    563.328003f,  651.846008f,  3.124140f

#define RAVENHOLDT 0,      30.462950f,  -1593.851562f,   196.240707f, 3.974367f
#define EXODAR     530, -4079.623535f, -11428.028320f,  -141.507874f, 1.810331f
#define UNDERCITYM 0,    1807.340332f,    266.445923f,    66.220222f, 3.699799f
#define ORGRIMMAR  1,    1604.256470f,  -4208.895508f,    44.970497f, 3.443723f
#define ORGRIMMARN 1,    1481.815796f,  -4395.206543f,    27.547464f, 5.639551f
#define IRONFORGE  0,   -4877.646484f,   -917.115540f,   502.484161f, 3.857564f

#define GURUBASHI  0,   -13239.008789,    229.664322f,   34.073196f,  0.204953f
#define TRIALS     530, -1983.075684f,   6576.570801f,   12.819382f,  2.586493f
#define MAUL       1,   -3747.376221f,   1163.778198f,  128.275238f,  2.330298f

#define DUELZONE   1,    3074.363281f,    567.076782f,    5.721445f,  0.789787f
#define DUELZONE2  0,   -1850.870728f,  -4251.149902f,    2.957376f,  1.126399f
#define DUELZONE3  1,    3182.092285f,    565.628784f,    0.350016f,  0.280709f
#define VIPMALL    571,  -111.450027f,  -3560.103760f,    4.432752f,  2.881422f

#define CUNNING    269, -1636.353271f,   7110.559570f,   24.068108f,  3.470572f
#define FEROCITY   269, -1666.985352f,   7315.426270f,   23.647949f,  2.119681f
#define TENACITY   269, -2048.650879f,   7409.015625f,   25.685230f,  5.261276f
#define EXOTIC     269, -2098.643066f,   6927.824707f,   24.871885f,  5.522799f

#define RPMALL       1,  7450.502441f,  -1552.279785f,  177.819031f,  0.190201f

#define HANDLE(action, teleport) case action:\
    Teleport(player, teleport);\
    break;


class teleport_npc : public CreatureScript
{
    public:
        teleport_npc() : CreatureScript("teleport_npc")
        {
        }

        void Teleport(Player* player, uint32 map, float x, float y, float z, float o)
        {
            player->TeleportTo(map, x, y, z, o);
            player->CastSpell(player, 27123, true);
            player->CLOSE_GOSSIP_MENU();
        }

        void ShowCitiesMenu(Player* player, Creature* creature)
        {
            player->PlayerTalkClass->ClearMenus();
            if (player->GetTeam() == ALLIANCE)
                player->ADD_GOSSIP_ITEM(GOSSIP_ICON_TAXI, "Stormwind", GOSSIP_SENDER_MAIN,
                    10);
            else
                player->ADD_GOSSIP_ITEM(GOSSIP_ICON_TAXI, "Undercity", GOSSIP_SENDER_MAIN,
                    11);
            player->ADD_GOSSIP_ITEM(GOSSIP_ICON_TAXI, "Dalaran", GOSSIP_SENDER_MAIN,
                12);
            player->ADD_GOSSIP_ITEM(GOSSIP_ICON_CHAT, "Back", GOSSIP_SENDER_MAIN, -1);
            player->SEND_GOSSIP_MENU(1, creature->GetGUID());
        }

        void ShowPvpZonesMenu(Player* player, Creature* creature)
        {
            player->PlayerTalkClass->ClearMenus();
            player->ADD_GOSSIP_ITEM(GOSSIP_ICON_BATTLE, "Gurubashi", GOSSIP_SENDER_MAIN,
                30);
            player->ADD_GOSSIP_ITEM(GOSSIP_ICON_BATTLE, "Ring of Trials", GOSSIP_SENDER_MAIN,
                31);
            player->ADD_GOSSIP_ITEM(GOSSIP_ICON_BATTLE, "Dire Maul", GOSSIP_SENDER_MAIN,
                32);
            player->ADD_GOSSIP_ITEM(GOSSIP_ICON_CHAT, "Back", GOSSIP_SENDER_MAIN, -1);
            player->SEND_GOSSIP_MENU(1, creature->GetGUID());
        }

        void ShowMallsMenu(Player* player, Creature* creature)
        {
            player->PlayerTalkClass->ClearMenus();
            if (player->GetTeam() == ALLIANCE)
            {
                player->ADD_GOSSIP_ITEM(GOSSIP_ICON_TAXI, "Ironforge", GOSSIP_SENDER_MAIN,
                    44);
                player->ADD_GOSSIP_ITEM(GOSSIP_ICON_TAXI, "Exodar", GOSSIP_SENDER_MAIN,
                    40);
                player->ADD_GOSSIP_ITEM(GOSSIP_ICON_TAXI, "Ravenholdt", GOSSIP_SENDER_MAIN,
                    42);
            }
            else
            {
                player->ADD_GOSSIP_ITEM(GOSSIP_ICON_TAXI, "Orgrimmar", GOSSIP_SENDER_MAIN,
                    41);
                player->ADD_GOSSIP_ITEM(GOSSIP_ICON_TAXI, "Undercity", GOSSIP_SENDER_MAIN,
                    43);
            }
            player->ADD_GOSSIP_ITEM(GOSSIP_ICON_CHAT, "Back", GOSSIP_SENDER_MAIN, -1);
            player->SEND_GOSSIP_MENU(1, creature->GetGUID());
        }

        void ShowPetMallMenu(Player* player, Creature* creature)
        {
            player->PlayerTalkClass->ClearMenus();
            player->ADD_GOSSIP_ITEM(GOSSIP_ICON_TAXI, "Cunning", GOSSIP_SENDER_MAIN, 100);
            player->ADD_GOSSIP_ITEM(GOSSIP_ICON_TAXI, "Ferocity", GOSSIP_SENDER_MAIN, 101);
            player->ADD_GOSSIP_ITEM(GOSSIP_ICON_TAXI, "Tenacity", GOSSIP_SENDER_MAIN, 102);
            player->ADD_GOSSIP_ITEM(GOSSIP_ICON_TAXI, "Exotic", GOSSIP_SENDER_MAIN, 103);
            player->SEND_GOSSIP_MENU(1, creature->GetGUID());
        }

        void ShowMainMenu(Player* player, Creature* creature)
        {
            player->PlayerTalkClass->ClearMenus();
            player->ADD_GOSSIP_ITEM(GOSSIP_ICON_TABARD, "Cities", GOSSIP_SENDER_MAIN,
                1);
            player->ADD_GOSSIP_ITEM(GOSSIP_ICON_TABARD, "Malls", GOSSIP_SENDER_MAIN,
                3);
            if (player->HasItemCount(45924, 1, false) ||
                player->GetSession()->GetSecurity() >= SEC_VIP)
                player->ADD_GOSSIP_ITEM(GOSSIP_ICON_TABARD, "VIP Mall", GOSSIP_SENDER_MAIN, 5);
            player->ADD_GOSSIP_ITEM(GOSSIP_ICON_BATTLE, "PvP Zones", GOSSIP_SENDER_MAIN,
                2);
            player->ADD_GOSSIP_ITEM(GOSSIP_ICON_BATTLE, "Duel Zone - Ashenvale", GOSSIP_SENDER_MAIN,
                4);
            player->ADD_GOSSIP_ITEM(GOSSIP_ICON_BATTLE, "Duel Zone - Arathi Highlands", GOSSIP_SENDER_MAIN,
                6);
            if (player->getClass() == CLASS_HUNTER)
            {
                player->ADD_GOSSIP_ITEM(GOSSIP_ICON_CHAT, "Pet Mall", GOSSIP_SENDER_MAIN, 7);
            }
            player->ADD_GOSSIP_ITEM(GOSSIP_ICON_CHAT, "RP/Transmog Mall", GOSSIP_SENDER_MAIN,
                8);
            player->SEND_GOSSIP_MENU(1, creature->GetGUID());
        }

        bool OnGossipHello(Player* player, Creature* creature)
        {
            if (player->isInCombat()) return false;
            ShowMainMenu(player, creature);
            return true;
        }

        bool OnGossipSelect(Player* player, Creature* creature,
                            uint32 /*sender*/, uint32 action)
        {
            switch (action)
            {
                case -1:
                    ShowMainMenu(player, creature);
                    break;
                case 1:
                    ShowCitiesMenu(player, creature);
                    break;
                case 2:
                    ShowPvpZonesMenu(player, creature);
                    break;
                case 3:
                    ShowMallsMenu(player, creature);
                    break;
                case 7:
                    ShowPetMallMenu(player, creature);
                    break;
                HANDLE(4,   DUELZONE3)
                HANDLE(5,   VIPMALL)
                HANDLE(6,   DUELZONE2)
                HANDLE(8,   RPMALL)
                HANDLE(10,  STORMWIND)
                HANDLE(11,  UNDERCITY)
                HANDLE(12,  DALARAN)
                HANDLE(30,  GURUBASHI)
                HANDLE(31,  TRIALS)
                HANDLE(32,  MAUL)
                HANDLE(40,  EXODAR)
                HANDLE(41,  ORGRIMMARN)
                HANDLE(42,  RAVENHOLDT)
                HANDLE(43,  UNDERCITYM)
                HANDLE(44,  IRONFORGE)
                HANDLE(100, CUNNING)
                HANDLE(101, FEROCITY)
                HANDLE(102, TENACITY)
                HANDLE(103, EXOTIC)
            }
        }
};

void AddSC_teleport_npc()
{
    new teleport_npc();
}
