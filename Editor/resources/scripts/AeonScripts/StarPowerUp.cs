using AeonScriptCore;

namespace AeonScripts
{
    public class StarPowerUp : AeonBehaviour
    {
        private PlayerInventory playerInv = null;
        private Transform playerTrans =  null;
        private MoveCharacter playerMov = null;

        private AudioComponent audioComp = null;

        private Vector3 starModeOriginalPos;
        
        public string starModeName; // Name for star mode GO
        private Entity starMode = null;
        private Transform starModeTrans = null;
        private StarCollide starCollide = null;

        private bool upSizing = false;
        private bool downSizing = false;
        private bool starActive = false;

        private Vector3 targetPos = new Vector3();
        private Vector3 targetScale = new Vector3();

        private float animTime = 0.2f;
        private float starAnimTimer = 0.2f;
        private int starAnimCounter = 0;

        private float originalSpeed;

        private float powerTimer = 3f;

        public override void OnCreate()
        {
            playerTrans = entity.GetComponent<Transform>();
            playerInv = entity.GetScript<PlayerInventory>();
            playerMov = entity.GetScript<MoveCharacter>();
            audioComp = entity.GetComponent<AudioComponent>();

            starMode = Entity.FindEntityByName(starModeName);
            starCollide = starMode.GetScript<StarCollide>();
            if (starMode != null)
            {
                starModeTrans = starMode.GetComponent<Transform>();
                starCollide.thisPlayerMov = playerMov;
            }
            starModeOriginalPos = starModeTrans.Position;
        }

        void ActivateStar()
        {
            upSizing = true;
            
            starModeTrans.Scale = new Vector3(11f, 11f, 11f);
            starModeTrans.Position = playerTrans.Position;

            targetScale = new Vector3(14f, 14f, 14f);
            targetPos = new Vector3(0, 2f, 0);

            starCollide.canStun = true;

            originalSpeed = playerMov.moveSpeed;
            playerMov.moveSpeed = originalSpeed / 2;
        }

        void DeactivateStar()
        {
            downSizing = true;
            starActive = false;
            playerMov.moveSpeed = originalSpeed / 2;
        }

