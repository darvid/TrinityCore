#include "ScriptPCH.h"

class duel_reset : public PlayerScript
{
    public:
        duel_reset() : PlayerScript("duel_reset") { }

        void OnDuelStart(Player* p1, Player* p2)
        {
        }

        void OnDuelEnd(Player* p1, Player* p2, DuelCompleteType)
        {
            if (p1->GetAreaId() != 441 && p1->GetAreaId() != 2401) return;
            p1->SetHealth(p1->GetMaxHealth());
            p1->SetPower(POWER_MANA, p1->GetMaxPower(POWER_MANA));
            p1->RemoveAllSpellCooldown();
            p1->RemoveAura(41425);
            p1->RemoveAura(25771);
            p1->RemoveAura(57724);
            p1->RemoveAura(57723);
            p1->RemoveAura(66233);
            p1->RemoveAura(11196);

            p2->SetHealth(p2->GetMaxHealth());
            p2->SetPower(POWER_MANA, p2->GetMaxPower(POWER_MANA));
            p2->RemoveAllSpellCooldown();
            p2->RemoveAura(41425);
            p2->RemoveAura(25771);
            p2->RemoveAura(57724);
            p2->RemoveAura(57723);
            p2->RemoveAura(66233);
            p2->RemoveAura(11196);
        }
};

void AddSC_duel_reset()
{
    new duel_reset();
}