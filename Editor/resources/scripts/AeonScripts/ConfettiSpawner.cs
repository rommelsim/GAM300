using AeonScriptCore;
using System;
using System.Collections.Generic;
using System.Linq;
using System.Runtime.CompilerServices;
using System.Text;
using System.Threading.Tasks;

namespace AeonScripts
{
    public class ConfettiSpawner : AeonBehaviour
    {
        // manipulate transform X = random, Y = 120, Z = 25
        // manipulate euler angle Z = random
        // manipulate scale X = 2, Y = 4, Z = 0 (can randomise Y for length)

        private int randomTransX = 0;
        private int randomEulerZ = 0;
        private int randomScaleY = 0;

        public int confettiNum = 50;

        private List<string> confettiList = new List<string> {
            "PlatformRed_initialShadingGroup_BaseColor.1001.dds",
            "Platform_initialShadingGroup_BaseColor.1001.dds",
            "PlatformWhite_initialShadingGroup_BaseColor.1001.dds"
        };

        public override void OnCreate()
        {
            for (int i = 0; i < confettiNum; i++)
            {
                SpawnConfetti();
            }
        }

        private void SpawnConfetti()
        {
            randomTransX = GameData.random.Next(-50, 84);
            randomEulerZ = GameData.random.Next(10, 170);
            randomScaleY = GameData.random.Next(1, 5);

            int randomColor = GameData.random.Next(confettiList.Count);
            string randomConfetti = confettiList[randomColor];

            EntityBuilder confettiBuilder = new EntityBuilder("Confetti");
            TransformBuilder transformBuilder = new TransformBuilder();
            BehaviourBuilder behaviourBuilder = new BehaviourBuilder();
            RenderBuilder renderBuilder = new RenderBuilder(
                ".\\resources\\objects\\PointBox.bin",
                ".\\resources\\textures\\" + randomConfetti);

            behaviourBuilder.AddScript<ConfettiSplash>();
            //transformBuilder.SetPosition(new Vector3(randomTransX, 120f, 25f));

            // default
            if (GameData.RedWon && !GrandWinnerScreen.grandWinner)
            {
                transformBuilder.SetPosition(new Vector3(-16f, 17f, 25f));
            }
            else if (GameData.BlueWon && !GrandWinnerScreen.grandWinner)
            {
                transformBuilder.SetPosition(new Vector3(18f, 17f, 25f));
            }
            else if ((!GameData.RedWon && !GameData.BlueWon) && !GrandWinnerScreen.grandWinner)
            {
                transformBuilder.SetPosition(new Vector3(0f, 17f, 25f));
            }
            else if (GrandWinnerScreen.grandWinner)
            {
                transformBuilder.SetPosition(new Vector3(18f, 17f, 40f));
            }

            transformBuilder.SetRotation(new Vector3(0, 0, randomEulerZ));
            transformBuilder.SetScale(new Vector3(2, randomScaleY, 0));

            confettiBuilder.AddComponent(transformBuilder);
            confettiBuilder.AddComponent(renderBuilder);
            confettiBuilder.AddComponent(behaviourBuilder);

            Entity.Create(confettiBuilder);
        }
    }
}
