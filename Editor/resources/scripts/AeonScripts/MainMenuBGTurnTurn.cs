﻿using AeonScriptCore;
using System;
using System.Collections.Generic;
using System.Linq;
using System.Text;
using System.Threading.Tasks;

namespace AeonScripts
{
    public class MainMenuBGTurnTurn : AeonBehaviour
    {
        public float speed;
        public override void Update()
        {
            entity.GetComponent<Transform>().Rotation += new Vector3(0, 0, speed) * Time.DeltaTime;
        }
    }
}
