#include "ScriptPCH.h"

class parachute_loginscript : public PlayerScript
{
    public:
        parachute_loginscript() : PlayerScript("parachute_loginscript") { }

        void OnFirstLogin(Player* player)
        {
            player->CastSpell(player, 44795, true);
        }
};

void AddSC_parachute_loginscript()
{
    new parachute_loginscript();
}