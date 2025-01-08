using AeonScriptCore;
using System;
using System.Collections.Generic;
using System.Linq;
using System.Text;
using System.Threading.Tasks;

namespace AeonScripts.Level2
{
    /*
    1. Each player have its own checkpoint ()
    2. Enable checkpoint once they trigger next 
    3. Respawn to the correct checkpoint
 */
    public class CheckpointL2 : AeonBehaviour
    {
        private Entity player1 = null;
        private Entity player2 = null;
        /*MoveCharacter move1 = null;
        MoveCharacter move2 = null;*/

        private MoveCharacter player1MoveChar = null;
        private MoveCharacter player2MoveChar = null;

        private bool beginFollowing = false;

        private Transform transform = null;

        private Entity camera = null;
        private CameraL2 camTransform = null;

        public override void OnCreate()
        {
            player1 = Entity.FindEntityByName("Player 1");
            player2 = Entity.FindEntityByName("player2");

            player1MoveChar = player1.GetScript<MoveCharacter>();
            player2MoveChar = player2.GetScript<MoveCharacter>();

            transform = entity.GetComponent<Transform>();
            camera = Entity.FindEntityByName("Camera");
            if (camera != null)
            {
                camTransform = camera.GetScript<CameraL2>();

            }
        }
        public override void OnTriggerEnter(uint triggerEntityID)
        {
            Entity otherEnt = Entity.GetEntity(triggerEntityID);
            if (otherEnt != null)
            {
                MoveCharacter playerHit = otherEnt.GetScript<MoveCharacter>();
                if (playerHit != null)
                {
                    beginFollowing = true;

                    player1MoveChar.currentCheckpoint = entity.Name;
                    player2MoveChar.currentCheckpoint = entity.Name;
                    /*if (playerHit.isPlayerOne && this.entity.Name.Contains("Red"))
                    {
                        playerHit.currentCheckpoint = entity.Name;
                        NativeLog.Log("Player 1 " + playerHit.currentCheckpoint);

                    }
                    else if (!playerHit.isPlayerOne && this.entity.Name.Contains("Blue"))
                    {
                        playerHit.currentCheckpoint = entity.Name;
                        NativeLog.Log("Player 2 " + playerHit.currentCheckpoint);
                    }*/


                }
            }
        }

        public override void Update()
        {
            if (beginFollowing)
            {
                transform.Position = new Vector3(transform.Position.X, transform.Position.Y, camTransform.CamPosition.Z + 126f);
            }
        }
    }
}