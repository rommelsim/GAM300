using AeonScriptCore;

namespace AeonScripts
{
    public class NarrowPathMidpoint : AeonBehaviour
    {
        public string NarrowPathName;

        private int firstPlayerReached = 0; // if 0, no player reached there first. If 1 or 2, means a player reached there

        private Entity entNarrowPath = null;
        private NarrowPathCollider scriptCollider = null;

        private static Vector3 playerSpawnPoint = new Vector3(-1.8f, 6.2f, 0);
        private Entity teleportingEntity = null;

        private bool isInTrigger = false;

        public override void OnCreate()
        {
            entNarrowPath = Entity.FindEntityByName(NarrowPathName);

            if (entNarrowPath != null)
                scriptCollider = entNarrowPath.GetScript<NarrowPathCollider>();
        }

        public override void OnTriggerEnter(uint triggerEntityID)
        {
            Entity otherEnt = Entity.GetEntity(triggerEntityID);
            if (otherEnt != null)
            {
                MoveCharacter moveChar = otherEnt.GetScript<MoveCharacter>();

                // means other character is a player
                if (moveChar != null && otherEnt.Name.Contains("Player")) {
                    if (firstPlayerReached == 0)
                    {
                        isInTrigger = true;
                        firstPlayerReached = otherEnt.Name.EndsWith("1") ? 1 : 2;
                        // when player reached, and if collider script is not null, and the players detected are both there, then teleport the other one
                        if (scriptCollider != null)
                        {
                            // TODO: If player that reached midpoint first
                            Entity otherPlayer = otherEnt.Name.EndsWith("1") ? scriptCollider.entP2 : scriptCollider.entP1;
                            if (otherPlayer != null)
                            {
                                NativeLog.LogError("NarrowPathMidpoint: Mark player for teleportation!");
                                teleportingEntity = otherPlayer;
                            }
                        }

                        NativeLog.LogError("NarrowPathMidpoint: OnTriggerEnter runs");
                    }
                }
            }
        }

        public override void OnTriggerExit(uint triggerEntityID)
        {
            Entity otherEnt = Entity.GetEntity(triggerEntityID);
            if (otherEnt != null)
            {
                MoveCharacter moveChar = otherEnt.GetScript<MoveCharacter>();

                // means other character is a player
                if (moveChar != null && otherEnt.Name.Contains("Player") && isInTrigger) {
                    NativeLog.LogError("NarrowPathMidpoint: OnTriggerExit runs");
                    if ((otherEnt.Name.EndsWith("1") && firstPlayerReached == 1) || (otherEnt.Name.EndsWith("2") && firstPlayerReached == 2))
                    {
                        isInTrigger = false;
                        teleportingEntity = null;
                    }
                }
            }
        }

        public override void Update()
        {
            if (entNarrowPath == null)
                entNarrowPath = Entity.FindEntityByName(NarrowPathName);
            else if (scriptCollider == null)
                scriptCollider = entNarrowPath.GetScript<NarrowPathCollider>();

            if (teleportingEntity != null && isInTrigger && firstPlayerReached > 0)
            {
                teleportingEntity.GetComponent<Transform>().Position = playerSpawnPoint;
                NativeLog.LogError("NarrowPathMidpoint: Player teleported to {0}", teleportingEntity.GetComponent<Transform>().Position);
                teleportingEntity = null;

                // if P1 reached first, we teleport P2 becos P2 is no longer colliding with the Narrow Path
                if (firstPlayerReached == 1)
                    scriptCollider.entP2 = null;
                else // if P2 reached first, we teleport P1 becos P1 is no longer colliding with the Narrow Path
                    scriptCollider.entP1 = null;
            }
        }
    }
}
