using AeonScriptCore;
using System;
using System.Collections.Generic;
using System.Linq;
using System.Runtime.CompilerServices;
using System.Text;
using System.Threading.Tasks;

namespace AeonScripts
{
    public class Death :AeonBehaviour
    {
        private Entity respawnZone = null;
        private Entity player1 = null;
        private Entity player2 = null;
        RespawnZoneTrigger rst = null;
        private string currentCheckPointName = "";

        public override void OnCreate()
        {
            respawnZone = Entity.FindEntityByName("Abyss.Trigger");
            if (respawnZone != null)
            {
                rst = respawnZone.GetScript<RespawnZoneTrigger>();
            }
            player1 = Entity.FindEntityByName("Player 1");
            player2 = Entity.FindEntityByName("player2");

        }
        public override void Update()
        {
            if (respawnZone == null)
            {
                respawnZone = Entity.FindEntityByName("Abyss.Trigger");
                return;
            }
            else if (rst == null)
            {
                rst = respawnZone.GetScript<RespawnZoneTrigger>();
                return;
            }

            //check per frame if theres a death 
            if (GameState.IsPaused) return;
            //if is player 1 spawn red balloon
            if (rst.player1Death)
            {
    
                EntityBuilder balloonBuilder = new EntityBuilder("Balloon");
                TransformBuilder transformBuilder = new TransformBuilder();
                BehaviourBuilder behaviourBuilder = new BehaviourBuilder();
                RenderBuilder renderBuilder = new RenderBuilder(
                    ".\\resources\\objects\\Balloon.bin",
                    ".\\resources\\textures\\red.dds");
                float orgX = player1.GetComponent<Transform>().Position.X;
                float randomX = (float)GameData.random.Next((int)(orgX - 10.0f), (int)(orgX + 10.0f));
                float orgZ = player1.GetComponent<Transform>().Position.Z;
                float randomZ = (float)GameData.random.Next((int)(orgZ - 10.0f), (int)(orgZ + 10.0f));
                behaviourBuilder.AddScript<DeathFly>();
                transformBuilder.SetPosition(new Vector3(randomX, player1.GetComponent<Transform>().Position.Y + 20.0f, randomZ));
                transformBuilder.SetRotation(new Vector3(0, 0, 0));
                transformBuilder.SetScale(new Vector3(3.0f, 3.0f, 3.0f));

                balloonBuilder.AddComponent(transformBuilder);
                balloonBuilder.AddComponent(renderBuilder);
                balloonBuilder.AddComponent(behaviourBuilder);

                Entity.Create(balloonBuilder);
                NativeLog.Log("P1 is dead");
            }
            else if (rst.player2Death)
            {
                EntityBuilder balloonBuilder = new EntityBuilder("Balloon");
                TransformBuilder transformBuilder = new TransformBuilder();
                BehaviourBuilder behaviourBuilder = new BehaviourBuilder();
                RenderBuilder renderBuilder = new RenderBuilder(
                    ".\\resources\\objects\\Balloon.bin",
                    ".\\resources\\textures\\blue.dds");
                float orgX = player2.GetComponent<Transform>().Position.X;
                float randomX = (float)GameData.random.Next((int)(orgX - 10.0f), (int)(orgX + 10.0f));
                float orgZ = player2.GetComponent<Transform>().Position.Z;
                float randomZ = (float)GameData.random.Next((int)(orgZ - 10.0f), (int)(orgZ + 10.0f));
                behaviourBuilder.AddScript<DeathFly>();
                transformBuilder.SetPosition(new Vector3(randomX, player1.GetComponent<Transform>().Position.Y + 20.0f, randomZ));
                transformBuilder.SetRotation(new Vector3(0, 0, 0));
                transformBuilder.SetScale(new Vector3(3.0f, 3.0f, 3.0f));

                balloonBuilder.AddComponent(transformBuilder);
                balloonBuilder.AddComponent(renderBuilder);
                balloonBuilder.AddComponent(behaviourBuilder);

                Entity.Create(balloonBuilder);
                NativeLog.Log("P2 is dead");
            }

            //if is player 2 spawn blue balloon

        }
    }
}
