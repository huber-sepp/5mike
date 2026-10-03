// This registers your custom entity class into the engine's meta-system
class FiveMikeAmbientSpawnClass : GenericEntityClass {}; 

class FiveMikeAmbientSpawn : GenericEntity 
{
	// EXPOSED ATTRIBUTE: Draws a field with a file browser box on the right-hand panel!
	[Attribute("{30ED11AA4F0D41E5}Prefabs/Groups/OPFOR/Group_USSR_FireGroup.et", uiwidget: UIWidgets.EditBox, desc: "Group prefab to spawn", params: "et")]
	protected ResourceName m_GroupPrefab;

	// EXPOSED ATTRIBUTE: Draws an edit box for loop time tracking, defaulting to 90 seconds
	[Attribute("90", uiwidget: UIWidgets.EditBox, desc: "Spawn Loop Delay (in Seconds)")]
	protected int m_LoopDelaySeconds;

	protected ref array<IEntity> spawned_opfor = {};
	
	void five_mike_loop()
	{
		// Stop execution if this code runs on a client machine
		if (GetGame().GetBackendApi() && !Replication.IsServer())
			return;

		PlayerManager playerManager = GetGame().GetPlayerManager();
		if (!playerManager) return;

		array<int> allPlayers = {};
		playerManager.GetPlayers(allPlayers);
		if (allPlayers.Count() == 0) return;

		EntitySpawnParams params = new EntitySpawnParams();
		params.TransformMode = ETransformMode.WORLD;
		
		// Load the custom prefab defined by your panel property
		Resource resource_opfor = Resource.Load(m_GroupPrefab);
		if (!resource_opfor)
		{
			Print("5mike loop error: Invalid Group Prefab path selected in World Editor!", LogLevel.WARNING);
			return;
		}
		
		Print("5mike loop ext execute", LogLevel.NORMAL);
		
		// Clean up far away units safely backwards
		for (int i = spawned_opfor.Count() - 1; i >= 0; i--)
		{
			IEntity opfor_entity = spawned_opfor[i];
			if (!opfor_entity)
			{
				spawned_opfor.Remove(i);
				continue;
			}

			bool too_far = true;				
			foreach (int oneplayerid : allPlayers)
			{
				IEntity player = playerManager.GetPlayerControlledEntity(oneplayerid);
				if (!player) continue;
				
				if (vector.Distance(opfor_entity.GetOrigin(), player.GetOrigin()) < 400)
				{
					too_far = false;
					break;
				}
			}
			
			if (too_far)
			{
				SCR_EntityHelper.DeleteEntityAndChildren(opfor_entity);
				spawned_opfor.Remove(i);
			}
		}
		
		int oneplayerid = allPlayers.GetRandomElement();
		IEntity player = playerManager.GetPlayerControlledEntity(oneplayerid);
		if (!player) return;
		
		vector spawn_pos = SCR_Math2D.GenerateRandomPointInRadius(400, 405, player.GetOrigin(), true);
		bool too_close = false;
		
		foreach (int oneplayerid2 : allPlayers)
		{
			IEntity player2 = playerManager.GetPlayerControlledEntity(oneplayerid2);
			if (!player2) continue;
			if (vector.Distance(player2.GetOrigin(), spawn_pos) < 400)
			{
				too_close = true;
				break;
			}
		}
		
		IEntity base_blufor = GetGame().GetWorld().FindEntityByName("base_blufor");
		if (base_blufor && vector.Distance(base_blufor.GetOrigin(), spawn_pos) < 450)
		{
			too_close = true;
		}
		
		if (!too_close)
		{
			params.Transform[3] = spawn_pos; // Fixed array position matrix index
			ResourceName wp_prefab = "{FFF9518F73279473}PrefabsEditable/Auto/AI/Waypoints/E_AIWaypoint_Move.et";
			
			IEntity spawner_opfor = GetGame().GetWorld().FindEntityByName("spawner_opfor");
			
			if (spawner_opfor && (spawned_opfor.Count() < 20) && (vector.Distance(spawner_opfor.GetOrigin(), spawn_pos) < 6000))
			{
				IEntity opfor = GetGame().SpawnEntityPrefab(resource_opfor, GetGame().GetWorld(), params);
				if (opfor)
				{
					spawned_opfor.Insert(opfor);
					
					EntitySpawnParams spawnParams_opfor = new EntitySpawnParams();
					// DEFINITIVE INDEX FIX: Explicitly target index 3 for the waypoint translation matrix
					spawnParams_opfor.Transform[3] = player.GetOrigin(); 
					
					AIWaypoint wp_opfor = AIWaypoint.Cast(GetGame().SpawnEntityPrefab(Resource.Load(wp_prefab), GetGame().GetWorld(), spawnParams_opfor));
					AIGroup group_opfor = AIGroup.Cast(opfor);
					
					if (group_opfor && wp_opfor)
					{
						group_opfor.AddWaypoint(wp_opfor);
					}
				}
			}
		}
	}

	// Standalone entities use EOnInit when they are initialized into the world
	override void EOnInit(IEntity owner)
	{
		if (GetGame().GetBackendApi() && !Replication.IsServer())
			return;

		// Guard check: Force to 90 seconds if the panel property is accidentally set to 0 or negative
		if (m_LoopDelaySeconds <= 0)
			m_LoopDelaySeconds = 90;

		Print("5mike loop ext initialized via Standalone Entity", LogLevel.NORMAL);
		
		// Converts your input seconds attribute directly into milliseconds for CallLater
		int delayMilliseconds = m_LoopDelaySeconds * 1000;
		GetGame().GetCallqueue().CallLater(this.five_mike_loop, delayMilliseconds, true);
	}

	// Constructor handles initialization registration
	void FiveMikeAmbientSpawn(IEntitySource src, IEntity parent)
	{
		SetEventMask(EntityEvent.INIT);
	}
};
