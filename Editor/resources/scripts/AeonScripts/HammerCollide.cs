using AeonScriptCore;
using AeonScriptCore.Physics;

namespace AeonScripts
{
    public class HammerCollide : AeonBehaviour
    {
        private AudioComponent audioComp = null;

        public Entity owner = null;
        [HideFromEditor]
        public bool shouldStun = false; // HammerPowerUp tells HammerCollide that it should stun!

        private float stunDuration = 0.0f;
        private bool isPlayerStunned = false;
        
        private Collider colliderComp = null;

        private MoveCharacter otherPlayer = null;
        private Vector3 otherPlayerOldScale = Vector3.ZERO;

        public override void OnCreate()
        {
            audioComp = entity.GetComponent<AudioComponent>();
            colliderComp = entity.GetComponent<Collider>();
        }

        public override void OnTriggerEnter(uint otherEntityID)
        {
            Entity player2 = Entity.GetEntity(otherEntityID);
            NativeLog.LogWarn("entity {0} collided with hammer trigger", player2.Name);

            if (player2 != null)
            {
                MoveCharacter move = player2.GetScript<MoveCharacter>();
                if (move != null /*&& owner != null && owner.Id != player2.Id*/)
                {
                    otherPlayer = move;
                    NativeLog.LogError("{0} has been detected inside hammer's trigger box", player2.Name);
                }
            }
        }

        public override void OnTriggerExit(uint triggerEntityID)
        {
            Entity player2 = Entity.GetEntity(triggerEntityID);
            if (player2 == null) return;

            MoveCharacter move = player2.GetScript<MoveCharacter>();
            if (move == null) return;

            // if player that exited the hammer's trigger box is the same as otherPlayer we detected, true!
            if (otherPlayer != null && otherPlayer.entity.Id == triggerEntityID && !isPlayerStunned)
            {
                NativeLog.LogError("HammerCollide: otherPlayer exited trigger");
                otherPlayer = null;
            }
        }

        public override void Update()
        {
            if (GameState.IsPaused) return;
            // if otherPlayer is not null, that means they are still inside the hammer's detection box
            if (otherPlayer != null)
            {
                // if other player is still detected, we stun the player
                if (!isPlayerStunned && shouldStun)
                {
                    otherPlayer.IsScriptActive = false;
                    isPlayerStunned = true;
                    shouldStun = false;

                    otherPlayer.DropPickable();

                    stunDuration = 2.0f;
                    otherPlayerOldScale = otherPlayer.PlayerOgScale;
                    otherPlayer.GetTransform().Scale = new Vector3(otherPlayerOldScale.X * 2,1, otherPlayerOldScale.Z);
                    var colliderShape = otherPlayer.GetCollider().GetColliderShape();
                    CubeColliderShape cubeCollider = (CubeColliderShape)colliderShape;
                    cubeCollider.HalfSize = new Vector3(5f, 0.5f, 4f);
                    otherPlayer.GetCollider().Offset = new Vector3(0, -3.05f, 0);
                    PlayHitSound();

                    otherPlayer.PlayHitByHammer();
                }

                if (isPlayerStunned)
                {
                    stunDuration -= Time.DeltaTime;
                    NativeLog.LogWarn("should stun running");

                    if (stunDuration < 0.0f)
                    {
                        NativeLog.LogWarn("Player should be NOT stunned");
                        stunDuration = 0.0f;

                        if (otherPlayer != null)
                        {
                            otherPlayer.GetTransform().Scale = otherPlayerOldScale;
                            otherPlayer.IsScriptActive = true;
                            var colliderShape = otherPlayer.GetCollider().GetColliderShape();
                            CubeColliderShape cubeCollider = (CubeColliderShape)colliderShape;
                            cubeCollider.HalfSize = new Vector3(2.5f, 2.7f, 4f);
                            otherPlayer.GetCollider().Offset = new Vector3(0, -0.5f, 0);
                            PlayUnflattenSound();
                        }
                        otherPlayer = null;

                        isPlayerStunned = false;
                        //isPlayerRecovered = true;
                        shouldStun = false;
                        owner = null;
                    }
                }

                //if (isPlayerRecovered)
                //{
                //    if (otherPlayer != null && otherPlayer.isPlayerOne)
                //    {
                //        PlayPlayer1Noise();
                //        isPlayerRecovered = false;
                //        owner = null;
                //    }
                //    else
                //    {
                //        PlayPlayer2Noise();
                //        isPlayerRecovered = false;
                //        owner = null;
                //    }
                //}
            }
        }

        public void PlayHitSound()
        {
            if (audioComp != null)
            {
                //int randNum = GameData.random.Next(1, 8);
                audioComp.SoundFilepath = ".\\resources\\audios\\Hammer_Flatten.wav";
                audioComp.Volume = 0.6f;
                audioComp.ChannelType = Audio.ChannelType.SFX;
                //NativeLog.LogWarn("Did it play Hit sound");
                audioComp.PlaySound();
            }
        }

        public void PlayMissSound()
        {
            if (audioComp != null)
            {
                int randNum = GameData.random.Next(1, 6);
                audioComp.SoundFilepath = $".\\resources\\audios\\Hammer_Miss_0{randNum}.wav";
                audioComp.Volume = 0.6f;
                audioComp.ChannelType = Audio.ChannelType.SFX;
                //NativeLog.LogWarn("Did it play Miss sound");
                audioComp.PlaySound();
            }
        }

        public void PlayUnflattenSound()
        {
            if (audioComp != null)
            {
                //int randNum = GameData.random.Next(1, 6);
                audioComp.SoundFilepath = ".\\resources\\audios\\Hammer_Unflatten.wav";
                audioComp.Volume = 0.6f;
                audioComp.ChannelType = Audio.ChannelType.SFX;
                //NativeLog.LogWarn("Did it play Unflatten sound");
                audioComp.PlaySound();
            }
        }

        public bool WasOtherPlayerStunned()
        {
            return isPlayerStunned;
        }

        public void SetCollisionOffset(Vector3 offset)
        {
            if (colliderComp == null) return;

            colliderComp.Offset = offset;
        }
    }
}
