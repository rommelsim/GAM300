using AeonScriptCore;
using System;
using System.Collections.Generic;
using System.Linq;
using System.Text;
using System.Threading.Tasks;

namespace AeonScripts
{
    public class CollectPowerUp : AeonBehaviour
    {
        public string powerType;
        public bool isPlacedDownbanana = false;

        //public bool IsScriptActive = true;
        private Transform transform = null;
        private RigidBody rbComp = null;
        private Collider colliderComp = null;
        //private bool display = false;
        //private int powerUpcount = 0;
        //private Vector3 _baseRotation;
        public Transform parentTrans = null;

        public Entity groundTile = null; // the tile that this power up spawned on
        public PowerUpSpawnManager powerUpSpawnManager = null;
        public Level2PowerUpSpawnManager powerUpSpawnManager2 = null;

        public bool isLevel2;
        public bool isLevel2AreaA;
        public bool isLevel2AreaB;
        public bool isLevel2AreaC;


        public override void OnCreate()
        {
            //powerType = powerType.ToLower();
            transform = entity.GetComponent<Transform>();
            rbComp = entity.GetComponent<RigidBody>();
            colliderComp = entity.GetComponent<Collider>();

            if (transform != null)
            {
                // _baseRotation = transform.Rotation;
                NativeLog.Log("PowerUp onCreate");
            }
        }

        public override void Update()
        {
            if (isLevel2)
            {
                if (parentTrans != null)
                {
                    entity.GetComponent<Transform>().Position = parentTrans.Position + new Vector3(0, 10, 0);
                    if (entity.GetComponent<Transform>().Position.Y < -150f)
                    {
                        if (powerUpSpawnManager2 != null)
                        {
                            Entity.Destroy(this.entity);
                        }

                    }
                    else
                    {
                        return;
                    }
                }

            }
            else
            {
                return;
            }

        }


