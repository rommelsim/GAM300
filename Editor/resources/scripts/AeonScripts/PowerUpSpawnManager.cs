using AeonScriptCore;
using System;
using System.Collections.Generic;
using System.Linq;
using System.Text;
using System.Threading.Tasks;

namespace AeonScripts
{
    public class PowerUpSpawnManager : AeonBehaviour
    {
        public bool part2Start = false;
        public bool spawnInAreaB = false;

        // Since not accessed by other scripts, make them private
        private List<Entity> areaAGround = null;
        private List<Entity> areaBGround = null;
        private List<Entity> spawnedPowerupList = null; // keep track of all the powerups spawned in the game

        public Entity checkPoint = null;
        private Checkpoint checkpoint2Script = null;

        private bool checkPointE = false;

        private int availablePowerUp = 0;

        private int entityListNum;
        public override void OnCreate()
        {
            //areaAGround.Clear();
            checkPoint = Entity.FindEntityByName("Checkpoint2");
            checkpoint2Script = checkPoint.GetScript<Checkpoint>();

            areaAGround = new List<Entity>();
            areaBGround = new List<Entity>();
            spawnedPowerupList = new List<Entity>();
            for (int i = 0; i <= 5; i++)
            {
                for (int j = 0; j <= 5; j++)
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
                    Entity ent = Entity.FindEntityByName($"Ground.C{i}{j}");
                    if (ent != null)
                    {
                        areaBGround.Add(ent);

                    }
                }
            }

            NativeLog.Log("list has {0}", areaAGround.Count);
            NativeLog.Log("list has {0}", areaBGround.Count);
            //LevelGround.Add()
        }

        public override void Update()
        {
            //NativeLog.Log("current Checkpoint is, {0}", checkPoint.GetScript<Checkpoint>().checkPointEnabled);
            if (checkpoint2Script.checkPointEnabled && !checkPointE)
            {
                part2Start = true;
                checkPointE = true;
            }
            if (part2Start)
            {
                // -- We want to destroy all the powerups spawned in Area A
                foreach(Entity entity in spawnedPowerupList)
                {
                    Entity.Destroy(entity);
                    availablePowerUp--;
                }
                spawnedPowerupList.Clear();

                spawnInAreaB = true;
                part2Start = false;
            }

            if (!spawnInAreaB)
            {
                entityListNum = areaAGround.Count;
            }
            else if (spawnInAreaB)
            {
                
                entityListNum = areaBGround.Count;

            }


            if (!spawnInAreaB)
            {
                if (availablePowerUp < 6)
                {
                    SpawnPowerUp();
                }
                else
                {
                    return;
                }
            }
            else
            {
                if (availablePowerUp < 10)
                {
                    SpawnPowerUp();
                }
                else
                {
                    return;
                }
            }
            
        }

        // Intended to be called from CollectPowerUp through it's PowerUpSpawnManager variable instance. When the collect power up is destroyed, it calls this function
        public void CollectPowerUpDestroyed(CollectPowerUp powerUpScript)
        {
            availablePowerUp--; // "give back" the powerup count
            spawnedPowerupList.Remove(powerUpScript.entity); // remove the destroyed powerup from the spawned list

            // Return the tile that the powerup was on, back to the available tile list to spawn on
            if (!spawnInAreaB)
            {
                areaAGround.Add(powerUpScript.groundTile);
            }
            else
            {
                areaBGround.Add(powerUpScript.groundTile);
            }
        }

        private void SpawnPowerUp()
        {
            CreatePowerup();
            availablePowerUp++;
        }

        private void CreatePowerup()
        {
            Entity spawnedGround = null;
            Vector3 randomLoc = new Vector3();
            if (!spawnInAreaB)
            {
                int randomNum = GameData.random.Next(0, entityListNum);
                spawnedGround = areaAGround[randomNum];

                // Check that the entity is alive
                while (!Entity.IsAlive(spawnedGround))
                {
                    areaAGround.Remove(spawnedGround); // since not alive, need to remove from the ground list
                    // now, must reroll!
                    entityListNum = areaAGround.Count;
                    randomNum = GameData.random.Next(0, entityListNum);
                    spawnedGround = areaAGround[randomNum];
                }

                randomLoc = spawnedGround.GetComponent<Transform>().Position;
                areaAGround.Remove(spawnedGround);
            }
            else if (spawnInAreaB)
            {
                int randomNum = GameData.random.Next(0, entityListNum);
                spawnedGround = areaBGround[randomNum];

                // Check that the entity is alive
                while (!Entity.IsAlive(spawnedGround))
                {
                    areaBGround.Remove(spawnedGround); // since not alive, need to remove from the ground list
                    // now, must reroll!
                    entityListNum = areaBGround.Count;
                    randomNum = GameData.random.Next(0, entityListNum);
                    spawnedGround = areaBGround[randomNum];
                }

                randomLoc = spawnedGround.GetComponent<Transform>().Position;
                areaBGround.Remove(spawnedGround);
            }


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
                new Vector3(0, 12, 0));
            transformBuilder.SetScale(new Vector3(7.0f, 7.0f, 7.0f));

            colliderBuilder.SetShape(new ColliderBuilder.CubeShapeBuilder().SetHalfSize(new Vector3(3,3,3)));


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
                collectPowerUp.isLevel2 = false;
                collectPowerUp.powerUpSpawnManager = this;
                collectPowerUp.groundTile = spawnedGround;
            });
        }

    }


}
