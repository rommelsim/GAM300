using AeonScriptCore;

namespace AeonScripts
{
    public class RiseAndDestroy : AeonBehaviour
    {
        private Vector3 targetPos = new Vector3();
        private Vector3 targetScale = new Vector3();
        private Transform transformComp = null;

        private float timer = 1f;
        private float scaleTimer = 0.2f;

        public override void OnCreate()
        {
            transformComp = entity.GetComponent<Transform>();
            targetPos = transformComp.Position + new Vector3(0, 10, 0);
            targetScale = Vector3.ZERO;
        }

        public override void Update()
        {
            transformComp.Position = Vector3.Lerp(transformComp.Position, targetPos, 1f * Time.DeltaTime);
            scaleTimer = Mathf.Lerp(0.2f, 100f, 1f * Time.DeltaTime);
            transformComp.Scale = Vector3.Lerp(transformComp.Scale, targetScale, scaleTimer * Time.DeltaTime);

            if (GameState.IsPaused) return;

            timer -= Time.DeltaTime;
            if (timer <= 0f)
            {
                Entity.Destroy(this.entity);
            }
        }
    }
}