        public override void OnTriggerEnter(uint otherEntityID)
        {
            Entity otherEnt = Entity.GetEntity(otherEntityID);
            if (otherEnt == null) return;

            MoveCharacter Player = otherEnt.GetScript<MoveCharacter>();
            if (Player == null) return;

            PlayerInventory count = otherEnt.GetScript<PlayerInventory>();
            if (count == null) return;


            NativeLog.Log(" otherentity :${0}", otherEnt);
            //BananaPowerUp bananaPower = otherEnt.GetScript<BananaPowerUp>();


            //setting of type of power up and which one the players have collected here           
            if (count.HoldingPowerUp == false)
            {
                NativeLog.Log("powerup triggered");
                if (powerType == null)
                {
                    
                    if (Player.IsCarryingPickable())
                    {
                        int randomNum = /*availablePowerUp % 3;*/GameData.random.Next(0, 3);
                        //int randomNum = 0;
                        if (randomNum == 0)
                        {
                            powerType = "weight";
                            
                        }
                        else if (randomNum == 1)
                        {
                            powerType = "dash";
                            
                        }
                        else if (randomNum == 2)
                        {
                            powerType = "spin";
                            
                        }
                    }
                    else if (!Player.IsCarryingPickable())
                    {
                        int randomNum = /*availablePowerUp % 3;*/GameData.random.Next(0, 3);
                        if (randomNum == 0)
                        {
                            powerType = "rangeball";
                            
                        }
                        else if (randomNum == 1)
                        {
                            powerType = "star";
                            
                        }
                        else if (randomNum == 2)
                        {
                            powerType = "dash";
                            
                        }
                    }

                }

                if (!isLevel2)
                {
                    if (powerType.Equals("weight"))
                    {
                        count.PlayPickupSfx = true;
                        count.HoldingPowerUp = true;

                        count.weightCollected = true;
                        CreateWeight(Player.entity);
                        powerUpSpawnManager.CollectPowerUpDestroyed(this);
                        NativeLog.Log("weight collected");
                        
                        Entity.Destroy(entity);
                    }
                    else if (powerType.Equals("banana") && isPlacedDownbanana)
                    {
                        count.HoldingPowerUp = false;
                        Entity.Destroy(entity);

                    }
                    else if (powerType.Equals("rangeball"))
                    {
                        count.PlayPickupSfx = true;
                        count.HoldingPowerUp = true;
                        count.rangeBallCollected = true;
                        CreateBall(Player.entity);
                        powerUpSpawnManager.CollectPowerUpDestroyed(this);
                        
                        Entity.Destroy(entity);
                    }
                    else if (powerType.Equals("star"))
                    {
                        count.PlayPickupSfx = true;
                        count.HoldingPowerUp = true;
                        count.starCollected = true;
                        Create2X(Player.entity);
                        powerUpSpawnManager.CollectPowerUpDestroyed(this);
                        
                        Entity.Destroy(entity);
                    }
                    else if (powerType.Equals("dash"))
                    {
                        count.PlayPickupSfx = true;
                        count.HoldingPowerUp = true;
                        count.dashCollected = true;
                        CreateDash(Player.entity);
                        powerUpSpawnManager.CollectPowerUpDestroyed(this);
                        NativeLog.Log("dash collected");
                        
                        Entity.Destroy(entity);
                    }
                    else if (powerType.Equals("spin"))
                    {
                        count.PlayPickupSfx = true;
                        count.HoldingPowerUp = true;
                        count.spinnerCollected = true;
                        CreateSpinny(Player.entity);
                        powerUpSpawnManager.CollectPowerUpDestroyed(this);
                        
                        Entity.Destroy(entity);
                    }
                }
                else
                {
                    if (powerType.Equals("weight"))
                    {
                        count.PlayPickupSfx = true;
                        count.HoldingPowerUp = true;

                        count.weightCollected = true;
                        CreateWeight(Player.entity);
                        powerUpSpawnManager2.CollectPowerUpDestroyed(this);
                        NativeLog.Log("weight collected");
                        
                        Entity.Destroy(entity);
                    }
                    else if (powerType.Equals("banana") && isPlacedDownbanana)
                    {
                        count.HoldingPowerUp = false;
                        Entity.Destroy(entity);

                    }
                    else if (powerType.Equals("rangeball"))
                    {
                        count.PlayPickupSfx = true;
                        count.HoldingPowerUp = true;
                        count.rangeBallCollected = true;
                        CreateBall(Player.entity);
                        powerUpSpawnManager2.CollectPowerUpDestroyed(this);
                        
                        Entity.Destroy(entity);
                    }
                    else if (powerType.Equals("star"))
                    {
                        count.PlayPickupSfx = true;
                        count.HoldingPowerUp = true;
                        count.starCollected = true;
                        Create2X(Player.entity);
                        powerUpSpawnManager2.CollectPowerUpDestroyed(this);
                        
                        Entity.Destroy(entity);
                    }
                    else if (powerType.Equals("dash"))
                    {
                        count.PlayPickupSfx = true;
                        count.HoldingPowerUp = true;
                        count.dashCollected = true;
                        CreateDash(Player.entity);
                        powerUpSpawnManager2.CollectPowerUpDestroyed(this);
                        NativeLog.Log("dash collected");
                        Entity.Destroy(entity);
                    }
                    else if (powerType.Equals("spin"))
                    {
                        count.PlayPickupSfx = true;
                        count.HoldingPowerUp = true;
                        count.spinnerCollected = true;
                        CreateSpinny(Player.entity);
                        powerUpSpawnManager2.CollectPowerUpDestroyed(this);
                        Entity.Destroy(entity);
                    }
                }
                
            }

        }

