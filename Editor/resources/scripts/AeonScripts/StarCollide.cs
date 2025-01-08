using AeonScriptCore;
using AeonScriptCore.Physics;

namespace AeonScripts
{
    public class StarCollide : AeonBehaviour
    {
        [HideFromEditor] public MoveCharacter thisPlayerMov = null;
        [HideFromEditor] public bool canStun = false;

        private float stunDuration = 0.0f;
        private bool isPlayerStunned = false;

        private AudioComponent audioComp = null;

        private MoveCharacter otherPlayer = null;
        private Vector3 otherPlayerOldScale = Vector3.ZERO;

        public override void OnCreate()
        {
            audioComp = entity.GetComponent<AudioComponent>();
        }

        public override void OnTriggerEnter(uint otherEntityID)
        {
            Entity player2 = Entity.GetEntity(otherEntityID);

            if (player2 != null)
            {
                if (thisPlayerMov != player2.GetScript<MoveCharacter>())
                {
                    MoveCharacter move = player2.GetScript<MoveCharacter>();

                    if (move != null)
                    {
                        otherPlayer = move;
                        if (!isPlayerStunned && canStun)
                        {
                            otherPlayer.IsScriptActive = false;
                            isPlayerStunned = true;
                            canStun = false;

                            otherPlayer.DropPickable();

                            stunDuration = 3.0f;
                            otherPlayerOldScale = otherPlayer.PlayerOgScale;
                            otherPlayer.GetTransform().Scale = new Vector3(otherPlayerOldScale.X * 2, 1, otherPlayerOldScale.Z);
                            var colliderShape = otherPlayer.GetCollider().GetColliderShape();
                            CubeColliderShape cubeCollider = (CubeColliderShape)colliderShape;
                            cubeCollider.HalfSize = new Vector3(5f, 0.5f, 4f);
                            otherPlayer.GetCollider().Offset = new Vector3(0, -3.05f, 0);
                            PlayFlattenSound();
                        }

                    }
                }
            }
        }

        public override void Update()
        {
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
                        var colliderShape = otherPlayer.GetCollider().GetColliderShape();
                        CubeColliderShape cubeCollider = (CubeColliderShape)colliderShape;
                        cubeCollider.HalfSize = new Vector3(2.5f, 2.7f, 4f);
                        otherPlayer.GetCollider().Offset = new Vector3(0, -0.5f, 0);
                        otherPlayer.IsScriptActive = true;
                        otherPlayer = null;
                        PlayUnflattenSound();
                    }

                    isPlayerStunned = false;
                    canStun = false;
                }
            }                
        }

        public void PlayFlattenSound()
        {
            if (audioComp != null)
            {
                audioComp.SoundFilepath = ".\\resources\\audios\\Hammer_Flatten.wav";
                audioComp.Volume = 0.6f;
                audioComp.ChannelType = Audio.ChannelType.SFX;
                //NativeLog.LogWarn("Did it play Hit sound");
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
    }
}
