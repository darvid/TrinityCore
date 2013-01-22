#include "ScriptPCH.h"


const int ALLIANCE_TITLES[15] = {
    1, 2, 3, 4, 5, 6, 7, 8, 9, 10, 11, 12, 13, 14, 126
};
const int HORDE_TITLES[15] = {
    15, 16, 17, 18, 19, 20, 21, 22, 23, 24, 25, 26, 27, 28, 127
};
const int HK_RANKS[15] = {
    100, 500, 1000, 2000, 4000, 5000, 6000, 8000, 10000, 15000,
    25000, 40000, 45000, 50000, 75000
};


class title_granter : public PlayerScript
{
    public:
        title_granter() : PlayerScript("title_granter") {}

        void OnPVPKill(Player* killer, Player* killed)
        {
            if (killer == NULL) return;

            uint32 hks = killer->GetUInt32Value(PLAYER_FIELD_LIFETIME_HONORABLE_KILLS);
            for (int rank=0; rank < 15; rank++)
            {
                if (hks >= HK_RANKS[rank])
                    killer->SetTitle(sCharTitlesStore.LookupEntry(
                        (killer->GetTeam() == ALLIANCE ? ALLIANCE_TITLES
                            : HORDE_TITLES)[rank]));
            }
        }
};

void AddSC_title_granter()
{
    new title_granter();
}