        public void CreateWeight(Entity parentEnt)
        {
            EntityBuilder entBuilder = new EntityBuilder("WeightAnim");
            TransformBuilder transformBuilder = new TransformBuilder();
            BehaviourBuilder behaviourBuilder = new BehaviourBuilder();

            RenderBuilder renderBuilder = new RenderBuilder("./resources/objects/One_Ton_Weight.bin", "./resources/textures/one_ton.dds");

            transformBuilder.SetScale(new Vector3(7, 7, 7));

            behaviourBuilder.AddScript<PowerUpGetAnim>();

            entBuilder.AddComponent(transformBuilder);
            entBuilder.AddComponent(renderBuilder);
            entBuilder.AddComponent(behaviourBuilder);

            Entity.Create(entBuilder, (entity) =>
            {
                PowerUpGetAnim powerUpGetAnim = entity.GetScript<PowerUpGetAnim>();
                powerUpGetAnim.ownerEnt = Entity.FindEntityByName(parentEnt.Name);
                powerUpGetAnim.yValue = parentEnt.GetComponent<Transform>().Position.Y + 10f;
            });
        }
        public void CreateSpinny(Entity parentEnt)
        {
            EntityBuilder entBuilder = new EntityBuilder("SpinnyAnim");
            TransformBuilder transformBuilder = new TransformBuilder();
            BehaviourBuilder behaviourBuilder = new BehaviourBuilder();

            RenderBuilder renderBuilder = new RenderBuilder("./resources/objects/Spinner2.bin", "./resources/textures/SpinnyWhacky.dds");

            transformBuilder.SetScale(new Vector3(7, 7, 7));

            behaviourBuilder.AddScript<PowerUpGetAnim>();

            entBuilder.AddComponent(transformBuilder);
            entBuilder.AddComponent(renderBuilder);
            entBuilder.AddComponent(behaviourBuilder);

            Entity.Create(entBuilder, (entity) =>
            {
                PowerUpGetAnim powerUpGetAnim = entity.GetScript<PowerUpGetAnim>();
                powerUpGetAnim.ownerEnt = Entity.FindEntityByName(parentEnt.Name);
                powerUpGetAnim.yValue = parentEnt.GetComponent<Transform>().Position.Y + 10f;
            });

        }
        public void CreateBall(Entity parentEnt)
        {
            EntityBuilder entBuilder = new EntityBuilder("BallAnim");
            TransformBuilder transformBuilder = new TransformBuilder();
            BehaviourBuilder behaviourBuilder = new BehaviourBuilder();

            RenderBuilder renderBuilder = new RenderBuilder("./resources/objects/PowerUpEmptyBubble.bin", "./resources/textures/Ball.dds");

            transformBuilder.SetScale(new Vector3(1, 1, 1));

            behaviourBuilder.AddScript<PowerUpGetAnim>();

            entBuilder.AddComponent(transformBuilder);
            entBuilder.AddComponent(renderBuilder);
            entBuilder.AddComponent(behaviourBuilder);

            Entity.Create(entBuilder, (entity) =>
            {
                PowerUpGetAnim powerUpGetAnim = entity.GetScript<PowerUpGetAnim>();
                powerUpGetAnim.ownerEnt = Entity.FindEntityByName(parentEnt.Name);
                powerUpGetAnim.yValue = parentEnt.GetComponent<Transform>().Position.Y + 10f;
            });
        }
        public void Create2X(Entity parentEnt)
        {
            EntityBuilder entBuilder = new EntityBuilder("StarAnim");
            TransformBuilder transformBuilder = new TransformBuilder();
            BehaviourBuilder behaviourBuilder = new BehaviourBuilder();

            RenderBuilder renderBuilder = new RenderBuilder("./resources/objects/Star.bin", "./resources/textures/StarPowerUp.dds");

            transformBuilder.SetScale(new Vector3(7, 7, 7));

            behaviourBuilder.AddScript<PowerUpGetAnim>();

            entBuilder.AddComponent(transformBuilder);
            entBuilder.AddComponent(renderBuilder);
            entBuilder.AddComponent(behaviourBuilder);

            Entity.Create(entBuilder, (entity) =>
            {
                PowerUpGetAnim powerUpGetAnim = entity.GetScript<PowerUpGetAnim>();
                powerUpGetAnim.ownerEnt = Entity.FindEntityByName(parentEnt.Name);
                powerUpGetAnim.yValue = parentEnt.GetComponent<Transform>().Position.Y + 10f;
            });
        }
        public void CreateDash(Entity parentEnt)
        {
            EntityBuilder entBuilder = new EntityBuilder("DashAnim");
            TransformBuilder transformBuilder = new TransformBuilder();
            BehaviourBuilder behaviourBuilder = new BehaviourBuilder();

            RenderBuilder renderBuilder = new RenderBuilder("./resources/objects/Shoe.bin", "./resources/textures/Shoe.dds");

            transformBuilder.SetScale(new Vector3(7, 7, 7));

            behaviourBuilder.AddScript<PowerUpGetAnim>();

            entBuilder.AddComponent(transformBuilder);
            entBuilder.AddComponent(renderBuilder);
            entBuilder.AddComponent(behaviourBuilder);

            Entity.Create(entBuilder, (entity) =>
            {
                PowerUpGetAnim powerUpGetAnim = entity.GetScript<PowerUpGetAnim>();
                powerUpGetAnim.ownerEnt = Entity.FindEntityByName(parentEnt.Name);
                powerUpGetAnim.yValue = parentEnt.GetComponent<Transform>().Position.Y + 10f;
            });
        }

    }
}