        public override void Update()
        {
            
            if (GameState.IsPaused) return;
            Vector3 rotation = playerMov.GetTransform().Rotation - playerMov.GetBaseRotation();

            float ownerRotY = rotation.Y * MathHelper.DegToRad; // convert the transform's rotation from deg to rad

            /* To rotate our point (which can be inferred by the player's forward direction) by the Y-axis, in which we only need to care about x and z coordinates
              x2 = x1 * cos  + z1 * sin
              z2 = x1 * -sin + z1 * cos
            */
            Vector3 ownerForwardDir = playerMov.forwardDir;
            float cosResult = MathHelper.Cos(ownerRotY);
            float sinResult = MathHelper.Sin(ownerRotY);

            Vector3 lookAtDir = new Vector3(ownerForwardDir.X * cosResult + ownerForwardDir.Z * sinResult, 0.0f, ownerForwardDir.X * (-sinResult) + ownerForwardDir.Z * cosResult); // first, calculate the rotated point
            Vector3 lookAtRotation = playerTrans.Rotation + lookAtDir + new Vector3(0, 90, 0);

            //hammerPowerUpLocation.Position = playerlocation.Position + (lookAtDir * 10);
            //hammerPowerUpLocation.Rotation = lookAtRotation;


            

            if (starActive == true)
            {
                starModeTrans.Scale = Vector3.Lerp(starModeTrans.Scale, targetScale, 10f * Time.DeltaTime);
                starModeTrans.Position = playerTrans.Position + new Vector3(0, 5f, 0);
                starModeTrans.Rotation = lookAtRotation + new Vector3(0, -90f, 0);

                powerTimer -= Time.DeltaTime;

                if (starCollide.canStun == false || powerTimer <= 0f)
                {
                    powerTimer = 3f;
                    DeactivateStar();
                }
            }

            if (upSizing == true && starActive == false)
            {
                starAnimTimer -= Time.DeltaTime;

                starModeTrans.Scale = Vector3.Lerp(starModeTrans.Scale, targetScale, 10f * Time.DeltaTime);
                starModeTrans.Position = Vector3.Lerp(starModeTrans.Position, playerTrans.Position + targetPos, 10f * Time.DeltaTime);
                starModeTrans.Rotation = lookAtRotation + new Vector3(0, -90f, 0);

                if (starAnimTimer <= 0 && starAnimCounter == 0)
                {
                    targetScale = new Vector3(12f, 12f, 12f);
                    targetPos = new Vector3(0, 1f, 0);
                    starAnimCounter++;
                    starAnimTimer = animTime;

                    if (audioComp != null)
                    {
                        audioComp.SoundFilepath = ".\\resources\\audios\\BecomeBigger.wav";
                        audioComp.Volume = 1.0f;
                        audioComp.ChannelType = Audio.ChannelType.SFX;
                        audioComp.PlaySound();
                    }
                }
                else if (starAnimTimer <= 0 && starAnimCounter == 1)
                {
                    targetScale = new Vector3(18f, 18f, 18f);
                    targetPos = new Vector3(0, 4f, 0);
                    starAnimCounter++;
                    starAnimTimer = animTime;
                }
                else if (starAnimTimer <= 0 && starAnimCounter == 2)
                {
                    targetScale = new Vector3(16f, 16f, 16f);
                    targetPos = new Vector3(0, 3f, 0);
                    starAnimCounter++;
                    starAnimTimer = animTime;
                }
                else if (starAnimTimer <= 0 && starAnimCounter == 3)
                {
                    targetScale = new Vector3(20f, 20f, 20f);
                    targetPos = new Vector3(0, 5f, 0);
                    starAnimCounter++;
                    upSizing = false;
                    starActive = true;
                    starAnimTimer = animTime;
                    starAnimCounter = 0;
                    playerMov.moveSpeed = originalSpeed;
                }
            }

            if (downSizing == true && starActive == false)
            {
                starAnimTimer -= Time.DeltaTime;

                starModeTrans.Scale = Vector3.Lerp(starModeTrans.Scale, targetScale, 10f * Time.DeltaTime);
                starModeTrans.Position = Vector3.Lerp(starModeTrans.Position, playerTrans.Position + targetPos, 10f * Time.DeltaTime);
                starModeTrans.Rotation = lookAtRotation + new Vector3(0, -90f, 0);

                if (starAnimTimer <= 0 && starAnimCounter == 0)
                {
                    targetScale = new Vector3(16f, 16f, 16f);
                    targetPos = new Vector3(0, 3f, 0);
                    starAnimCounter++;
                    starAnimTimer = animTime;
                }
                else if (starAnimTimer <= 0 && starAnimCounter == 1)
                {
                    targetScale = new Vector3(18f, 18f, 18f);
                    targetPos = new Vector3(0, 4f, 0);
                    starAnimCounter++;
                    starAnimTimer = animTime;
                }
                else if (starAnimTimer <= 0 && starAnimCounter == 2)
                {
                    targetScale = new Vector3(10f, 10f, 10f);
                    targetPos = new Vector3(0, 0, 0);
                    starAnimCounter++;
                    starAnimTimer = animTime;
                }
                else if (starAnimTimer <= 0 && starAnimCounter == 3)
                {
                    starAnimCounter++;
                    downSizing = false;
                    starAnimTimer = animTime;
                    starAnimCounter = 0;
                    starModeTrans.Position = starModeOriginalPos;
                    playerInv.HoldingPowerUp = false;
                    playerMov.moveSpeed = originalSpeed;
                }
            }

            

            if (playerInv.starCollected) //Need change
            {
                
                if (!starActive && playerMov.WasInteractKeyPressed())
                {
                    ActivateStar();
                    playerInv.starCollected = false;

                }
            }
        }
    }
}
