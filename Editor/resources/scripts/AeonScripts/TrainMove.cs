using AeonScriptCore;
using System;
using System.Collections.Generic;
using System.Linq;
using System.Text;
using System.Threading.Tasks;

namespace AeonScripts
{
    public class TrainMove : AeonBehaviour
    {
        //public float TrainXTransform;
        //public float TrainYTransform;
        //public float TrainZTransform;

        private Transform TrainNewTransform = null;
        private Transform TrainOldTransform = null;

        public float TrainResetLocation = 0.0f;

        public MoveCharacter player1MoveScript = null;
        public MoveCharacter player2MoveScript = null;


        public Transform playerTransform = null;

        private bool shouldFlatten = false;
        private bool shouldP2Flatten = false;

        public float stunDuration = 0.0f;
        public float p2StunDuration = 0.0f;
        public float TrainSpeed = 0.0f;

        private float TrainMoveTimer = 0.0f;
      
        private Vector3 Player1OldScale = Vector3.ZERO;
        private Vector3 Player2OldScale = Vector3.ZERO;
        private Vector3 TargetLocation = Vector3.ZERO;
        private Vector3 newPosition = Vector3.ZERO;


        public override void OnCreate()
        {
            TrainOldTransform = entity.GetComponent<Transform>();
            TargetLocation = TrainOldTransform.Position + new Vector3(0, 0, -100.0f);

        }

        public override void OnTriggerEnter(uint otherEntityID)
        {
            Entity player = Entity.GetEntity(otherEntityID);
            if (player == null) return;

            MoveCharacter localMoveScript = player.GetScript<MoveCharacter>();
            if (localMoveScript == null) return;
            
            playerTransform = player.GetComponent<Transform>();

            if (localMoveScript.isPlayerOne)
            {
                player1MoveScript = localMoveScript;
                shouldFlatten = true;
                stunDuration = 2.0f;
            }
            else
            {
                player2MoveScript = localMoveScript;
                shouldP2Flatten = true;
                p2StunDuration = 2.0f;
            }
        }


        public override void Update()
        {
            TrainMoveTimer += Time.DeltaTime;
            Vector3 trainPos = TrainOldTransform.Position;

            NativeLog.Log("${0}", TargetLocation);
            if (TrainMoveTimer >= 4.0f)
            {
                trainPos.Z += Time.DeltaTime * TrainSpeed; // multiply movement speed by delta time
                if (trainPos.Z >= TrainResetLocation)
                {
                    trainPos.Z = -417.3f;
                    TrainOldTransform.Position = trainPos;
                }
                TrainOldTransform.Position = trainPos; //Vector3.Lerp(TrainOldTransform.Position, TargetLocation, Time.DeltaTime);

            }
            



            if (player1MoveScript != null)
            {
                if (shouldFlatten)
                {
                    Player1OldScale = player1MoveScript.PlayerOgScale;
                    player1MoveScript.GetTransform().Scale = new Vector3(Player1OldScale.X * 2, Player1OldScale.Y / 2, Player1OldScale.Z);
                    stunDuration -= Time.DeltaTime;
                    player1MoveScript.IsScriptActive = false;
                }

                if (stunDuration <= 0.0f)
                {
                    player1MoveScript.GetTransform().Scale = Player1OldScale;
                    player1MoveScript.IsScriptActive = true;
                    shouldFlatten = false;
                    player1MoveScript = null;
                }
            }

            if (player2MoveScript != null)
            {
                if (shouldP2Flatten)
                {
                    Player2OldScale = player2MoveScript.PlayerOgScale;
                    player2MoveScript.GetTransform().Scale = new Vector3(Player2OldScale.X * 2, Player2OldScale.Y / 2, Player2OldScale.Z);
                    p2StunDuration -= Time.DeltaTime;
                    player2MoveScript.IsScriptActive = false;
                }

                if (p2StunDuration <= 0.0f)
                {
                    player2MoveScript.GetTransform().Scale = Player2OldScale;
                    player2MoveScript.IsScriptActive = true;
                    shouldP2Flatten = false;
                    player2MoveScript = null;

                }
            }

        }
    }
}
