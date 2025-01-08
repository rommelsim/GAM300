using AeonScriptCore;
using System.Collections.Generic;

namespace AeonScripts.Powerups
{
    public class WeightCollide : AeonBehaviour
    {
        public WeightPowerUp owner = null;

        private Transform transform = null;
        private AudioComponent audioComp = null;
        private bool hasRecover = false;
        //private bool hasWeightAnimStarted = false;
        //private bool isEndOfWeightAnim = false;

        private List<StunPlayer> stunPlayers = null;

        public override void OnCreate()
        {
            transform = entity.GetComponent<Transform>();
            audioComp = entity.GetComponent<AudioComponent>();
            stunPlayers = new List<StunPlayer>();
        }

        public override void OnTriggerEnter(uint triggerEntityID)
        {
            Entity otherEnt = Entity.GetEntity(triggerEntityID);
            if (otherEnt == null) return;

            MoveCharacter moveScript = otherEnt.GetScript<MoveCharacter>();
            if (moveScript == null) return;

            bool foundDuplicate = false;

            foreach (var player in stunPlayers)
            {
                if (player.moveScript.entity.Id == moveScript.entity.Id)
                {
                    foundDuplicate = true;
                }
            }

            if (!foundDuplicate)
            {
                AddPlayerToStunList(moveScript);
            }

            //NativeLog.Log("WeightCollider - Trigger {0}", otherEnt.Name);
            //if (otherEnt != null && otherEnt.Name.Contains("Ground.") && hasWeightAnimStarted)
            //{
            //    NativeLog.Log("Entered ground");
            //    isEndOfWeightAnim = true;
            //    Entity.Destroy(otherEnt);
            //}
        }

        public override void OnTriggerExit(uint triggerEntityID)
        {
            Entity otherEnt = Entity.GetEntity(triggerEntityID);
            if (otherEnt == null) return;

            MoveCharacter moveScript = otherEnt.GetScript<MoveCharacter>();
            if (moveScript == null) return;

            bool foundDuplicate = false;

            foreach (var player in stunPlayers)
            {
                if (player.moveScript.entity.Id == moveScript.entity.Id)
                {
                    foundDuplicate = true;
                }
            }

            if (!foundDuplicate)
            {
                AddPlayerToStunList(moveScript);
            }
            
            //NativeLog.Log("WeightCollider - Trigger {0}", otherEnt.Name);
            //if (otherEnt != null && otherEnt.Name.Contains("Ground.") && hasWeightAnimStarted && !isEndOfWeightAnim)
            //{
            //    NativeLog.Log("Exited ground");
            //    isEndOfWeightAnim = true;
            //    Entity.Destroy(otherEnt);
            //}
        }

        public override void Update()
        {
            if (GameState.IsPaused) return;
            // List of indices to remove from stunPlayers
            List<int> removeList = new List<int>();

            for (int i = 0; i < stunPlayers.Count; i++)
            {
                StunPlayer stun = stunPlayers[i];

                if (stun.stunDuration < 0)
                {
                    removeList.Add(i);
                    stun.moveScript.GetTransform().Scale = stun.oldScale;
                    stun.moveScript.IsScriptActive = true;
                }

                stun.stunDuration -= Time.DeltaTime;
                if (audioComp != null && !hasRecover)
                {
                    audioComp.SoundFilepath = ".\\resources\\audios\\Hammer_Unflatten.wav";
                    audioComp.Volume = 0.6f;
                    audioComp.ChannelType = Audio.ChannelType.SFX;
                    //NativeLog.LogWarn("Did it play Unflatten sound");
                    audioComp.PlaySound();
                    hasRecover = true;
                }
            }

            for (int i = removeList.Count - 1; i >= 0; i--)
            {
                stunPlayers.RemoveAt(i);
            }

            //if (owner != null && isEndOfWeightAnim)
            //{
            //    transform.Position = new Vector3(0, 1000, 0);
            //    owner.EndOfWeightAnim();
            //    isEndOfWeightAnim = false;
            //    hasWeightAnimStarted = false;
            //}
        }

        public void SetLocation(Vector3 location)
        {
            transform.Position = location;
        }

        public Transform GetTransform()
        {
            return transform;
        }

        private void AddPlayerToStunList(MoveCharacter move)
        {
            move.IsScriptActive = false;
            move.DropPickable();

            stunPlayers.Add(new StunPlayer(move));
            Vector3 oldScale = move.PlayerOgScale;
            move.GetTransform().Scale = new Vector3(oldScale.X * 2, oldScale.Y /2, oldScale.Y);
            move.PlayHitByWeight();
        }

        public void PlayCrashingSound()
        {
            if (audioComp != null)
            {
                audioComp.SoundFilepath = ".\\resources\\audios\\SFX Weight.wav";
                audioComp.ChannelType = Audio.ChannelType.SFX;
                audioComp.Volume = 0.5f;
                audioComp.PlaySound();
            }
        }

        private class StunPlayer
        {
            public MoveCharacter moveScript;
            public float stunDuration;
            public Vector3 oldScale;

            public StunPlayer(MoveCharacter moveScript)
            {
                this.moveScript = moveScript;
                this.oldScale = moveScript.PlayerOgScale;
                this.stunDuration = 2.0f;
            }
        }
    }
}
