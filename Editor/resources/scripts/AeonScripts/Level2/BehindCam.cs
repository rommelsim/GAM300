using AeonScriptCore;
using System.Runtime.CompilerServices;
using static AeonScripts.GameData;

namespace AeonScripts.Level2
{
    public class BehindCam : AeonBehaviour
    {
        private Transform transform = null;

        private Entity camera = null;
        private CameraL2 camTransform = null;

        private bool activateTransformer1 = false;
        private bool activateTransformer2 = false;

        private bool activateTransformerBox = false;

        private Entity player1 = null;
        private Entity player2 = null;

        private Transform player1Trans = null;
        private Transform player2Trans = null;

        private Entity box = null;
        private Transform boxTrans = null;

        public override void OnCreate()
        {
            transform = entity.GetComponent<Transform>();
            camera = Entity.FindEntityByName("Camera");
            if (camera != null)
            {
                camTransform = camera.GetScript<CameraL2>();
            }

            player1 = Entity.FindEntityByName("Player 1");
            player2 = Entity.FindEntityByName("player2");

            player1Trans = player1.GetComponent<Transform>();
            player2Trans = player2.GetComponent<Transform>();

            box = Entity.FindEntityByName("Box");
            boxTrans = box.GetComponent<Transform>();
        }

        public override void Update()
        {
            if (GameState.IsPaused) return;
            transform.Position = new Vector3(transform.Position.X, transform.Position.Y, camTransform.CamPosition.Z + 40f);

            if (activateTransformer1)
            {
                player1Trans.Position = new Vector3(24f, -14f, -672f);
                activateTransformer1 = false;
            }

            if (activateTransformer2)
            {
                player2Trans.Position = new Vector3(24f, -14f, -672f);
                activateTransformer2 = false;
            }

            if (activateTransformerBox)
            {
                boxTrans.Position = new Vector3(24f, -14f, -672f);
                activateTransformerBox = false;
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
                    if (otherEnt.Name == "Player 1")
                    {
                        activateTransformer1 = true;
                    }
                    else if (otherEnt.Name == "player2")
                    {
                        activateTransformer2 = true;
                    }
                    
                }
                else
                {
                    if (otherEnt.Name == "Box")
                    {
                        activateTransformerBox = true;
                    }
                }
            }
        }

        public override void OnTriggerExit(uint triggerEntityID)
        {
            Entity otherEnt = Entity.GetEntity(triggerEntityID);
            if (otherEnt != null)
            {
                MoveCharacter playerHit = otherEnt.GetScript<MoveCharacter>();
                if (playerHit != null)
                {
                    if (otherEnt.Name == "Player 1")
                    {
                        activateTransformer1 = true;
                    }
                    else if (otherEnt.Name == "player2")
                    {
                        activateTransformer2 = true;
                    }
                }
                else
                {
                    if (otherEnt.Name == "Box")
                    {
                        activateTransformerBox = true;
                    }
                }
            }
        }
    }
}