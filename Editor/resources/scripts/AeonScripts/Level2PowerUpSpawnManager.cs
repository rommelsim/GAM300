using AeonScriptCore;
using System;
using System.Collections.Generic;
using System.Linq;
using System.Text;
using System.Threading.Tasks;

namespace AeonScripts
{
    public class Level2PowerUpSpawnManager : AeonBehaviour
    {
        private List<Entity> areaAGround = null;
        private List<Entity> areaBGround = null;
        private List<Entity> areaCGround = null;

        private List<Entity> spawnedPowerupList = null; // keep track of all the powerups spawned in the game

        public bool spawnInAreaA = false;
        public bool spawnInAreaB = false;
        public bool spawnInAreaC = false;

        private int availablePowerUpA = 0;
        private int availablePowerUpB = 0;
        private int availablePowerUpC = 0;

        int entityListNum;


        public override void OnCreate()
        {
            areaAGround = new List<Entity>();
            areaBGround = new List<Entity>();
            areaCGround = new List<Entity>();

            spawnedPowerupList = new List<Entity>();
            
            for (int i = 0; i <= 20; i++)
            {
                for (int j = 0; j <= 20; j++)
                {
                    Entity ent = Entity.FindEntityByName($"Ground.A{i}{j}");
                    if (ent != null)
                    {
                        areaAGround.Add(ent);

                    }
                }
            }

            for (int i = -5; i <= 20; i++)
            {
                for (int j = -5; j <= 20; j++)
                {
                    Entity ent = Entity.FindEntityByName($"Ground.B{i}{j}");
                    if (ent != null)
                    {
                        areaBGround.Add(ent);

                    }
                }
            }

            for (int i = -5; i <= 20; i++)
            {
                for (int j = -5; j <= 20; j++)
                {
                    Entity ent = Entity.FindEntityByName($"Ground.C{i}{j}");
                    if (ent != null)
                    {
                        areaCGround.Add(ent);

                    }
                }
            }

            NativeLog.Log("A list has {0}", areaAGround.Count);
            NativeLog.Log("B list has {0}", areaBGround.Count);
            NativeLog.Log("C list has {0}", areaCGround.Count);
        }

        public override void Update()
        {
            if (availablePowerUpA < 10)
            {
                CreatePowerup(areaAGround);
                availablePowerUpA++;
            }
            else
            {
                return;
            }

            if (availablePowerUpB < 15)
            {
                CreatePowerup(areaBGround);
                availablePowerUpB++;
            }
            else
            {
                return;
            }

            if (availablePowerUpC < 10)
            {
                CreatePowerup(areaCGround);
                availablePowerUpC++;

            }
            else
            {
                return;
            }

        }
        private void CreatePowerup(List<Entity> area)
        {
            Entity spawnedGround = null;
            List<Entity> usingList = new List<Entity>();
            Vector3 randomLoc = new Vector3();
            usingList = area;
            bool isA = false;
            bool isB = false;
            bool isC = false;

            entityListNum = usingList.Count;
            if (usingList == areaAGround)
            {
                //spawnInAreaA = true;
                //entityListNum = areaAGround.Count;
                isA = true;
            }
            else if (usingList == areaBGround) 
            {
                //spawnInAreaB = true;
                //entityListNum = areaBGround.Count;
                isB = true;
            }
            else if (usingList == areaCGround)
            {
                //spawnInAreaC = true;
                //entityListNum = areaCGround.Count;
                isC = true;
            }

            int randomNum = GameData.random.Next(0, entityListNum);
            spawnedGround = area[randomNum];
            randomLoc = spawnedGround.GetComponent<Transform>().Position;

            area.Remove(spawnedGround);

            EntityBuilder entBuilder = new EntityBuilder("PowerUp");
            TransformBuilder transformBuilder = new TransformBuilder();
            BehaviourBuilder behaviourBuilder = new BehaviourBuilder();
            ColliderBuilder colliderBuilder = new ColliderBuilder();
            RigidBodyBuilder rbBuilder = new RigidBodyBuilder();

            colliderBuilder.SetIsTrigger(true);
            rbBuilder.SetType(AeonScriptCore.Physics.RigidBodyType.DYNAMIC);
            rbBuilder.SetUseGravity(false);

            behaviourBuilder.AddScript<CollectPowerUp>();
            behaviourBuilder.AddScript<PowerUpSpin>();



            transformBuilder.SetPosition(randomLoc +
                new Vector3(0, 10, 0));
            transformBuilder.SetScale(new Vector3(7.0f, 7.0f, 7.0f));

            colliderBuilder.SetShape(new ColliderBuilder.CubeShapeBuilder().SetHalfSize(new Vector3(2, 2, 2)));


            RenderBuilder renderBuilder = new RenderBuilder("./resources/objects/PowerupCube.bin", "./resources/textures/powerCube.dds");

            entBuilder.AddComponent(transformBuilder);
            entBuilder.AddComponent(colliderBuilder);
            entBuilder.AddComponent(renderBuilder);
            entBuilder.AddComponent(behaviourBuilder);
            entBuilder.AddComponent(rbBuilder);

            Entity.Create(entBuilder, (entity) =>
            {
                spawnedPowerupList.Add(entity);
                CollectPowerUp collectPowerUp = entity.GetScript<CollectPowerUp>();
                collectPowerUp.isLevel2 = true;
                collectPowerUp.powerUpSpawnManager2 = this;
                collectPowerUp.parentTrans = spawnedGround.GetComponent<Transform>();
                if (isA)
                {
                    collectPowerUp.isLevel2AreaA = true;
                }
                else if (isB)
                {
                    collectPowerUp.isLevel2AreaB = true;
                }
                else if (isC)
                {
                    collectPowerUp.isLevel2AreaC = true;
                }
                collectPowerUp.groundTile = spawnedGround;
            });
        }

        public void CollectPowerUpDestroyed(CollectPowerUp powerUpScript)
        {
            // "give back" the powerup count
            if (powerUpScript.isLevel2AreaA)
            {
                //availablePowerUpA--;
                spawnedPowerupList.Remove(powerUpScript.entity);
                areaAGround.Add(powerUpScript.groundTile);
            }
            else if (powerUpScript.isLevel2AreaB)
            {
                availablePowerUpB--;
                spawnedPowerupList.Remove(powerUpScript.entity);
                areaBGround.Add(powerUpScript.groundTile);
            }
            else if (powerUpScript.isLevel2AreaC)
            {
                availablePowerUpC--;
                spawnedPowerupList.Remove(powerUpScript.entity);
                areaCGround.Add(powerUpScript.groundTile);
            }
            
             // remove the destroyed powerup from the spawned list

            // Return the tile that the powerup was on, back to the available tile list to spawn on
        }

    }
}
