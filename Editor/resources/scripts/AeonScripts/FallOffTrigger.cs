using AeonScriptCore;
using System.Collections.Generic;
using System.Configuration;

namespace AeonScripts
{
    public class FallOffTrigger : AeonBehaviour
    {
        private AudioComponent respawSFX = null;


        private Entity checkpoint = null;
        //private string currentCheckPointName = "";
        //private static Vector3 _respawnPos = new Vector3(-1.8f, 5.5f, 0);
        //private static Vector3 _newRespawnPos = Vector3.ZERO;
        private static List<Entity> _respawningEntities = new List<Entity>();
        //private bool playerdieaudio = false;

        public Checkpoint checkpointScript = null;

        private Entity player1 = null;
        private Entity player2 = null;
        //private BoxWithPoint box;

        private float delay = 1;
        private float currentTimer = 0.0f;

        //public bool player1Death = false;
        //public bool player2Death = false;

        MoveCharacter moveChara = null;
        public override void OnCreate()
        {
            respawSFX = entity.GetComponent<AudioComponent>();
            checkpoint = Entity.FindEntityByName("Checkpoint2");
            player1 = Entity.FindEntityByName("Player 1");
            moveChara =player1.GetScript<MoveCharacter>();
            _respawningEntities.Clear();

        }
        public override void OnTriggerEnter(uint triggerEntityID)
        {
            Entity otherEnt = Entity.GetEntity(triggerEntityID);

            // when the player and the box drops, minus the player's score

            if (otherEnt != null)
            {
                NativeLog.Log("Abyss's trigger enter detected {0}", otherEnt.Name);
                MoveCharacter movey = otherEnt.GetScript<MoveCharacter>();

                if (movey != null)
                {
                    movey.PlayFallingSound();
                }
                
                _respawningEntities.Add(otherEnt);
            }

        }

        public override void OnTriggerExit(uint triggerEntityID)
        {
            Entity otherEnt = Entity.GetEntity(triggerEntityID);

            // when the player and the box drops, minus the player's score

            if (otherEnt != null)
            {
                NativeLog.Log("Abyss's trigger exit detected {0}", otherEnt.Name);

                bool foundEnt = false;
                foreach (Entity ent in _respawningEntities)
                {
                    if (ent.Id == triggerEntityID)
                        foundEnt = true;
                }

                if (!foundEnt)
                {
                    MoveCharacter movey = otherEnt.GetScript<MoveCharacter>();
                    if (movey != null)
                    {
                        movey.PlayFallingSound();
                    }
                }
                else
                {
                    _respawningEntities.Remove(otherEnt);
                }
            }
        }
   }
}