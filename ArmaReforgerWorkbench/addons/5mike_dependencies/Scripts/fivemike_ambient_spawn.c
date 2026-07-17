class GameMode_Class: SCR_GameModeEditor 
{
	// user script
	// NOT const!!! // about arrays: scripts/Core/proto/Types.c
	protected ref array<IEntity> spawned_opfor = {};
	protected ref array<int> to_delete = {};
	
	
	void five_mike_loop()
    {
		protected PlayerManager playerManager = GetGame().GetPlayerManager();
		protected array<int> allPlayers = {};
		playerManager.GetPlayers(allPlayers);

		protected EntitySpawnParams params = new EntitySpawnParams();
		params.TransformMode = ETransformMode.WORLD;
		
		protected Resource resource_opfor = Resource.Load("{30ED11AA4F0D41E5}Prefabs/Groups/OPFOR/Group_USSR_FireGroup.et");
		protected Resource resource_opfor_02 = Resource.Load("{30ED11AA4F0D41E5}Prefabs/Groups/OPFOR/Group_USSR_FireGroup.et");
		
		Print("5mike loop ext execute", LogLevel.NORMAL);
		
		// OPFOR
		for (int i = 0; i < spawned_opfor.Count(); i++)
		{
			bool too_far = true;				
			foreach (int oneplayerid : allPlayers)
			{
				protected IEntity player = playerManager.GetPlayerControlledEntity(oneplayerid);
				if (!player) continue;
				if ( spawned_opfor[i] != NULL )
				{
					if ( vector.Distance(spawned_opfor[i].GetOrigin(), player.GetOrigin()) < 400 )
					{
						too_far = false;
					}
				}
			}
			if ( too_far )
			{
				to_delete.Insert(i);
			}
		}
		
		for (int i = 0; i < to_delete.Count(); i++)
		{
			if ( spawned_opfor.IsIndexValid(to_delete[i]) )
			{
				if ( spawned_opfor[to_delete[i]] != NULL )
				{
					SCR_EntityHelper.DeleteEntityAndChildren( spawned_opfor[to_delete[i]] );
				}
				spawned_opfor.Remove(to_delete[i]);
			}
		}
		
		to_delete.Clear();
		
		
		// foreach (int oneplayerid : allPlayers){}
		int oneplayerid = allPlayers.GetRandomElement();
		Print("oneplayerid: " + oneplayerid, LogLevel.NORMAL);
		
		protected IEntity player = playerManager.GetPlayerControlledEntity(oneplayerid);
		if (!player) return;
		protected vector spawn_pos = SCR_Math2D.GenerateRandomPointInRadius(400, 405, player.GetOrigin(), true);
		bool too_close = false;
		foreach (int oneplayerid2 : allPlayers)
		{
			protected IEntity player2 = playerManager.GetPlayerControlledEntity(oneplayerid2);
			if (!player2) continue;
			if ( vector.Distance(player2.GetOrigin(), spawn_pos) < 400 )
			{
				too_close = true;
			}
		}
		
		if ( vector.Distance(GetGame().GetWorld().FindEntityByName("base_blufor").GetOrigin(), spawn_pos) < 450 )
		{
			too_close = true;
		}
		
		if ( !too_close )
		{
			//params.Transform = {"1 0 0","0 1 0","0 0 1",spawn_pos};
			params.Transform[3] = spawn_pos;
			ResourceName wp_prefab = "{FFF9518F73279473}PrefabsEditable/Auto/AI/Waypoints/E_AIWaypoint_Move.et";
			
			if( (spawned_opfor.Count() < 20) && (vector.Distance(GetGame().GetWorld().FindEntityByName("spawner_opfor").GetOrigin(), spawn_pos) < 6000)  )
			{
				protected IEntity opfor = GetGame().SpawnEntityPrefab(resource_opfor, GetGame().GetWorld(), params);
				spawned_opfor.Insert(opfor);
				
				EntitySpawnParams spawnParams_opfor = new EntitySpawnParams();
				spawnParams_opfor.Transform[3] = player.GetOrigin(); // player.GetOrigin() // spawn_pos
				AIWaypoint wp_opfor =  AIWaypoint.Cast(GetGame().SpawnEntityPrefab(Resource.Load(wp_prefab), GetGame().GetWorld(), spawnParams_opfor));
				
				protected AIGroup group_opfor = AIGroup.Cast(opfor);
				group_opfor.AddWaypoint(wp_opfor);
			}
		}
	}

	override void OnGameStart()
	{
		Print("5mike loop ext initialized", LogLevel.NORMAL);
		GetGame().GetCallqueue().CallLater(this.five_mike_loop, 90000, true);
	}

};

