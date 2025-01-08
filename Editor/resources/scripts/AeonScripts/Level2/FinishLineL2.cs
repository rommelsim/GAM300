using AeonScriptCore;
using System;
using System.Collections.Generic;
using System.Linq;
using System.Runtime.CompilerServices;
using System.Text;
using System.Threading.Tasks;

namespace AeonScripts.Level2
{
    public class FinishlineL2 : AeonBehaviour
    {
        /**
         * Check who crosses the finish line 
            - Account for Blue then red lose 
            - Account for Red then blue win 
            - Account for Draw ?
         * Add amount of points to the winner player 
         * Then confettis for winner   
         *////
        //Variables 
        private Entity finishTracker = null;
        public FinishTrackL2 playerWin = null;
        private MoveCharacter player = null;
        public override void OnCreate()
        {
            finishTracker = Entity.FindEntityByName("FinishTrack");
            if (finishTracker != null)
            {
                playerWin = finishTracker.GetScript<FinishTrackL2>();
            }
        }
        public override void OnTriggerEnter(uint triggerEntityID)
        {
            Entity ent = Entity.GetEntity(triggerEntityID);
            if (ent.Name == "Player 1" || ent.Name == "player2")
            {
                 player = ent.GetScript<MoveCharacter>();
            }
            FinishTrackL2 point = Entity.FindEntityByName("FinishTrack").GetScript<FinishTrackL2>();

            if (player != null)
            {
                if (player.isPlayerOne && ent.Name == "Box")
                {
                    GameData.P1_Score += point.bonusPoints;
                    playerWin.player1win = true;
                    GameData.RedWon = true;

                }
                else if (!player.isPlayerOne && ent.Name == "Box")
                {
                    GameData.P2_Score += point.bonusPoints;
                    playerWin.player2win = true;
                    GameData.BlueWon = true;
                }
            }

            NativeLog.Log("Entity got triggered: {0}", ent.Name);
        }

      
    }
}
